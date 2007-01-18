#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

// For all those who read that code, I DO know that it is horrible and that it should have been split in another file.

typedef int TileIdType;

struct GameElement
{
        std::string description;
        TileIdType tile_id;
};

std::vector<GameElement> tile_array[16][16];

void add_tile(int x, int y, TileIdType tile_id)
{
    GameElement elem;
    elem.tile_id = tile_id;
    tile_array[x][y].push_back(elem);
}

void clean_tiles(int x, int y)
{
    tile_array[x][y].clear();
}

bool must_quit = false;

void handle_input()
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

            //KeyDown( event.key.keysym.sym );
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

SDL_Surface* screen;

SDL_Surface* load_tile(const char* filename)
{
    SDL_Surface* result = IMG_Load(filename);
    if (result)
    {
        std::cout << "Loading tile " << filename << "\n";
        SDL_SetAlpha(result, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
    }
    else
    {
        std::cout << "Error loading tile " << filename << "\n";
    }
    return result;
}

void draw_tile(int x, int y, TileIdType id)
{
    static SDL_Surface *ground = NULL;
    static SDL_Surface *wall = NULL;
    static SDL_Surface *player = NULL;

    if (!ground)
    {
        ground = load_tile("client/data/ground.png");
        wall = load_tile("client/data/wall.png");
        player = load_tile("client/data/player.png");
    }

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = 32;
    dest.h = 32;
    
    switch (id)
    {
    case 0:
        SDL_BlitSurface(ground, NULL, screen, &dest);
        break;
    case 1:
        SDL_BlitSurface(wall, NULL, screen, &dest);
        break;
    case 2:
        SDL_BlitSurface(player, NULL, screen, &dest);
        break;
    }
}

void draw_screen()
{
    int ii, jj;
    for (jj = 0; jj < 16; ++jj)
        for (ii = 0; ii < 16; ++ii)
        {
            std::vector<GameElement>::const_iterator iter, end;
            end = tile_array[ii][jj].end();
            for (iter = tile_array[ii][jj].begin(); iter != end; ++iter)
            {
                draw_tile(ii*32, jj*32, iter->tile_id);
            }
        }
    SDL_Flip(screen);
}

void prepare_screen()
{
    int ii, jj;
    for (jj = 0; jj < 16; ++jj)
        for (ii = 0; ii < 16; ++ii)
        {
            add_tile(ii, jj, (ii+jj)&1);
        }
        add_tile(8, 8, 2);
}

int main()
{
    std::cout << "Client\n";

    atexit( SDL_Quit );
 
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        //fprintf( stderr, "Unable to init SDL: %s\n", SDL_GetError() );
        exit( 1 );
    }

    screen = SDL_SetVideoMode( 512, 512, 0, SDL_SWSURFACE );

    if (!screen)
    {
        //fprintf( stderr, "Unable to set up video: %s\n", SDL_GetError() );
        exit( 1 );
    }

    prepare_screen();

    while ( !must_quit )
    {
        // Handle mouse and keyboard input
        draw_screen();
        handle_input();
    }
}
