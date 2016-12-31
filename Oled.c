/*
 * Oled.c - Function definitions for Oled
 *
 * This library is use for Tiva Arm Cotex M4
 * Device: OLED 1.3", 128x64 dot matrix panel,
 * Communication: SPI interface
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

#include "Oled.h"
#include "spi.h"
#include "../include.h"
//****************************Private Definitions******************************
#define DISPLAY			0x40
#define COMMAND			0

//*********************************Variables***********************************
#ifndef USE_MULTI_PAGE
static uint8_t Oled_buff[OLED_COLUMNSIZE][OLED_PAGESIZE];
#else
static uint8_t Oled_buff_mpg[NUM_PAGE][OLED_COLUMNSIZE][OLED_PAGESIZE];
static uint8_t (*Oled_buff)[OLED_PAGESIZE] = Oled_buff_mpg[0];
static uint8_t page_index = 0;
#endif

static const char * const g_pcHex = "0123456789abcdef";
// ASCII table
const uint8_t ASCII[] ={
		0x00, 0x00, 0x00, 0x00, 0x00, // 20
		0x00, 0x00, 0x5f, 0x00, 0x00, // 21 !
		0x00, 0x07, 0x00, 0x07, 0x00, // 22 "
		0x14, 0x7f, 0x14, 0x7f, 0x14, // 23 #
		0x24, 0x2a, 0x7f, 0x2a, 0x12, // 24 $
		0x23, 0x13, 0x08, 0x64, 0x62, // 25 %
		0x36, 0x49, 0x55, 0x22, 0x50, // 26 &
		0x00, 0x05, 0x03, 0x00, 0x00, // 27 '
		0x00, 0x1c, 0x22, 0x41, 0x00, // 28 (
		0x00, 0x41, 0x22, 0x1c, 0x00, // 29 )
		0x14, 0x08, 0x3e, 0x08, 0x14, // 2a *
		0x08, 0x08, 0x3e, 0x08, 0x08, // 2b +
		0x00, 0x50, 0x30, 0x00, 0x00, // 2c ,
		0x08, 0x08, 0x08, 0x08, 0x08, // 2d -
		0x00, 0x60, 0x60, 0x00, 0x00, // 2e .
		0x20, 0x10, 0x08, 0x04, 0x02, // 2f /
		0x3e, 0x51, 0x49, 0x45, 0x3e, // 30 0
		0x00, 0x42, 0x7f, 0x40, 0x00, // 31 1
		0x42, 0x61, 0x51, 0x49, 0x46, // 32 2
		0x21, 0x41, 0x45, 0x4b, 0x31, // 33 3
		0x18, 0x14, 0x12, 0x7f, 0x10, // 34 4
		0x27, 0x45, 0x45, 0x45, 0x39, // 35 5
		0x3c, 0x4a, 0x49, 0x49, 0x30, // 36 6
		0x01, 0x71, 0x09, 0x05, 0x03, // 37 7
		0x36, 0x49, 0x49, 0x49, 0x36, // 38 8
		0x06, 0x49, 0x49, 0x29, 0x1e, // 39 9
		0x00, 0x36, 0x36, 0x00, 0x00, // 3a :
		0x00, 0x56, 0x36, 0x00, 0x00, // 3b ;
		0x08, 0x14, 0x22, 0x41, 0x00, // 3c <
		0x14, 0x14, 0x14, 0x14, 0x14, // 3d =
		0x00, 0x41, 0x22, 0x14, 0x08, // 3e >
		0x02, 0x01, 0x51, 0x09, 0x06, // 3f ?
		0x32, 0x49, 0x79, 0x41, 0x3e, // 40 @
		0x7e, 0x11, 0x11, 0x11, 0x7e, // 41 A
		0x7f, 0x49, 0x49, 0x49, 0x36, // 42 B
		0x3e, 0x41, 0x41, 0x41, 0x22, // 43 C
		0x7f, 0x41, 0x41, 0x22, 0x1c, // 44 D
		0x7f, 0x49, 0x49, 0x49, 0x41, // 45 E
		0x7f, 0x09, 0x09, 0x09, 0x01, // 46 F
		0x3e, 0x41, 0x49, 0x49, 0x7a, // 47 G
		0x7f, 0x08, 0x08, 0x08, 0x7f, // 48 H
		0x00, 0x41, 0x7f, 0x41, 0x00, // 49 I
		0x20, 0x40, 0x41, 0x3f, 0x01, // 4a J
		0x7f, 0x08, 0x14, 0x22, 0x41, // 4b K
		0x7f, 0x40, 0x40, 0x40, 0x40, // 4c L
		0x7f, 0x02, 0x0c, 0x02, 0x7f, // 4d M
		0x7f, 0x04, 0x08, 0x10, 0x7f, // 4e N
		0x3e, 0x41, 0x41, 0x41, 0x3e, // 4f O
		0x7f, 0x09, 0x09, 0x09, 0x06, // 50 P
		0x3e, 0x41, 0x51, 0x21, 0x5e, // 51 Q
		0x7f, 0x09, 0x19, 0x29, 0x46, // 52 R
		0x46, 0x49, 0x49, 0x49, 0x31, // 53 S
		0x01, 0x01, 0x7f, 0x01, 0x01, // 54 T
		0x3f, 0x40, 0x40, 0x40, 0x3f, // 55 U
		0x1f, 0x20, 0x40, 0x20, 0x1f, // 56 V
		0x3f, 0x40, 0x38, 0x40, 0x3f, // 57 W
		0x63, 0x14, 0x08, 0x14, 0x63, // 58 X
		0x07, 0x08, 0x70, 0x08, 0x07, // 59 Y
		0x61, 0x51, 0x49, 0x45, 0x43, // 5a Z
		0x00, 0x7f, 0x41, 0x41, 0x00, // 5b [
		0x02, 0x04, 0x08, 0x10, 0x20, // 5c ¥
		0x00, 0x41, 0x41, 0x7f, 0x00, // 5d ]
		0x04, 0x02, 0x01, 0x02, 0x04, // 5e ^
		0x40, 0x40, 0x40, 0x40, 0x40, // 5f _
		0x00, 0x01, 0x02, 0x04, 0x00, // 60 `
		0x20, 0x54, 0x54, 0x54, 0x78, // 61 a
		0x7f, 0x48, 0x44, 0x44, 0x38, // 62 b
		0x38, 0x44, 0x44, 0x44, 0x20, // 63 c
		0x38, 0x44, 0x44, 0x48, 0x7f, // 64 d
		0x38, 0x54, 0x54, 0x54, 0x18, // 65 e
		0x08, 0x7e, 0x09, 0x01, 0x02, // 66 f
		0x0c, 0x52, 0x52, 0x52, 0x3e, // 67 g
		0x7f, 0x08, 0x04, 0x04, 0x78, // 68 h
		0x00, 0x44, 0x7d, 0x40, 0x00, // 69 i
		0x20, 0x40, 0x44, 0x3d, 0x00, // 6a j
		0x7f, 0x10, 0x28, 0x44, 0x00, // 6b k
		0x00, 0x41, 0x7f, 0x40, 0x00, // 6c l
		0x7c, 0x04, 0x18, 0x04, 0x78, // 6d m
		0x7c, 0x08, 0x04, 0x04, 0x78, // 6e n
		0x38, 0x44, 0x44, 0x44, 0x38, // 6f o
		0x7c, 0x14, 0x14, 0x14, 0x08, // 70 p
		0x08, 0x14, 0x14, 0x18, 0x7c, // 71 q
		0x7c, 0x08, 0x04, 0x04, 0x08, // 72 r
		0x48, 0x54, 0x54, 0x54, 0x20, // 73 s
		0x04, 0x3f, 0x44, 0x40, 0x20, // 74 t
		0x3c, 0x40, 0x40, 0x20, 0x7c, // 75 u
		0x1c, 0x20, 0x40, 0x20, 0x1c, // 76 v
		0x3c, 0x40, 0x30, 0x40, 0x3c, // 77 w
		0x44, 0x28, 0x10, 0x28, 0x44, // 78 x
		0x0c, 0x50, 0x50, 0x50, 0x3c, // 79 y
		0x44, 0x64, 0x54, 0x4c, 0x44, // 7a z
		0x00, 0x08, 0x36, 0x41, 0x00, // 7b {
		0x00, 0x00, 0x7f, 0x00, 0x00, // 7c |
		0x00, 0x41, 0x36, 0x08, 0x00, // 7d }
		0x10, 0x08, 0x08, 0x10, 0x08, // 7e ->
		0x78, 0x46, 0x41, 0x46, 0x78 // 7f <-
};
const FONT_CHAR_INFO ASCIIDescriptors[] = {
	{5, 0}, 		/*   */ 
	{5, 5}, 		/* ! */ 
	{5, 10}, 		/* " */ 
	{5, 15}, 		/* # */ 
	{5, 20}, 		/* $ */ 
	{5, 25}, 		/* % */ 
	{5, 30}, 		/* & */ 
	{5, 35}, 		/* ' */ 
	{5, 40}, 		/* ( */ 
	{5, 45}, 		/* ) */ 
	{5, 50}, 		/* * */ 
	{5, 55}, 		/* + */ 
	{5, 60}, 		/* , */ 
	{5, 65}, 		/* - */ 
	{5, 70}, 		/* . */ 
	{5, 75}, 		/* / */ 
	{5, 80}, 		/* 0 */ 
	{5, 85}, 		/* 1 */ 
	{5, 90}, 		/* 2 */ 
	{5, 95}, 		/* 3 */ 
	{5, 100}, 		/* 4 */ 
	{5, 105}, 		/* 5 */ 
	{5, 110}, 		/* 6 */ 
	{5, 115}, 		/* 7 */ 
	{5, 120}, 		/* 8 */ 
	{5, 125}, 		/* 9 */ 
	{5, 130}, 		/* : */ 
	{5, 135}, 		/* ; */ 
	{5, 140}, 		/* < */ 
	{5, 145}, 		/* = */ 
	{5, 150}, 		/* > */ 
	{5, 155}, 		/* ? */ 
	{5, 160}, 		/* @ */ 
	{5, 165}, 		/* A */ 
	{5, 170}, 		/* B */ 
	{5, 175}, 		/* C */ 
	{5, 180}, 		/* D */ 
	{5, 185}, 		/* E */ 
	{5, 190}, 		/* F */ 
	{5, 195}, 		/* G */ 
	{5, 200}, 		/* H */ 
	{5, 205}, 		/* I */ 
	{5, 210}, 		/* J */ 
	{5, 215}, 		/* K */ 
	{5, 220}, 		/* L */ 
	{5, 225}, 		/* M */ 
	{5, 230}, 		/* N */ 
	{5, 235}, 		/* O */ 
	{5, 240}, 		/* P */ 
	{5, 245}, 		/* Q */ 
	{5, 250}, 		/* R */ 
	{5, 255}, 		/* S */ 
	{5, 260}, 		/* T */ 
	{5, 265}, 		/* U */ 
	{5, 270}, 		/* V */ 
	{5, 275}, 		/* W */ 
	{5, 280}, 		/* X */ 
	{5, 285}, 		/* Y */ 
	{5, 290}, 		/* Z */ 
	{5, 295}, 		/* [ */ 
	{5, 300}, 		/* \ */ 
	{5, 305}, 		/* ] */ 
	{5, 310}, 		/* ^ */ 
	{5, 315}, 		/* _ */ 
	{5, 320}, 		/* ` */ 
	{5, 325}, 		/* a */ 
	{5, 330}, 		/* b */ 
	{5, 335}, 		/* c */ 
	{5, 340}, 		/* d */ 
	{5, 345}, 		/* e */ 
	{5, 350}, 		/* f */ 
	{5, 355}, 		/* g */ 
	{5, 360}, 		/* h */ 
	{5, 365}, 		/* i */ 
	{5, 370}, 		/* j */ 
	{5, 375}, 		/* k */ 
	{5, 380}, 		/* l */ 
	{5, 385}, 		/* m */ 
	{5, 390}, 		/* n */ 
	{5, 395}, 		/* o */ 
	{5, 400}, 		/* p */ 
	{5, 405}, 		/* q */ 
	{5, 410}, 		/* r */ 
	{5, 415}, 		/* s */ 
	{5, 420}, 		/* t */ 
	{5, 425}, 		/* u */ 
	{5, 430}, 		/* v */ 
	{5, 435}, 		/* w */ 
	{5, 440}, 		/* x */ 
	{5, 445}, 		/* y */ 
	{5, 450}, 		/* z */ 
	{5, 455}, 		/* { */ 
	{5, 460}, 		/* | */ 
	{5, 465}, 		/* } */ 
	{5, 470}, 		/* -> */
	{5, 475}			/* <- */
};
const FONT_INFO fi_default = {8, ' ', 0x7F, ASCIIDescriptors, ASCII};
static FONT_INFO *CurrentFont = &fi_default;	//default: ASCII 5x8
static uint8_t currentX = 0, currentY = 0;	//for print text

