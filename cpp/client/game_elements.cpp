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
#include <iostream>

GameWorld::GameWorld() : elem_array(0)
{
}

GameWorld::~GameWorld(){
	delete elem_array;
}

void GameWorld::Resize(int _sizex, int _sizey)
{
	sizex = _sizex;
	sizey = _sizey;
	if (elem_array){
		delete elem_array;
	}
	elem_array = new std::vector<std::vector<TileIdType> >(sizex * sizey);
	ClearAll();
}

const std::vector<TileIdType>& GameWorld::GetTileList(int x, int y) const
{
    if (x >= sizex || x < 0 || y >= sizey || y < 0)
    {
        std::cout << "Tile out of range :" << x << ", " << y << "\n";
    }
    return elem_array->at(x + y * sizex);
}

void GameWorld::ClearAll()
{
    for (unsigned ii=0; ii < elem_array->size(); ++ii)
        elem_array->at(ii).clear();
}

void GameWorld::ClearTile(int posx, int posy)
{
    if (posx >= sizex || posx < 0 || posy >= sizey || posy < 0)
    {
        std::cout << "Tile out of range :" << posx << ", " << posy << "\n";
    }
    elem_array->at(posx + posy * sizex).clear();
    //TODO Add dirty bit for quick refresh (but how will this interact with scrolling? A.)
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
    elem_array->at(posx + posy * sizex).push_back(id);
    //TODO Add dirty bit for quick refresh (but how will this interact with scrolling? A.)
};

//-------------------------------------------------------------------------------------------------

GameView::GameView(const GameWorld& world, const TileLib& tile_lib, SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy) : dest_surface(dest_surface), world(world), tile_lib(tile_lib), sizex(sizex), sizey(sizey), originx(originx), originy(originy)
{
}

void GameView::DrawTile(int posx, int posy, int scrolled_posx, int scrolled_posy) const
{
    SDL_Rect dest;
    dest.x = scrolled_posx*tile_lib.sizex;
    dest.y = scrolled_posy*tile_lib.sizey;

    const std::vector<TileIdType>& tile_list = world.GetTileList(posx, posy);
    if (tile_list.empty())
    {
		SDL_FillRect(dest_surface,&dest,0);
    }
    else
    {

          //TODO Blit without alpha

        TileIdType id;
        SDL_Surface* surface;

        id = tile_list[0];
        surface = tile_lib.GetTileById(id);
        SDL_BlitSurface(surface, NULL, dest_surface, &dest);
        for (unsigned ii = 1; ii < tile_list.size(); ++ii )
        {
            id = tile_list[ii];
            surface = tile_lib.GetTileById(id);
            SDL_BlitSurface( surface, NULL, dest_surface, &dest);
        }
    }
}

void GameView::DrawBlack(int scrolled_posx, int scrolled_posy) const
{
    SDL_Rect dest;
    dest.x = scrolled_posx*tile_lib.sizex;
    dest.y = scrolled_posy*tile_lib.sizey;
	SDL_FillRect(dest_surface,&dest,0);
}

void GameView::DrawView() const
{
    for (int scrolled_jj = 0; scrolled_jj < sizey; ++scrolled_jj){
		int jj = scrolled_jj + originy - ((sizey-1)/2);
       	for (int scrolled_ii = 0; scrolled_ii <= sizex; ++scrolled_ii){
			int ii = scrolled_ii + originx - ((sizex-1)/2);
			if (ii>=0 && ii<world.sizex && jj>=0 && jj<world.sizey){
				DrawTile(ii, jj, scrolled_ii, scrolled_jj);
			} else {
				DrawBlack(scrolled_ii, scrolled_jj);
			}
		}
	}
}

void GameView::SetCharacterPos(int posx, int posy){
	// handles scrolling, a bit messy currently
	if (originx < 0){
		originx = (posx < (sizex+1)/2) ? ((sizex-1)/2) : ((posx >= world.sizex - (sizex+1)/2) ? world.sizex - (sizex+1)/2 : sizex);
		originy = (posy < (sizey+1)/2) ? ((sizey-1)/2) : ((posy >= world.sizey - (sizey+1)/2) ? world.sizey - (sizey+1)/2 : sizey);
	} else {
		if (posx > originx + 5 || posx < originx - 5){
			if (posx >= (sizex-1)/2 && posx < world.sizex-(sizex-1)/2){
				originx = posx;
			} else if (posx < (sizex-1)/2){
				originx = (sizex-1)/2;
			} else if (posx >= (world.sizex-(sizex-1)/2)){
				originx = world.sizex - (sizex+1)/2;
			}
		}
		if (posy > originy + 5 || posy < originy - 5){
			if (posy >= (sizey-1)/2 && posy < world.sizey-(sizey-1)/2){
				originy = posy;
			} else if (posy < (sizey-1)/2){
				originy = (sizey-1)/2;
			} else if (posy >= (world.sizey-(sizey-1)/2)){
				originy = world.sizey - (sizey+1)/2;
			}
		}
	}
}
