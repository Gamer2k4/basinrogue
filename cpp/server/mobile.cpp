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

Mobile::Mobile() : posx(0), posy(0), level(0)
{
}

void Mobile::SetLevel(Level& newlevel, int newposx, int newposy)
{
    if (level)
        level->BeforeMoveEvent(*this);
    posx = newposx;
    posy = newposy;
    level = &newlevel;
    if (level)
        level->AfterMoveEvent(*this);
}

void Mobile::Translate(int decx, int decy)
{
    if (level)
        level->BeforeMoveEvent(*this);
    posx += decx;
    posy += decy;
    if (level)
        level->AfterMoveEvent(*this);
}
