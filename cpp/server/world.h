//
// C++ Interface: world
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <map>

class Dungeon;
class Mobile;
class Player;

typedef std::map<std::string, Dungeon*> DungeonDatabase;

/**
	@author
*/
class World
{
	friend class Dungeon;

	private:
		DungeonDatabase dungeon_list;
		std::string starting_dungeon;

		void AddDungeon(Dungeon& dungeon);

	public:
		World();
		~World();

		bool HasDungeon(Dungeon& dungeon) const;
		void SetStartingDungeon(const std::string& dungeon_name);

		void AddPlayer(Mobile* mobile);
		void PlayerChangesDungeon(Player& player, const std::string& dungeon_name, int depth, const std::string& entrance_tag);
};

#endif
