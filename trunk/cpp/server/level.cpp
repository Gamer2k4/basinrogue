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

#include "level.h"
#include <sstream>
#include <string>

LevelTile::LevelTile() : tile ( 0 )
{}

Level::Level ( int sizex, int sizey ) : sizex ( sizex ), sizey ( sizey )
{
	level_table.resize ( sizex * sizey );
}

void Level::SendTileInfo ( NetworkCommandBuffer* buffer, int x, int y ) const
{
	int index = x + y * sizex;
	buffer->SendChar ( 'c' );
	buffer->SendInt ( x );
	buffer->SendInt ( y );
	if ( level_table[index].tile )
	{
		buffer->SendChar ( 't' );
		buffer->SendInt ( x );
		buffer->SendInt ( y );
		buffer->SendInt ( level_table[index].tile->GetTileId() );
	}

	//TODO better mobile search algorithm is needed

	std::set<Mobile*>::const_iterator iter;
	for ( iter = mobile_list.begin(); iter != mobile_list.end(); ++iter )
	{
		Mobile* mob = *iter;
		if ( mob->posx == x && mob->posy == y )
		{
			buffer->SendChar ( 't' );
			buffer->SendInt ( x );
			buffer->SendInt ( y );
			buffer->SendInt ( mob->GetAppearance()->GetTileId() );
		}
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

Tile* Level::GetTile ( int posx, int posy )
{
	int index = posx + posy * sizex;
	return level_table[index].tile;
}

void Level::SetTile ( int posx, int posy, Tile* tile )
{
	int index = posx + posy * sizex;
	level_table[index].tile = tile;
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

void Level::AddMobile ( Mobile& mob )
{
	mobile_list.insert ( &mob );
}

void Level::RemoveMobile ( Mobile& mob )
{
	mobile_list.erase ( &mob );
}

void Level::AddViewPort ( LevelViewPort* viewport )
{
	viewport_list.insert ( viewport );
}

void Level::RemoveViewPort ( LevelViewPort* viewport )
{
	viewport_list.erase ( viewport );
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
		buffer->SendChar ( 'd' );
		buffer->SendInt ( level->sizex );
		buffer->SendInt ( level->sizey );
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
	buffer->SendChar ( '@' );
	buffer->SendInt ( posx );
	buffer->SendInt ( posy );
	for ( int jj=0; jj < sizey; ++jj )
		for ( int ii=0; ii < sizex; ++ii )
			if ( IsDirty ( ii, jj ) )
				level->SendTileInfo ( buffer, ii, jj );
}
