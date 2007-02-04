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
#include "dungeon.h"

#include "world.h"
#include "level.h"
#include "levelgenerator.h"

Dungeon::Dungeon ( const std::string& name, World& world ) :
		name ( name ),
		world ( world )
{
	world.AddDungeon ( *this );
}


Dungeon::~Dungeon()
{}

std::string Dungeon::getName() const
{
	return name;
}

Level* Dungeon::GetLevel ( int depth )
{
	Level* level = VirtualGetLevel( depth );
	level->SetDungeon(*this);
	return level;
}


TownLevel::TownLevel ( const std::string& name, World& world, Level& town_level ) :
		Dungeon ( name, world ),
		town_level ( town_level )
{}

TownLevel::~TownLevel()
{
	delete &town_level;
}

Level* TownLevel::VirtualGetLevel ( int depth )
{
	return &town_level;
}



MultilevelDungeon::MultilevelDungeon ( const std::string& name, World& world, LevelGenerator& generator ) :
		Dungeon ( name, world ),
		generator ( generator )
{}

MultilevelDungeon::~MultilevelDungeon()
{
	delete &generator;
}

Level* MultilevelDungeon::VirtualGetLevel ( int depth )
{
	Level& level = generator.GenerateLevel ( depth );
	return &level;
}


World& Dungeon::getWorld() const
{
	return world;
}


int MultilevelDungeon::getLevelmax() const
{
	return levelmax;
}


void MultilevelDungeon::setLevelmax ( int theValue )
{
	levelmax = theValue;
}
