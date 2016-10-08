/*
 * Oled.h - Header file of Oled library
 *
 * This library is use for Tiva Arm Cotex M4
 * Device: OLED 1.3", 128x64 dot matrix panel
 * Communication: I2C interface
 * Driver: SH1106
 *
 * Revision: 2.00
 * Author: QUANG
 */

#ifndef OLED_H_
#define OLED_H_
#include <stdint.h>
#include <stdbool.h>
//*******************************Definitions***********************************
#define	OLED_ADD									0x3C	//Oled address

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
//Increase or decrease Oled's contrast
#define INCREASE									true
#define DECREASE									false
//Enter or exit the sleep mode
#define ENTER										true
#define EXIT										false
//*****************************************************************************

//****************************Function prototypes******************************
void Oled_Command(unsigned char Code);
void Oled_Init(void);
void Oled_Write(unsigned char *data, uint16_t length);
void Oled_SetPosition(uint8_t column_address, uint8_t page_address);
void Oled_Clear(uint8_t ui8Start_column, uint8_t ui8Start_page,
		uint8_t ui8Column_size, uint8_t ui8Page_size);
void Oled_Read(unsigned char *data, uint16_t length);
void Oled_printf(const char *pcString, ...);
void Oled_DrawImage(const unsigned char *Image, uint8_t ui8Start_column,
		uint8_t ui8Start_page, uint8_t ui8Column_size, uint8_t ui8Page_size);
void Oled_ReadImage(unsigned char *Image, uint8_t ui8Start_column,
		uint8_t ui8Start_page, uint8_t ui8Column_size, uint8_t ui8Page_size);
void Oled_Sleepmode(bool bEnter);
void Oled_Contrast(bool bInc);
//*****************************************************************************
#endif /* OLED_H_ */
