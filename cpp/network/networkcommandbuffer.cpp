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

NetworkCommandBuffer::NetworkCommandBuffer ( TCPsocket socket ) :
		socket ( socket ),
		is_connected ( true ),
		current_command_size( 0 ),
		size_buffer_used( 0 )
{}


NetworkCommandBuffer::~NetworkCommandBuffer()
{
	SDLNet_TCP_Close ( socket );
}


bool NetworkCommandBuffer::GetIsConnected()
{
	return is_connected;
}

int NetworkCommandBuffer::PeekReadInt()
{
	if ( buffer.empty() )
		throw ReadSocketError();
	std::string data = buffer.front();
	return atoi ( data.c_str() );
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
	int nb_elements = buffer.size();
	if (nb_elements)
	{
		int command_size = PeekReadInt();
		return command_size < nb_elements;
	}
	return false;
}

void NetworkCommandBuffer::SendString ( const std::string message )
{
	char buffer[4];
	SDLNet_Write32(message.size(), buffer);
	SDLNet_TCP_Send ( socket, buffer, 4 );
	SDLNet_TCP_Send ( socket, const_cast<char*> ( message.data() ), message.size() );
}

void NetworkCommandBuffer::Command::SendString ( const std::string message )
{
	outbound_command_buffer.push_back( message );
}

void NetworkCommandBuffer::Command::SendChar ( char message )
{
	SendString( std::string( 1, message ) );
}

void NetworkCommandBuffer::Command::SendInt ( int message )
{
	std::stringstream buffer;
	buffer << message;
	SendString( buffer.str() );
}

void NetworkCommandBuffer::Command::SendDouble ( double message )
{
	std::stringstream buffer;
	buffer << message;
	SendString( buffer.str() );
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
			if (current_command_size > 0)
				AddCharToBuffer ( msg );
			else
				AddCharToCommandSize( msg );
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
	current_command+=chr;
	if (current_command.size() == current_command_size)
	{
		buffer.push ( current_command );
		if ( debug_all_message )
			std::cout << "New message received: " << current_command << "\n";
		current_command = "";
		current_command_size = 0;
	}
}

void NetworkCommandBuffer::AddCharToCommandSize ( char chr )
{
	size_buffer[size_buffer_used] = chr;
	++size_buffer_used;
	if (size_buffer_used == 4)
	{
		current_command_size = SDLNet_Read32(size_buffer);
		size_buffer_used = 0;
	}
}

void NetworkCommandBuffer::DumpBuffer()
{}

NetworkCommandBuffer::Command::Command(NetworkCommandBuffer& buffer) : buffer( buffer )
{}

NetworkCommandBuffer::Command::~Command()
{
	FlushCommand();
}

void NetworkCommandBuffer::Command::FlushCommand()
{
	int nb_elements = outbound_command_buffer.size();

	if (nb_elements)
	{
		std::stringstream nb_elements_as_string;
		nb_elements_as_string << nb_elements;
		buffer.SendString( nb_elements_as_string.str() );

		for (int ii = 0; ii < nb_elements; ++ii)
			buffer.SendString( outbound_command_buffer[ii] );
		outbound_command_buffer.clear();
	}
}

void RaiseSocketError ( const char* message )
{
	std::cout << message << ": " << SDLNet_GetError() << "\n";
	perror ( "Socket error" );
	throw ReadSocketError();
}
