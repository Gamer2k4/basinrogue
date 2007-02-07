//
// C++ Interface: levelgenerator
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include "level.h"
#include "tile.h"

/**
	@author
*/
class LevelGenerator
{
	public:
		virtual ~LevelGenerator();
		virtual Level& GenerateLevel(int depth) = 0;
};

class StaticLevelGenerator : public LevelGenerator
{
	private:
		Level& level;
	public:
		StaticLevelGenerator(Level& level);
		virtual Level& GenerateLevel(int depth);
};

class InstanceLevelGenerator : public LevelGenerator
{
	private:
		int sizex;
		int sizey;
		const char* pattern;
		Tile* ground;
		Tile* wall;
		Tile* stairs_down;
		Tile* stairs_up;
   		Tile* goblin;
	public:
		InstanceLevelGenerator(int sizex, int sizey, const char* pattern, Tile* goblin, Tile* ground, Tile* wall, Tile* stairs_down = 0, Tile* stairs_up = 0);
		virtual ~InstanceLevelGenerator();
		virtual Level& GenerateLevel(int depth);
};


#endif
