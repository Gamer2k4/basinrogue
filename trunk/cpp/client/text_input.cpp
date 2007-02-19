//
// C++ Implementation: text_input
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "text_input.h"

#include <iostream>

TextInput::TextInput ( SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy ) : Widget ( dest_surface, sizex, sizey, originx, originy )
{
	font = TTF_OpenFont ( "client/data/Vera.ttf", 12 );
	if ( !font )
	{
		std::cerr << "Error creating TTF_Font: " << TTF_GetError() << "\n";
		exit ( 1 );
	}
	font_height = TTF_FontLineSkip ( font );

	message_color.r = 255;
	message_color.g = 255;
	message_color.b = 0;
	message_color.unused = 0;

	cursor_pos = 0;
	//setValue("Hello world");
}


TextInput::~TextInput()
{}


std::wstring TextInput::getValue() const
{
	return value;
}


void TextInput::setValue ( const std::wstring& theValue )
{
	value = theValue;
	cursor_pos = value.size();
}


bool TextInput::OnKeyDown ( SDL_keysym& keysym )
{
	switch (keysym.sym)
	{
		case SDLK_LEFT:
			cursor_pos = cursor_pos > 0 ? cursor_pos - 1 : 0;
			break;
		case SDLK_RIGHT:
			cursor_pos = cursor_pos < value.size() ? cursor_pos + 1 : value.size();
			break;
		case SDLK_BACKSPACE:
			if (cursor_pos > 0)
			{
				--cursor_pos;
				value.erase(cursor_pos, 1);
			}
			break;
		case SDLK_DELETE:
			value.erase(cursor_pos, 1);
			break;
		case SDLK_CLEAR:
			value.clear();
			cursor_pos = 0;
			break;
		default:
			if ( keysym.unicode && keysym.unicode > 31 )
			{
				wchar_t ch = keysym.unicode;
				value.insert ( cursor_pos, 1, ch );
				++cursor_pos;
			}
	}
}

void TextInput::Draw()
{
	SDL_Rect dest;
	SDL_Rect cursor;
	int x, h;

	dest.x = originx;
	dest.y = originy;
	dest.w = sizex;
	dest.h = sizey;
	SDL_FillRect ( dest_surface, &dest, 0 );

	int size = value.size() +1;
	if (size > 1)
	{
		Uint16 data[size];
		for ( int ii = 0; ii < size; ++ii )
			data[ii] = ( Uint16 ) value.c_str() [ii];
		SDL_Surface* textSurface = TTF_RenderUNICODE_Blended ( font, data, message_color );

		if ( !textSurface )
			return;

		SDL_BlitSurface ( textSurface, 0, dest_surface, &dest );
		SDL_FreeSurface ( textSurface );

		data[cursor_pos] = 0;
		TTF_SizeUNICODE ( font, data, &x, &h );
	}
	else
	{
		x = 0;
		h = TTF_FontHeight( font );
	}

	if (focus)
	{
		cursor.x = x + dest.x;
		cursor.h = h;
		cursor.w = 1;
		cursor.y = dest.y;
		SDL_FillRect ( dest_surface, &cursor, SDL_MapRGB ( dest_surface->format, 255, 255, 255 ) );
	}

	dest.x = originx;
	dest.y = originy;
	dest.w = sizex;
	dest.h = sizey;
	SDL_UpdateRect ( dest_surface, dest.x, dest.y, dest.w, dest.h );
}
