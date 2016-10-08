/*
 * Oled.c - Function definitions for Oled
 *
 * This library is use for Tiva Arm Cotex M4
 * Device: OLED 1.3", 128x64 dot matrix panel,
 * Communication: I2C interface
 * Driver: SH1106
 *
 * Revision: 2.00
 * Author: QUANG
 */

#include "Oled.h"
#include "I2C.h"
#include "../global_include.h"
//****************************Private Definitions******************************
#define I2C_OLED_BASE	I2C1_BASE
#define DISPLAY			0x40
#define COMMAND			0
#define OLED_COLUMNSIZE	128
#define OLED_PAGESIZE	8

//*********************************Variables***********************************
static const char * const g_pcHex = "0123456789abcdef";
// ASCII table
const unsigned char ASCII[][6] ={
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 20
		,{0x00, 0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
		,{0x00, 0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
		,{0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
		,{0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
		,{0x00, 0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
		,{0x00, 0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
		,{0x00, 0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
		,{0x00, 0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
		,{0x00, 0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
		,{0x00, 0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
		,{0x00, 0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
		,{0x00, 0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
		,{0x00, 0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
		,{0x00, 0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
		,{0x00, 0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
		,{0x00, 0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
		,{0x00, 0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
		,{0x00, 0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
		,{0x00, 0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
		,{0x00, 0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
		,{0x00, 0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
		,{0x00, 0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
		,{0x00, 0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
		,{0x00, 0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
		,{0x00, 0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
		,{0x00, 0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
		,{0x00, 0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
		,{0x00, 0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
		,{0x00, 0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
		,{0x00, 0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
		,{0x00, 0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
		,{0x00, 0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
		,{0x00, 0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
		,{0x00, 0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
		,{0x00, 0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
		,{0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
		,{0x00, 0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
		,{0x00, 0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
		,{0x00, 0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
		,{0x00, 0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
		,{0x00, 0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
		,{0x00, 0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
		,{0x00, 0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
		,{0x00, 0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
		,{0x00, 0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
		,{0x00, 0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
		,{0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
		,{0x00, 0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
		,{0x00, 0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
		,{0x00, 0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
		,{0x00, 0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
		,{0x00, 0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
		,{0x00, 0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
		,{0x00, 0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
		,{0x00, 0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
		,{0x00, 0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
		,{0x00, 0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
		,{0x00, 0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
		,{0x00, 0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
		,{0x00, 0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
		,{0x00, 0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
		,{0x00, 0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
		,{0x00, 0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
		,{0x00, 0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
		,{0x00, 0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
		,{0x00, 0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
		,{0x00, 0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
		,{0x00, 0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
		,{0x00, 0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
		,{0x00, 0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
		,{0x00, 0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
		,{0x00, 0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
		,{0x00, 0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
		,{0x00, 0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
		,{0x00, 0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
		,{0x00, 0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
		,{0x00, 0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
		,{0x00, 0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
		,{0x00, 0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
		,{0x00, 0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
		,{0x00, 0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
		,{0x00, 0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
		,{0x00, 0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
		,{0x00, 0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
		,{0x00, 0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
		,{0x00, 0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
		,{0x00, 0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
		,{0x00, 0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
		,{0x00, 0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
		,{0x00, 0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
		,{0x00, 0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
		,{0x00, 0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
		,{0x00, 0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
		,{0x00, 0x10, 0x08, 0x08, 0x10, 0x08} // 7e ->
		,{0x00, 0x78, 0x46, 0x41, 0x46, 0x78} // 7f <-
};
extern uint32_t ui32Contrast;

//*************************Private function prototypes*************************
//static uint8_t Oled_ReadStatus(void);
static void Oled_Putchar(char c);
static void Oled_Putstring(const char *pcBuf, uint8_t ui8Len);

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
	I2C_Write(I2C_OLED_BASE, OLED_ADD, &Code, 1, COMMAND);
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
	//Init default values according to datasheet
	Oled_Command(SEGMENT_REMAP_R);
	Oled_Command(COMMON_PADS_HARDWARE_CONFIG_MODE);
	Oled_Command(0x12);
	Oled_Command(COMMON_OUTPUT_SCAN_DIRECTION_0);
	Oled_Command(MULTIPLEX_RATION_MODE);
	Oled_Command(0x3F);
	Oled_Command(DISPLAY_DIVIDE_RATIO_OSC_MODE);
	Oled_Command(0x50);
	Oled_Command(VCOM_DESELECT_LEVEL_MODE);
	Oled_Command(0x35);
	Oled_Command(CONTRAST_CONTROL_MODE);
	Oled_Command(ui32Contrast);
	Oled_Command(PUMP_VOLTAGE_7V8);
	Oled_Command(DC_DC_CONTROL_MODE);
	Oled_Command(DC_DC_CONTROL_MODE_ON);
	SysCtlDelay(ROM_SysCtlClockGet()/30);

	//Set start line for COM0 -> 0
	Oled_Command(0x40);

	Oled_Clear(0, 0, 128, 8);

	//Turn on the display
//	Oled_Command(DISPLAY_ON);
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
	I2C_Write(I2C_OLED_BASE, OLED_ADD, data, length, DISPLAY);
	SysCtlDelay(length * 2);
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
 * Simply write 0x00 to specific area
 *
 * Parameter:
 * 	ui8Start_column	: The first left column to erase
 * 	ui8Start_page	: The first top page
 * 	ui8Column_size	: area column size
 * 	ui8Page_size	: area page size
 *
 * Return: none
 *****************************************************************************/
void Oled_Clear(uint8_t ui8Start_column, uint8_t ui8Start_page,
		uint8_t ui8Column_size, uint8_t ui8Page_size)
{
	unsigned char ui8clear[128];
	unsigned char ui8index;
	//check condition
	if (((ui8Start_column + ui8Column_size) > OLED_COLUMNSIZE)
		|| ((ui8Start_page + ui8Page_size) > OLED_PAGESIZE))
		return;

	for(ui8index = 0; ui8index < ui8Column_size; ui8index++)
		ui8clear[ui8index] = 0;
	//Write 0x00 to entire RAM
	for(ui8index = ui8Start_page; ui8index < (ui8Start_page + ui8Page_size); ui8index++)
	{
		Oled_SetPosition(ui8Start_column, ui8index);
		Oled_Write(ui8clear, ui8Column_size);
	}
	SysCtlDelay(1000);
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
void Oled_Read(unsigned char *data, uint16_t length)
{
	I2C_Read(I2C_OLED_BASE, OLED_ADD, data, length, DISPLAY, true);
	SysCtlDelay(1000);
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
void Oled_printf(const char *pcString, ...)
{
	static unsigned int ulPos, ulCount, ulNeg;
	unsigned long ulValue, ulBase, ulIdx;
	char *pcStr, pcBuf[16], cFill;
	va_list vaArgP;

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
	SysCtlDelay(10000);		//Wait a little time for system to complete the task
}

void Oled_ReadImage(unsigned char *Image, uint8_t ui8Start_column,
		uint8_t ui8Start_page, uint8_t ui8Column_size, uint8_t ui8Page_size)
{
	uint8_t ui8Pagecount;
	uint16_t ui16ImagePointer = 0;
	
	if (((ui8Start_column + ui8Column_size) > OLED_COLUMNSIZE)
		|| ((ui8Start_page + ui8Page_size) > OLED_PAGESIZE))
		return;

	for(ui8Pagecount = 0; ui8Pagecount < ui8Page_size; ui8Pagecount++)
	{
		//Set the position at the first column and a page which is prepared to read
		Oled_SetPosition(ui8Start_column, ui8Start_page + ui8Pagecount);
		Oled_Read(&Image[ui16ImagePointer], ui8Column_size);				//Read
		ui16ImagePointer += (uint16_t)ui8Column_size;						//Update the image pointer
	}
}
/******************************************************************************
 * Make Oled enter the sleep mode to save power
 * Simply turn off the display
 *
 * Parameter:
 * 	bEnter:
 * 		true : enter the sleep mode
 * 		false: exit the sleep mode
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
 * 	bInc: indicate whether the Oled should increase or decrease the contrast value
 * 			true : increase
 * 			false: decrease
 *
 * Return: none
 *****************************************************************************/
void Oled_Contrast(bool bInc)
{
	Oled_Command(CONTRAST_CONTROL_MODE);

	if (bInc)
	{
		if(ui32Contrast < 255)
			ui32Contrast++;
	}
	else
	{
		if(ui32Contrast > 0)
			ui32Contrast--;
	}
	Oled_Command(ui32Contrast);
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
		char i;
		c -= 0x20;
		for(i = 0; i < 6; i++)
			Oled_Write(&ASCII[c][i], 1);
	}
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
