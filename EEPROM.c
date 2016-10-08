/*
 * EEPROM.c
 *
 * Read/Write data from/to EEPROM
 *
 *		Revision: 2.00
 *      Author: QUANG
 */

#include "EEPROM.h"
#include "../global_include.h"
#include "driverlib/eeprom.h"
//****************************Function definitions*****************************

/******************************************************************************
 * Configure EEPROM
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void EEPROM_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);	//Enable EEPROM
	EEPROMInit();
}

/******************************************************************************
 * Write data to EEPROM
 *
 * Parameter:
 *  *pui32_Data    : A pointer to the data
 *  ui32WordAddress: EEPROM start address (0 - 511)
 *  NumOfWords     : Number of words. (start address + number of words < 512)
 * 4bytes/word
 *
 * Return: A message to tell the result of the function
 *****************************************************************************/
EEPROM_MSG EEPROM_Write(uint32_t *pui32_Data, uint32_t ui32WordAddress, uint32_t NumOfWords)
{
	if (ui32WordAddress > 511)
		return (EEPROM_MSG_INVALID_ADDRESS);

	if ((ui32WordAddress + NumOfWords) > 512)
		return (EEPROM_MSG_INVALID_NUMOFWORDS);

	ROM_EEPROMProgram(pui32_Data, ui32WordAddress << 2, NumOfWords << 2);

	return EEPROM_MSG_OK;
}

/******************************************************************************
 * Read data from the EEPROM
 *
 * Parameter:
 *  *pui32_Data    : A pointer to the data received
 *  ui32WordAddress: EEPROM start address (0 - 511)
 *  NumOfWord      : Number of words (start address + number of words < 512)
 * 4bytes/word
 *
 * Return:
 *****************************************************************************/
EEPROM_MSG EEPROM_ReadWords(uint32_t *pui32_Data, uint32_t ui32WordAddress, uint32_t NumOfWords)
{
	if (ui32WordAddress > 511)
		return (EEPROM_MSG_INVALID_ADDRESS);

	if ((ui32WordAddress + NumOfWords) > 512)
		return (EEPROM_MSG_INVALID_NUMOFWORDS);

	ROM_EEPROMRead(pui32_Data, ui32WordAddress << 2, NumOfWords << 2);

	return EEPROM_MSG_OK;
}

