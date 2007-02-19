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

TownLevelGenerator::TownLevelGenerator (
	TileLib& tile_lib ) :
		tile_lib ( tile_lib )
{}

Level& TownLevelGenerator::GenerateLevel ( int depth )
{
	int levelsizex = 35;
	int levelsizey = 30;
	const char* pattern =
	"###################################"
    "#...................#...###########"
    "#.............####..#...###########"
    "#......#####..#..#......###########"
    "#......#...#.....#..###############"
    "#......#...#..#..#......###########"
    "#......##.##..####................#"
    "#..............#........#########.#"
    "#..............#........#########.#"
    "#>#............#........#########.#"
    "################..##.############.#"
    "#.................#.....#########.#"
    "#.................#.....#########.#"
    "#.................#.....#########.#"
    "#.................###############.#"
    "#....#####.....#####....#########.#"
    "#....#...#.....#...#....#########.#"
    "#....#...###.###...#....#########.#"
    "#....#.............#....#########.#"
    "#....####.......####....#########.#"
    "#.......#.......#.................#"
    "#.......#...............###########"
    "#.......#.......#.......###########"
    "#.......#.......#.......###########"
    "##.#########.######################"
    "##.#########.######################"
    "#..#########.######################"
    "#.##########.######################"
    "#............######################"
    "###################################"
    ;
	Tile* ground      = tile_lib.GetTileByName ( "town_ground" );
	Tile* wall        = tile_lib.GetTileByName ( "town_wall" );
	Tile* stairs_down = tile_lib.GetTileByName ( "town_stairs_down" );
	Level& level = *new Level( levelsizex, levelsizey );
	for ( int jj=0; jj < levelsizey; ++jj )
		for ( int ii=0; ii < levelsizex; ++ii )
		{
			char tile_char = pattern[ii+jj*levelsizex];
			Tile* this_tile;
			TileTrigger* trigger = 0;
			if ( tile_char=='#' )
				this_tile = wall;
			else if ( tile_char == '>' )
			{
				this_tile = stairs_down;
				trigger = new StairsChangeDungeonTrigger("sewer", 0, "up");
				trigger->tag = "sewer";
			}
			else
				this_tile = ground;
			level.SetTile ( ii, jj, this_tile, trigger );
		}
	return level;
}

TownLevelGenerator::~TownLevelGenerator()
{}

InstanceLevelGenerator::InstanceLevelGenerator (
    TileLib& tile_lib ) :
		tile_lib ( tile_lib )
{}

InstanceLevelGenerator::~InstanceLevelGenerator()
{}

Level& InstanceLevelGenerator::GenerateLevel ( int depth )
{
	int sizex = 10;
	int sizey = 10;
	const char* pattern =
		"##########"
		"#...>....#"
		"#........#"
		"#........#"
		"#<.......#"
		"#........#"
		"#........#"
		"#........#"
		"#........#"
		"##########";
	Tile* goblin      = tile_lib.GetTileByName ( "goblin" );
	Tile* ground      = tile_lib.GetTileByName ( "ground" );
	Tile* wall        = tile_lib.GetTileByName ( "wall" );
	Tile* stairs_down = tile_lib.GetTileByName ( "stairs_down" );
	Tile* stairs_up   = tile_lib.GetTileByName ( "stairs_up" );
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
						trigger = new StairsDownTrigger("up");
						trigger->tag = "down";
					}
					else
						tile = ground;
					break;
				case '>':
					tile = stairs_up;
					if (depth > 0)
						trigger = new StairsUpTrigger("down");
					else
						trigger = new StairsChangeDungeonTrigger("town", 0, "sewer");
					trigger->tag = "up";
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
