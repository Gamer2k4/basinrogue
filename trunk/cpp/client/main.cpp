#include <iostream>
#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_net.h"

#include "game_elements.h"
#include "tile_lib.h"
#include "sound_lib.h"
#include "server_connection.h"

bool must_quit = false;

void send_command(ServerConnection& connection, SDLKey key)
{
    if (key == SDLK_KP1)
        connection.SendCommand(move_sw);
    else if (key == SDLK_KP2)
        connection.SendCommand(move_s);
    else if (key == SDLK_KP3)
        connection.SendCommand(move_se);
    else if (key == SDLK_KP4)
        connection.SendCommand(move_w);
    else if (key == SDLK_KP6)
        connection.SendCommand(move_e);
    else if (key == SDLK_KP7)
        connection.SendCommand(move_nw);
    else if (key == SDLK_KP8)
        connection.SendCommand(move_n);
    else if (key == SDLK_KP9)
        connection.SendCommand(move_ne);
}

void handle_input(ServerConnection& connection)
{
    SDL_Event event;
    while ( SDL_PollEvent( &event ) )
    {
        switch ( event.type )
        {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                    must_quit = true;
                    break;
            }

            send_command(connection, event.key.keysym.sym);
            break;

        case SDL_KEYUP:
            //KeyUp( event.key.keysym.sym );
            break;

        case SDL_QUIT:
            must_quit = true;
            break;
        }

    }
}

const int levelsizex = 25;
const int levelsizey = 25;

int main(int argc, char* argv[])
{
    std::cout << "Client\n";

    atexit( SDL_Quit );

    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_AUDIO ) < 0 )
    {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << "\n";
        exit( 1 );
    }

    if ( SDLNet_Init() < 0 )
    {
        std::cerr << "Unable to init SDLNet: " << SDLNet_GetError() << "\n";
        exit( 1 );
    }

    int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0)
	{
		std::cerr << "Unable to initialize audio: \n" << Mix_GetError() << '\n';
		exit( 1 );
	}

    GameWorld world(levelsizex, levelsizey);
    TileLib main_view_tile_lib(32, 32);
    SoundLib sound_lib;

    ServerConnection connection(world, main_view_tile_lib, sound_lib);
    if (argc > 1)
        connection.Connect(argv[1], 1664);
    else
        connection.Connect("localhost", 1664);

    SDL_Surface* screen = SDL_SetVideoMode( 800, 800, 0, SDL_SWSURFACE );

    if (!screen)
    {
        //fprintf( stderr, "Unable to set up video: %s\n", SDL_GetError() );
        exit( 1 );
    }

    GameView main_view(world, main_view_tile_lib, screen, levelsizex, levelsizey, 0, 0);
    main_view.DrawView();
    SDL_UpdateRect(screen, 0, 0, 0, 0);

    while ( !must_quit )
    {
        // Handle mouse and keyboard input
        main_view.DrawView();
        SDL_UpdateRect(screen, 0, 0, 0, 0);
        connection.Update();
        handle_input(connection);
    }

    Mix_CloseAudio();

    return 0;
}
