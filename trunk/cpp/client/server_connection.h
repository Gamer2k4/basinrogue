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

class ServerConnection
{
    private:
        GameWorld& world;
        TileLib& tile_lib;
        int server_socket;
    public:
        void Update();
        ServerConnection(GameWorld& world, TileLib& tile_lib);
        void Connect(const std::string server_URL, int server_port);
};

#endif

