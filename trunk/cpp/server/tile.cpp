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
#include <string>
#include <sstream>

#pragma warning(disable:4786)

#include "tile.h"

Tile::Tile ( const std::string name ) : name ( name ), tile_id ( -1 )
{}


Tile::~Tile()
{}

int Tile::GetTileId() const
{
	return tile_id;
}

bool Tile::HasOneFlag ( int test_flags ) const
{
	return ( flags & test_flags ) != 0;
}

bool Tile::HasAllFlag ( int test_flags ) const
{
	return ( flags & test_flags ) == test_flags;
}

TileLib::~TileLib()
{
	for ( unsigned ii=0; ii < tile_list.size(); ++ii )
		delete tile_list[ii];
	tile_list.clear();
}

Tile& TileLib::AddTile ( const std::string name, int row_of_bmp, int col_of_bmp, int flags )
{
	Tile* tile = new Tile ( name );
	int new_id = tile_list.size();
	tile_list.push_back ( tile );
	tile->tile_id = new_id;
	tile->flags = flags;
	tile->row_of_bmp = row_of_bmp;
	tile->col_of_bmp = col_of_bmp;
	return *tile;
}

Tile* TileLib::GetTileByName ( const std::string& name )
{
	for ( int i=0; i < tile_list.size() ; i++)
	{
		if ( tile_list[i]->name == name )
		{
			return tile_list[i];
		}
	}
	return 0;
}

void TileLib::SendTileLib ( NetworkCommandBuffer& buffer ) const
{
	for ( unsigned ii=0; ii < tile_list.size(); ++ii )
	{
		NetworkCommandBuffer::Command command(buffer);
		command.SendChar ( MSG_SENDTILE );
		command.SendInt ( tile_list[ii]->GetTileId() );
		command.SendInt ( tile_list[ii]->row_of_bmp );
		command.SendInt ( tile_list[ii]->col_of_bmp );
	}
}
