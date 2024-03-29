//
// C++ Interface: server_connection
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <string>
#include "game_elements.h"
#include "tile_lib.h"
#include "sound_lib.h"
#include "networkcommandbuffer.h"
#include "message_area.h"

enum ClientCommand
{
    move_se,
    move_s,
    move_sw,
    move_ne,
    move_n,
    move_nw,
    move_e,
    move_w
};

class ServerConnection
{

	private:
		GameWorld& world;
		GameView& view;
		TileLib& tile_lib;
		SoundLib& sound_lib;
		MessageArea& message_area;
		NetworkCommandBuffer* server_socket;
	public:
		ServerConnection ( GameWorld& world, GameView& view, TileLib& tile_lib, SoundLib& sound_lib, MessageArea& message_area );
		~ServerConnection();

		void Update();
		void Connect ( const std::string server_URL, int server_port );
		void SendCommand ( ClientCommand command );
};

#endif

