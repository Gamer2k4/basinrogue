#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "tile.h"
#include "sound.h"
#include "level.h"
#include "player.h"
#include "networkcommandbuffer.h"

#include "SDL.h"
#include "SDL_main.h"
#include "SDL_net.h"

const int levelsizex = 35;
const int levelsizey = 30;

Tile* ground_tile;
Tile* wall_tile;
Tile* player_tile;

Sound* wind_sound;

const char* level_map =
    "###################################"
    "#...................#...###########"
    "#.............####..#...###########"
    "#......#####..#..#......###########"
    "#......#...#.....#..###############"
    "#......#...#..#..#......###########"
    "#......##.##..####................#"
    "#..............#........#########.#"
    "#..............#........#########.#"
    "#..............#........#########.#"
    "################..##.############.#"
    "#.................#.....#########.#"
    "#.................#.....#########.#"
    "#.................#.....#########.#"
    "#.................###############.#"
    "#....#####.....#####....#########.#"
    "#....#...#.....#...#....#########.#"
    "#....#...###.###...#....#########.#"
    "#....#.............#....#########.#"
    "#....####.......####....#########.#"
    "#.......#.......#.................#"
    "#.......#...............###########"
    "#.......#.......#.......###########"
    "#.......#.......#.......###########"
    "##.#########.######################"
    "##.#########.######################"
    "#..#########.######################"
    "#.##########.######################"
    "#............######################"
    "###################################";

Tile* get_tile_id ( char tile_char )
{
	switch ( tile_char )
	{
		case '#':
			return wall_tile;
		case '.':
			return ground_tile;
		default:
			return 0;
	}
}

void set_level_map ( Level& level )
{
	for ( int jj=0; jj < levelsizey; ++jj )
		for ( int ii=0; ii < levelsizex; ++ii )
		{
			level.SetTile ( ii, jj, get_tile_id ( level_map[ii+jj*levelsizex] ) );
		}
}

bool MustRemovePlayer ( Player* player )
{
	bool must_remove = player->GetIsDisconnected();
	if ( must_remove )
	{
		std::cout << "Removing player\n";
		delete player;
	}
	return must_remove;
}

int main ( int argc, char* argv[] )
{
	std::cout << "Server\n";

	unsigned ii;

	TileLib tile_lib;
	ground_tile = &tile_lib.AddTile ( "ground", 23, 13, 0 );
	wall_tile = &tile_lib.AddTile ( "wall", 25, 0, FLAG_BLOCKS_MOVEMENT );
	player_tile = &tile_lib.AddTile ( "player", 3, 2, FLAG_BLOCKS_MOVEMENT );

	SoundLib sound_lib;
	wind_sound = &sound_lib.AddSound ( "whoosh", "wind" );

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
	std::vector<Player*> player_list;
	Level level ( levelsizex, levelsizey );
	set_level_map ( level );

	Uint32 last_wind_whoosh = 0; // make a wind noise every 20s from now on

	int numready;

	for ( ;; )
	{
//         std::cout << "Waiting for network activity\n";
		numready = SDLNet_CheckSockets ( main_set, ( Uint32 )-1 );  // -1 => a lot of time, but not infinite
		if ( numready==-1 )
		{
			std::cerr << "SDLNet_CheckSockets: " << SDLNet_GetError() << "\n";
			perror ( "SDLNet_CheckSockets" );
			exit ( 2 );
		}
		if ( numready > 0 )
		{
//             std::cout << "Player think loop\n";
			for ( ii=0; ii < player_list.size(); ++ii )
			{
				Player* player = player_list[ii];
				player->Think();
			}
//             std::cout << "Removing disconnected players\n";
			for ( ii=0; ii < player_list.size(); ++ii )
			{
				Player* player = player_list[ii];
				if ( player->GetIsDisconnected() )
				{
					SDLNet_TCP_DelSocket ( main_set, player->socket );
				}
			}
			player_list.erase (
			    std::remove_if (
			        player_list.begin(),
			        player_list.end(),
			        MustRemovePlayer
			    ),
			    player_list.end()
			);
//             std::cout << "Real-time based events\n";
			if ( SDL_GetTicks() - last_wind_whoosh > 20000 )
			{
				// 20 s have elapsed - make wind noise
				last_wind_whoosh = SDL_GetTicks();
				for ( ii=0; ii < player_list.size(); ++ii )
				{
					Player* player = player_list[ii];
					player->MakeSound ( wind_sound, 0.5 );
				}
			}

//             std::cout << "Checking for new players\n";
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
					Mobile* player_mobile = new Mobile();
					player_mobile->SetAppearance ( player_tile );
					player_mobile->SetLevel ( level, 1, 1 );
					Player* player = new Player ( client_socket, player_mobile );
					tile_lib.SendTileLib ( player->command_buffer );
					sound_lib.SendSoundLib ( player->command_buffer );
					player->SendLevelInfo();
					player_list.push_back ( player );
				}
			}
//             std::cout << "Sending player information\n";
			for ( ii=0; ii < player_list.size(); ++ii )
				player_list[ii]->SendLevelInfo();
		}
	}
}
