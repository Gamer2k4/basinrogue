//
// C++ Implementation: networkcommandbuffer
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#pragma warning(disable:4786)

#include "networkcommandbuffer.h"

#include <iostream>
#include <sstream>

const bool debug_all_message = false;

NetworkCommandBuffer::NetworkCommandBuffer ( TCPsocket socket ) : socket ( socket ), is_connected ( true )
{}


NetworkCommandBuffer::~NetworkCommandBuffer()
{
	SDLNet_TCP_Close ( socket );
}


bool NetworkCommandBuffer::GetIsConnected()
{
	return is_connected;
}

int NetworkCommandBuffer::ReadInt()
{
	if ( buffer.empty() )
		throw ReadSocketError();
	std::string data = buffer.front();
	buffer.pop();
	return atoi ( data.c_str() );
}

char NetworkCommandBuffer::PeekReadChar()
{
	if ( buffer.empty() )
		throw ReadSocketError();
	std::string data = buffer.front();
	if ( data.length() != 1 )
		throw ReadSocketError();
	return data[0];
}

char NetworkCommandBuffer::ReadChar()
{
	char result = PeekReadChar();
	buffer.pop();
	return result;
}

std::string NetworkCommandBuffer::ReadString()
{
	if ( buffer.empty() )
		throw ReadSocketError();
	std::string data = buffer.front();
	buffer.pop();
	return data;
}

double NetworkCommandBuffer::ReadDouble()
{
	if ( buffer.empty() )
		throw ReadSocketError();
	std::string data = buffer.front();
	buffer.pop();
	return atof ( data.c_str() );
}

int NetworkCommandBuffer::GetNbCommands()
{
	return buffer.size();
}

bool NetworkCommandBuffer::HasCommands()
{
	return !buffer.empty();
}

void NetworkCommandBuffer::SendString ( const std::string message )
{
	SDLNet_TCP_Send ( socket, const_cast<char*> ( message.data() ), message.size() );
	SDLNet_TCP_Send ( socket, const_cast<char*> ( "\n" ), 1 );
}

void NetworkCommandBuffer::SendChar ( char message )
{
	SDLNet_TCP_Send ( socket, &message, 1 );
	SDLNet_TCP_Send ( socket, const_cast<char*> ( "\n" ), 1 );
}

void NetworkCommandBuffer::SendInt ( int message )
{
	std::stringstream buffer;
	buffer << message << "\n";
	std::string data ( buffer.str() );

	SDLNet_TCP_Send ( socket, const_cast<char*> ( data.data() ), data.size() );
}

void NetworkCommandBuffer::SendDouble ( double message )
{
	std::stringstream buffer;
	buffer << message << "\n";
	std::string data ( buffer.str() );

	SDLNet_TCP_Send ( socket, const_cast<char*> ( data.data() ), data.size() );
}


void NetworkCommandBuffer::Think()
{
	SDLNet_SocketSet set;

	set = SDLNet_AllocSocketSet ( 1 );
	if ( !set )
	{
		RaiseSocketError ( "SDLNet_AllocSocketSet" );
	}
	SocketSetEncapsulator free_on_quit ( set );
	if ( SDLNet_TCP_AddSocket ( set, socket ) < 0 )
		{
			RaiseSocketError ( "SDLNet_TCP_AddSocket" );
		}
	int numready = SDLNet_CheckSockets ( set, 0 );
	for ( ; numready == 1; numready = SDLNet_CheckSockets ( set, 0 ) )
		{
			char msg;
			int nbread = SDLNet_TCP_Recv ( socket, &msg, 1 );
			if ( nbread < 0 )
			{
				RaiseSocketError ( "SDLNet_TCP_Recv: " );
			}
			else if ( nbread == 0 )
			{
				std::cout << "SDLNet_TCP_Recv: Unexpected read of size 0\n";
				throw ReadSocketError();
			}
			if ( debug_all_message )
			{
				if ( msg < 32 )
					std::cout << "Read: 0x" << ( int ) msg << "\n";
				else
					std::cout << "Read: " << msg << "\n";
			}
			AddCharToBuffer ( msg );
		}
	if ( numready < 0 )
	{
		std::cout << "SDLNet_CheckSockets: " << SDLNet_GetError() << "\n";
		perror ( "SDLNet_CheckSockets" );
		throw ReadSocketError();
	}
}

void NetworkCommandBuffer::AddCharToBuffer ( char chr )
{
	if ( chr == '\n' )
	{
		if ( current_command.size() == 0 )
			throw ReadSocketError();
		buffer.push ( current_command );
		if ( debug_all_message )
			std::cout << "New message received: " << current_command << "\n";
		current_command = "";
	}
	else
	{
		current_command+=chr;
	}
}

void NetworkCommandBuffer::DumpBuffer()
{}

void RaiseSocketError ( const char* message )
{
	std::cout << message << ": " << SDLNet_GetError() << "\n";
	perror ( "Socket error" );
	throw ReadSocketError();
}
