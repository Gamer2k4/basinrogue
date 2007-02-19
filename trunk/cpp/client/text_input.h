//
// C++ Interface: text_input
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include "widget.h"
#include <string>
#include <SDL_ttf.h>

/**
	@author
*/
class TextInput : public Widget
{
	private:
		TTF_Font *font;
		int font_height;
		SDL_Color message_color;

	protected:
		std::wstring value;

		int cursor_pos;
	public:
		TextInput( SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy );
		~TextInput();

		void Draw();

		void setValue ( const std::wstring& theValue );
		std::wstring getValue() const;

		bool OnKeyDown(SDL_keysym& keysym);
};

#endif
