//
// C++ Implementation: widget
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "widget.h"

Widget::Widget( SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy ) :
	dest_surface ( dest_surface ),
	sizex ( sizex ),
	sizey ( sizey ),
	originx ( originx ),
	originy ( originy ),
	focus( false )
{}


Widget::~Widget()
{}


bool Widget::OnKeyDown(SDL_keysym& keysym)
{
	return false;
}


bool Widget::getFocus() const
{
	return focus;
}


void Widget::setFocus ( bool theValue )
{
	focus = theValue;
}
