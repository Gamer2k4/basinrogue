//
// C++ Interface: level
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <set>
#include "tile.h"
#include "mobile.h"
#include "networkcommandbuffer.h"

class Dungeon;
class Player;

const int FLAG_BLOCKS_MOVEMENT = 1 << 0;
const int FLAG_BLOCKS_VIEW = 1 << 1;

class TileTrigger
{
	public:
		virtual void OnPlayerStepsOnTile ( Player& player );
};

class StairsDownTrigger : public TileTrigger
{
	public:
		virtual void OnPlayerStepsOnTile ( Player& player );
};

class StairsUpTrigger : public TileTrigger
{
	public:
		virtual void OnPlayerStepsOnTile ( Player& player );
};

class StairsChangeDungeonTrigger : public TileTrigger
{
	private:
		std::string next_dungeon;
		int depth;
	public:
		StairsChangeDungeonTrigger ( const std::string& next_dungeon, int depth = 0 );
		virtual void OnPlayerStepsOnTile ( Player& player );
};

class LevelTile
{
	private:
		//LevelTile(const LevelTile&);
		//const LevelTile& operator =(const LevelTile&);
	public:
		Tile* tile;
		TileTrigger* trigger;
		LevelTile();
};

class LevelViewPort;

class Level
{
	private:
		std::vector<LevelTile> level_table;
		std::set<Mobile*> mobile_list;
		std::set<LevelViewPort*> viewport_list;
		Dungeon* dungeon;
		bool must_kill_on_leave;

		void SetIsDirty ( int x, int y );
	public:
		const int sizex;
		const int sizey;

		int depth;

		Level ( int sizex, int sizey );
		LevelTile& GetTile ( int posx, int posy );
		void SetTile ( int posx, int posy, Tile* tile, TileTrigger* trigger = 0 );
		void SetTile ( int posx, int posy, LevelTile* tile );

		void BeforeMoveEvent ( Mobile& mob );
		void AfterMoveEvent ( Mobile& mob );

		void AddMobile ( Mobile& mob );
		void RemoveMobile ( Mobile& mob );

		void AddViewPort ( LevelViewPort* viewport );
		void RemoveViewPort ( LevelViewPort* viewport );

		void SendTileInfo ( NetworkCommandBuffer* buffer, int x, int y ) const;

		void SetDungeon ( Dungeon& dungeon );
		Dungeon& GetDungeon();

		void ActivateKillOnLeave();
		bool getMustKillOnLeave();

};

class LevelViewPort
{
	private:
		std::vector<bool> dirty_table;

		Level* level;
		int sizex;
		int sizey;
	public:
		LevelViewPort();
		~LevelViewPort();

		void AttachToLevel ( Level* new_level, NetworkCommandBuffer* buffer );
		bool IsDirty ( int x, int y );
		void SetIsDirty ( int x, int y );
		void MarkAllDirty();
		void MarkAllClean();

		void SendLevelInfo ( NetworkCommandBuffer* buffer, int posx, int posy );
};

#endif
