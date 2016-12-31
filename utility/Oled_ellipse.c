/*
 * Oled_ellipse.c - Utility to draw empty and filled ellipses.
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
static void Oled_draw_ellipse_section(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0, uint8_t option);
static void Oled_draw_filled_ellipse_section(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0, uint8_t option);

/****************************Function definitions*****************************/

/******************************************************************************
 * Oled_DrawEllipse - Draw ellipse
 *
 * Parameter:
 * 	(x0, y0): ellipse center
 * 	rx		: horizontal radius
 * 	ry		: vertical radius
 * 	option	: option for drawing
 * 		- DRAW_UPPER_RIGHT: draw from upper right corner
 * 		- DRAW_UPPER_LEFT : draw from upper left corner
 * 		- DRAW_LOWER_LEFT : draw from lower left corner
 * 		- DRAW_LOWER_RIGHT: draw from lower left corner
 * 		- DRAW_ALL		  : draw from all corner
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t option)
{
  uint8_t x, y;
  long xchg, ychg;
  long err;
  long rxrx2;
  long ryry2;
  long stopx, stopy;
  
  rxrx2 = rx*rx*2;
  ryry2 = ry*ry*2;
  x = rx;
  y = 0;
  xchg = (1-2*rx)*ry*ry;
  ychg = rx*rx;
  err = 0;
  stopx = ryry2*rx;
  stopy = 0;
  
  while( stopx >= stopy )
  {
    Oled_draw_ellipse_section(x, y, x0, y0, option);
    y++;
    stopy += rxrx2;
    err += ychg;
    ychg += rxrx2;
    if ( 2*err+xchg > 0 )
    {
      x--;
      stopx -= ryry2;
      err += xchg;
      xchg += ryry2;      
    }
  }

  x = 0;
  y = ry;
  xchg = ry*ry;
  ychg = (1-2*ry)*rx*rx;
  err = 0;
  stopx = 0;
  stopy = rxrx2*ry;

  while( stopx <= stopy )
  {
    Oled_draw_ellipse_section(x, y, x0, y0, option);
    x++;
    stopx += ryry2;
    err += xchg;
    xchg += ryry2;
    if ( 2*err+ychg > 0 )
    {
      y--;
      stopy -= rxrx2;
      err += ychg;
      ychg += rxrx2;
    }
  }
}

/******************************************************************************
 * Oled_DrawFilledEllipse - Draw filled ellipse
 *
 * Parameter:
 * 	(x0, y0): ellipse center
 * 	rx		: horizontal radius
 * 	ry		: vertical radius
 * 	option	: option for drawing
 * 		- DRAW_UPPER_RIGHT: draw from upper right corner
 * 		- DRAW_UPPER_LEFT : draw from upper left corner
 * 		- DRAW_LOWER_LEFT : draw from lower left corner
 * 		- DRAW_LOWER_RIGHT: draw from lower left corner
 * 		- DRAW_ALL		  : draw from all corner
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawFilledEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t option)
{
  uint8_t x, y;
  long xchg, ychg;
  long err;
  long rxrx2;
  long ryry2;
  long stopx, stopy;
  
  rxrx2 = rx*rx*2;
  ryry2 = ry*ry*2;
  x = rx;
  y = 0;
  xchg = (1-2*rx)*ry*ry;
  ychg = rx*rx;
  err = 0;
  stopx = ryry2*rx;
  stopy = 0;
  
  while( stopx >= stopy )
  {
    Oled_draw_filled_ellipse_section(x, y, x0, y0, option);
    y++;
    stopy += rxrx2;
    err += ychg;
    ychg += rxrx2;
    if ( 2*err+xchg > 0 )
    {
      x--;
      stopx -= ryry2;
      err += xchg;
      xchg += ryry2;      
    }
  }

  x = 0;
  y = ry;
  xchg = ry*ry;
  ychg = (1-2*ry)*rx*rx;
  err = 0;
  stopx = 0;
  stopy = rxrx2*ry;

  while( stopx <= stopy )
  {
    Oled_draw_filled_ellipse_section(x, y, x0, y0, option);
    x++;
    stopx += ryry2;
    err += xchg;
    xchg += ryry2;
    if ( 2*err+ychg > 0 )
    {
      y--;
      stopy -= rxrx2;
      err += ychg;
      ychg += rxrx2;
    }
  } 
}

/******************************************************************************
 * Oled_draw_ellipse_section - draw the ellipse point
 * This function support the draw ellipse function (Oled_DrawEllipse)
 *
 * Parameter:
 * 	(x, y): a point to draw (already calculate by the draw ellipse function)
 * 	(x0, y0): ellipse center
 * 	option: option for drawing
 * 		- DRAW_UPPER_RIGHT: draw from upper right corner
 * 		- DRAW_UPPER_LEFT : draw from upper left corner
 * 		- DRAW_LOWER_LEFT : draw from lower left corner
 * 		- DRAW_LOWER_RIGHT: draw from lower left corner
 * 		- DRAW_ALL		  : draw from all corner
 *
 * Return: none
 *****************************************************************************/
static void Oled_draw_ellipse_section(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0, uint8_t option)
{
    /* upper right */
    if ( option & DRAW_UPPER_RIGHT )
    {
      Oled_DrawPixel(x0 + x, y0 - y, 1);
    }
    
    /* upper left */
    if ( option & DRAW_UPPER_LEFT )
    {
      Oled_DrawPixel(x0 - x, y0 - y, 1);
    }
    
    /* lower right */
    if ( option & DRAW_LOWER_RIGHT )
    {
      Oled_DrawPixel(x0 + x, y0 + y, 1);
    }
    
    /* lower left */
    if ( option & DRAW_LOWER_LEFT )
    {
      Oled_DrawPixel(x0 - x, y0 + y, 1);
    }
}

/******************************************************************************
 * Oled_draw_filled_ellipse_section - a minor step when drawing a filled ellipse
 * This function support the draw filled ellipse function (Oled_DrawFilledEllipse)
 * This function draw a vertical line at a specific location by the parameters
 *
 * Parameter:
 * 	(x, y): a first point of a line (already calculate by the draw filled
 * 	ellipse function)
 * 	(x0, y0): ellipse center
 * 	option: option for drawing
 * 		- DRAW_UPPER_RIGHT: draw from upper right corner
 * 		- DRAW_UPPER_LEFT : draw from upper left corner
 * 		- DRAW_LOWER_LEFT : draw from lower left corner
 * 		- DRAW_LOWER_RIGHT: draw from lower left corner
 * 		- DRAW_ALL		  : draw from all corner
 *
 * Return: none
 *****************************************************************************/
static void Oled_draw_filled_ellipse_section(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0, uint8_t option)
{
    /* upper right */
    if ( option & DRAW_UPPER_RIGHT )
    {
      Oled_DrawVLine(x0+x, y0-y, y+1);
    }
    
    /* upper left */
    if ( option & DRAW_UPPER_LEFT )
    {
      Oled_DrawVLine(x0-x, y0-y, y+1);
    }
    
    /* lower right */
    if ( option & DRAW_LOWER_RIGHT )
    {
      Oled_DrawVLine(x0+x, y0, y+1);
    }
    
    /* lower left */
    if ( option & DRAW_LOWER_LEFT )
    {
      Oled_DrawVLine(x0-x, y0, y+1);
    }
}

 /* End of Oled_ellipse.c */
