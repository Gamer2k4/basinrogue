//
// C++ Interface: dungeon
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DUNGEON_H
#define DUNGEON_H

#include <string>

class Level;

class World;

/**
	@author
*/
class Dungeon
{
	private:
		World& world;
		std::string name;

		Dungeon(const Dungeon& level);
		const Dungeon& operator =(const Dungeon& level);
	public:
		Dungeon(const std::string& name, World& world);
		virtual ~Dungeon();

		std::string getName() const;

		virtual Level* GetLevel(int depth) = 0;
};

class TownLevel : public Dungeon
{
	private:
		Level& town_level;
	public:
		TownLevel(const std::string& name, World& world, Level& town_level);
		virtual ~TownLevel();
		virtual Level* GetLevel(int depth);
};

#endif
