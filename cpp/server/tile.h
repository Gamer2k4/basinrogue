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
    public:
        Tile ( const std::string name );
        ~Tile();

        int GetTileId() const;
};

class TileLib
{
    private:
        std::vector<Tile*> tile_list;
    public:
        ~TileLib();

        Tile& AddTile(const std::string name);
        void SendTileLib(int socket) const;
};

#endif
