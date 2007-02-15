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

#include <iostream>

#pragma warning(disable:4786)

#include "game_elements.h"


GameWorld::GameWorld() : elem_array ( 0 ), back_buffer_elem_array ( 0 )
{}

GameWorld::~GameWorld()
{
	delete elem_array;
	delete back_buffer_elem_array;
}

void GameWorld::Resize ( int _sizex, int _sizey )
{
	sizex = _sizex;
	sizey = _sizey;
	delete elem_array;
	delete back_buffer_elem_array;
	elem_array = new std::vector<std::vector<TileIdType> > ( sizex * sizey );
	back_buffer_elem_array = new std::vector<std::vector<TileIdType> > ( sizex * sizey );
}

const std::vector<TileIdType>& GameWorld::GetTileList ( int x, int y ) const
{
	if ( x >= sizex || x < 0 || y >= sizey || y < 0 )
	{
		std::cout << "Tile out of range :" << x << ", " << y << "\n";
	}
	return elem_array->at ( x + y * sizex );
}

void GameWorld::ClearAll()
{
	for ( unsigned ii=0; ii < elem_array->size(); ++ii )
		back_buffer_elem_array->at ( ii ).clear();
}

void GameWorld::ClearTile ( int posx, int posy )
{
	if ( posx >= sizex || posx < 0 || posy >= sizey || posy < 0 )
	{
		std::cout << "Tile out of range :" << posx << ", " << posy << "\n";
	}
	back_buffer_elem_array->at ( posx + posy * sizex ).clear();
}

void GameWorld::AddTile ( int posx, int posy, TileIdType id )
{
	if ( posx >= sizex || posx < 0 || posy >= sizey || posy < 0 )
	{
		std::cout << "Tile out of range :" << posx << ", " << posy << "\n";
	}
	back_buffer_elem_array->at ( posx + posy * sizex ).push_back ( id );
};

void GameWorld::SwapBuffers()
{
	*elem_array = *back_buffer_elem_array;
}

//-------------------------------------------------------------------------------------------------

GameView::GameView ( const GameWorld& world, const TileLib& tile_lib, SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy ) :
		dest_surface ( dest_surface ),
		world ( world ),
		tile_lib ( tile_lib ),
		sizex ( sizex ),
		sizey ( sizey ),
		originx ( originx ),
		originy ( originy ),
		viewx ( -1 ),
		viewy ( -1 ),
		ready_to_go( 0 )
{
	dest.x = originx;
	dest.y = originy;
	dest.w = tile_lib.sizex * sizex;
	dest.h = tile_lib.sizey * sizey;
}

void GameView::SetReadyToGo()
{
	ready_to_go = 1;
}

int GameView::CheckReadyToGo()
{
	return ready_to_go;
}

void GameView::DrawTile ( int posx, int posy, int scrolled_posx, int scrolled_posy )
{
	SDL_Rect this_tile;
	this_tile.x = originx + scrolled_posx * tile_lib.sizex;
	this_tile.y = originy + scrolled_posy * tile_lib.sizey;
	this_tile.w = tile_lib.sizex;
	this_tile.h = tile_lib.sizey;
	const std::vector<TileIdType>& tile_list = world.GetTileList ( posx, posy );
	if ( tile_list.empty() )
	{
		SDL_FillRect ( dest_surface,&this_tile,0 );
	}
	else
	{

		//TODO Blit without alpha

		TileIdType id;
		SDL_Surface* surface;

		id = tile_list[0];
		surface = tile_lib.GetTileById ( id );
		SDL_BlitSurface ( surface, NULL, dest_surface, &this_tile );
		for ( unsigned ii = 1; ii < tile_list.size(); ++ii )
		{
			id = tile_list[ii];
			surface = tile_lib.GetTileById ( id );
			SDL_BlitSurface ( surface, NULL, dest_surface, &this_tile );
		}
	}
}

void GameView::DrawView()
{
	SDL_FillRect ( dest_surface, &dest, 0 );
	for ( int scrolled_jj = 0; scrolled_jj < sizey; scrolled_jj++ )
	{
		int jj = scrolled_jj + viewy - ( ( sizey-1 ) /2 );
		for ( int scrolled_ii = 0; scrolled_ii < sizex; scrolled_ii++ )
		{
			int ii = scrolled_ii + viewx - ( ( sizex-1 ) /2 );
			if ( ii>=0 && ii<world.sizex && jj>=0 && jj<world.sizey )
			{
				DrawTile ( ii, jj, scrolled_ii, scrolled_jj );
			}
		}
	}
	SDL_UpdateRect ( dest_surface, dest.x, dest.y, dest.w, dest.h );
}

void GameView::SetCharacterPos ( int posx, int posy )
{
	// handles scrolling, a bit messy currently
	if ( viewx < 0 )
	{
		viewx = ( posx < ( sizex+1 ) /2 ) ? ( ( sizex-1 ) /2 ) : ( ( posx >= world.sizex - ( sizex+1 ) /2 ) ? world.sizex - ( sizex+1 ) /2 : sizex );
		viewy = ( posy < ( sizey+1 ) /2 ) ? ( ( sizey-1 ) /2 ) : ( ( posy >= world.sizey - ( sizey+1 ) /2 ) ? world.sizey - ( sizey+1 ) /2 : sizey );
	}
	else
	{
		if ( posx > viewx + 5 || posx < viewx - 5 )
		{
			if ( posx >= ( sizex-1 ) /2 && posx < world.sizex- ( sizex-1 ) /2 )
			{
				viewx = posx;
			}
			else if ( posx < ( sizex-1 ) /2 )
			{
				viewx = ( sizex-1 ) /2;
			}
			else if ( posx >= ( world.sizex- ( sizex-1 ) /2 ) )
			{
				viewx = world.sizex - ( sizex+1 ) /2;
			}
		}
		if ( posy > viewy + 5 || posy < viewy - 5 )
		{
			if ( posy >= ( sizey-1 ) /2 && posy < world.sizey- ( sizey-1 ) /2 )
			{
				viewy = posy;
			}
			else if ( posy < ( sizey-1 ) /2 )
			{
				viewy = ( sizey-1 ) /2;
			}
			else if ( posy >= ( world.sizey- ( sizey-1 ) /2 ) )
			{
				viewy = world.sizey - ( sizey+1 ) /2;
			}
		}
	}
}
