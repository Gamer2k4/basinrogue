//
// C++ Implementation: tile_lib
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "tile_lib.h"
#include <SDL/SDL_image.h>

TileLib::TileLib(int sizex, int sizey) : sizex(sizex), sizey(sizey)
{
}

TileLib::~TileLib()
{
    for (std::map<TileIdType, TileDesc>::iterator cur = tile_list.begin(); cur != tile_list.end(); ++cur)
    {
        SDL_FreeSurface(cur->second.image);
    }
    tile_list.clear();
}

SDL_Surface* TileLib::GetTileById(const TileIdType id) const
{
    const TileDesc& tile_desc = tile_list.find(id)->second;
    return const_cast<SDL_Surface*>(tile_desc.image);
}

void TileLib::AddTile(const TileIdType id, const std::string tile_name)
{
    SDL_Surface* tile_surface = IMG_Load(("client/data/" + tile_name + ".png").c_str());
    if (!tile_surface)
        throw TileLoadError(IMG_GetError());
    TileDesc tile;
    tile.image = tile_surface;
    tile_list[id] = tile;
}
