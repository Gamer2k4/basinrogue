//
// C++ Implementation: world
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "world.h"

#include "dungeon.h"
#include "mobile.h"

World::World()
{}


World::~World()
{
	for (DungeonDatabase::iterator iter = dungeon_list.begin(); iter != dungeon_list.end(); ++iter)
	{
		delete iter->second;
	}
	dungeon_list.clear();
}


void World::AddDungeon(Dungeon& dungeon)
{
	std::string name = dungeon.getName();
	if (dungeon_list.find(name) != dungeon_list.end())
		throw std::string("Inserting duplicate dungeon name.");
	dungeon_list[name]  = &dungeon;
}

bool World::HasDungeon(Dungeon& dungeon) const
{
	std::string name = dungeon.getName();
	return (dungeon_list.find(name) != dungeon_list.end());
}

void World::SetStartingDungeon(const std::string& dungeon_name)
{
	if (dungeon_list.find(dungeon_name) == dungeon_list.end())
		throw std::string("Invalid dungeon.");
	starting_dungeon = dungeon_name;
}

void World::AddPlayer(Mobile* mobile)
{
	Level* level = dungeon_list[starting_dungeon]->GetLevel(0);
	mobile->SetLevel( *level, 1, 1 );
}
