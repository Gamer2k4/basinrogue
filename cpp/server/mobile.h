//
// C++ Interface: mobile
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MOBILE_H
#define MOBILE_H

#include <vector>

/**
    @author Cavalaria Christophe <chris.cavalaria@free.fr>
*/

class Level;

class Mobile
{
    private:
        Level* level;
    public:
        int posx;  // Note : readonly for outside
        int posy;

        Mobile();
        void SetLevel(Level& newlevel, int newposx, int newposy);
        void Translate(int decx, int decy);
};

#endif
