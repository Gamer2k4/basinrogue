#include <iostream>
#include <sstream>
#include <list>
#include <algorithm>
#include <ctime>

#pragma warning(disable:4786)

#include "tile.h"
#include "sound.h"
#include "level.h"
#include "player.h"
#include "networkcommandbuffer.h"
#include "world.h"
#include "dungeon.h"
#include "levelgenerator.h"

#include "SDL.h"
#include "SDL_main.h"
#include "SDL_net.h"

bool IsPlayerConnected ( Player* player )
{
	bool must_remove = player->GetIsDisconnected();
	return !must_remove;
}

int main ( int argc, char* argv[] )
{
	std::cout << "Server\n";

	srand((unsigned)time(0));

	std::list<Player*>::iterator iter;

	TileLib tile_lib;
	tile_lib.AddTile ( "town_ground", 23, 13, 0 );
	tile_lib.AddTile ( "town_wall", 25, 0, FLAG_BLOCKS_MOVEMENT );
	tile_lib.AddTile ( "town_stairs_down", 25, 29, 0 );
	tile_lib.AddTile ( "town_stairs_down", 25, 28, 0 );
	tile_lib.AddTile ( "ground", 23, 1, 0 );
	tile_lib.AddTile ( "wall", 22, 0, FLAG_BLOCKS_MOVEMENT );
	tile_lib.AddTile ( "stairs_down", 22, 16, 0 );
	tile_lib.AddTile ( "stairs_up", 22, 15, 0 );
	tile_lib.AddTile ( "player", 3, 2, FLAG_BLOCKS_MOVEMENT );
	tile_lib.AddTile ( "goblin", 21, 9, FLAG_BLOCKS_MOVEMENT );

	SoundLib sound_lib;
	sound_lib.AddSound ( "whoosh", "wind" );

	atexit ( SDL_Quit );

	if ( SDL_Init ( SDL_INIT_NOPARACHUTE ) < 0 )
	{
		std::cerr << "Unable to init SDL: " << SDL_GetError() << "\n";
		exit ( 1 );
	}

	if ( SDLNet_Init() < 0 )
	{
		std::cerr << "Unable to init SDLNet: " << SDLNet_GetError() << "\n";
		exit ( 1 );
	}

	IPaddress ip;
	TCPsocket listen_socket;

	if ( SDLNet_ResolveHost ( &ip, 0, 1664 ) == -1 )
	{
		std::cerr << "SDLNet_ResolveHost:" << SDLNet_GetError() << "\n";
		exit ( 1 );
	}

	listen_socket = SDLNet_TCP_Open ( &ip );
	if ( !listen_socket )
	{
		std::cerr << "SDLNet_TCP_Open:" << SDLNet_GetError() << "\n";
		exit ( 2 );
	}

	SDLNet_SocketSet main_set = SDLNet_AllocSocketSet ( 32 ); //TODO handle max players here
	if ( !main_set )
	{
		std::cerr << "Error creating SocketSet: " << SDLNet_GetError() << "\n";
		exit ( 1 );
	}

	SDLNet_TCP_AddSocket ( main_set, listen_socket );
	std::list<Player*> player_list;

	TownLevelGenerator town_generator ( tile_lib ) ;
	Level level_for_town = town_generator.GenerateLevel ( 0 );
	World world;
	TownLevel town_level ( "town", "You are back in town.", world, level_for_town );
	InstanceLevelGenerator generator ( tile_lib );
	MultilevelDungeon sewer( "sewer", "You cautiously enter the sewer...", world, generator);
	sewer.setLevelmax(10);

	world.SetStartingDungeon ( "town" );

	//Uint32 last_wind_whoosh = SDL_GetTicks(); // make a wind noise every 20s from now on
	//// I just do not trust SDL_GetTicks to return the correct value :)
	//// What's better? A.

	int numready;

	std::cout << "Server ready\n";

	for ( ;; )
	{
		//std::cout << "Waiting for network activity\n";

		// -1 => a lot of time, but not infinite
		numready = SDLNet_CheckSockets ( main_set, ( Uint32 )-1 );
		if ( numready==-1 )
		{
			std::cerr << "SDLNet_CheckSockets: " << SDLNet_GetError() << "\n";
			perror ( "SDLNet_CheckSockets" );
			exit ( 2 );
		}
		if ( numready > 0 )
		{
			//std::cout << "Player think loop\n";
			for ( iter = player_list.begin(); iter != player_list.end(); ++iter )
				( *iter )->Think();

			//std::cout << "Removing disconnected players\n";
			std::list<Player*>::iterator middle = std::partition (
			                                          player_list.begin(),
			                                          player_list.end(),
			                                          IsPlayerConnected );
			for ( iter = middle ; iter != player_list.end(); ++iter )
			{
				Player* player = *iter;
				if ( player->GetIsDisconnected() )
				{
					std::cout << "Removing player\n";
					SDLNet_TCP_DelSocket ( main_set, player->socket );
					delete player;
				}
			}
			player_list.erase ( middle, player_list.end() );

			//std::cout << "Real-time based events\n";
			//if ( SDL_GetTicks() - last_wind_whoosh > 20000 )
			//{
			//	// 20 s have elapsed - make wind noise
			//	last_wind_whoosh = SDL_GetTicks();
			//	for ( iter = player_list.begin(); iter != player_list.end(); ++iter )
			//		( *iter )->MakeSound ( wind_sound, 0.5 );
			//}

			//std::cout << "Checking for new players\n";
			if ( SDLNet_SocketReady ( listen_socket ) )
			{
				TCPsocket client_socket = SDLNet_TCP_Accept ( listen_socket );
				if ( !client_socket )
				{
					std::cerr << "SDLNet_TCP_Accept: " << SDLNet_GetError() << "\n";
				}
				else
				{
					SDLNet_TCP_AddSocket ( main_set, client_socket );
					std::cout << "New player connected\n";
					Mobile* player_mobile = new PlayerMonster();
					player_mobile->SetAppearance ( tile_lib.GetTileByName ( "player" ) );
					world.AddPlayer ( player_mobile );
					Player* player = new Player ( client_socket, player_mobile );
					tile_lib.SendTileLib ( player->command_buffer );
					sound_lib.SendSoundLib ( player->command_buffer );
					player->SendLevelInfo();
					player->SendReadyToGo();
					player->SendMessage ( "Welcome to the demo. Try walking around a bit and see what happens." );
					player->SendMessage ( "This is really only to test the messaging." );
					player->SendMessage ( "So's this." );
					player_list.push_back ( player );
				}
			}
			//std::cout << "Sending player information\n";
			for ( iter = player_list.begin(); iter != player_list.end(); ++iter )
				( *iter )->SendLevelInfo();
		}
	}
}
