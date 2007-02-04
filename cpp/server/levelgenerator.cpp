//
// C++ Implementation: levelgenerator
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "levelgenerator.h"

LevelGenerator::~LevelGenerator()
{}


StaticLevelGenerator::StaticLevelGenerator ( Level& level ) : level ( level )
{}

Level& StaticLevelGenerator::GenerateLevel()
{
	return level;
}


InstanceLevelGenerator::InstanceLevelGenerator ( int sizex, int sizey, const char* pattern, Tile* ground, Tile* wall ) :
		sizex ( sizex ),
		sizey ( sizey ),
		pattern ( pattern ),
		ground ( ground ),
		wall ( wall )
{}

InstanceLevelGenerator::~InstanceLevelGenerator()
{}

Level& InstanceLevelGenerator::GenerateLevel()
{
	Level& level = *new Level ( sizex, sizey );
	for ( int jj=0; jj < sizey; ++jj )
		for ( int ii=0; ii < sizex; ++ii )
		{
			Tile* tile;
			switch ( pattern[ii+jj*sizex] )
			{
				case '#':
					tile = wall;
				case '.':
					tile = ground;
				default:
					tile = 0;
			}
			level.SetTile ( ii, jj, tile );
		}
	return level;
}
