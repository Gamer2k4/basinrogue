//
// C++ Implementation: dungeon
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#pragma warning(disable:4786)

#include "dungeon.h"
#include "world.h"
#include "level.h"
#include "levelgenerator.h"

Dungeon::Dungeon ( const std::string& name, const std::string& entry_message, World& world ) :
		name ( name ),
		world ( world ),
		entry_message ( entry_message )
{
	world.AddDungeon ( *this );
}


Dungeon::~Dungeon()
{}

std::string Dungeon::getName() const
{
	return name;
}

std::string Dungeon::GetEntryMessage() const
{
	return entry_message;
}

Level* Dungeon::GetLevel ( int depth )
{
	Level* level = VirtualGetLevel( depth );
	return level;
}


TownLevel::TownLevel ( const std::string& name, const std::string& entry_message, World& world, LevelGenerator& generator ) :
		Dungeon ( name, entry_message, world ),
		town_level ( generator.GenerateLevel(0, *this) )
{

}

TownLevel::~TownLevel()
{
	delete &town_level;
}

Level* TownLevel::VirtualGetLevel ( int depth )
{
	return &town_level;
}



MultilevelDungeon::MultilevelDungeon ( const std::string& name, const std::string& entry_message, World& world, LevelGenerator& generator ) :
		Dungeon ( name, entry_message, world ),
		generator ( generator )
{}

MultilevelDungeon::~MultilevelDungeon()
{
	delete &generator;
}

Level* MultilevelDungeon::VirtualGetLevel ( int depth )
{
	Level& level = generator.GenerateLevel ( depth, *this );
	level.ActivateKillOnLeave();
	return &level;
}


World& Dungeon::getWorld() const
{
	return world;
}


int Dungeon::getLevelmax() const
{
	return levelmax;
}


void Dungeon::setLevelmax ( int theValue )
{
	levelmax = theValue;
}