//*************************Private function prototypes*************************
static void Oled_Putchar(char c);
static void Oled_Putstring(const char *pcBuf, uint8_t ui8Len);
static void Oled_Draw8PixelV(uint8_t x, uint8_t y, uint8_t pixel, uint8_t n_pixel);
static void Oled_Draw8PixelH(uint8_t x, uint8_t y, uint8_t pixel, uint8_t n_pixel);
// static void Oled_UpdateScreen(void);
/* TODO: put Update screen to private scope */

//****************************Function definitions*****************************

/******************************************************************************
 * This function is to write commands to SH1106 driver to control the Oled
 *
 * Parameter:
 *   Code  : command code, check datasheet sh1106 for more information
 *
 * Return: none
 *****************************************************************************/
void Oled_Command(unsigned char Code)
{
	SH1106_DC_LOW();
	SH1106_CS_LOW();
	SPI_SendByte(Code);
	SH1106_CS_HIGH();
	SysCtlDelay(10);
}

/******************************************************************************
 * Initialize the Oled
 * 	- Configure the default setting
 *	- Turn on the display
 *	- Clear entire display
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void Oled_Init(void)
{
	SysCtlDelay(ROM_SysCtlClockGet()/2500);		//Power on time
//	Oled_Command(SEGMENT_REMAP_L);
	Oled_Command(COMMON_PADS_HARDWARE_CONFIG_MODE);
	Oled_Command(0x12);
//	Oled_Command(COMMON_OUTPUT_SCAN_DIRECTION_1);
	Oled_Command(NORMAL_DISPLAY);
	Oled_Command(MULTIPLEX_RATION_MODE);
	Oled_Command(0x3F);
	Oled_Command(DISPLAY_DIVIDE_RATIO_OSC_MODE);
	Oled_Command(0x50);
	Oled_Command(VCOM_DESELECT_LEVEL_MODE);
	Oled_Command(0x35);
	Oled_Command(CONTRAST_CONTROL_MODE);
//	Oled_Command(ui32Contrast);
	Oled_Command(PUMP_VOLTAGE_7V8);
	Oled_Command(DC_DC_CONTROL_MODE);
	Oled_Command(DC_DC_CONTROL_MODE_ON);
	SysCtlDelay(ROM_SysCtlClockGet()/30);

	//Set start line for COM0 -> 0
	Oled_Command(0x40);

	Oled_Clear(0, 0, OLED_COLUMNSIZE, OLED_PAGESIZE);

	//Turn on the display
	Oled_Command(DISPLAY_ON);
	Oled_Command(COMMON_OUTPUT_SCAN_DIRECTION_1);
	Oled_Command(SEGMENT_REMAP_L);
	SysCtlDelay(ROM_SysCtlClockGet()/60);
}

/******************************************************************************
 * Write the display data to Oled
 * Before using this function, we should use Oled_SetPosition to set the
 * position of where we gonna write the display data
 *
 * Parameter:
 * 	data  : display data
 * 	length: number of display bytes
 *
 * Return: none
 *****************************************************************************/
