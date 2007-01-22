//
// C++ Interface: networkcommandbuffer
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NETWORKCOMMANDBUFFER_H
#define NETWORKCOMMANDBUFFER_H

#include <iostream>
#include <string>
#include <queue>
#include "SDL_net.h"

class ReadSocketError
{
    public:
        ReadSocketError()
        {
            std::cout << "In ReadSocketError\n";
        }
};

void RaiseSocketError(const char* message);

class SocketSetEncapsulator
{
    private:
        SDLNet_SocketSet set;
    public:
        SocketSetEncapsulator(SDLNet_SocketSet set) : set(set) {}
        ~SocketSetEncapsulator()
        {
            SDLNet_FreeSocketSet(set);
        }
};

/**
    @author Cavalaria Christophe <chris.cavalaria@free.fr>
*/
class NetworkCommandBuffer
{
    private:
        TCPsocket socket;
        std::queue<std::string> buffer;
        bool is_connected;
        std::string current_command;

        void AddCharToBuffer(char chr);
    public:
        NetworkCommandBuffer(TCPsocket socket);
        ~NetworkCommandBuffer();

        bool GetIsConnected();
        int ReadInt();
        char PeekReadChar();
        char ReadChar();
        std::string ReadString();

        void SendString(const std::string message);
        void SendChar(char message);
        void SendInt(int message);

        bool HasCommands();
        int GetNbCommands();

        void Think();

        void DumpBuffer();
};

#endif