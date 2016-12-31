/*
 * Oled.h - Header file of Oled library
 *
 * This library is use for Tiva Arm Cotex M4
 * Device: OLED 1.3", 128x64 dot matrix panel
 * Communication: I2C interface
 * Driver: SH1106
 *
 * Revision: 2.02
 * Author: QUANG
 *
 * Functions that depend on external library
 * 	- Oled_Command
 * 	- Oled_Write
 * 	- Oled_UpdateScreen
 * 	- Oled_Init (Delay function)
 * Fix these functions if run in other platform
 */

#ifndef OLED_H_
#define OLED_H_
#include <stdint.h>
#include <stdbool.h>
#include "font/bitmap_db.h"
#include "font/font_include.h"
//*******************************Definitions***********************************
#define OLED_COLUMNSIZE	128
#define OLED_HEIGHT		64
#define OLED_PAGESIZE	8
// Note: Some Commands is double bytes command. The first byte
// is set mode and the second byte usually the data.
// Check datasheet for more information.
//  (2) stand for double bytes command
//	(2ds) stand for double-byte command and search the second byte prototype
// in datasheet
//  (d) stand for default setting
#define	PUMP_VOLTAGE_7V8							0x30	//(d)
#define PUMP_VOLTAGE_8V								0x31
#define PUMP_VOLTAGE_8V4							0x32
#define PUMP_VOLTAGE_9V								0x33
#define	CONTRAST_CONTROL_MODE						0x81	//(2ds)
#define SEGMENT_REMAP_R								0xA0	//(d)
#define	SEGMENT_REMAP_L								0xA1
#define ENTIRE_DISPLAY_ON							0xA5
#define ENTIRE_DISPLAY_OFF							0xA4	//(d)
#define	NORMAL_DISPLAY								0xA6	//(d)
#define REVERSE_DISPLAY								0xA7
#define MULTIPLEX_RATION_MODE						0xA8	//(2ds)
#define	DC_DC_CONTROL_MODE							0xAD	//(2)
#define DC_DC_CONTROL_MODE_ON						0x8B	//(d)
#define DC_DC_CONTROL_MODE_OFF						0x8A
#define DISPLAY_ON									0xAF
#define DISPLAY_OFF									0xAE	//(d)
#define COMMON_OUTPUT_SCAN_DIRECTION_0				0xC0	//(d)
#define COMMON_OUTPUT_SCAN_DIRECTION_1				0xC8
#define	DISPLAY_OFFSET_MODE							0xD3	//(2ds)
#define DISPLAY_DIVIDE_RATIO_OSC_MODE				0xD5	//(2ds)
#define DISCHARGE_PRECHARGE_PERIOD_MODE				0xD9	//(2ds)
#define	COMMON_PADS_HARDWARE_CONFIG_MODE			0xDA	//(2ds)
#define	COMMON_PADS_HARDWARE_CONFIG_MODE_SEQ		0x02
#define	COMMON_PADS_HARDWARE_CONFIG_MODE_ALT		0x12	//(d)
#define	VCOM_DESELECT_LEVEL_MODE					0xDB	//(2ds)
#define READ_MODIFY_WRITE							0xE0
#define END											0xEE
#define NOP											0xE3
//Enter or exit the sleep mode
#define ENTER										true
#define EXIT										false

/* For Oled_UpdateScreen and Oled_Clear function */
#define WHOLE_SCREEN								0,0,OLED_COLUMNSIZE,OLED_HEIGHT	//update whole screen

/* Oled_Draw8Pixel */
#define VERTICAL									true
#define HORIZONTAL									false

/* Oled_circle.c */
/* Oled_ellipse.c */
#define DRAW_UPPER_RIGHT 0x01
#define DRAW_UPPER_LEFT  0x02
#define DRAW_LOWER_LEFT 0x04
#define DRAW_LOWER_RIGHT  0x08
#define DRAW_ALL (DRAW_UPPER_RIGHT|DRAW_UPPER_LEFT|DRAW_LOWER_RIGHT|DRAW_LOWER_LEFT)

/* Oled_page.c */
//#define USE_MULTI_PAGE
#ifdef USE_MULTI_PAGE
#define NUM_PAGE									3
#endif
//*****************************************************************************

//****************************Function prototypes******************************
void Oled_Command(unsigned char Code);
void Oled_Init(void);
void Oled_Write(unsigned char *data, uint16_t length);
void Oled_SetPosition(uint8_t column_address, uint8_t page_address);
void Oled_Clear(uint8_t ui8Startx, uint8_t ui8Starty,
				uint8_t ui8Width, uint8_t ui8Height);
void Oled_DrawImage(const unsigned char *Image, uint8_t ui8Start_column,		//will be removed in later version
		uint8_t ui8Start_page, uint8_t ui8Column_size, uint8_t ui8Page_size);
void Oled_ReadImage(unsigned char *Image, uint8_t ui8Start_column,
		uint8_t ui8Start_page, uint8_t ui8Column_size, uint8_t ui8Page_size);	//will be removed in later version
																				//and replace read image from buffer
//void Oled_Read(unsigned char *data, uint16_t length);
void Oled_Sleepmode(bool bEnter);
void Oled_Contrast(uint8_t Value);
void Oled_UpdateScreen(uint8_t start_x, uint8_t start_y, uint8_t width, uint8_t height);

void Oled_DrawPixel(uint8_t x, uint8_t y, uint8_t value);
void Oled_Draw8Pixel(uint8_t x, uint8_t y, uint8_t pixel, uint8_t n_pixel, uint8_t dir_v);
void Oled_DrawHLine(uint8_t x, uint8_t y, uint8_t w);
void Oled_DrawVLine(uint8_t x, uint8_t y, uint8_t h);
void Oled_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void Oled_DrawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void Oled_DrawRFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
void Oled_DrawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void Oled_DrawRBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);

void Oled_DrawCircle(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t option);
void Oled_DrawDisc(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t option);
 
void Oled_DrawEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t option);
void Oled_DrawFilledEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t option);

void Oled_DrawPolygon(uint8_t nPoint, ...);
void Oled_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void Oled_DrawFilledPolygon(uint8_t nPoint, ...);
void Oled_DrawFilledTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void Oled_DrawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap);
void Oled_DrawBitmapH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap);

void Oled_SetFont(FONT_INFO *font);
void Oled_printf(uint8_t x, uint8_t y, const char *pcString, ...);

#ifdef USE_MULTI_PAGE
uint8_t Oled_CurentPage(void);
void Oled_FirstPage(void);
void Oled_NextPage(void);
void Oled_PreviousPage(void);
void Oled_gotoPage(uint8_t page);
#endif
//*****************************************************************************
#endif /* OLED_H_ */
