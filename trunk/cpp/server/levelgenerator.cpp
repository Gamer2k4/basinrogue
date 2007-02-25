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
#include "dungeon.h"

LevelGenerator::~LevelGenerator()
{}

MapLevelGenerator::MapLevelGenerator (
    int sizex, int sizey,
    const char* pattern,
    Tile* ground ) :
		sizex ( sizex ),
		sizey ( sizey ),
		pattern ( pattern ),
		ground ( ground )
{}

MapLevelGenerator::~MapLevelGenerator()
{}

void MapLevelGenerator::SetTileConverter(unsigned char tile_id, Tile* tile, GenerateEvent event, void* closure)
{
	TileConverter& converter = converter_array[tile_id];
	converter.tile = tile;
	converter.event = event;
	converter.closure = closure;
}

Level& MapLevelGenerator::GenerateLevel ( int depth, const Dungeon& dungeon )
{
	Level& level = *new Level ( sizex, sizey );
	level.SetDungeon(dungeon);
	level.depth = depth;
	for ( int jj=0; jj < sizey; ++jj )
		for ( int ii=0; ii < sizex; ++ii )
		{
			unsigned char ch = pattern[ii+jj*sizex];
			TileConverter& converter = converter_array[ch];
			if (converter.tile)
				level.SetTile ( ii, jj, converter.tile );
			else
				level.SetTile ( ii, jj, ground );
			if (converter.event)
				converter.event(level, ii, jj, converter.closure);
		}
	return level;
}

void GenerateUpStairs(Level& level, int x, int y, void* closure)
{
	const Dungeon& dungeon = level.GetDungeon();
	TileTrigger* trigger;
	if (level.depth > 0)
		trigger = new StairsUpTrigger("down");
	else
		trigger = new StairsChangeDungeonTrigger("town", 0, dungeon.getName());
	trigger->tag = "up";
	level.SetTrigger(x, y, trigger);
}

void GenerateDownStairs(Level& level, int x, int y, void* closure)
{
	const Dungeon& dungeon = level.GetDungeon();
	if (level.depth < dungeon.getLevelmax())
	{
		TileTrigger* trigger = new StairsDownTrigger("up");
		trigger->tag = "down";
		level.SetTrigger(x, y, trigger);
	}
}

void ChangeDungeonTeleport(Level& level, int x, int y, void* closure)
{
	const Dungeon& dungeon = level.GetDungeon();
	const char* dungeon_id = (const char*) closure;
	TileTrigger* trigger = new StairsChangeDungeonTrigger(dungeon_id, 0, "up");
	trigger->tag = dungeon_id;
	level.SetTrigger(x, y, trigger);
}

void GenerateMonster(Level& level, int x, int y, void* closure)
{
	const Dungeon& dungeon = level.GetDungeon();
	Tile* monster_type = (Tile*) closure;
	Monster* mob = new Monster();
	mob->SetLevel ( level, x, y );
	mob->SetAppearance ( monster_type );
}
