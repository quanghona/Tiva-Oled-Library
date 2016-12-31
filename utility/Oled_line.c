/*
 * Oled_line.c - Utility to draw lines
 * Oled Graphics library
 * This Library is based on Universal 8-bit Graphics Library
 *
 * This library is use for Tiva Arm Cotex M4
 * Device: OLED 1.3", 128x64 dot matrix panel
 * Communication: SPI interface
 * Driver: SH1106
 *
 * Revision: 2.01
 * Date: 9/11/2016
 * Author: QUANG
 */

#include "../Oled.h"
/****************************Private Definitions******************************/
#define swap(x,y) tmp=(x);(x)=(y);(y)=tmp

/****************************Function definitions*****************************/

/******************************************************************************
 * Draw a line from (x1, y1) to (x2, y2)
 *
 * Parameter:
 * 	x1,y1: position of the first point
 * 	x2,y2: position of the second point
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	uint8_t tmp;
	uint8_t x,y;
	uint8_t dx, dy;
	int8_t err;
	int8_t ystep;

	bool swapxy = false;

	/* no BBX intersection check at the moment, should be added... */

	dx = ( x1 > x2 ) ? (x1-x2) : (x2-x1);
	dy = ( y1 > y2 ) ? (y1-y2) : (y2-y1);

	if (dy > dx) 
	{
		swapxy = true;
		swap(dx,dy);
		swap(x1,y1);
		swap(x2,y2);
	}

	if (x1 > x2) 
	{
		swap(x1,x2);
		swap(y1,y2);
	}
	
	err = dx >> 1;
	ystep = (y2 > y1) ?  1 : -1;
	y = y1;
	for( x = x1; x <= x2; x++ )
	{
		swapxy ? Oled_DrawPixel(y, x, 1) : Oled_DrawPixel(x, y, 1);
		err -= (uint8_t)dy;
		if ( err < 0 ) 
		{
		  y += (uint8_t)ystep;
		  err += (uint8_t)dx;
		}
	}
}

/* End of Oled_line.c */
