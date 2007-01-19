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

GameWorld::GameWorld(int sizex, int sizey) : sizex(sizex), sizey(sizey)
{
    elem_array.resize(sizex * sizey);
}

const std::vector<TileIdType>& GameWorld::GetTileList(int x, int y) const
{
    return elem_array[x * sizey+y];
}

void GameWorld::ClearAll()
{
    for (int ii=0; ii < elem_array.size(); ++ii)
        elem_array[ii].clear();
}

void GameWorld::ClearTile(int posx, int posy)
{
    elem_array[posx * sizex + posy].clear();
    //TODO Add dirty bit for quick refresh
}

void GameWorld::AddTile(int posx, int posy, TileIdType id)
{
    elem_array[posx * sizex + posy].push_back(id);
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
        SDL_BlitSurface(tile_lib.GetTileById(tile_list[0]), NULL, dest_surface, &dest);
        for (int ii = 1; ii < tile_list.size(); ++ii )
        {
            dest.x = posx*tile_lib.sizex;
            dest.y = posy*tile_lib.sizey;
            SDL_BlitSurface( tile_lib.GetTileById(tile_list[ii]), NULL, dest_surface, &dest);
        }
    }
}

void GameView::DrawView() const
{
    for (int jj = 0; jj < sizey; ++jj)
        for (int ii = 0; ii < sizex; ++ii)
            DrawTile(ii, jj);
}
