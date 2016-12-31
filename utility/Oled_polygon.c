/*
 * Oled_polygon.c - Implementation of a polygon drawing algorithm for "convex"
 * polygons.
 * Oled Graphics library
 * This Library is based on Universal 8-bit Graphics Library
 *
 * This library is use for Tiva Arm Cotex M4
 * Device: OLED 1.3", 128x64 dot matrix panel
 * Communication: SPI interface
 * Driver: SH1106
 *
 * Revision: 2.02
 * Date: 11/11/2016
 * Author: QUANG
 */
 
#include "../Oled.h"
#include <stdint.h>
#include <stdarg.h>

/****************************Private Definitions******************************/
#define swap(x,y) tmp=(x);(x)=(y);(y)=tmp

/*************************Private function prototypes*************************/
static void Oled_draw_filled_triangle_section(uint8_t x0, uint8_t y0,
							uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/****************************Function definitions*****************************/

/******************************************************************************
 * Oled_DrawPolygon - draw polygon
 * This function can also draw concave of self-intersection polygon because its
 * points are order-dependence
 *
 * Parameter:
 * 	nPoint: number of points of the polygon
 * 	(xn, yn)...: points's position. 
 *	Restriction: the number of parameters behind nPoint = 2 * nPoint
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawPolygon(uint8_t nPoint, ...)
{
	va_list vaArgP;
	uint8_t x0, y0, x1, y1, x2, y2;
	
	//sanity check
	if (nPoint < 2)
		return;
	
	va_start(vaArgP,nPoint);	// Start the varargs processing.
	//Get the first point of the polygon
	x0 = va_arg(vaArgP,uint8_t);
	y0 = va_arg(vaArgP,uint8_t);
	x2 = x0;
	y2 = y0;
	
	while(--nPoint)
	{
		//Get new point and draw a line defined by 2 recent point
		x1 = x2;
		y1 = y2;
		
		x2 = va_arg(vaArgP,uint8_t);
		y2 = va_arg(vaArgP,uint8_t);
		
		Oled_DrawLine(x1,y1,x2,y2);
	}
	Oled_DrawLine(x0,y0,x2,y2);	//Draw the final line
	
	va_end(vaArgP);		// End the varargs processing.
}

/******************************************************************************
 * Oled_DrawTriangle - draw triangle
 * This is a special case of the draw polygon function
 *
 * Parameter:
 * 	(x0,y0)
 * 	(x1, y1): 3 point of the triangle
 * 	(x2, y2)
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
												 uint8_t x2, uint8_t y2)
{
	Oled_DrawPolygon(3,x0,y0,x1,y1,x2,y2);	//Draw polygon with 3 points
}

/******************************************************************************
 * Oled_DrawFilledTriangle - Draw filled triangle
 *
 * Parameter:
 * 	(x0,y0)
 * 	(x1, y1): 3 point of the triangle
 * 	(x2, y2)
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawFilledTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
							 uint8_t x2, uint8_t y2)
{
	uint8_t tmp;
	
	//sort accending (x0 < x1 < x2) before call the section 
	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}
	if (x0 > x2)
	{
		swap(x0, x2);
		swap(y0, y2);
	}
	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}
	
	Oled_draw_filled_triangle_section(x0, y0, x1, y1, x2, y2);
}

 /*****************************************************************************
 * Oled_DrawPolygon - draw filled polygon
 * This function will draw multiple filled-triangles until the entire polygon
 * is totally filled. Order-dependence, work correctly with convex polygon.
 *
 * Parameter:
 * 	nPoint: number of points of the polygon
 * 	(xn, yn)...: points's position. 
 *	Restriction: the number of parameters behind nPoint = 2 * nPoint
 *
 * Return: none
 ****************************************************************************/
 void Oled_DrawFilledPolygon(uint8_t nPoint, ...)
 {
	va_list vaArgP;
	uint8_t x0, y0, x1, y1, x2, y2;
	
	//sanity check
	if (nPoint < 3)
		return;
	
	va_start(vaArgP,nPoint);	// Start the varargs processing.
	//Get the first 2 points of the polygon
	x0 = va_arg(vaArgP,uint8_t);
	y0 = va_arg(vaArgP,uint8_t);
	x2 = va_arg(vaArgP,uint8_t);
	y2 = va_arg(vaArgP,uint8_t);
	nPoint -= 2;
	
	while(nPoint--)
	{
		//Get new point and draw a filled triangle defined by the first point
		//and 2 recent points
		x1 = x2;
		y1 = y2;
		
		x2 = va_arg(vaArgP,uint8_t);
		y2 = va_arg(vaArgP,uint8_t);
		
		Oled_DrawFilledTriangle(x0,y0,x1,y1,x2,y2);
	}
	
	va_end(vaArgP);		// End the varargs processing.
 }

/******************************************************************************
 * Oled_draw_filled_triangle_section - Draw filled triangle section
 * This algorithm is inherited from algorithm in Oled_DrawLine function and
 * from Bresenham algorithm.
 * This section has 2 steps:
 * 	1. Fill the area inside line ({x0, y0},{x1, y1}) and line
 * 	({x0, y0},{x2, y2})
 * 	2. Fill the area inside line ({x0, y0},{x2, y2}) and line
 * 	({x1, y1},{x2, y2})
 *
 * Parameter:
 * 	(x0, y0)
 * 	(x1, y1) : coordinate of the triangle points
 * 	(x2, y2)
 *
 * Return: none
 *****************************************************************************/
static void Oled_draw_filled_triangle_section(uint8_t x0, uint8_t y0,
						uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	/* TODO: use less variable to save memory resource. Need more optimization */
    uint8_t tmp,tmpy;
	uint8_t x, x_, y, y_;
	uint8_t dx01, dy01, dx02, dy02, dx12, dy12;
	int8_t err01, err02, err12;
	int8_t ystep01, ystep02, ystep12;
    uint8_t x0_ = x0, y0_ = y0, x1_ = x1, y1_ = y1, x2_ = x2, y2_ = y2;
	bool swapxy01 = false, swapxy02 = false, swapxy12 = false;

    /* algorithm initialization */
    //line {x0,y0} {x1, y1}
	dx01 = ( x0 > x1 ) ? (x0-x1) : (x1-x0);
	dy01 = ( y0 > y1 ) ? (y0-y1) : (y1-y0);
	if (dy01 > dx01)
	{
		swapxy01 = true;
		swap(dx01,dy01);
		swap(x1,y1);
		swap(x0,y0);
	}
	if (x1 < x0)
	{
		swap(x1,x0);
		swap(y1,y0);
	}
	err01 = dx01 >> 1;
	ystep01 = (y1 > y0) ?  1 : -1;
	y = y0;

    //line {x0,y0} {x2, y2}
    dx02 = ( x0_ > x2 ) ? (x0_-x2) : (x2-x0_);
	dy02 = ( y0_ > y2 ) ? (y0_-y2) : (y2-y0_);
	if (dy02 > dx02)
	{
		swapxy02 = true;
		swap(dx02,dy02);
		swap(x2,y2);
		swap(x0_,y0_);
	}
	if (x2 < x0_)
	{
		swap(x2,x0_);
		swap(y2,y0_);
	}
	ystep02 = (y2 > y0_) ?  1 : -1;

    //line {x1,y1} {x2, y2}	
    dx12 = (x1_ > x2_) ? (x1_-x2_) : (x2_-x1_);
	dy12 = (y1_ > y2_) ? (y1_-y2_) : (y2_-y1_);
	if (dy12 > dx12)
	{
		swapxy12 = true;
		swap(dx12,dy12);
		swap(x2_,y2_);
		swap(x1_,y1_);
	}
	if (x2_ < x1_)
	{
		swap(x2_,x1_);
		swap(y2_,y1_);
	}
	err12 = dx12 >> 1;
	ystep12 = (y2_ > y1_) ?  1 : -1;

    /* scan from x0 to x1 and draw vertical lines inside 
    /line ({x0, y0},{x1,y1}) and  ({x0, y0}, {x2, y2}) */
	for (x = x0; x <= x1; x++)
	{
		if (!dy01 && swapxy01)	//save time when drawing square triangle
		{
			x_ = x0_;
			y_=y0_;
			break;
		}

        x0 = swapxy01 ? y : x;	//x0 store the current x-coordinate of the line ({x0, y0},{x1,y1})
		y_ = y0_;
		err02 = dx02 >> 1;
        for (x_= x0_;x_ <= x2;x_++)	//the x-coordinate in 2 loops can go in the same or reverse direction
        {
        	tmp = swapxy02 ? y_ : x_;//tmp store the current x-coordinate of the line ({x0, y0},{x2,y2})
	        if (x0 == tmp)	//condition to draw a vertical line
	        {
		        y0 = swapxy01 ? x : y;//y0 store the current y-coordinate of the line ({x0, y0},{x1,y1})
	            tmpy = swapxy02 ? x_ : y_;//tmpy store the current x-coordinate of the line ({x0, y0},{x2,y2})
	            Oled_DrawVLine(x0, (tmpy > y0) ? y0 : tmpy,
	            		(tmpy > y0) ? tmpy-y0 : y0-tmpy);	//get the length of the line
	            break;	//escape the loop if the line had been drawn
	        }
            err02 -= (int8_t)dy02;
            if ( err02 < 0 ) 
            {
              y_ += (uint8_t)ystep02;
              err02 += (uint8_t)dx02;
            }
        }
        //update for next step
		err01 -= (int8_t)dy01;
		if ( err01 < 0 ) 
		{
		  y += (uint8_t)ystep01;
		  err01 += (uint8_t)dx01;
		}
	}

    /* scan from x0 to x1 and draw vertical lines inside 
    line ({x1, y1},{x2,y2}) and  ({x0, y0}, {x2, y2}) */
    y = y1_;
    x0_ = x_;
    y0_ = y_;
	for (x = x1_; x <= x2_; x++)
	{
		if (!dy12 && swapxy12)	//save time when drawing square triangle
			break;

        x0 = swapxy12 ? y : x;	//x0 store the current x-coordinate of the line ({x1, y1},{x2,y2})
        y_ = y0_;
        err02 = dx02 >> 1;
        for (x_ = x0_; x_ <= x2;x_++)	//the x-coordinate in 2 loops can go in the same or reverse direction
        {
    		tmp = swapxy02 ? y_ : x_;//tmp store the current x-coordinate of the line ({x0, y0},{x2,y2})
	        if (x0 == tmp)	//condition to draw a vertical line
	        {
		        y0 = swapxy12 ? x : y;//y0 store the current y-coordinate of the line ({x1, y1},{x2,y2})
	            tmpy = swapxy02 ? x_ : y_;//tmpy store the current x-coordinate of the line ({x0, y0},{x2,y2})
	            Oled_DrawVLine(x0, (tmpy > y0) ? y0 : tmpy,
	            		(tmpy > y0) ? tmpy-y0 : y0-tmpy);	//get the length of the line
	            break;	//escape the loop if the line had been drawn
	        }
            err02 -= (int8_t)dy02;
            if ( err02 < 0 ) 
            {
              y_ += (uint8_t)ystep02;
              err02 += (uint8_t)dx02;
            }
        }
        //update for next step
		err12 -= (int8_t)dy12;
		if ( err12 < 0 ) 
		{
		  y += (uint8_t)ystep12;
		  err12 += (uint8_t)dx12;
		}
	}
}

/* End of Oled_polygon.c */
