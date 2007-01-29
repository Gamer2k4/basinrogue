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
#include "SDL_image.h"

TileLib::TileLib ( int sizex, int sizey ) : sizex ( sizex ), sizey ( sizey )
{
	full_bmp = IMG_Load ( "client/data/32x32.bmp" );
	if ( !full_bmp )
		throw TileLoadError ( IMG_GetError() );
}

TileLib::~TileLib()
{
	for ( std::map<TileIdType, TileDesc>::iterator cur = tile_list.begin(); cur != tile_list.end(); ++cur )
	{
		SDL_FreeSurface ( cur->second.image );
	}
	tile_list.clear();
	delete full_bmp;
}

SDL_Surface* TileLib::GetTileById ( const TileIdType id ) const
{
	const TileDesc& tile_desc = tile_list.find ( id )->second;
	return const_cast<SDL_Surface*> ( tile_desc.image );
}

void TileLib::AddTile ( const TileIdType id, const int row_of_bmp, const int col_of_bmp )
{
	/* rows and columns count from 0 */
	SDL_Surface* new_image = SDL_CreateRGBSurface ( 0,sizex,sizey,32,0,0,0,0 );
	SDL_SetColorKey ( new_image, SDL_SRCCOLORKEY, 0 );
	SDL_Rect source_selector;
	source_selector.x = col_of_bmp*sizex;
	source_selector.y = row_of_bmp*sizey;
	source_selector.w = sizex;
	source_selector.h = sizey;
	SDL_BlitSurface ( full_bmp, &source_selector, new_image, NULL );
	TileDesc tile;
	tile.image = new_image;
	tile_list[id] = tile;
}
