//
// C++ Implementation: message_area
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <iostream>

#pragma warning(disable:4786)

#include "message_area.h"

MessageArea::MessageArea ( SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy ) : Widget( dest_surface, sizex, sizey, originx, originy )
{
	dest.x = originx;
	dest.y = originy;
	dest.w = sizex;
	dest.h = sizey;

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

	input_received = true;
	blocked = 0;
	current_message = -1;
}

MessageArea::~MessageArea()
{
	// delete font; this gets me a compile-time warning - not sure why?
	// because the font type is an internal hidden type from SDL, and one
	// does not call delete on internal types.
	// That is what TTF_CloseFont is meant for anyway :)
	TTF_CloseFont ( font );
}

void MessageArea::InputReceived()
{
	input_received = true; // ready for more input with no 'more' prompt
}

int MessageArea::GetBlocked() const
{
	return blocked;
}

void MessageArea::SetUnblocked()
{
	blocked = blocked - 1;
	input_received = false;
	current_message = current_message + 1;
}

void MessageArea::AddMessage ( const std::string& message )
{
	if ( message.length() == 0 )
	{
		return;
	}
	if ( input_received )
	{
		input_received = false;
		current_message = current_message + 1;
	}
	else
	{
		blocked = blocked + 1;
	}
	messages.push_back ( message );
}

void MessageArea::Draw()
{
	SDL_FillRect ( dest_surface, &dest, 0 );
	if (current_message < 0)
		return;
	std::vector < std::string > wrapped_text = WrapText ( messages[current_message] + ( blocked ? " --more--" : "" ), font, dest.w );
	SDL_Rect this_line_dest;
	this_line_dest.x = originx;
	this_line_dest.y = originy;
	// from SDL manual : Only the position is used in the dstrect (the width and height are ignored).
	// this_line_dest.w = sizex;
	// this_line_dest.h = font_height;
	for ( int i=0 ; i < wrapped_text.size() ; i++ )
	{
		DrawText ( wrapped_text[i], dest_surface, font, message_color, this_line_dest );
		this_line_dest.y += font_height;
	}
	SDL_UpdateRect ( dest_surface, dest.x, dest.y, dest.w, dest.h );
}

/* SOME CODE CRIBBED OFF THE WEB FOLLOWS. NEEDS A TIDY UP

This is C looking code used for standard C strings. Fortunately,
C++ strings are far more powerful and useful so the end result
will look better :) */

/* There was a small bug that caused that function to return a space in the front of each line
So I wrote a new one :)
*/

void Tokenize ( const std::string& str,
                std::vector<std::string>& tokens,
                const std::string& delimiters = " " )
{
	std::string::size_type lastPos = 0;
	std::string::size_type pos = str.find_first_of ( delimiters, lastPos );

	while ( std::string::npos != pos || std::string::npos != lastPos )
	{
		tokens.push_back ( str.substr ( lastPos, pos - lastPos ) );
		if ( pos == std::string::npos )
			return;
		lastPos = pos + 1;
		pos = str.find_first_of ( delimiters, lastPos );
	}
}

void WrapSingleLine ( const std::string& line, std::vector<std::string>& result, TTF_Font* textFont, int maxWidth )
{
	int w;
	if ( TTF_SizeUTF8 ( textFont, line.c_str(), &w, 0 ) )
		return; // Silently ignoring errors ?
	if ( w <= maxWidth )
	{
		result.push_back ( line );
		return;
	}
	std::string right_part = line;
	std::string::size_type pos = right_part.size();
	while ( pos > 0 )
	{
		pos = right_part.find_last_of ( ' ', pos );
		if ( pos == std::string::npos )
		{
			// Word wrap failed, bailing out
			result.push_back ( right_part );
			return;
		}
		pos = right_part.find_last_not_of ( ' ', pos );
		if ( pos == std::string::npos )
		{
			// Word wrap failed, bailing out
			result.push_back ( right_part );
			return;
		}
		std::string left_part = right_part.substr ( 0, pos+1 );
		if ( TTF_SizeUTF8 ( textFont, left_part.c_str(), &w, 0 ) )
			return; // Silently ignoring errors ?
		if ( w <= maxWidth )
		{
			result.push_back ( left_part );
			right_part = right_part.substr ( pos+2, std::string::npos );
			pos = std::string::npos;
		}
	}
}

std::vector<std::string> WrapText ( const std::string& message, TTF_Font* textFont, int maxWidth )
{
	std::vector<std::string> result;
	std::vector<std::string> line_list;

	Tokenize ( message, line_list, "\n" );
	for ( int ii = 0; ii < line_list.size(); ++ii )
		WrapSingleLine ( line_list[ii], result, textFont, maxWidth );

	return result;
}

void DrawText ( std::string& line, SDL_Surface* surface, TTF_Font* textFont, SDL_Color textColor,
                SDL_Rect& drawingPos )
{
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended ( textFont, line.c_str(), textColor );
	// RenderBlended because some fonts will slightly overlap when drawn one above the
	// other. Things like the letter 'g' for example extends slightly on the line bellow
	// when using BitstreamVeraMono

	if ( !textSurface )
		return;

	SDL_BlitSurface ( textSurface, 0, surface, &drawingPos );
	SDL_FreeSurface ( textSurface );
	return;
}
