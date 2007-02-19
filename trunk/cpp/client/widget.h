//
// C++ Interface: widget
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WIDGET_H
#define WIDGET_H

#include <SDL.h>

/**
	@author
*/
class Widget
{
	protected:
		SDL_Surface* dest_surface;
		int sizex;
		int sizey;
		int originx;
		int originy;
		bool focus;
	public:
		Widget( SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy );
		virtual ~Widget();
		virtual bool OnKeyDown(SDL_keysym& keysym);

		virtual void Draw() = 0;

	void setFocus ( bool theValue );


	bool getFocus() const;

};

#endif
