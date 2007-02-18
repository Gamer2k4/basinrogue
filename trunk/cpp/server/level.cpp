//
// C++ Implementation: level
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <sstream>
#include <string>

#pragma warning(disable:4786)

#include "level.h"
#include "player.h"
#include "dungeon.h"
#include "world.h"

TileTrigger::TileTrigger() : temporary( true )
{}

TileTrigger::~TileTrigger()
{}

void TileTrigger::OnPlayerStepsOnTile ( Player& player )
{}

bool TileTrigger::getIsTemporary() const
{
	return temporary;
}

void TileTrigger::setIsTemporary ( bool theValue )
{
	temporary = theValue;
}

LevelChangeTrigger::LevelChangeTrigger ( const std::string& target_tag ) : target_tag ( target_tag )
{}

StairsDownTrigger::StairsDownTrigger( const std::string& target_tag ) :
		LevelChangeTrigger( target_tag )
{}

void StairsDownTrigger::OnPlayerStepsOnTile ( Player& player )
{
	Level* level = player.mobile->GetLevel();
	Dungeon& current_dungeon = level->GetDungeon();
	current_dungeon.getWorld().PlayerChangesDungeon ( player, current_dungeon.getName(), level->depth + 1, target_tag );
	player.SendMessage ( "You go down the stairs..." );
}

StairsUpTrigger::StairsUpTrigger( const std::string& target_tag ) :
		LevelChangeTrigger( target_tag )
{}

void StairsUpTrigger::OnPlayerStepsOnTile ( Player& player )
{
	Level* level = player.mobile->GetLevel();
	Dungeon& current_dungeon = level->GetDungeon();
	current_dungeon.getWorld().PlayerChangesDungeon ( player, current_dungeon.getName(), level->depth - 1, target_tag );
	player.SendMessage ( "You climb the stairs..." );
}

StairsChangeDungeonTrigger::StairsChangeDungeonTrigger ( const std::string& next_dungeon, int depth, const std::string& target_tag ) :
		LevelChangeTrigger( target_tag ),
		next_dungeon ( next_dungeon ),
		depth ( depth )
{}

void StairsChangeDungeonTrigger::OnPlayerStepsOnTile ( Player& player )
{
	player.SendMessage ( "You pass through a portal..." );
	Level* level = player.mobile->GetLevel();
	Dungeon& current_dungeon = level->GetDungeon();
	current_dungeon.getWorld().PlayerChangesDungeon ( player, next_dungeon, depth, target_tag );
}

LevelTile::LevelTile() : tile ( 0 ), trigger ( 0 )
{}

LevelTile::~LevelTile()
{
	if (trigger && trigger->getIsTemporary() )
		delete trigger;
}

Level::Level ( int sizex, int sizey ) :
		sizex ( sizex ),
		sizey ( sizey ),
		depth ( 0 ),
		dungeon ( 0 ),
		clock ( 0.0 ),
		must_kill_on_leave ( false )
{
	level_table.resize ( sizex * sizey );
}

void Level::SendTileInfo ( NetworkCommandBuffer* buffer, int x, int y )
{
	int index = x + y * sizex;
	buffer->SendChar ( MSG_CLEARTILE );
	buffer->SendInt ( x );
	buffer->SendInt ( y );
	if ( level_table[index].tile )
	{
		buffer->SendChar ( MSG_ADDTILE );
		buffer->SendInt ( x );
		buffer->SendInt ( y );
		buffer->SendInt ( level_table[index].tile->GetTileId() );
	}
	Mobile* mob = GetMobileAt ( x, y );
	if ( mob )
	{
		buffer->SendChar ( MSG_ADDTILE );
		buffer->SendInt ( x );
		buffer->SendInt ( y );
		buffer->SendInt ( mob->GetAppearance()->GetTileId() );
	}
}

void Level::SetIsDirty ( int x, int y )
{
	std::set<LevelViewPort*>::iterator iter;
	for ( iter = viewport_list.begin(); iter != viewport_list.end(); ++iter )
	{
		LevelViewPort* viewport = *iter;
		viewport->SetIsDirty ( x, y );
	}
}

LevelTile& Level::GetTile ( int posx, int posy )
{
	int index = posx + posy * sizex;
	return level_table[index];
}

void Level::SetTile ( int posx, int posy, Tile* tile, TileTrigger* trigger )
{
	int index = posx + posy * sizex;
	level_table[index].tile = tile;
	level_table[index].trigger = trigger;
	SetIsDirty ( posx, posy );
}

void Level::BeforeMoveEvent ( Mobile& mob )
{
	SetIsDirty ( mob.posx, mob.posy );
}

