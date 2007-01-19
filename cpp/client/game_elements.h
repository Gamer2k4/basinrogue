//
// C++ Interface: game_elements
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef GAME_ELEMENTS_H
#define GAME_ELEMENTS_H

#include "tile_lib.h"
#include <vector>

class GameWorld
{
    private:
        std::vector<std::vector<TileIdType> > elem_array;
    public:
        int sizex;
        int sizey;

        GameWorld(int sizex, int sizey);
        const std::vector<TileIdType>& GetTileList(int x, int y) const;

        void ClearAll();
        void ClearTile(int posx, int posy);
        void AddTile(int posx, int posy, TileIdType id);
};

class GameView
{
    private:
        SDL_Surface* dest_surface;
        const GameWorld& world;
        const TileLib& tile_lib;
        int sizex;
        int sizey;
        int originx;
        int originy;
        void DrawTile(int posx, int posy) const;
    public:
        GameView(const GameWorld& world, const TileLib& tile_lib, SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy);
        void DrawView() const;
};

#endif
