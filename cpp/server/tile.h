//
// C++ Interface: tile
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TILE_H
#define TILE_H

#include <string>
#include <vector>

#include "networkcommandbuffer.h"

class TileLib;

/**
    @author Cavalaria Christophe <chris.cavalaria@free.fr>
*/
class Tile
{
    friend class TileLib;
    private:
        const std::string name;
        int tile_id;
        int flags;
    public:
        Tile ( const std::string name );
        ~Tile();

        int GetTileId() const;
        bool HasOneFlag(int test_flags) const;
        bool HasAllFlag(int test_flags) const;
};

class TileLib
{
    private:
        std::vector<Tile*> tile_list;
    public:
        ~TileLib();

        Tile& AddTile(const std::string name, int flags);
        void SendTileLib(NetworkCommandBuffer* buffer) const;
};

#endif
