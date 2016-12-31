/*
 * Oled_circle.c - Utility to draw circles and discs
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

/*************************Private function prototypes*************************/
static void Oled_draw_circle_section(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0, uint8_t option);
static void Oled_draw_disc_section(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0, uint8_t option);

/****************************Function definitions*****************************/

/******************************************************************************
 * Oled_DrawCircle - Draw full or part of a circle
 *
 * Parameter:
 * 	(x0, y0): circle center
 * 	rad		: radius (pixel unit)
 * 	opt		: option for drawing
 * 		- DRAW_UPPER_RIGHT: draw from upper right corner
 * 		- DRAW_UPPER_LEFT : draw from upper left corner
 * 		- DRAW_LOWER_LEFT : draw from lower left corner
 * 		- DRAW_LOWER_RIGHT: draw from lower left corner
 * 		- DRAW_ALL		  : draw from all corner
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawCircle(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t option)
{
	int8_t f;
	int8_t ddF_x;
	int8_t ddF_y;
	uint8_t x;
	uint8_t y;
	//calculate, setting up parameter
	f = 1;
	f -= rad;
	ddF_x = 1;
	ddF_y = 0;
	ddF_y -= rad;
	ddF_y *= 2;
	x = 0;
	y = rad;

	Oled_draw_circle_section(x, y, x0, y0, option);
	//Draw
	while (x < y)
	{
		if (f >= 0) 
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		Oled_draw_circle_section(x, y, x0, y0, option);    
	}
}

/******************************************************************************
 * Oled_DrawCircle - Draw full or part of a disc (filled circle)
 *
 * Parameter:
 * 	(x0, y0): disc center
 * 	rad		: disc corner (2*pi for whole circle) in radian
 * 	opt		: option for drawing
 * 		- DRAW_UPPER_RIGHT: draw from upper right corner
 * 		- DRAW_UPPER_LEFT : draw from upper left corner
 * 		- DRAW_LOWER_LEFT : draw from lower left corner
 * 		- DRAW_LOWER_RIGHT: draw from lower left corner
 * 		- DRAW_ALL		  : draw from all corner
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawDisc(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t option)
{
	int8_t f;
	int8_t ddF_x;
	int8_t ddF_y;
	uint8_t x;
	uint8_t y;
	//calculate, setting up parameter
	f = 1;
	f -= rad;
	ddF_x = 1;
	ddF_y = 0;
	ddF_y -= rad;
	ddF_y *= 2;
	x = 0;
	y = rad;

	Oled_draw_disc_section(x, y, x0, y0, option);
	//Draw
	while (x < y)
	{
		if (f >= 0) 
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		Oled_draw_disc_section(x, y, x0, y0, option);
	}
}

/******************************************************************************
 * Oled_draw_circle_section - draw the circle point
 * This function support the draw circle function (Oled_DrawCircle)
 *
 * Parameter:
 * 	(x, y): a point to draw (already calculate by the draw circle function)
 * 	(x0, y0): circle center
 * 	option: option for drawing
 * 		- DRAW_UPPER_RIGHT: draw from upper right corner
 * 		- DRAW_UPPER_LEFT : draw from upper left corner
 * 		- DRAW_LOWER_LEFT : draw from lower left corner
 * 		- DRAW_LOWER_RIGHT: draw from lower left corner
 * 		- DRAW_ALL		  : draw from all corner
 *
 * Return: none
 *****************************************************************************/
static void Oled_draw_circle_section(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0, uint8_t option)
{
    /* upper right */
    if (option & DRAW_UPPER_RIGHT)
    {
      Oled_DrawPixel(x0 + x, y0 - y, 1);
      Oled_DrawPixel(x0 + y, y0 - x, 1);
    }
    
    /* upper left */
    if (option & DRAW_UPPER_LEFT)
    {
      Oled_DrawPixel(x0 - x, y0 - y, 1);
      Oled_DrawPixel(x0 - y, y0 - x, 1);
    }
    
    /* lower right */
    if (option & DRAW_LOWER_RIGHT)
    {
      Oled_DrawPixel(x0 + x, y0 + y, 1);
      Oled_DrawPixel(x0 + y, y0 + x, 1);
    }
    
    /* lower left */
    if (option & DRAW_LOWER_LEFT)
    {
      Oled_DrawPixel(x0 - x, y0 + y, 1);
      Oled_DrawPixel(x0 - y, y0 + x, 1);
    }
}

/******************************************************************************
 * Oled_draw_disc_section - a minor step when drawing a disc
 * This function support the draw disc function (Oled_DrawDisc)
 * This function draw a vertical line at a specific location by the parameters
 *
 * Parameter:
 * 	(x, y): a first point of a line (already calculate by the draw disc function)
 * 	(x0, y0): disc center
 * 	option: option for drawing
 * 		- DRAW_UPPER_RIGHT: draw from upper right corner
 * 		- DRAW_UPPER_LEFT : draw from upper left corner
 * 		- DRAW_LOWER_LEFT : draw from lower left corner
 * 		- DRAW_LOWER_RIGHT: draw from lower left corner
 * 		- DRAW_ALL		  : draw from all corner
 *
 * Return: none
 *****************************************************************************/
static void Oled_draw_disc_section(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0, uint8_t option)
{
    /* upper right */
    if ( option & DRAW_UPPER_RIGHT )
    {
      Oled_DrawVLine(x0+x, y0-y, y+1);
      Oled_DrawVLine(x0+y, y0-x, x+1);
    }
    
    /* upper left */
    if ( option & DRAW_UPPER_LEFT )
    {
      Oled_DrawVLine(x0-x, y0-y, y+1);
      Oled_DrawVLine(x0-y, y0-x, x+1);
    }
    
    /* lower right */
    if ( option & DRAW_LOWER_RIGHT )
    {
      Oled_DrawVLine(x0+x, y0, y+1);
      Oled_DrawVLine(x0+y, y0, x+1);
    }
    
    /* lower left */
    if ( option & DRAW_LOWER_LEFT )
    {
      Oled_DrawVLine(x0-x, y0, y+1);
      Oled_DrawVLine(x0-y, y0, x+1);
    }
}

 /* End of Oled_circle.c */