void Oled_Write(unsigned char *data, uint16_t length)
{
	 unsigned int i;

	 SH1106_DC_HIGH();
	 SH1106_CS_LOW();

	 for (i = 0; i < length; i++)
		 SPI_SendByte(data[i]);
	
	 SH1106_CS_HIGH();
//	 SysCtlDelay(length * 2);
}

/******************************************************************************
 * Set the pointer's current position for display data
 * This function is usually be used before Oled_Write/Oled_Read to determine
 * the position before write/read display data
 *
 * Parameter:
 * 	column_address: range [0 127]
 * 	page_address  : range [0 7]
 * 	The actual number of column is 132 but because of some issue the column 0, 1,
 * 130 and 131 do not work, so the display size become 128x64.
 *
 * Return: none
 *****************************************************************************/
void Oled_SetPosition(uint8_t column_address, uint8_t page_address)
{
	//plus 2 to the column_address to match with the display column
	Oled_Command(page_address | 0xB0);
	Oled_Command((column_address + 2) & 0x0F);
	Oled_Command(0x10 | ((column_address + 2) >> 4));
}

/******************************************************************************
 * Clear specific area of the Oled
 * Simply write 0x00 to specific area buffer
 *
 * Tips: to clear whole screen, use
 *  Oled_Clear(WHOLE_SCREEN);
 *
 * Parameter:
 * 	ui8Startx	: The upper left x-coordinate of the area to erase
 * 	ui8Starty	: The upper left y-coordinate of the area to erase
 * 	ui8Width	: the area's width
 * 	ui8Height	: the area's height
 *
 * Return: none
 *****************************************************************************/
