//
// C++ Interface: tile_lib
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef TILE_LIB_H
#define TILE_LIB_H

#include <string>
#include <map>
#include "SDL.h"

typedef unsigned long TileIdType;

class TileDesc
{
    public:
        SDL_Surface* image;
};

class TileLib
{
    private:
        std::map<TileIdType, TileDesc> tile_list;
    public:
        const int sizex;
        const int sizey;
        TileLib(int sizex, int sizey);
        ~TileLib();

        SDL_Surface* GetTileById(const TileIdType id) const;
        void AddTile(const TileIdType id, const std::string tile_name);
};

class TileLoadError
{
    public:
        const char* error;
        TileLoadError(const char* arg) : error(arg) { }
};

#endif
