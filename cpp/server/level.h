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
#include <map>
#include "tile.h"
#include "mobile.h"
#include "networkcommandbuffer.h"

class Dungeon;
class Player;

const int FLAG_BLOCKS_MOVEMENT = 1 << 0;
const int FLAG_BLOCKS_VIEW = 1 << 1;

class TileTrigger
{
	private:
		bool temporary;
	public:
		TileTrigger();
		virtual ~TileTrigger();

		virtual void OnPlayerStepsOnTile ( Player& player );

		void setIsTemporary ( bool theValue );


		bool getIsTemporary() const;

		std::string tag;
};

class LevelChangeTrigger : public TileTrigger
{
	public:
		LevelChangeTrigger( const std::string& target_tag = "" );
		std::string target_tag;
};

class StairsDownTrigger : public LevelChangeTrigger
{
	public:
		StairsDownTrigger( const std::string& target_tag = "" );
		virtual void OnPlayerStepsOnTile ( Player& player );
};

class StairsUpTrigger : public LevelChangeTrigger
{
	public:
		StairsUpTrigger( const std::string& target_tag = "" );
		virtual void OnPlayerStepsOnTile ( Player& player );
};

class StairsChangeDungeonTrigger : public LevelChangeTrigger
{
	private:
		std::string next_dungeon;
		int depth;
	public:
		StairsChangeDungeonTrigger ( const std::string& next_dungeon, int depth = 0, const std::string& target_tag = "" );
		virtual void OnPlayerStepsOnTile ( Player& player );
};

class LevelTile
{
	private:
		//LevelTile(const LevelTile&);
		//const LevelTile& operator =(const LevelTile&);
	public:
		virtual ~LevelTile();

		Tile* tile;
		TileTrigger* trigger;
		LevelTile();
};

class LevelViewPort;

class Level
{
	private:
		std::vector<LevelTile> level_table;
		std::set<Mobile*> mobile_list;                     // any changes to one of these
		std::map<std::pair<int,int>, Mobile*> mobile_hash; // must be reflected in the other!
		std::set<LevelViewPort*> viewport_list;
		Dungeon* dungeon;
		bool must_kill_on_leave;
		double clock;

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

		void TimeElapse ( double time );
		void StuffCanHappen();

		void AddMobile ( Mobile& mob );
		void RemoveMobile ( Mobile& mob );
		void MoveMobile ( Mobile& mob, int old_posx, int old_posy );
		Mobile* GetMobileAt ( int posx, int posy ); // return 0 if none there

		void AddViewPort ( LevelViewPort* viewport );
		void RemoveViewPort ( LevelViewPort* viewport );

		void SendTileInfo ( NetworkCommandBuffer* buffer, int x, int y );

		void SetDungeon ( Dungeon& dungeon );
		Dungeon& GetDungeon();

		void ActivateKillOnLeave();
		bool getMustKillOnLeave();

		bool FindTileWithTag(const std::string& tag, int& outx, int& outy);
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
