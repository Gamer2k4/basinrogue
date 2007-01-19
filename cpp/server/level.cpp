//
// C++ Implementation: level
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "level.h"
#include <sstream>
#include <string>
#include <sys/socket.h>

LevelTile::LevelTile() : dirty(true), tile(0)
{
}

bool LevelTile::MustSendData() const
{
    //return true;
    return dirty && tile;
}

Level::Level(int sizex, int sizey) : sizex(sizex), sizey(sizey)
{
    level_table.resize(sizex * sizey);
}

void Level::SendTileInfo(int socket, int x, int y) const
{
    int index = x + y * sizey;
    if (level_table[index].MustSendData())
    {
        std::stringstream message;
        message << "t\n";
        message << x << "\n";
        message << y << "\n";
        message << level_table[index].tile->GetTileId() << "\n";
        std::string buff = message.str();
        send(socket, buff.c_str(), buff.length(), 0);
        level_table[index].dirty = false;
    }
}

void Level::SendLevelInfo(int socket) const
{
    for (int jj=0; jj < sizey; ++jj)
        for (int ii=0; ii < sizex; ++ii)
        {
            SendTileInfo(socket, ii, jj);
        }
}

Tile* Level::GetTile(int posx, int posy)
{
    int index = posx + posy * sizey;
    return level_table[index].tile;
}

void Level::SetTile(int posx, int posy, Tile* tile)
{
    int index = posx + posy * sizey;
    level_table[index].tile = tile;
    level_table[index].dirty = true;
}

void Level::BeforeMoveEvent(Mobile& mob)
{
    int index = mob.posx + mob.posy * sizey;
    level_table[index].dirty = true;
}

void Level::AfterMoveEvent(Mobile& mob)
{
    int index = mob.posx + mob.posy * sizey;
    level_table[index].dirty = true;
}
