//
// C++ Implementation: game_elements
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "game_elements.h"

GameWorld::GameWorld(int sizex, int sizey) : elem_array(sizex * sizey), sizex(sizex), sizey(sizey)
{
}

#include <iostream>

const std::vector<TileIdType>& GameWorld::GetTileList(int x, int y) const
{
    if (x >= sizex || x < 0 || y >= sizey || y < 0)
    {
        std::cout << "Tile out of range :" << x << ", " << y << "\n";
    }
    return elem_array.at(x + y * sizex);
}

void GameWorld::ClearAll()
{
    for (unsigned ii=0; ii < elem_array.size(); ++ii)
        elem_array.at(ii).clear();
}

void GameWorld::ClearTile(int posx, int posy)
{
    if (posx >= sizex || posx < 0 || posy >= sizey || posy < 0)
    {
        std::cout << "Tile out of range :" << posx << ", " << posy << "\n";
    }
    elem_array.at(posx + posy * sizex).clear();
    //TODO Add dirty bit for quick refresh
}

void GameWorld::AddTile(int posx, int posy, TileIdType id)
{
    if (posx >= sizex || posx < 0 || posy >= sizey || posy < 0)
    {
        std::cout << "Tile out of range :" << posx << ", " << posy << "\n";
    }
    if (id > 3)
    {
        std::cout << "Invalid tile id: " << id << "\n";
    }
    elem_array.at(posx + posy * sizex).push_back(id);
    //TODO Add dirty bit for quick refresh
};

//-------------------------------------------------------------------------------------------------

GameView::GameView(const GameWorld& world, const TileLib& tile_lib, SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy) : dest_surface(dest_surface), world(world), tile_lib(tile_lib), sizex(sizex), sizey(sizey), originx(originx), originy(originy)
{
}

void GameView::DrawTile(int posx, int posy) const
{
    const std::vector<TileIdType>& tile_list = world.GetTileList(posx, posy);
    if (tile_list.empty())
    {
        //TODO Draw background here
    }
    else
    {
        SDL_Rect dest;
        dest.x = posx*tile_lib.sizex;
        dest.y = posy*tile_lib.sizey;
          //TODO Blit without alpha

        TileIdType id;
        SDL_Surface* surface;

        id = tile_list[0];
        surface = tile_lib.GetTileById(id);
        SDL_BlitSurface(surface, NULL, dest_surface, &dest);
        for (unsigned ii = 1; ii < tile_list.size(); ++ii )
        {
            dest.x = posx*tile_lib.sizex;
            dest.y = posy*tile_lib.sizey;
            id = tile_list[ii];
            surface = tile_lib.GetTileById(id);
            SDL_BlitSurface( surface, NULL, dest_surface, &dest);
        }
    }
}

void GameView::DrawView() const
{
    for (int jj = 0; jj < sizey; ++jj)
        for (int ii = 0; ii < sizex; ++ii)
            DrawTile(ii, jj);
}