void Oled_Clear(uint8_t ui8Startx, uint8_t ui8Starty,
				uint8_t ui8Width, uint8_t ui8Height)
{
	// unsigned char ui8clear[128];
	// unsigned char ui8index;
	// //check condition
	// if (((ui8Start_column + ui8Column_size) > OLED_COLUMNSIZE)
	// 	|| ((ui8Start_page + ui8Page_size) > OLED_PAGESIZE))
	// 	return;

	// for(ui8index = 0; ui8index < ui8Column_size; ui8index++)
	// 	ui8clear[ui8index] = 0;
	// //Write 0x00 to entire RAM
	// for(ui8index = ui8Start_page; ui8index < (ui8Start_page + ui8Page_size); ui8index++)
	// {
	// 	Oled_SetPosition(ui8Start_column, ui8index);
	// 	Oled_Write(ui8clear, ui8Column_size);
	// }
	// SysCtlDelay(1000);
	uint8_t tmp, tmp_x, tmp_pxl;
	
	//sanity check
	if (ui8Startx+ui8Width > OLED_COLUMNSIZE || ui8Starty+ui8Height > OLED_HEIGHT)
		return;
	
	while(ui8Height)
	{
		//draw 1 column each loop
		tmp = ui8Width;
		tmp_x = ui8Startx;
		tmp_pxl = (ui8Height > 8) ? 8 : ui8Height;
		while(tmp--)
		{
			//draw maximum 8 bits at once
			Oled_Draw8Pixel(tmp_x, ui8Starty, 0x00,tmp_pxl, VERTICAL);
			tmp_x++;
		}

		ui8Starty += tmp_pxl;
		ui8Height -= tmp_pxl;
	}
}

/******************************************************************************
 * Read the Oled's display RAM
 * Before using this function, we should use Oled_SetPosition
 * to set the position of where we gonna read the display data
 *
 * Parameter:
 * 	data  : the variable that using for saving the data
 * 	length: the number of byte we want to read
 *
 * Return: none
 *****************************************************************************/
//void Oled_Read(unsigned char *data, uint16_t length)
//{
//	I2C_Read(I2C_OLED_BASE, OLED_ADD, data, length, DISPLAY, true);
//	SysCtlDelay(1000);
//}

