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


Player::Player ( TCPsocket socket, Mobile* mobile ) :
		socket ( socket ),
		mobile ( mobile ),
		is_disconnected ( false ),
		command_buffer ( socket )
{
	viewport.AttachToLevel ( mobile->GetLevel(), command_buffer );
}

Player::~Player()
{
	delete mobile;
}

/*!
    \fn Player::GetIsDisconnected()
 */
bool Player::GetIsDisconnected()
{
	return is_disconnected;
}

void Player::TryMove(int decx, int decy)
{
	if (mobile->TryMove ( decx,  decy ))
	{
		TileTrigger* trigger = mobile->GetLevel()->GetTile ( mobile->posx, mobile->posy ).trigger;
		mobile->GetLevel()->TimeElapse ( 1.0 / mobile->GetMovementSpeed () );
		if (trigger)
			trigger->OnPlayerStepsOnTile ( *this );
	}
}

void Player::Think()
{
	try
	{
		command_buffer.Think();

		while ( command_buffer.HasCommands() )
		{
			int cmd_size = command_buffer.ReadInt();
			char cmd = command_buffer.ReadChar();
			switch ( cmd )
			{
				case '1':
					TryMove ( -1,  1 );
					break;
				case '2':
					TryMove ( 0,  1 );
					break;
				case '3':
					TryMove ( 1,  1 );
					break;
				case '4':
					TryMove ( -1,  0 );
					break;
				case '6':
					TryMove ( 1,  0 );
					break;
				case '7':
					TryMove ( -1, -1 );
					break;
				case '8':
					TryMove ( 0, -1 );
					break;
				case '9':
					TryMove ( 1, -1 );
					break;
			}
			mobile->GetLevel()->StuffCanHappen();
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

void Player::SendReadyToGo()
{
	NetworkCommandBuffer::Command command(command_buffer);
	command.SendChar( MSG_VIEWISREADY );
}

void Player::MakeSound ( const Sound* s, double volume )
{
	NetworkCommandBuffer::Command command(command_buffer);
	command.SendChar ( MSG_PLAYSOUND );
	command.SendInt ( s->GetSoundId() );
	command.SendDouble ( volume );
}

void Player::SendMessage ( char* message )
{
	NetworkCommandBuffer::Command command(command_buffer);
	command.SendChar ( MSG_SENDMESSAGE );
	command.SendString ( std::string( message ) );

}

void Player::SendMessage ( std::string message )
{
	NetworkCommandBuffer::Command command(command_buffer);
	command.SendChar ( MSG_SENDMESSAGE );
	command.SendString ( std::string( message ) );

}
