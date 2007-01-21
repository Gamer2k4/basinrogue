//
// C++ Implementation: mobile
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mobile.h"

#include "level.h"

Mobile::Mobile() : level(0), appearance(0), posx(0), posy(0), dirty(true)
{
}

Mobile::~Mobile()
{
    if (level)
    {
        level->BeforeMoveEvent(*this);
        level->RemoveMobile(*this);
    }
}

void Mobile::SetLevel(Level& newlevel, int newposx, int newposy)
{
    if (level)
    {
        level->BeforeMoveEvent(*this);
        level->RemoveMobile(*this);
    }
    posx = newposx;
    posy = newposy;
    level = &newlevel;
    if (level)
    {
        level->AddMobile(*this);
        level->AfterMoveEvent(*this);
    }
}

void Mobile::Translate(int decx, int decy)
{
    if (level)
        level->BeforeMoveEvent(*this);
    posx += decx;
    posy += decy;
    dirty = true;
    if (level)
        level->AfterMoveEvent(*this);
}

void Mobile::TryMove(int decx, int decy)
{
    int x, y;
    x = posx + decx;
    y = posy + decy;
    if ( ! level->GetTile(x, y)->HasOneFlag(FLAG_BLOCKS_MOVEMENT) )
    {
        Translate(decx, decy);
    }
}

Level* Mobile::GetLevel()
{
    return level;
}

void Mobile::SetAppearance(Tile* tile)
{
    appearance = tile;
}

Tile* Mobile::GetAppearance()
{
    return appearance;
}
