//
// C++ Implementation: player
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "player.h"

#include <iostream>


Player::Player(TCPsocket socket, Mobile* mobile) : socket(socket), mobile(mobile), is_disconnected(false)
{
    command_buffer = new NetworkCommandBuffer(socket);
    viewport.AttachToLevel(mobile->GetLevel());
}

Player::~Player()
{
    std::cout << "Removing player\n";
    delete command_buffer;
    delete mobile;
}

/*!
    \fn Player::GetIsDisconnected()
 */
bool Player::GetIsDisconnected()
{
    return is_disconnected;
}

void Player::Think()
{
    try
    {
        command_buffer->Think();

        while (command_buffer->GetNbCommands() > 0)
        {
            char cmd = command_buffer->ReadChar();
            switch (cmd)
            {
                case '1':
                    mobile->TryMove(-1,  1);
                    break;
                case '2':
                    mobile->TryMove( 0,  1);
                    break;
                case '3':
                    mobile->TryMove( 1,  1);
                    break;
                case '4':
                    mobile->TryMove(-1,  0);
                    break;
                case '6':
                    mobile->TryMove( 1,  0);
                    break;
                case '7':
                    mobile->TryMove(-1, -1);
                    break;
                case '8':
                    mobile->TryMove( 0, -1);
                    break;
                case '9':
                    mobile->TryMove( 1, -1);
                    break;
            }
        }
    }
    catch (ReadSocketError)
    {
        is_disconnected = true;
        std::cout << "Player disconnected\n";
    }
}

void Player::SendLevelInfo()
{
    viewport.SendLevelInfo(command_buffer);
    viewport.MarkAllClean();
}
