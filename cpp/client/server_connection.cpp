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

#include <string.h>
#include "SDL_net.h"
#include <iostream>

#pragma warning(disable:4786)

#include "server_connection.h"


ServerConnection::ServerConnection ( GameWorld& world, GameView& view, TileLib& tile_lib, SoundLib& sound_lib, MessageArea& message_area ) : world ( world ), view ( view ), tile_lib ( tile_lib ), sound_lib ( sound_lib ), message_area ( message_area ), server_socket ( 0 )
{}

ServerConnection::~ServerConnection()
{
	if ( server_socket )
		delete server_socket;
	server_socket = 0;
}

void ServerConnection::Connect ( const std::string server_URL, int server_port )
{
	std::cout << "Initiating server connection\n";
	IPaddress ip;
	if ( SDLNet_ResolveHost ( &ip, server_URL.c_str(), server_port ) == -1 )
		RaiseSocketError ( "SDLNet_ResolveHost" );
	std::cout << "Server name resolved\n";
	TCPsocket socket = SDLNet_TCP_Open ( &ip );
	if ( !socket )
		RaiseSocketError ( "SDLNet_TCP_Open" );
	server_socket = new NetworkCommandBuffer ( socket );
	std::cout << "Connection succeded\n";
}

void ServerConnection::Update()
{
	server_socket->Think();
	while ( server_socket->HasCommands() )
	{
		server_socket->ReadInt();
		char command = server_socket->ReadChar();
		switch ( command )
		{
			case MSG_SENDTILE:
			{
				int id = server_socket->ReadInt();
				int row_of_bmp = server_socket->ReadInt();
				int col_of_bmp = server_socket->ReadInt();
				tile_lib.AddTile ( id, row_of_bmp, col_of_bmp );
				break;
			}
			case MSG_SENDSOUND:
			{
				int id = server_socket->ReadInt();
				std::string filename_prefix = server_socket->ReadString();
				sound_lib.AddSound ( id, filename_prefix );
				break;
			}
			case MSG_RESIZEWORLD:
			{
				int sizex = server_socket->ReadInt();
				int sizey = server_socket->ReadInt();
				world.Resize ( sizex,sizey );
				break;
			}
			case MSG_ADDTILE:
			{
				int x = server_socket->ReadInt();
				int y = server_socket->ReadInt();
				int id = server_socket->ReadInt();
				world.AddTile ( x, y, id );
				break;
			}
			case MSG_CLEARTILE:
			{
				int x = server_socket->ReadInt();
				int y = server_socket->ReadInt();
				world.ClearTile ( x, y );
				break;
			}
			case MSG_SETCHARPOS:
			{
				int posx = server_socket->ReadInt();
				int posy = server_socket->ReadInt();
				view.SetCharacterPos ( posx, posy );
				break;
			}
			case MSG_PLAYSOUND:
			{
				int id = server_socket->ReadInt();
				double volume = server_socket->ReadDouble();
				sound_lib.GetSoundById ( id )->Play ( volume );
				break;
			}
			case MSG_CLEARLEVEL:
			{
				world.ClearAll();
				break;
			}
			case MSG_VIEWISREADY:
			{
				view.SetReadyToGo();
				break;
			}
			case MSG_SWAPBUFFERS:
			{
				world.SwapBuffers();
				break;
			}
			case MSG_SENDMESSAGE:
			{
				std::string message = server_socket->ReadString();
				message_area.AddMessage ( message );
				break;
			}
			default:
				std::cout << "Unknown command received : " << command << std::endl;
				RaiseSocketError ( "Unknown command recieved" );
		}
	}
}

void ServerConnection::SendCommand ( ClientCommand cmd )
{
	char k;
	switch ( cmd )
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
	NetworkCommandBuffer::Command command(*server_socket);
	command.SendChar( k );
}