void Level::AfterMoveEvent ( Mobile& mob )
{
	SetIsDirty ( mob.posx, mob.posy );
}

void Level::TimeElapse ( double time )
{
	clock = clock + time;
	std::set<Mobile*>::iterator iter;
	for ( iter = mobile_list.begin(); iter != mobile_list.end(); ++iter )
		( *iter )->GainEnergy ( time );
}

void Level::StuffCanHappen()
{
	std::set<Mobile*>::iterator iter;
	for ( iter = mobile_list.begin(); iter != mobile_list.end(); ++iter )
	{
		if ( ( *iter )->HasEnergy() )
		{
			if ( ( *iter )->IsMonster() )
			{
				Monster* this_mob = dynamic_cast<Monster*> ( *iter );
				this_mob->MonsterTurn();
			}
		}
	}
}

void Level::AddMobile ( Mobile& mob )
{
	mobile_list.insert ( &mob );
	mobile_hash [ std::pair<int,int> ( mob.posx,mob.posy ) ] = &mob;
}

void Level::RemoveMobile ( Mobile& mob )
{
	mobile_list.erase ( &mob );
	mobile_hash.erase ( std::make_pair ( mob.posx,mob.posy ) );
}

void Level::MoveMobile ( Mobile& mob, int old_posx, int old_posy )
{
	mobile_hash.erase ( std::make_pair ( old_posx,old_posy ) );
	mobile_hash [ std::pair<int,int> ( mob.posx,mob.posy ) ] = &mob;
}

Mobile* Level::GetMobileAt ( int posx, int posy )
{
	if ( mobile_hash.find ( std::make_pair ( posx, posy ) ) == mobile_hash.end ( ) )
		return 0;
	else
		return mobile_hash [ std::make_pair ( posx, posy ) ];
}

void Level::AddViewPort ( LevelViewPort* viewport )
{
	viewport_list.insert ( viewport );
}

void Level::RemoveViewPort ( LevelViewPort* viewport )
{
	viewport_list.erase ( viewport );
}

void Level::SetDungeon ( Dungeon& dungeon )
{
	this->dungeon = &dungeon;
}

Dungeon& Level::GetDungeon()
{
	return *dungeon;
}

void Level::ActivateKillOnLeave()
{
	must_kill_on_leave = true;
}

bool Level::getMustKillOnLeave()
{
	return must_kill_on_leave;
}

bool Level::FindTileWithTag(const std::string& tag, int& outx, int& outy)
{
	if (tag.empty())
		return false;
	int ii, jj;
	for (jj = 0; jj < sizey; ++jj)
		for (ii = 0; ii < sizex; ++ii)
		{
			LevelTile& current = level_table[ii+jj*sizex];
			if (current.trigger && current.trigger->tag == tag)
			{
				outx = ii;
				outy = jj;
				return true;
			}
		}
	return false;
}


LevelViewPort::LevelViewPort() : level ( 0 )
{}

LevelViewPort::~LevelViewPort()
{
	if ( level )
		AttachToLevel ( 0,0 );
}

void LevelViewPort::AttachToLevel ( Level* new_level, NetworkCommandBuffer* buffer )
{
	if ( level )
		level->RemoveViewPort ( this );
	level = new_level;
	if ( level )
	{
		level->AddViewPort ( this );
		sizex = level->sizex;
		sizey = level->sizey;
		MarkAllDirty();
		buffer->SendChar ( MSG_RESIZEWORLD );
		buffer->SendInt ( level->sizex );
		buffer->SendInt ( level->sizey );
		MarkAllDirty();
	}
}

bool LevelViewPort::IsDirty ( int x, int y )
{
	return dirty_table[x + y * sizex];
}

void LevelViewPort::SetIsDirty ( int x, int y )
{
	dirty_table[x + y * sizex] = true;
}

void LevelViewPort::MarkAllDirty()
{
	std::vector<bool> new_dirty_table ( sizex * sizey, true );
	dirty_table.swap ( new_dirty_table );
}

void LevelViewPort::MarkAllClean()
{
	std::vector<bool> new_dirty_table ( sizex * sizey, false );
	dirty_table.swap ( new_dirty_table );
}

void LevelViewPort::SendLevelInfo ( NetworkCommandBuffer* buffer, int posx, int posy )
{
	buffer->SendChar ( MSG_SETCHARPOS );
	buffer->SendInt ( posx );
	buffer->SendInt ( posy );
	for ( int jj=0; jj < sizey; ++jj )
		for ( int ii=0; ii < sizex; ++ii )
			if ( IsDirty ( ii, jj ) )
				level->SendTileInfo ( buffer, ii, jj );
	buffer->SendChar ( MSG_SWAPBUFFERS );
}

