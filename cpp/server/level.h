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

const int FLAG_BLOCKS_MOVEMENT = 1 << 0;
const int FLAG_BLOCKS_VIEW = 1 << 1;

class LevelTile
{
    public:
        Tile* tile;
        LevelTile();
};

class LevelViewPort;

class Level
{
    private:
        std::vector<LevelTile> level_table;
        std::set<Mobile*> mobile_list;
        std::set<LevelViewPort*> viewport_list;

        void SetIsDirty(int x, int y);
    public:
        const int sizex;
        const int sizey;

        Level(int sizex, int sizey);
        Tile* GetTile(int posx, int posy);
        void SetTile(int posx, int posy, Tile* tile);

        void BeforeMoveEvent(Mobile& mob);
        void AfterMoveEvent(Mobile& mob);

        void AddMobile(Mobile& mob);
        void RemoveMobile(Mobile& mob);

        void AddViewPort(LevelViewPort* viewport);
        void RemoveViewPort(LevelViewPort* viewport);

        void SendTileInfo(NetworkCommandBuffer* buffer, int x, int y) const;
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

        void AttachToLevel(Level* new_level);
        bool IsDirty(int x, int y);
        void SetIsDirty(int x, int y);
        void MarkAllDirty();
        void MarkAllClean();

        void SendLevelInfo(NetworkCommandBuffer* buffer);
};

#endif