/******************************************************************************
 * Oled_SetFont - Set font for printing text
 *
 * Parameter:
 * 	font: pointer to font information
 *
 * Return: none
 *****************************************************************************/
void Oled_SetFont(FONT_INFO *font)
{
	CurrentFont = font;
}

/******************************************************************************
 * A simple Oled based printf function supporting \%c, \%d, \%p, \%s, \%u,
 * \%x, and \%X.
 *
 * Parameter:
 * 	pcString is the format string.
 * 	param ... are the optional arguments, which depend on the contents of the
 * format string.
 *
 * This function is very similar to the C library <tt>fprintf()</tt> function.
 * All of its output will be sent to the Oled.  Only the following formatting
 * characters are supported:
 *
 * - \%c to print a character
 * - \%d or \%i to print a decimal value
 * - \%s to print a string
 * - \%u to print an unsigned decimal value
 * - \%x to print a hexadecimal value using lower case letters
 * - \%X to print a hexadecimal value using lower case letters (not upper case
 * letters as would typically be used)
 * - \%p to print a pointer as a hexadecimal value
 * - \%\% to print out a \% character
 *
 * For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
 * between the \% and the format character, which specifies the minimum number
 * of characters to use for that value; if preceded by a 0 then the extra
 * characters will be filled with zeros instead of spaces.  For example,
 * ``\%8d'' will use eight characters to print the decimal value with spaces
 * added to reach eight; ``\%08d'' will use eight characters as well but will
 * add zeroes instead of spaces.
 *
 * The type of the arguments after \e pcString must match the requirements of
 * the format string.  For example, if an integer was passed where a string
 * was expected, an error of some kind will most likely occur.
 *
 * Return: none.
 *****************************************************************************/
void Oled_printf(uint8_t x, uint8_t y, const char *pcString, ...)
{
	static unsigned int ulPos, ulCount, ulNeg;
	unsigned long ulValue, ulBase, ulIdx;
	char *pcStr, pcBuf[16], cFill;
	va_list vaArgP;

	currentX = x;
	currentY = y;
	va_start(vaArgP, pcString);		// Start the varargs processing.

	// Loop while there are more characters in the string.
	while(*pcString)
	{
		for(ulIdx = 0; (pcString[ulIdx] != '%') && (pcString[ulIdx] != '\0');
				ulIdx++);		// Find the first non-% character, or the end of the string.

		Oled_Putstring(pcString, ulIdx);		// Write this portion of the string.

		pcString += ulIdx;		// Skip the portion of the string that was written.

		if(*pcString == '%')		// See if the next character is a %.
		{
			pcString++;			// Skip the %.

			// Set the digit count to zero, and the fill character to space
			// (i.e. to the defaults).
			ulCount = 0;
			cFill = ' ';

			// It may be necessary to get back here to process more characters.
			// Goto's aren't pretty, but effective.  I feel extremely dirty for
			// using not one but two of the beasts.
			again:

			// Determine how to handle the next character.
			switch(*pcString++)
			{
			// Handle the digit characters.
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			{
				// If this is a zero, and it is the first digit, then the
				// fill character is a zero instead of a space.
				if((pcString[-1] == '0') && (ulCount == 0))
					cFill = '0';

				// Update the digit count.
				ulCount *= 10;
				ulCount += pcString[-1] - '0';

				// Get the next character.
				goto again;
			}

			// Handle the %c command.
			case 'c':
			{
				ulValue = va_arg(vaArgP, unsigned long);	// Get the value from the varargs.
				Oled_Putstring((char *)&ulValue, 1);		// Print out the character.

				// This command has been handled.
				break;
			}

			// Handle the %d and %i commands.
			case 'd':
			case 'i':
			{
				ulValue = va_arg(vaArgP, unsigned long);		// Get the value from the varargs.
				ulPos = 0;		// Reset the buffer position.

				// If the value is negative, make it positive and indicate
				// that a minus sign is needed.
				if((long)ulValue < 0)
				{
					ulValue = -(long)ulValue;			// Make the value positive.
					ulNeg = 1;	// Indicate that the value is negative.
				}
				else
				{
					// Indicate that the value is positive so that a minus
					// sign isn't inserted.
					ulNeg = 0;
				}

				ulBase = 10;		// Set the base to 10.

				// Convert the value to ASCII.
				goto convert;
			}

			// Handle the %s command.
			case 's':
			{
				pcStr = va_arg(vaArgP, char *);		// Get the string pointer from the varargs.

				for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++);		// Determine the length of the string.
				Oled_Putstring(pcStr, ulIdx);		// Write the string.

				// Write any required padding spaces
				if(ulCount > ulIdx)
				{
					ulCount -= ulIdx;
					while(ulCount--)
						Oled_Putstring(" ", 1);
				}
				// This command has been handled.
				break;
			}

			// Handle the %u command.
			case 'u':
			{
				ulValue = va_arg(vaArgP, unsigned long);	// Get the value from the varargs.
				ulPos = 0;			// Reset the buffer position.
				ulBase = 10;		// Set the base to 10.

				// Indicate that the value is positive so that a minus sign
				// isn't inserted.
				ulNeg = 0;

				goto convert;		// Convert the value to ASCII.
			}

			// Handle the %x and %X commands.  Note that they are treated
			// identically; i.e. %X will use lower case letters for a-f
			// instead of the upper case letters is should use.  We also
			// alias %p to %x.
			case 'x':
			case 'X':
			case 'p':
			{
				ulValue = va_arg(vaArgP, unsigned long);		// Get the value from the varargs.
				ulPos = 0;		// Reset the buffer position.
				ulBase = 16;			// Set the base to 16.

				// Indicate that the value is positive so that a minus sign
				// isn't inserted.
				ulNeg = 0;

				// Determine the number of digits in the string version of
				// the value.
				convert:
				for(ulIdx = 1;
						(((ulIdx * ulBase) <= ulValue) &&
								(((ulIdx * ulBase) / ulBase) == ulIdx));
						ulIdx *= ulBase, ulCount--);

				// If the value is negative, reduce the count of padding
				// characters needed.
				if(ulNeg)
					ulCount--;

				// If the value is negative and the value is padded with
				// zeros, then place the minus sign before the padding.
				if(ulNeg && (cFill == '0'))
				{
					pcBuf[ulPos++] = '-';			// Place the minus sign in the output buffer.

					// The minus sign has been placed, so turn off the
					// negative flag.
					ulNeg = 0;
				}

				// Provide additional padding at the beginning of the
				// string conversion if needed.
				if((ulCount > 1) && (ulCount < 16))
				{
					for(ulCount--; ulCount; ulCount--)
						pcBuf[ulPos++] = cFill;
				}

				// If the value is negative, then place the minus sign
				// before the number.
				if(ulNeg)
					pcBuf[ulPos++] = '-';			// Place the minus sign in the output buffer.

				// Convert the value into a string.
				for(; ulIdx; ulIdx /= ulBase)
					pcBuf[ulPos++] = g_pcHex[(ulValue / ulIdx) % ulBase];

				Oled_Putstring(pcBuf, ulPos);		// Write the string.

				// This command has been handled.
				break;
			}

			// Handle the %% command.
			case '%':
			{
				Oled_Putstring(pcString - 1, 1);		// Simply write a single %.

				// This command has been handled.
				break;
			}

			// Handle all other commands.
			default:
			{
				Oled_Putstring("ERROR", 5);		// Indicate an error.

				// This command has been handled.
				break;
			}
			}
		}
	}
	va_end(vaArgP);	// End the varargs processing.
}

