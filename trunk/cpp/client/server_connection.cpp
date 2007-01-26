//
// C++ Implementation: server_connection
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "server_connection.h"

#include <string.h>
#include "SDL_net.h"

#include <iostream>

ServerConnection::ServerConnection(GameWorld& world, TileLib& tile_lib, SoundLib& sound_lib) : world(world), tile_lib(tile_lib), sound_lib(sound_lib), server_socket(0)
{
}

ServerConnection::~ServerConnection()
{
    if (server_socket)
        delete server_socket;
    server_socket = 0;
}

void ServerConnection::Connect(const std::string server_URL, int server_port)
{
    std::cout << "Initiating server connection\n";
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, server_URL.c_str(), server_port) == -1)
        RaiseSocketError("SDLNet_ResolveHost");
    std::cout << "Server name resolved\n";
    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if (!socket)
        RaiseSocketError("SDLNet_TCP_Open");
    server_socket = new NetworkCommandBuffer(socket);
    std::cout << "Connection succeded\n";
}

void ServerConnection::Update()
{
    server_socket->Think();
    while (server_socket->HasCommands())
    {
        char command = server_socket->PeekReadChar();
        switch (command)
        {
            case 'l':
                if (server_socket->GetNbCommands() >= 4)
                {
                    server_socket->ReadChar();
                    int id = server_socket->ReadInt();
                    int row_of_bmp = server_socket->ReadInt();
                    int col_of_bmp = server_socket->ReadInt();
                    tile_lib.AddTile(id, row_of_bmp, col_of_bmp);
                    break;
                }
                return;
            case 's':
                if (server_socket->GetNbCommands() >= 3)
                {
                    server_socket->ReadChar();
                    int id = server_socket->ReadInt();
                    std::string filename_prefix = server_socket->ReadString();
                    sound_lib.AddSound(id, filename_prefix);
                    break;
                }
                return;
            case 't':
                if (server_socket->GetNbCommands() >= 4)
                {
                    server_socket->ReadChar();
                    int x = server_socket->ReadInt();
                    int y = server_socket->ReadInt();
                    int id = server_socket->ReadInt();
                    world.AddTile(x, y, id);
                    break;
                }
                return;
            case 'c':
                if (server_socket->GetNbCommands() >= 3)
                {
                    server_socket->ReadChar();
                    int x = server_socket->ReadInt();
                    int y = server_socket->ReadInt();
                    world.ClearTile(x, y);
                    break;
                }
                return;
            case 'p':
                if (server_socket->GetNbCommands() >= 3)
                {
                    server_socket->ReadChar();
                    int id = server_socket->ReadInt();
                    double volume = server_socket->ReadDouble();
                    sound_lib.GetSoundById(id)->Play(volume);
                    break;
                }
                return;
            case 'r':
                server_socket->ReadChar();
                world.ClearAll();
                break;
        }
    }
}

void ServerConnection::SendCommand(ClientCommand command)
{
    char k;
    switch (command)
    {
        case move_sw:
            k = '1';
            break;
        case move_s:
            k = '2';
            break;
        case move_se:
            k = '3';
            break;
        case move_w:
            k = '4';
            break;
        case move_e:
            k = '6';
            break;
        case move_nw:
            k = '7';
            break;
        case move_n:
            k = '8';
            break;
        case move_ne:
            k = '9';
            break;
        default:
            return;
    }
    server_socket->SendChar(k);
}
