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
#pragma warning(disable:4786)

#include "levelgenerator.h"

LevelGenerator::~LevelGenerator()
{}


StaticLevelGenerator::StaticLevelGenerator ( Level& level ) : level ( level )
{}

Level& StaticLevelGenerator::GenerateLevel ( int depth )
{
	return level;
}


InstanceLevelGenerator::InstanceLevelGenerator (
    int sizex, int sizey,
    const char* pattern,
    Tile* goblin,
    Tile* ground,
    Tile* wall,
    Tile* stairs_down,
    Tile* stairs_up ) :
		sizex ( sizex ),
		sizey ( sizey ),
		goblin ( goblin ),
		pattern ( pattern ),
		ground ( ground ),
		wall ( wall ),
		stairs_down ( stairs_down ),
		stairs_up ( stairs_up )
{}

InstanceLevelGenerator::~InstanceLevelGenerator()
{}

Level& InstanceLevelGenerator::GenerateLevel ( int depth )
{
	Level& level = *new Level ( sizex, sizey );
	for ( int jj=0; jj < sizey; ++jj )
		for ( int ii=0; ii < sizex; ++ii )
		{
			Tile* tile;
			TileTrigger* trigger = 0;
			switch ( pattern[ii+jj*sizex] )
			{
				case '#':
					tile = wall;
					break;
				case '.':
					tile = ground;
					break;
				case '<':
					if (depth < 10)
					{
						tile = stairs_down;
						trigger = new StairsDownTrigger();
					}
					else
						tile = ground;
					break;
				case '>':
					tile = stairs_up;
					if (depth > 0)
						trigger = new StairsUpTrigger();
					else
						trigger = new StairsChangeDungeonTrigger("town");
					break;
				default:
					tile = 0;
			}
			level.SetTile ( ii, jj, tile, trigger );
		}
	Monster* mob = new Monster();
	mob->SetLevel ( level, 7, 7 );
	mob->SetAppearance ( goblin );
	level.depth = depth;
	level.ActivateKillOnLeave();
	return level;
}
