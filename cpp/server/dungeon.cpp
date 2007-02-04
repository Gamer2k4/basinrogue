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

Dungeon::Dungeon(const std::string& name, World& world) :
	name(name),
	world(world)
{
	world.AddDungeon(*this);
}


Dungeon::~Dungeon()
{}

std::string Dungeon::getName() const
{
	return name;
}




TownLevel::TownLevel(const std::string& name, World& world, Level& town_level) :
		Dungeon(name, world),
		town_level(town_level)
{}

TownLevel::~TownLevel()
{
	delete &town_level;
}

Level* TownLevel::GetLevel(int depth)
{
	return &town_level;
}
