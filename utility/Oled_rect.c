/*
 * Oled_rect.c - Utility to draw rectangles and boxes
 * Oled Graphics library
 * This Library is based on Universal 8-bit Graphics Library
 *
 * This library is use for Tiva Arm Cotex M4
 * Device: OLED 1.3", 128x64 dot matrix panel
 * Communication: SPI interface
 * Driver: SH1106
 *
 * Revision: 2.01
 * Date: 10/11/2016
 * Author: QUANG
 */
 
#include "../Oled.h"

/****************************Function definitions*****************************/

/******************************************************************************
 * Oled_DrawHLine - Draw horizontal line
 *
 * Parameter:
 * 	(x, y): left point position
 * 	w	  : line width
 *
 * Restriction: x+w < 128
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawHLine(uint8_t x, uint8_t y, uint8_t w)
{
	uint8_t dest_x = x+w;
	
	if (dest_x > OLED_COLUMNSIZE)
	{
		dest_x = OLED_COLUMNSIZE;
		w = dest_x - x;
	}
	
	while(x < dest_x)
		Oled_DrawPixel(x++,y,1);//Draw pixel until it reach the destination point
}

/******************************************************************************
 * Oled_DrawVLine - Draw vertical line
 *
 * Parameter:
 * 	(x, y): upper point position
 * 	h	  : line height
 *
 * Restriction: y+h < 64
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawVLine(uint8_t x, uint8_t y, uint8_t h)
{
	uint8_t dest_y = y+h;
	
	if (dest_y > OLED_HEIGHT)
	{
		dest_y = OLED_HEIGHT;
		h = dest_y - y;
	}
	
	while(h >= 8)	//Draw 8 pixels at a time
	{
		Oled_Draw8Pixel(x,y, 0xFF, 8 - (y % 8), VERTICAL);
		h -= 8 - (y % 8);
		y += 8 - (y % 8);
	}
	Oled_Draw8Pixel(x, y, 0xFF, h, VERTICAL);
}

/******************************************************************************
 * Oled_DrawFrame - Draw a rectangle frame
 *
 * Parameter:
 * 	(x, y): upper left position
 * 	w	  : rectangle width
 * 	h	  : rectangle height
 *
 * Restriction: x+w < 128
 * 				y+h < 64
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	uint8_t xtmp = x;
	//Draw 4 lines
	Oled_DrawHLine(x, y, w);
	Oled_DrawVLine(x, y, h);
	x+=w;
	x--;
	Oled_DrawVLine(x, y, h);
	y+=h;
	y--;
	Oled_DrawHLine(xtmp, y, w);	
}

/******************************************************************************
 * Oled_DrawRFrame - Draw a rectangle frame with rounded corner
 *
 * Parameter:
 * 	(x, y): upper left position
 * 	w	  : rectangle width
 * 	h	  : rectangle height
 *  r	  : radius of the corner circles
 *
 * Restriction: x+w < 128
 * 				y+h < 64
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawRFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
	uint8_t xl, yu;
	uint8_t yl, xr;
	uint8_t ww, hh;

	xl = x+r;
	yu = y+r;

	xr = x+w-r-1;
	yl = y+h-r-1;
	//draw 4 cirles at 4 corners
	Oled_DrawCircle(xl, yu, r, DRAW_UPPER_LEFT);
	Oled_DrawCircle(xr, yu, r, DRAW_UPPER_RIGHT);
	Oled_DrawCircle(xl, yl, r, DRAW_LOWER_LEFT);
	Oled_DrawCircle(xr, yl, r, DRAW_LOWER_RIGHT);

	ww = w - ((r << 1)+2);
	hh = h - ((r << 1)+2);

	xl++;
	yu++;
	h--;
	w--;
	//draw 4 lines
	Oled_DrawHLine(xl, y, ww);
	Oled_DrawHLine(xl, y+h, ww);
	Oled_DrawVLine(x, yu, hh);
	Oled_DrawVLine(x+w, yu, hh);
}

/******************************************************************************
 * Oled_DrawBox - Draw a filled rectangle
 *
 * Parameter:
 * 	(x, y): upper left position
 * 	w	  : rectangle width
 * 	h	  : rectangle height
 * 
 * Restriction: x+w < 128
 * 				y+h < 64
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	do
	{ 
		Oled_DrawVLine(x, y, h);
		x++;
		w--;
	} while(w);
}

/******************************************************************************
 * Oled_DrawRBox - Draw a filled rectangle with rounded corner
 *
 * Parameter:
 * 	(x, y): upper left position
 * 	w	  : rectangle width
 * 	h	  : rectangle height
 * 	r	  : radius of the corner discs
 * 
 * Restriction: x+w < 128
 * 				y+h < 64
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawRBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
	uint8_t xl, yu;
	uint8_t yl, xr;
	uint8_t ww, hh;

	xl = x+r;
	yu = y+r;

	xr = x+w-r-1;

	yl = y+h-r-1;
	//Draw 4 discs at 4 corners
	Oled_DrawDisc(xl, yu, r, DRAW_UPPER_LEFT);
	Oled_DrawDisc(xr, yu, r, DRAW_UPPER_RIGHT);
	Oled_DrawDisc(xl, yl, r, DRAW_LOWER_LEFT);
	Oled_DrawDisc(xr, yl, r, DRAW_LOWER_RIGHT);

	ww = w-((r << 1)+2);
	hh = h-((r << 1)+2);

	xl++;
	yu++;
	h--;
	//fill the box
	Oled_DrawBox(xl, y, ww, r+1);
	Oled_DrawBox(xl, yl, ww, r+1);
	Oled_DrawBox(x, yu, w, hh);
}

/* End of Oled_rect.c */