/******************************************************************************
 * Draw a given image
 * Image format: An 1 dimension array. Each element in array is a 1-byte constant.
 * Assuming that every 7 rows = 1 page. The format is given below:
 *
 * In 1 byte:
 * 			|d0|
 * 			|d1|
 * 			...
 * 			|d7|
 *
 * whole image:
 * 		     Column(0)	 Column(1)	...	  Column(m)
 * 		   -------------------------------------------
 * page(0) |  byte[0]  |  byte[1]  |...|	byte[m]  |
 * page(1) |byte[m + 1]|byte[m + 2]|	...	  		 |
 * ...	   |					...				     |
 * page(n) |		...			 	   |byte[m*n - 1]|
 * 		   -------------------------------------------
 *
 * Parameter:
 * 	*Image			: A constant pointer to an image
 * 	ui8Start_column	: The first left column of an image
 * 	ui8Start_page	: The first top page of an image
 * 	ui8Column_size	: Image's column size
 * 	ui8Page_size	: Image's page size
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawImage(const unsigned char *Image, uint8_t ui8Start_column, uint8_t ui8Start_page,
		uint8_t ui8Column_size, uint8_t ui8Page_size)
{
	uint8_t ui8Pagecount;
	uint16_t ui16ImagePointer = 0;
	
	if (((ui8Start_column + ui8Column_size) > OLED_COLUMNSIZE)
		|| ((ui8Start_page + ui8Page_size) > OLED_PAGESIZE))
		return;

	for(ui8Pagecount = 0; ui8Pagecount < ui8Page_size; ui8Pagecount++)
	{
		//Set the position at the first column and a page which is prepared to draw
		Oled_SetPosition(ui8Start_column, ui8Start_page + ui8Pagecount);
		Oled_Write(&Image[ui16ImagePointer], ui8Column_size);				//Draw
		ui16ImagePointer += (uint16_t)ui8Column_size;						//Update the image pointer
	}

	// SysCtlDelay(1000);		//Wait a little time for system to complete the task
}

//void Oled_ReadImage(unsigned char *Image, uint8_t ui8Start_column,
//		uint8_t ui8Start_page, uint8_t ui8Column_size, uint8_t ui8Page_size)
//{
//	uint8_t ui8Pagecount;
//	uint16_t ui16ImagePointer = 0;
//
//	if (((ui8Start_column + ui8Column_size) > OLED_COLUMNSIZE)
//		|| ((ui8Start_page + ui8Page_size) > OLED_PAGESIZE))
//		return;
//
//	for(ui8Pagecount = 0; ui8Pagecount < ui8Page_size; ui8Pagecount++)
//	{
//		//Set the position at the first column and a page which is prepared to read
//		Oled_SetPosition(ui8Start_column, ui8Start_page + ui8Pagecount);
//		Oled_Read(&Image[ui16ImagePointer], ui8Column_size);				//Read
//		ui16ImagePointer += (uint16_t)ui8Column_size;						//Update the image pointer
//	}
//}
/******************************************************************************
 * Make Oled enter the sleep mode to save power
 * Simply turn off the display
 *
 * Parameter:
 * 	bEnter:
 * 		ENTER (true): enter the sleep mode
 * 		EXIT (false): exit the sleep mode
 *
 * Return: none
 *****************************************************************************/
