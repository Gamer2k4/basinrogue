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

#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"

class MessageArea
{
	private:
		SDL_Surface* dest_surface;
		int sizex;
		int sizey;
		int originx;
		int originy;
		SDL_Rect dest;
		TTF_Font *font;
		int font_height;
		SDL_Color message_color;

		std::vector < std::string > messages;
		int input_received;
		int blocked;
		int current_message;

	public:
		MessageArea ( SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy );
		~MessageArea();

		void AddMessage ( std::string& message );
		void Show();
		void Clear();
		int GetBlocked() const;
		void SetUnblocked();
		void InputReceived(); // call this when the player has had a chance to read the message, so no need for a --more-- prompt
};

/* SDL_ttf helper functions cribbed off the Web follow */

enum ETextHAlign
{
   ETextHAlign_Left,
   ETextHAlign_Center,
   ETextHAlign_Right
};

enum ETextVAlign
{
   ETextVAlign_Top,
   ETextVAlign_Center,
   ETextVAlign_Bottom
};

bool CanWrapText(std::string& sText, int iWrapOffset);
int CalcTextWidth(TTF_Font* pTextFont, std::string& sText);
std::string CalcTextLine(std::string& sEntireText, TTF_Font* pTextFont, int iMaxWidth);
std::vector<std::string> WrapText(std::string sEntireText, TTF_Font* pTextFont, int iMaxWidth);
bool DrawText(std::string& sLine, SDL_Surface* pSurface, TTF_Font* pTextFont, SDL_Color vTextColor,
              SDL_Rect& vDrawingRect, ETextHAlign eHAlignment, ETextVAlign eVAlignment);

/* End cribbed code */

#endif
