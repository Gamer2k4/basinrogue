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
#include <iostream>

#pragma warning(disable:4786)

#include "player.h"


Player::Player ( TCPsocket socket, Mobile* mobile ) : socket ( socket ), mobile ( mobile ), is_disconnected ( false )
{
	command_buffer = new NetworkCommandBuffer ( socket );
	viewport.AttachToLevel ( mobile->GetLevel(),command_buffer );
}

Player::~Player()
{
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

		while ( command_buffer->GetNbCommands() > 0 )
		{
			char cmd = command_buffer->ReadChar();
			switch ( cmd )
			{
				case '1':
					mobile->TryMove ( -1,  1 );
					break;
				case '2':
					mobile->TryMove ( 0,  1 );
					break;
				case '3':
					mobile->TryMove ( 1,  1 );
					break;
				case '4':
					mobile->TryMove ( -1,  0 );
					break;
				case '6':
					mobile->TryMove ( 1,  0 );
					break;
				case '7':
					mobile->TryMove ( -1, -1 );
					break;
				case '8':
					mobile->TryMove ( 0, -1 );
					break;
				case '9':
					mobile->TryMove ( 1, -1 );
					break;
			}
		}
	}
	catch ( ReadSocketError )
	{
		is_disconnected = true;
		std::cout << "Player disconnected\n";
	}
}

void Player::SendLevelInfo()
{
	viewport.SendLevelInfo ( command_buffer,mobile->posx,mobile->posy );
	viewport.MarkAllClean();
}

void Player::MakeSound ( const Sound* s, double volume )
{
	command_buffer->SendChar ( 'p' );
	command_buffer->SendInt ( s->GetSoundId() );
	command_buffer->SendDouble ( volume );
}
