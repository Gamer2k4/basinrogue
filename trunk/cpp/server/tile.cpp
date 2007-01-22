//
// C++ Implementation: tile
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tile.h"

#include <string>
#include <sstream>

Tile::Tile ( const std::string name ) : name ( name ), tile_id( -1 )
{}


Tile::~Tile()
{}

int Tile::GetTileId() const
{
    return tile_id;
}

bool Tile::HasOneFlag(int test_flags) const
{
    return (flags & test_flags) != 0;
}

bool Tile::HasAllFlag(int test_flags) const
{
    return (flags & test_flags) == test_flags;
}

TileLib::~TileLib()
{
    for (unsigned ii=0; ii < tile_list.size(); ++ii)
        delete tile_list[ii];
    tile_list.clear();
}

Tile& TileLib::AddTile(const std::string name, int flags)
{
    Tile* tile = new Tile(name);
    int new_id = tile_list.size();
    tile_list.push_back(tile);
    tile->tile_id = new_id;
    tile->flags = flags;
    return *tile;
}

void TileLib::SendTileLib(NetworkCommandBuffer* buffer) const
{
    for (unsigned ii=0; ii < tile_list.size(); ++ii)
    {
        buffer->SendChar('l');
        buffer->SendInt(tile_list[ii]->GetTileId());
        buffer->SendString(tile_list[ii]->name);
    }
}
