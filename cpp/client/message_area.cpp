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

MessageArea::MessageArea ( SDL_Surface* dest_surface, int sizex, int sizey, int originx, int originy ) :
		dest_surface ( dest_surface ),
		sizex ( sizex ),
		sizey ( sizey ),
		originx ( originx ),
		originy ( originy )
{
	dest.x = originx;
	dest.y = originy;
	dest.w = sizex;
	dest.h = sizey;

	font = TTF_OpenFont( "client\\data\\cour.ttf", 12 );
	if ( !font )
	{
		std::cerr << "Error creating TTF_Font: " << TTF_GetError() << "\n";
		exit ( 1 );
	}
	int junk;
    TTF_SizeText( font, "abacab", &junk, &font_height );

	message_color.r = 255;
	message_color.g = 255;
	message_color.b = 0;
	message_color.unused = 0;

	input_received = 1;
	blocked = 0;
	current_message = -1;
}

MessageArea::~MessageArea()
{
	// delete font; this gets me a compile-time warning - not sure why?
}

void MessageArea::InputReceived()
{
	input_received = 1; // ready for more input with no 'more' prompt
}

int MessageArea::GetBlocked() const
{
	return blocked;
}

void MessageArea::SetUnblocked()
{
	blocked = blocked - 1;
	input_received = 0;
	current_message = current_message + 1;
}

void MessageArea::AddMessage ( std::string& message )
{
	if ( message.length() == 0 )
	{
		return;
	}
	if (input_received)
	{
		input_received = 0;
		current_message = current_message + 1;
	}
	else
	{
		blocked = blocked + 1;
	}
	messages.push_back ( message );
}

void MessageArea::Show()
{
	SDL_FillRect ( dest_surface, &dest, 0 );
	std::vector < std::string > wrapped_text = WrapText ( messages[current_message] + (blocked ? " --more--" : ""), font, dest.w );
	SDL_Rect this_line_dest;
	this_line_dest.x = originx;
	this_line_dest.y = originy;
	this_line_dest.w = sizex;
	this_line_dest.h = font_height + 5;
	for ( int i=0 ; i < wrapped_text.size() ; i++ )
	{
		DrawText ( wrapped_text[i], dest_surface, font, message_color, this_line_dest, ETextHAlign_Left, ETextVAlign_Top );
		this_line_dest.y = this_line_dest.y + 5 + font_height;
	}
	SDL_UpdateRect ( dest_surface, dest.x, dest.y, dest.w, dest.h );
}

void MessageArea::Clear()
{
	SDL_FillRect ( dest_surface, &dest, 0 ); // just set area to black, for now
	SDL_UpdateRect ( dest_surface, dest.x, dest.y, dest.w, dest.h );
}

/* SOME CODE CRIBBED OFF THE WEB FOLLOWS. NEEDS A TIDY UP */

bool CanWrapText(std::string& sText, int iWrapOffset)
{
   if (iWrapOffset >= sText.length()-1)
      return true;

   if (sText[iWrapOffset+1] == ' ' ||
       sText[iWrapOffset+1] == '\r' ||
       sText[iWrapOffset+1] == '\n' ||
       sText[iWrapOffset+1] == '\t')
   {
      return true;
   }
   else
      return false;
}

int CalcTextWidth(TTF_Font* pTextFont, std::string& sText)
{
   int iWidth = 0;
   int iHeight = 0;
   TTF_SizeText(pTextFont, sText.c_str(), &iWidth, &iHeight);
   return iWidth;
}

std::string CalcTextLine(std::string& sEntireText, TTF_Font* pTextFont, int iMaxWidth)
{
   int iLastWrapPos = -1;

   for (int iCharPos = 0; iCharPos < sEntireText.length(); iCharPos++)
   {
      // always break on line-feed
      if (sEntireText[iCharPos] == '\n' || sEntireText[iCharPos] == '\r')
      {
         iLastWrapPos = iCharPos;
         break;
      }

      // can wrap at this location?
      if (CanWrapText(sEntireText, iCharPos))
      {
         // wrap pos is end of line; get current line
		  std::string sCurAttempt = sEntireText.substr(0,iCharPos+1);

         // fits within max width?
         if (CalcTextWidth(pTextFont, sCurAttempt) < iMaxWidth)
            iLastWrapPos = iCharPos;
         else
            break;
      }
   }

   if (iLastWrapPos == -1)
   {
      // no place to wrap; give whole line
      return sEntireText;
   }
   else
   {
      // wrap position is end of line
      return sEntireText.substr(0,iLastWrapPos+1);
   }
}

std::vector<std::string> WrapText(std::string sEntireText, TTF_Font* pTextFont, int iMaxWidth)
{
	std::vector<std::string> result;
	int iLastWrapPos = -1;
	while ( sEntireText.size() > 0 ){
		std::string this_line = CalcTextLine(sEntireText,pTextFont,iMaxWidth);
		result.push_back(this_line);
		iLastWrapPos = iLastWrapPos + this_line.length();
		sEntireText = sEntireText.substr(this_line.length(),sEntireText.length());
	}
	return result;
}

bool DrawText(std::string& sLine, SDL_Surface* pSurface, TTF_Font* pTextFont, SDL_Color vTextColor,
              SDL_Rect& vDrawingRect, ETextHAlign eHAlignment, ETextVAlign eVAlignment)
{
   SDL_Surface* pText = TTF_RenderText_Solid(pTextFont, sLine.c_str(), vTextColor);
   if (!pText)
      return false;

   switch (eHAlignment)
   {
   case ETextHAlign_Left:
      break;
   case ETextHAlign_Center:
	  vDrawingRect.x += (vDrawingRect.w - pText->w) / 2 > 0 ? (vDrawingRect.w - pText->w) / 2 : 0;
      break;
   case ETextHAlign_Right:
	   vDrawingRect.x += (vDrawingRect.w - pText->w) > 0 ? (vDrawingRect.w - pText->w) : 0;
      break;
   }

   switch (eVAlignment)
   {
   case ETextVAlign_Top:
      break;
   case ETextVAlign_Center:
      vDrawingRect.y += (vDrawingRect.h - pText->h) / 2;
      break;
   case ETextVAlign_Bottom:
      vDrawingRect.y += vDrawingRect.h - pText->h;
      break;
   }

   vDrawingRect.w = pText->w;
   vDrawingRect.h = pText->h;

   // We don't let SDL_BlitSurface change vDrawingRect, since we already know that it's
   // correct, and SDL_BlitSurface doesn't handle negative x and y values that way we want to.
   SDL_Rect vTmpRect = vDrawingRect;
   SDL_BlitSurface(pText, NULL, pSurface, &vTmpRect);

   SDL_FreeSurface(pText);
   return true;
}

/* CRIBBED MATERIAL ENDS */