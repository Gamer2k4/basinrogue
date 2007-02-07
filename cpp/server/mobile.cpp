//
// C++ Implementation: mobile
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#pragma warning(disable:4786)

#include "mobile.h"
#include "level.h"

Mobile::Mobile() : level ( 0 ), appearance ( 0 ), posx ( 0 ), posy ( 0 ), dirty ( true )
{}

Mobile::~Mobile()
{
	if ( level )
	{
		level->BeforeMoveEvent ( *this );
		level->RemoveMobile ( *this );
	}
}

void Mobile::SetLevel ( Level& newlevel, int newposx, int newposy )
{
	if ( level )
	{
		level->BeforeMoveEvent ( *this );
		level->RemoveMobile ( *this );
	}
	posx = newposx;
	posy = newposy;
	level = &newlevel;
	if ( level )
	{
		level->AddMobile ( *this );
		level->AfterMoveEvent ( *this );
	}
}

void Mobile::Translate ( int decx, int decy )
{
	if ( level )
		level->BeforeMoveEvent ( *this );
	int old_posx = posx;
	int old_posy = posy;
	posx += decx;
	posy += decy;
	if ( level )
		level->MoveMobile ( *this, old_posx, old_posy );
	dirty = true;
	if ( level )
		level->AfterMoveEvent ( *this );
}

bool Mobile::TryMove ( int decx, int decy )
{
	int x, y;
	x = posx + decx;
	y = posy + decy;
	if ( x < 0 || y < 0 || x >= level->sizex || y >= level->sizey)
	{
		return false;
	}
	Mobile* mob = level->GetMobileAt ( x, y );
	if ( mob )
	{
		BumpInto ( mob );
		return false;
	}
	LevelTile& tile = level->GetTile ( x, y );
	if ( ! tile.tile->HasOneFlag ( FLAG_BLOCKS_MOVEMENT ) )
	{
		Translate ( decx, decy );
		return true;
	}
	return false;
}

Level* Mobile::GetLevel()
{
	return level;
}

void Mobile::SetAppearance ( Tile* tile )
{
	appearance = tile;
}

Tile* Mobile::GetAppearance()
{
	return appearance;
}

//-----------------------------------------------------------------------------


Monster::Monster() : Hp ( 10 ),
                     energy ( 0.0 )
{}

void Monster::GainEnergy ( double energy_gain )
{
	energy = energy + energy_gain;
}

void Monster::LoseEnergy ( double energy_loss )
{
	energy = energy - energy_loss;
}

int Monster::HasEnergy()
{
	return energy>0;
}

int Monster::ComputeDamage ( Mobile& target )
{
	return 1;
}

void Monster::BumpInto ( Mobile* target )
{
	if ( target->IsMonster() )
	{
		Monster* enemy = dynamic_cast<Monster*> ( target );
		int damage = ComputeDamage ( *enemy );
		enemy->TakeDamage ( damage );
	}
}

void Monster::TakeDamage ( int damage )
{
	Hp = Hp - damage;
	CheckHp();
}

void Monster::CheckHp()
{
	if ( Hp < 0 )
	{
		level->RemoveMobile ( *this );
	}
}

void Monster::MonsterTurn()
{
 	/// @todo obviously lots to do here!
 	int decx = rand() % 3 -1;
 	int decy = rand() % 3 -1;
	TryMove ( decx, decy );
	LoseEnergy ( 1.0 / GetMovementSpeed() );
}

float Monster::GetMovementSpeed()
{
	return 1.0;
}

//-----------------------------------------------------------------------------

PlayerMonster::PlayerMonster() :
		Strength ( 1, 20 ),
		Dexterity ( 1, 20 ),
		Intelligence ( 1, 20 ),
		Magic ( 1, 20 )
{}

int PlayerMonster::ComputeDamage ( Mobile& target )
{
	return Strength;
}

float PlayerMonster::GetMovementSpeed()
{
	return (40.0 + Dexterity) / 50.0;
}

void PlayerMonster::CheckHp()
{
	if ( Hp < 0 )
	{
		/// @todo - for now he is immortal!
	}
}
