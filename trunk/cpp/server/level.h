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
#include "tile.h"
#include "mobile.h"

class LevelTile
{
    public:
        mutable bool dirty;
        Tile* tile;
        LevelTile();
        bool MustSendData() const;
};

class Level
{
    private:
        std::vector<LevelTile> level_table;
        std::vector<Mobile*> mobile_list;
        void SendTileInfo(int socket, int x, int y) const;
    public:
        const int sizex;
        const int sizey;

        Level(int sizex, int sizey);
        void SendLevelInfo(int socket) const;
        void SetTile(int posx, int posy, Tile* tile);

        void BeforeMoveEvent(Mobile& mob);
        void AfterMoveEvent(Mobile& mob);
};

#endif