void Oled_Sleepmode(bool bEnter)
{
	Oled_Command(DISPLAY_OFF | (unsigned char)(!bEnter));
}

/******************************************************************************
 * Increase or decrease the Oled's contrast
 * There are 256 contrast steps from 0 to 0xFF
 * The default contrast value when first time running is 0x80
 *
 * Parameter:
 * 	Value: The contrast value (0 - 255)
 *
 * Return: none
 *****************************************************************************/
void Oled_Contrast(uint8_t Value)
{
	Oled_Command(CONTRAST_CONTROL_MODE);

	// if (bInc)
	// {
		// if(ui32Contrast < 255)
			// ui32Contrast++;
	// }
	// else
	// {
		// if(ui32Contrast > 0)
			// ui32Contrast--;
	// }
	Oled_Command(Value);
}

/******************************************************************************
 * Read status from the SH1106 driver
 * Check SH1106 datasheet for more information
 *
 * Parameter: none
 *
 * Return: a status byte
 * 	bit 7: BUSY. When high, the SH1106 is busy due to internal operation or reset.
 * 				 Any command is reject when the driver is busy.
 * 	bit 6: ON/OFF. When low, the display is turn on and vice versa.
 * 	bits 5 to 0: Don't care
 *****************************************************************************/
//static uint8_t Oled_ReadStatus(void)
//{
//	uint8_t status;
//
//	I2C_Read(I2C_OLED_BASE, OLED_ADD, &status, 1, COMMAND);
//	return status;
//}

/******************************************************************************
 * Print a single char to Oled
 * char size: 6x8
 *
 * Parameter:
 * 	c: an ASCII character
 *
 * Return: none
 *****************************************************************************/
static void Oled_Putchar(char c)
{
	if((c >= 0x20) && (c <= 0x7f))
	{
		// char i;
		c -= CurrentFont->startChar;
		// for(i = 0; i < 6; i++)
			// Oled_Write(&ASCII[c][i], 1);
		//treat character as a image whose size determined by their descriptor
		Oled_DrawBitmap(currentX, currentY, 1,CurrentFont->heightPages,
								&CurrentFont->data[0]);//add 1-pixel-wide space
		Oled_DrawBitmap(currentX+1, currentY, CurrentFont->charInfo[c].widthBits,
							  CurrentFont->heightPages,
						&CurrentFont->data[CurrentFont->charInfo[c].offset]);
	}
	currentX += 1 + CurrentFont->charInfo[c].widthBits;
}

/******************************************************************************
 * Print a string to Oled
 *
 * Parameter:
 * 	pcBuf :  input string. Note that this is a constant string
 * 	ui8Len: Length of a string
 *
 * Return: none
 *****************************************************************************/
static void Oled_Putstring(const char *pcBuf, uint8_t ui8Len)
{
	uint8_t i;
	for(i = 0; i < ui8Len; i++)
	{
		Oled_Putchar(*pcBuf);
		pcBuf++;
	}
}

/******************************************************************************
 * Update part of the screen from internal buffer to Oled
 * Tips: to Update whole screen, use:
 * 	Oled_UpdateScreen(WHOLE_SCREEN);
 *
 * Parameter:
 * - start_x, start_y: upper left pixel
 * - width: width of the update area (bit)
 * - height: height of the update area (bit)
 *
 * Return: none
 *****************************************************************************/
// static void Oled_UpdateScreen(void)
void Oled_UpdateScreen(uint8_t start_x, uint8_t start_y, uint8_t width, uint8_t height)
{
	 uint8_t i, j;

	 // The screen is actually updated from the page start_y/8 to (start_y + height)/8 + 1
	 //page since the Oled hardware is page orientation. (1 page = 8 line)
	 for (i = start_y/8; i < (height/8) ; i++)	//convert to byte-based data
	 {
		 Oled_SetPosition(start_x,i);
		 SH1106_DC_HIGH();
		 SH1106_CS_LOW();
		 for(j = start_x; j < (start_x + width); j++)
		 {
			 SPI_SendByte(Oled_buff[j][i]);
		 }
	 }
	 SH1106_CS_HIGH();
}

