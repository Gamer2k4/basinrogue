//
// C++ Interface: message_area
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef MESSAGE_AREA_H
#define MESSAGE_AREA_H

#include "widget.h"

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

class MessageArea : public Widget
{
	private:
		SDL_Rect dest;
		TTF_Font *font;
		int font_height;
		SDL_Color message_color;

		std::vector < std::string > messages;
		bool input_received;
		int blocked;
		int current_message;

	public:
		MessageArea ( SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy );
		~MessageArea();

		void Draw();

		void AddMessage ( const std::string& message );
		int GetBlocked() const;
		void SetUnblocked();
		void InputReceived(); // call this when the player has had a chance to read the message, so no need for a --more-- prompt
};

/* SDL_ttf helper functions cribbed off the Web follow */

std::vector<std::string> WrapText ( const std::string& message, TTF_Font* textFont, int maxWidth );
void DrawText ( std::string& line, SDL_Surface* surface, TTF_Font* textFont, SDL_Color textColor,
                SDL_Rect& drawingPos );

/* End cribbed code */

#endif
