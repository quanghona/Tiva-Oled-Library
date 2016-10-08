/*
 * I2C.h - I2C interface for the project
 *
 * This library support the communication between Tiva and
 * devices via I2C interface.
 * This library support 4 I2C module (from 0 to 3)
 *
 * Hardware connection:
 * 				|  SDA	|  SCL
 * 		 -------+-------+--------
 * 		  I2C0	|  PB3	|  PB2
 * 		  I2C1	|  PA7	|  PA6
 * 		  I2C2	|  PE5	|  PE4
 * 		  I2C3	|  PD1	|  PD0
 *
 * Revision: 2.00
 * Author: QUANG
 */
#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>
#include <stdbool.h>

//****************************Function prototypes******************************
void I2C_Init(uint32_t ui32Base, bool bFast);
void I2C_Write(uint32_t ui32Base, unsigned char uiSlave_add, unsigned char *ucData,
		uint16_t uiCount, unsigned char ucStart_add );
void I2C_Read(uint32_t ui32Base, unsigned char uiSlave_add, unsigned char *ucRec_Data,
		uint16_t uiCount, unsigned char ucStart_add, bool bDummyRead);
//*****************************************************************************
#endif	/* I2C_H_ */
