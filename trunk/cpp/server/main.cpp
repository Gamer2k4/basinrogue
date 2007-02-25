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

#include "luabinding.h"

Tile* town_ground_tile;
Tile* town_wall_tile;
Tile* town_stairs_down_tile;
Tile* town_stairs_up_tile;
Tile* ground_tile;
Tile* wall_tile;
Tile* stairs_down_tile;
Tile* stairs_up_tile;
Tile* player_tile;
Tile* goblin_tile;

Sound* wind_sound;

const int levelsizex = 35;
const int levelsizey = 30;

const char* level_map =
		"###################################"
		"#...................#...###########"
		"#.......###...####..#...###########"
		"#......#####..#..#......###########"
		"#......##T##.....#..###############"
		"#......##.##..#..#......###########"
		"#.......#.#...####................#"
		"#..............#........#########.#"
		"#..............#........#########.#"
		"#S#............#........#########.#"
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

const int dungeonsizex = 10;
const int dungeonsizey = 10;

const char* dungeon_map =
		"##########"
		"#...>....#"
		"#........#"
		"#........#"
		"#<.......#"
		"#........#"
		"#........#"
		"#......m.#"
		"#........#"
		"##########";

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
	town_ground_tile = &tile_lib.NewTile ( "town_ground", 23, 13, 0 );
	town_wall_tile = &tile_lib.NewTile ( "town_wall", 25, 0, FLAG_BLOCKS_MOVEMENT );
	town_stairs_down_tile = &tile_lib.NewTile ( "town_stairs_down", 25, 29, 0 );
	town_stairs_up_tile = &tile_lib.NewTile ( "town_stairs_down", 25, 28, 0 );
	ground_tile = &tile_lib.NewTile ( "ground", 23, 1, 0 );
	wall_tile = &tile_lib.NewTile ( "wall", 22, 0, FLAG_BLOCKS_MOVEMENT );
	stairs_down_tile = &tile_lib.NewTile ( "stairs_down", 22, 16, 0 );
	stairs_up_tile = &tile_lib.NewTile ( "stairs_down", 22, 15, 0 );
	player_tile = &tile_lib.NewTile ( "player", 3, 2, FLAG_BLOCKS_MOVEMENT );
	goblin_tile = &tile_lib.NewTile ( "goblin", 21, 9, FLAG_BLOCKS_MOVEMENT );

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

	/*TownLevelGenerator town_generator ( tile_lib ) ;
	Level level_for_town = town_generator.GenerateLevel ( 0 );*/

	World world;

	MapLevelGenerator& town_generator = *new MapLevelGenerator(levelsizex, levelsizey, level_map, town_ground_tile );
	town_generator.SetTileConverter('#', town_wall_tile);
	town_generator.SetTileConverter('S', town_stairs_down_tile, ChangeDungeonTeleport, (char*)"sewer");
	town_generator.SetTileConverter('T', town_stairs_up_tile, ChangeDungeonTeleport, (char*)"tower");
	TownLevel& town_level = *new TownLevel( "town", "You are back in town.", world, town_generator );

	MapLevelGenerator& sewer_generator = *new MapLevelGenerator(dungeonsizex, dungeonsizey, dungeon_map, ground_tile );
	sewer_generator.SetTileConverter('#', wall_tile);
	sewer_generator.SetTileConverter('<', stairs_down_tile, GenerateDownStairs);
	sewer_generator.SetTileConverter('>', stairs_up_tile, GenerateUpStairs);
	sewer_generator.SetTileConverter('m', 0, GenerateMonster, goblin_tile);
	MultilevelDungeon& sewer = *new MultilevelDungeon( "sewer", "You cautiously enter the sewer...", world, sewer_generator);

	MapLevelGenerator& tower_generator = *new MapLevelGenerator(dungeonsizex, dungeonsizey, dungeon_map, ground_tile );
	tower_generator.SetTileConverter('#', wall_tile);
	tower_generator.SetTileConverter('<', stairs_up_tile, GenerateDownStairs);
	tower_generator.SetTileConverter('>', stairs_down_tile, GenerateUpStairs);
	tower_generator.SetTileConverter('m', 0, GenerateMonster, goblin_tile);
	MultilevelDungeon& tower = *new MultilevelDungeon( "tower", "You climb up the wizard's tower...", world, tower_generator);
	tower.setLevelmax(5);

	LuaInterpreter li(world, tile_lib);
	li.RegisterEngine();
	li.RunFile("main.lua");

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
					player_mobile->SetAppearance ( player_tile );
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
