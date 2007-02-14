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
		std::vector<std::vector<TileIdType> >* elem_array;
		std::vector<std::vector<TileIdType> >* back_buffer_elem_array;
	public:
		int sizex;
		int sizey;

		GameWorld();
		~GameWorld();
		const std::vector<TileIdType>& GetTileList ( int x, int y ) const;

		void Resize ( int _sizex, int _sizey );
		void ClearAll();
		void ClearTile ( int posx, int posy );
		void AddTile ( int posx, int posy, TileIdType id );
		void SwapBuffers();
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
		int viewx;
		int viewy;
		int ready_to_go;
		void DrawTile ( int posx, int posy, int scrolled_posx, int scrolled_posy ) const;
	public:
		GameView ( const GameWorld& world, const TileLib& tile_lib, SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy );
		void SetCharacterPos ( int posx, int posy );
		void DrawView() const;
		void SetReadyToGo();
		int CheckReadyToGo();
};

#endif
