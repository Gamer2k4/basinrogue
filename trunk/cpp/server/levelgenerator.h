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

class TownLevelGenerator : public LevelGenerator
{
	private:
		TileLib tile_lib;
	public:
		TownLevelGenerator ( TileLib& tile_lib );
		virtual ~TownLevelGenerator();

		Level& GenerateLevel(int depth);
};

class InstanceLevelGenerator : public LevelGenerator
{
	private:
		TileLib tile_lib;
	public:
		InstanceLevelGenerator ( TileLib& tile_lib ) ;
		virtual ~InstanceLevelGenerator();
		virtual Level& GenerateLevel(int depth);
};


#endif
