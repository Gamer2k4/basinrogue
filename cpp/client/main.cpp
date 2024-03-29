#include <iostream>
#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_net.h"

#pragma warning(disable:4786)

#include "game_elements.h"
#include "tile_lib.h"
#include "sound_lib.h"
#include "server_connection.h"
#include "message_area.h"
#include "text_input.h"

bool must_quit = false;

void send_command ( ServerConnection& connection, SDLKey key )
{
	if ( key == SDLK_KP1 )
		connection.SendCommand ( move_sw );
	else if ( key == SDLK_KP2 )
		connection.SendCommand ( move_s );
	else if ( key == SDLK_KP3 )
		connection.SendCommand ( move_se );
	else if ( key == SDLK_KP4 )
		connection.SendCommand ( move_w );
	else if ( key == SDLK_KP6 )
		connection.SendCommand ( move_e );
	else if ( key == SDLK_KP7 )
		connection.SendCommand ( move_nw );
	else if ( key == SDLK_KP8 )
		connection.SendCommand ( move_n );
	else if ( key == SDLK_KP9 )
		connection.SendCommand ( move_ne );
}

void handle_input ( ServerConnection& connection, MessageArea& message_area, TextInput& input )
{
	if (message_area.GetBlocked())
	{
		int not_unblocked_yet = 1;
		SDL_Event event;
		while ( SDL_PollEvent ( &event ) && not_unblocked_yet)
		{
			switch ( event.type )
			{
				case SDL_KEYDOWN:
					not_unblocked_yet = false;
					message_area.SetUnblocked();
					break;

				case SDL_QUIT:
					must_quit = true;
					not_unblocked_yet = false;
					break;
			}
		}
	}
	else
	{
		SDL_Event event;
		while ( SDL_PollEvent ( &event ) )
		{
			switch ( event.type )
			{
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							must_quit = true;
							break;
						case SDLK_RETURN:
						case SDLK_KP_ENTER:
							if (input.getFocus())
								std::cout << "Player said : " << input.getValue().c_str() << std::endl;
							input.setValue( std::wstring() );
							input.setFocus(!input.getFocus());
							break;
						default:
							if (input.getFocus())
								input.OnKeyDown(event.key.keysym);
							else
							{
								send_command ( connection, event.key.keysym.sym );
								message_area.InputReceived();
							}
					}
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
}

const int levelsizex = 25; // for testing, increase later?
const int levelsizey = 21;

const int screenwidth = 1000;
const int screenheight = 672;

int main ( int argc, char* argv[] )
{
	std::cout << "Client\n";

	atexit ( SDL_Quit );

	if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_AUDIO ) < 0 )
	{
		std::cerr << "Unable to init SDL: " << SDL_GetError() << "\n";
		exit ( 1 );
	}

	if ( SDLNet_Init() < 0 )
	{
		std::cerr << "Unable to init SDLNet: " << SDLNet_GetError() << "\n";
		exit ( 1 );
	}

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if ( Mix_OpenAudio ( audio_rate, audio_format, audio_channels, audio_buffers ) < 0 )
	{
		std::cerr << "Unable to initialize audio: \n" << Mix_GetError() << '\n';
		exit ( 1 );
	}

	if ( TTF_Init() < 0 )
	{
		std::cerr << "Unable to init TTF: " << TTF_GetError() << "\n";
		exit ( 1 );
	}
	atexit(TTF_Quit);

	SDL_Surface* screen = SDL_SetVideoMode ( screenwidth, screenheight, 0, SDL_SWSURFACE | SDL_DOUBLEBUF );

	if ( !screen )
	{
		//fprintf( stderr, "Unable to set up video: %s\n", SDL_GetError() );
		exit ( 1 );
	}

	SDL_EnableKeyRepeat ( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL );
	SDL_EnableUNICODE( 1 );

	GameWorld world;
	TileLib main_view_tile_lib ( 32, 32 );
	SoundLib sound_lib;
	GameView main_view ( world, main_view_tile_lib, screen, levelsizex, levelsizey, 0, 0 );
	MessageArea message_area ( screen, screenwidth-levelsizex*32-20, 180, levelsizex*32+10, screenheight-190 );
	message_area.Draw();
	TextInput text_input ( screen, screenwidth-levelsizex*32-20, 20, levelsizex*32+10, 0 );
	// text_input.setValue( std::wstring(L"Hello world") );
	text_input.Draw();

	ServerConnection connection ( world, main_view, main_view_tile_lib, sound_lib, message_area );
	if ( argc > 1 )
		connection.Connect ( argv[1], 1664 );
	else
		connection.Connect ( "localhost", 1664 );

	while (main_view.CheckReadyToGo()==0) connection.Update();
	main_view.DrawView();

	while ( !must_quit )
	{
		// Handle mouse and keyboard input
		main_view.DrawView();
		message_area.Draw();
		text_input.Draw();
		connection.Update();
		handle_input ( connection, message_area, text_input );
	}

	Mix_CloseAudio();

	return 0;
}
