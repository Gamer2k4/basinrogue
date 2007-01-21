//
// C++ Interface: player
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"
#include "SDL_net.h"
#include "networkcommandbuffer.h"
#include "mobile.h"

/**
    @author Cavalaria Christophe <chris.cavalaria@free.fr>
*/
class Player
{
    public:
        NetworkCommandBuffer* command_buffer;
        TCPsocket socket;
        Mobile* mobile;
        LevelViewPort viewport;
    private:
        bool is_disconnected;
    public:
        Player(TCPsocket socket, Mobile* mobile);
        ~Player();

        bool GetIsDisconnected();
        void Think();

        void SendLevelInfo();
};

#endif
