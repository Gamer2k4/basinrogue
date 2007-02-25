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

class Dungeon;

/**
	@author
*/
class LevelGenerator
{
	public:
		virtual ~LevelGenerator();
		virtual Level& GenerateLevel ( int depth, const Dungeon& dungeon ) = 0;
};

typedef void ( *GenerateEvent ) ( Level& level, int x, int y, void* closure );

struct TileConverter
{
	Tile* tile;
	GenerateEvent event;
	void* closure;

	TileConverter ( Tile* tile = 0, GenerateEvent event = 0, void* closure = 0 ) :
			tile ( tile ),
			event ( event ),
			closure ( closure )
	{}
};

class MapLevelGenerator : public LevelGenerator
{
	private:
		int sizex;
		int sizey;
		const char* pattern;
		TileConverter converter_array[256];
		Tile* ground;
	public:
		MapLevelGenerator ( int sizex, int sizey, const char* pattern, Tile* ground );

		void SetTileConverter ( unsigned char tile_id, Tile* tile, GenerateEvent event = 0, void* closure = 0 );
		virtual ~MapLevelGenerator();
		virtual Level& GenerateLevel ( int depth, const Dungeon& dungeon );
};

void GenerateUpStairs ( Level& level, int x, int y, void* closure );
void GenerateDownStairs ( Level& level, int x, int y, void* closure );
void ChangeDungeonTeleport ( Level& level, int x, int y, void* closure );
void GenerateMonster ( Level& level, int x, int y, void* closure );

#endif