/******************************************************************************
 * Oled_DrawPixel - Draw a pixel to screen buffer
 *
 * Parameter:
 * 	(x, y): pixel position
 * 	value:
 * 		- 0: clear the pixel
 * 		- 1: set the pixel
 *
 * Return: none
 *****************************************************************************/
void Oled_DrawPixel(uint8_t x, uint8_t y, uint8_t value)
{
	value ? (Oled_buff[x][y / 8] |= 1 << (y % 8)):
			(Oled_buff[x][y / 8] &= ~(1 << (y % 8)));
}

/******************************************************************************
 * Oled_Draw8Pixel - Draw maximum 8 pixel to screen buffer
 *
 * Parameter:
 * 	(x, y): first pixel position
 * 	pixel: pixels value
 * 	n_pixel: number of pixel to draw (0 to 8)
 * 	dir: drawing direction
 * 		- VERTICAL: vertical direction (pixel: up->down: LSB->MSB)
 * 		- HORIZONTAL: horizontal direction (pixel: left->right: MSB->LSB)
 *
 * Return: none
 *****************************************************************************/
void Oled_Draw8Pixel(uint8_t x, uint8_t y, uint8_t pixel, uint8_t n_pixel, uint8_t dir_v)
{
	if (n_pixel)
		dir_v ? Oled_Draw8PixelV(x, y, pixel, n_pixel):
				  Oled_Draw8PixelH(x, y, pixel, n_pixel);
}

/******************************************************************************
 * Oled_Draw8PixelV - draw maximum 8 pixels in vertical direction
 *
 * Parameter:
 * 	(x, y): first pixel position
 * 	pixel : 8 pixels value (up->down: LSB->MSB)
 * 	n_pixel: number of pixel to draw (1 to 8)
 *
 * Return: none
 *****************************************************************************/
static void Oled_Draw8PixelV(uint8_t x, uint8_t y, uint8_t pixel, uint8_t n_pixel)
{
	uint8_t ymod8 = y % 8;
	uint8_t tmp = 0xFF << ymod8;
	uint8_t tmp_y = ymod8 + n_pixel;
	
	if (tmp_y <= 8)	//all the pixels are at the same page
	{
		tmp = (tmp << (8 - tmp_y));
		tmp >>= 8 - tmp_y;
	}
	Oled_buff[x][y / 8] &= ~tmp;
	Oled_buff[x][y / 8] |= (pixel << ymod8) & tmp;
	if (tmp_y <= 8)
		return;
	//case the remain pixel(s) cross the next page
	n_pixel -= 8 - ymod8;
	tmp = 0xFF >> (8 - n_pixel);
	Oled_buff[x][(y / 8) + 1] &= ~tmp;
	Oled_buff[x][(y / 8) + 1] |= (pixel >> (8 - ymod8)) & tmp;
}

/******************************************************************************
 * Oled_Draw8PixelH - draw maximum 8 pixels in horizontal direction
 *
 * Parameter:
 * 	(x, y): first pixel position
 * 	pixel : 8 pixels value (left->right: MSB->LSB)
 * 	n_pixel: number of pixel to draw (1 to 8)
 *
 * Return: none
 *****************************************************************************/
static void Oled_Draw8PixelH(uint8_t x, uint8_t y, uint8_t pixel, uint8_t n_pixel)
{
	uint8_t i;	//consider using "register" type variable
	
	for (i = 0; i < n_pixel; i++, x++)	//draw 1 pixel each loop
		Oled_DrawPixel(x, y, pixel & (0x80 >> i));
}

#ifdef USE_MULTI_PAGE
/******************************************************************************
 * Oled_CurentPage - Get the current page index
 *
 * Parameter: none
 *
 * Return: the current page index (0 to NUM_PAGE)
 *****************************************************************************/
uint8_t Oled_CurentPage(void)
{
	return page_index;
}

/******************************************************************************
 * Oled_FirstPage - Back to the first page
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void Oled_FirstPage(void)
{
	page_index = 0;
	Oled_buff = Oled_buff_mpg[0];
}

/******************************************************************************
 * Oled_NextPage - go to next page
 * if current page is the last page, this function will set it back to the first
 * page
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void Oled_NextPage(void)
{
	page_index = (page_index + 1) % NUM_PAGE;
	Oled_buff = Oled_buff_mpg[page_index];
}

/******************************************************************************
 * Oled_PreviousPage - go back to the previous page
 * if the current page is the first page, this function will set it to the last
 * page
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void Oled_PreviousPage(void)
{
	page_index = (page_index + NUM_PAGE - 1) % NUM_PAGE;
	Oled_buff = Oled_buff_mpg[page_index];
}

/******************************************************************************
 * Oled_gotoPage - go to the specific page
 *
 * Parameter:
 * 	page: destination page index (0 to NUM_PAGE)
 *
 * Return: none
 *****************************************************************************/
void Oled_gotoPage(uint8_t page)
{
	if (page > NUM_PAGE)
		return;
		
	page_index = page;
	Oled_buff = Oled_buff_mpg[page_index];
}
#endif

/* End of Oled.c */
