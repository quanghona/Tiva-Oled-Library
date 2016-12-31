/*
 * Oled_bitmap.c - draw bitmap image
 * Oled Graphics library
 * This Library is based on Universal 8-bit Graphics Library
 *
 * This library is use for Tiva Arm Cotex M4
 * Device: OLED 1.3", 128x64 dot matrix panel
 * Communication: SPI interface
 * Driver: SH1106
 *
 * Revision: 2.01
 * Date: 11/11/2016
 * Author: QUANG
 */
 
#include "../Oled.h"

/****************************Function definitions*****************************/

/******************************************************************************
 * Oled_DrawBitmap - draw a bitmap image
 * an image must already be converted to a single dimension array using tools:
 * such as LCD Assistant or LCD Dot Factory.
 *
 * Array setting:
 * 	- Byte orientation: vertical
 * 	- Size endianess: little
 *
 * Parameter:
 * 	(x,y): upper left position of the image
 * 	w	 : image width in pixel
 * 	h	 : image height in pixel
 * Restriction: x+w < Oled's column size
 * 				y+h < Oled's height size
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap)
{
	uint8_t tmp, tmp_x, tmp_pxl;
	
	//sanity check
	if (x+w > OLED_COLUMNSIZE || y+h > OLED_HEIGHT)
		return;
	
	while(h)
	{
		//draw 1 column each loop
		tmp = w;
		tmp_x = x;
		tmp_pxl = (h > 8) ? 8 : h;
		while(tmp--)
		{
			//draw maximum 8 bits at once
			Oled_Draw8Pixel(tmp_x, y, *(bitmap++),tmp_pxl, VERTICAL);
			tmp_x++;
		}

		y += tmp_pxl;
		h -= tmp_pxl;
	}
}

/******************************************************************************
 * Oled_DrawBitmapH - draw a bitmap image
 * an image must already be converted to a single dimension array using tools:
 * such as LCD Assistant or LCD Dot Factory.
 *
 * Array setting:
 * 	- Byte orientation: Horizontal
 * 	- Size endianess: little
 *
 * Parameter:
 * 	(x,y): upper left position of the image
 * 	w	 : image width in pixel
 * 	h	 : image height in pixel
 * Restriction: x+w < Oled's column size
 * 				y+h < Oled's height size
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawBitmapH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap)
{
	uint8_t tmp, tmp_x, tmp_pxl;
	
	//sanity check
	if (x+w > OLED_COLUMNSIZE || y+h > OLED_HEIGHT)
		return;
	
	while(h--)
	{
		//draw 1 line each loop
		tmp = w;
		tmp_x = x;
		while(tmp)
		{
			tmp_pxl =  tmp >= 8 ? 8 : tmp;
			//draw maximum 8 bits at once
			Oled_Draw8Pixel(tmp_x, y, *(bitmap++),tmp_pxl, HORIZONTAL);
			tmp -= tmp_pxl;
			tmp_x += tmp_pxl;
		}
		// u8g_DrawHBitmap(u8g, x, y, cnt, bitmap);

		// bitmap += cnt;
		y++;
		// h--;
	}
}
