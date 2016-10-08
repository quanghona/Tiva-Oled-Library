/*
 * EEPROM.h - EEPROM header file
 *
 * Read/Write data from/to EEPROM
 *
 *		Revision: 2.00
 *      Author: QUANG
 */

#ifndef EEPROM_H_
#define EEPROM_H_
#include <stdint.h>

//*******************************Definitions***********************************
typedef enum
{
	EEPROM_MSG_INVALID_ADDRESS = 0,
	EEPROM_MSG_INVALID_NUMOFWORDS,
	EEPROM_MSG_OK
} EEPROM_MSG;

//****************************Function prototypes******************************
void EEPROM_Config(void);
EEPROM_MSG EEPROM_Write(uint32_t *pui32_Data, uint32_t ui32WordAddress, uint32_t NumOfWords);
EEPROM_MSG EEPROM_ReadWords(uint32_t *pui32_Data, uint32_t ui32WordAddress, uint32_t NumOfWords);
//*****************************************************************************
#endif /* EEPROM_H_ */
