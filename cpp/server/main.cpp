#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "tile.h"
#include "level.h"
#include "player.h"

const int levelsizex = 25;
const int levelsizey = 25;

Tile* ground_tile;
Tile* wall_tile;
Tile* player_tile;

const char* level_map =
        "#########################"
        "#...................#...#"
        "#.............####..#...#"
        "#......#####..#..#......#"
        "#......#...#.....#..#####"
        "#......#...#..#..#......#"
        "#......##.##..####......#"
        "#..............#........#"
        "#..............#........#"
        "#..............#........#"
        "################..##.####"
        "#.................#.....#"
        "#.................#.....#"
        "#.................#.....#"
        "#.................#######"
        "#....#####.....#####....#"
        "#....#...#.....#...#....#"
        "#....#...###.###...#....#"
        "#....#.............#....#"
        "#....####.......####....#"
        "#.......#.......#.......#"
        "#.......#...............#"
        "#.......#.......#.......#"
        "#.......#.......#.......#"
        "#########################";

Tile* get_tile_id(char tile_char)
{
    switch (tile_char)
    {
        case '#':
            return wall_tile;
        case '.':
            return ground_tile;
        default:
            return 0;
    }
}

void set_level_map(Level& level)
{
    for (int jj=0; jj < levelsizey; ++jj)
        for (int ii=0; ii < levelsizex; ++ii)
            level.SetTile(ii, jj, get_tile_id(level_map[ii+jj*levelsizey]));
}

void send_player_position(int socket, int x, int y, int player_tile_id)
{
    std::stringstream message;
    message << "t\n";
    message << x << "\n";
    message << y << "\n";
    message << player_tile_id << "\n";
    std::string buff = message.str();
    send(socket, buff.c_str(), buff.length(), 0);
}

void try_move(Player& player, int decx, int decy)
{
    int x, y;
    x = player.posx + decx;
    y = player.posy + decy;
    if ( !player.GetLevel()->GetTile(x, y)->HasOneFlag(FLAG_BLOCKS_MOVEMENT) )
    {
        player.Translate(decx, decy);
    }
}

void read_message(int socket, Player& player)
{
    char command;
    int recv_length;
    int x, y, tile_id;
    fd_set rread;
    timeval to;
    FD_ZERO(&rread);
    FD_SET(socket,&rread);
    memset((char *)&to,0,sizeof(to));
    to.tv_usec=10000;
    if (select(socket+1, &rread, (fd_set *)0, (fd_set *)0, &to) <= 0)
        return;
    recv_length = recv(socket, &command, 1, 0);
    if (recv_length < 1)
    {
        perror("Socket error");
        exit(2);
    }
    std::cout << "Received command " << command << "\n";
    switch (command)
    {
        case '1':
            try_move(player, -1,  1);
            break;
        case '2':
            try_move(player,  0,  1);
            break;
        case '3':
            try_move(player,  1,  1);
            break;
        case '4':
            try_move(player, -1,  0);
            break;
        case '6':
            try_move(player,  1,  0);
            break;
        case '7':
            try_move(player, -1, -1);
            break;
        case '8':
            try_move(player,  0, -1);
            break;
        case '9':
            try_move(player,  1, -1);
            break;
    }

    std::cout << "Received client command : " << command << "\n";
}

int main()
{
    std::cout << "Server\n";
    int listen_socket;
    int client_socket;

    listen_socket = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1664);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(listen_socket, (struct sockaddr *)&addr, sizeof addr);
    listen(listen_socket, 1);

    client_socket = accept(listen_socket, NULL, NULL);
    if (client_socket < 0)
    {
        perror("accept");
        exit(2);
    }

    TileLib tile_lib;
    ground_tile = &tile_lib.AddTile("ground", 0);
    wall_tile = &tile_lib.AddTile("wall", FLAG_BLOCKS_MOVEMENT);
    player_tile = &tile_lib.AddTile("player", FLAG_BLOCKS_MOVEMENT);
    tile_lib.SendTileLib(client_socket);

    Player player;
    Level level(levelsizex, levelsizey);
    player.SetLevel(level, 1, 1);
    set_level_map(level);

    level.SendLevelInfo(client_socket);

    int lastx, lasty;
    lastx = player.posx+1;
    lasty = player.posy;
    for (;;)
    {
        level.SendLevelInfo(client_socket);
        if (player.posx != lastx || player.posy != lasty)
        {
            send_player_position(client_socket, player.posx, player.posy, player_tile->GetTileId());
            lastx = player.posx;
            lasty = player.posy;
        }
        read_message(client_socket, player);
    }
}
