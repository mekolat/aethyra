/*
 *  The Mana World
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id$
 */

#include "progressbar.h"
#include "gui.h"

// To draw primitives without dependencies
void DrawPixel(SDL_Surface * screen, int x, int y, Uint8 R, Uint8 G, Uint8 B);
void DrawLine(SDL_Surface * screen, int x1, int y1, int x2, int y2, unsigned char Red, unsigned char Green, unsigned char Blue);


ProgressBar::ProgressBar(float progress, int x, int y, int width, unsigned char red, unsigned green, unsigned char blue)
{
    setProgress(progress);
    Red = red;
    Green = green;
    Blue = blue;
    X = x;
    Y = y;
    Width = width;
}

void ProgressBar::draw(gcn::Graphics *graphics)
{
    int absx, absy;
    getAbsolutePosition(absx, absy);
    
    // outer bar
    DrawLine(screen, absx+X+PROGRESSBAR_HEIGHT, absy+Y, absx+X+Width, absy+Y, abs(Red-70), abs(Green-70), abs(Blue-70));
    DrawLine(screen, absx+X, absy+Y+PROGRESSBAR_HEIGHT, absx+X+Width-PROGRESSBAR_HEIGHT, absy+Y+PROGRESSBAR_HEIGHT, abs(Red-70), abs(Green-70), abs(Blue-70));
    DrawLine(screen, absx+X+PROGRESSBAR_HEIGHT, absy+Y, absx+X, absy+Y+PROGRESSBAR_HEIGHT, abs(Red-70), abs(Green-70), abs(Blue-70));
    DrawLine(screen, absx+X+Width, absy+Y, absx+X+Width-PROGRESSBAR_HEIGHT, absy+Y+PROGRESSBAR_HEIGHT, abs(Red-70), abs(Green-70), abs(Blue-70));
    
    // Shadow of outer bar
    DrawLine(screen, absx+X+1, absy+Y+PROGRESSBAR_HEIGHT+1, absx+X+Width-PROGRESSBAR_HEIGHT, absy+Y+PROGRESSBAR_HEIGHT+1, 20, 20, 20);
    DrawLine(screen, absx+X+Width+1, absy+Y, absx+X+Width-PROGRESSBAR_HEIGHT+1, absy+Y+PROGRESSBAR_HEIGHT, 20, 20, 20);
    
    
    // Inner bar
    int Temp = 0;
    
    for(int i = 1; i < PROGRESSBAR_HEIGHT; i++)
    {
        Temp = absx+X+int(float(Width)*progress)-i-1;
        if (Temp < (absx + X + PROGRESSBAR_HEIGHT + 1 - i)) Temp = (absx + X + PROGRESSBAR_HEIGHT + 1 - i);
        DrawLine(screen, absx + X + PROGRESSBAR_HEIGHT + 1 - i, absy+Y+i, Temp, absy+Y+i, Red, Green, Blue);
    }
    
    // Shadow of inner bar
    Temp = absx+X+int(float(Width)*progress)-2;
    if ( Temp < (absx+X+PROGRESSBAR_HEIGHT+1) ) Temp = absx+X+PROGRESSBAR_HEIGHT;
    DrawLine(screen, absx+X+PROGRESSBAR_HEIGHT+1, absy+Y+1, Temp, absy+Y+1, abs(Red-40), abs(Green-40), abs(Blue-40));
    DrawLine(screen, absx+X+PROGRESSBAR_HEIGHT, absy+Y+1, absx+X+2, absy+Y+PROGRESSBAR_HEIGHT-1, abs(Red-40), abs(Green-40), abs(Blue-40));
    
}

void ProgressBar::setProgress(float progress)
{
    this->progress = progress;
}

float ProgressBar::getProgress()
{
    return progress;
}

 // This function draws a pixel on the screen depending on the resolution etc...
 void DrawPixel(SDL_Surface * screen, int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
	Uint32 color = SDL_MapRGB(screen->format, R, G, B);
	if ( (x >= screen->w) || (x < 0) )
		return;
	if ( (y >= screen->h) || (y < 0) )
		return;
	
	switch (screen->format->BytesPerPixel)
	{
		case 1:   // Pour 8 BPP
		{	
			Uint8 *bufp;
			bufp = (Uint8 *) screen->pixels + y * screen ->pitch + x;
			*bufp = color;
		}	
			break;
		
		case 2:  // 15 ou 16 BPP
		{	
			Uint16 * bufp;
			bufp = (Uint16 *)screen->pixels + y * screen->pitch/2 + x;
			*bufp = color;
		}
			break;
			
		case 3: // 24 BPP
		{
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y * screen->pitch + x * 3;
			
			if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
			{
				bufp[0] = color;
				bufp[1] = color >> 8;
				bufp[2] = color >> 16;
			} else {
				bufp[2] = color;
				bufp[1] = color >> 8;
				bufp[0] = color >> 16;
			}
		}
			break;
			
		case 4: // 32 BPP, the most useful !
		{
			Uint32 *bufp;
			bufp = (Uint32 *)screen->pixels + y * screen->pitch/4 + x;
			*bufp = color;
		}
			break;
	} 
} // End of DrawPixel

// Draw A line.
void DrawLine(SDL_Surface * screen, int x1, int y1, int x2, int y2, unsigned char Red, unsigned char Green, unsigned char Blue)
{
	float a, b, Temp_x1, Temp_x2, Temp_y1, Temp_y2;
	
	Temp_x1 = x1;
	Temp_x2 = x2;
	Temp_y1 = y1;
	Temp_y2 = y2;
	
	if ( (x1-x2) != 0) 
	{
		a=(Temp_y1-Temp_y2)/(Temp_x1-Temp_x2); // the a in y=ax+b
	}
	else // The line is vertical
	{
		if ( y1 < y2)
		{
			for (int Y=y1; Y<=y2; Y++)
			{
				DrawPixel(screen, x1, Y, Red, Green, Blue);
			}
		}
		else
		{
			for (int Y=y2; Y<=y1; Y++)
			{
				DrawPixel(screen, x1, Y, Red, Green, Blue);
			}
		}
		
		return;
	}
	
	b=y1-a*x1;
	
	if ( x1 < x2)
	{
		for (int X = x1; X <= x2; X++)
		{
			DrawPixel(screen, X, int(a*X+b), Red, Green, Blue);
		}
	}
	else
	{
		for (int X = x2; X <= x1; X++)
		{
			DrawPixel(screen, X, int(a*X+b), Red, Green, Blue);
		}
	}
	
	if ( y1 < y2)
	{
		for (int Y = y1; Y <= y2; Y++)
		{
			if (a != 0) DrawPixel(screen, int((Y-b)/a), Y, Red, Green, Blue);
		}
	}
	else
	{
		for (int Y = y2; Y <= y1; Y++)
		{
			if (a != 0) DrawPixel(screen, int((Y-b)/a), Y, Red, Green, Blue);
		}
	}

} // End of DrawLine 

