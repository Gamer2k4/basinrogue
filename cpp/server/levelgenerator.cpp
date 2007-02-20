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

void LevelGenerator::SetFloorAndWallTiles ( TileLib& tile_lib, std::string floor_theme, std::string walls_theme )
{
	// so far: floor_theme = "dirt" or "grey", walls_theme = "wood" or "stone"
	// can add more later if desired
	if ( floor_theme == "dirt" )
	{
		ground = tile_lib.GetTileByName ( "town_ground" );
	}
	else if ( floor_theme == "grey" )
	{
		ground = tile_lib.GetTileByName ( "ground" );
	}
	if ( walls_theme == "wood" )
	{
		wall        = tile_lib.GetTileByName ( "town_wall" );
		stairs_down = tile_lib.GetTileByName ( "town_stairs_down" );
		stairs_up   = tile_lib.GetTileByName ( "town_stairs_up" );
	}
	else if ( walls_theme == "stone" )
	{
		wall        = tile_lib.GetTileByName ( "wall" );
		stairs_down = tile_lib.GetTileByName ( "stairs_down" );
		stairs_up   = tile_lib.GetTileByName ( "stairs_up" );
	}
}

Level& LevelGenerator::MakeFromPattern ( TileLib& tile_lib,
										 int levelsizex, int levelsizey, std::string pattern,
				   			             std::vector< std::pair<int, int> > monster_locations, std::vector<std::string> monster_types,
							             int depth, int kill_on_leave )
{
	Level& level = *new Level( levelsizex, levelsizey );
	Monster* mob;
	for ( int jj=0; jj < levelsizey; ++jj )
		for ( int ii=0; ii < levelsizex; ++ii )
		{
			char tile_char = pattern[ii+jj*levelsizex];
			Tile* this_tile;
			TileTrigger* trigger = 0;
			if ( tile_char=='#' )
				this_tile = wall;
			else if ( tile_char == 'S' )
			{
				this_tile = stairs_down;
				trigger = new StairsChangeDungeonTrigger("sewer", 1, "up");
				trigger->tag = "sewer";
			}
			else if ( tile_char == 'T' )
			{
				this_tile = stairs_up;
				trigger = new StairsChangeDungeonTrigger("town", 0, "sewer");
				trigger->tag = "up";
			}
			else if ( tile_char == '>' )
			{
				this_tile = stairs_down;
				trigger = new StairsDownTrigger("up");
				trigger->tag = "down";
			}
			else if ( tile_char == '<' )
			{
				this_tile = stairs_up;
				trigger = new StairsUpTrigger("down");
				trigger->tag = "up";
			}
			else
				this_tile = ground;
			level.SetTile ( ii, jj, this_tile, trigger );
		}
	for ( int i = 0; i < monster_locations.size(); i++)
	{
		mob = new Monster();
		mob->SetLevel ( level, monster_locations[i].first, monster_locations[i].second );
		mob->SetAppearance ( tile_lib.GetTileByName ( monster_types[i] ) );
	}
	level.depth = depth;
	if ( kill_on_leave )
	{
		level.ActivateKillOnLeave();
	}
	return level;
}

StaticLevelGenerator::StaticLevelGenerator ( Level& level ) : level ( level )
{}

Level& StaticLevelGenerator::GenerateLevel ( int depth )
{
	return level;
}

TownLevelGenerator::TownLevelGenerator (
	TileLib& tile_lib ) :
		tile_lib ( tile_lib )
{
	SetFloorAndWallTiles ( tile_lib, "dirt", "wood" );
}

TownLevelGenerator::~TownLevelGenerator()
{}

Level& TownLevelGenerator::GenerateLevel ( int depth )
{
	int levelsizex = 35;
	int levelsizey = 30;
	std::string pattern =
	"###################################"
    "#...................#...###########"
    "#.............####..#...###########"
    "#......#####..#..#......###########"
    "#......#...#.....#..###############"
    "#......#...#..#..#......###########"
    "#......##.##..####................#"
    "#..............#........#########.#"
    "#..............#........#########.#"
    "#S#............#........#########.#"
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
    std::vector< std::pair<int,int> > no_monster_locs;
    std::vector<std::string> no_monsters;
	return MakeFromPattern ( tile_lib, levelsizex, levelsizey, pattern, no_monster_locs, no_monsters, depth, 0);
}

InstanceLevelGenerator::InstanceLevelGenerator (
    TileLib& tile_lib ) :
		tile_lib ( tile_lib )
{
	SetFloorAndWallTiles ( tile_lib, "grey", "stone" );
}

InstanceLevelGenerator::~InstanceLevelGenerator()
{}

Level& InstanceLevelGenerator::GenerateLevel ( int depth )
{
	int levelsizex = 10;
	int levelsizey = 10;
	std::string pattern;
	if ( depth==1 )
	{
		pattern =
		"##########"
		"#...>....#"
		"#........#"
		"#........#"
		"#T.......#"
		"#........#"
		"#........#"
		"#........#"
		"#........#"
		"##########";
	}
	else if ( depth==10 )
	{
		pattern =
		"##########"
		"#........#"
		"#........#"
		"#........#"
		"#<.......#"
		"#........#"
		"#........#"
		"#........#"
		"#........#"
		"##########";
	}
	else
	{
		pattern =
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
	}
    std::vector< std::pair<int,int> > monster_locs;
    std::vector<std::string> monsters;
    monster_locs.push_back ( std::make_pair ( 7, 7 ) );
    monsters.push_back ( "goblin" );
    monster_locs.push_back ( std::make_pair ( 8, 8 ) );
    monsters.push_back ( "goblin" );
	return MakeFromPattern ( tile_lib, levelsizex, levelsizey, pattern, monster_locs, monsters, depth);
}
