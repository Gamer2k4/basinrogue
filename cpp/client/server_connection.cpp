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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>

ServerConnection::ServerConnection(GameWorld& world, TileLib& tile_lib) : world(world), tile_lib(tile_lib)
{
}

void ServerConnection::Connect(const std::string server_URL, int server_port)
{
    std::cout << "Initiating server connection\n";
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cout << "Error connecting to the server : " << strerror(errno) << "\n";
        exit(-1);
    }
    std::cout << "Socket initialised\n";
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int conn_result = connect(server_socket, (struct sockaddr *)&addr, sizeof addr);
    if (conn_result < 0)
    {
        std::cout << "Error connecting to the server : " << strerror(errno) << "\n";
        exit(-1);
    }
    std::cout << "Connection succeded\n";
    send(server_socket, "Hello\n", 6, 0);
}

int ReadNumber(int socket)
{
    char buffer;
    int result = 0;
    int recv_length;
    for(;;)
    {
        recv_length = recv(socket, &buffer, 1, 0);
        if (recv_length < 0)
            exit(2);
        if (buffer >= '0' && buffer <= '9')
            result = 10*result + (buffer - '0');
        else if (buffer == '\n')
            break;
        else
            exit(3);
    }
    return result;
}

char ReadChar(int socket)
{
    char buffer;
    std::string result;
    int recv_length;
    recv_length = recv(socket, &buffer, 1, 0);
    if (recv_length < 0)
        exit(2);
    return buffer;
}

std::string ReadString(int socket)
{
    char buffer;
    std::string result;
    int recv_length;
    for(;;)
    {
        recv_length = recv(socket, &buffer, 1, 0);
        if (recv_length < 0)
            exit(2);
        if (buffer == '\n')
            break;
        result += buffer;
    }
    return result;
}

void ServerConnection::Update()
{
    char command;
    int recv_length;
    int x, y, tile_id;
    std::string tile_name;
    fd_set rread;
    timeval to;
    for (;;)
    {
        FD_ZERO(&rread);
        FD_SET(server_socket,&rread);
        memset((char *)&to,0,sizeof(to));
        to.tv_usec=100000;
        if (select(server_socket+1, &rread, (fd_set *)0, (fd_set *)0, &to) <= 0)
            return;
        recv_length = recv(server_socket, &command, 1, 0);
        if (recv_length < 0)
        {
            perror("Socket error");
            exit(2);
        }
        if (recv_length < 1)
            return;
        ReadChar(server_socket);
        switch (command)
        {
            case 'l':
                x = ReadNumber(server_socket);
                tile_name = ReadString(server_socket);
                tile_lib.AddTile(x, tile_name);
                break;
            case 't':
                x = ReadNumber(server_socket);
                y = ReadNumber(server_socket);
                tile_id = ReadNumber(server_socket);
                world.AddTile(x, y, tile_id);
                break;
            case 'c':
                x = ReadNumber(server_socket);
                y = ReadNumber(server_socket);
                world.ClearTile(x, y);
            case 'r':
                world.ClearAll();
        }
    }
}
