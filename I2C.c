/*
 * I2C.c - I2C interface function definition
 *
 * This library is support the communication between Tiva and
 * devices via I2C interface.
 *  * This library support 4 I2C module (from 0 to 3).
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
#include "I2C.h"
#include "../global_include.h"
#include "inc/hw_i2c.h"
//#include "driverlib/uart.h"

//****************************Function definitions*****************************

/******************************************************************************
 * Initialize and configure I2C module in Tiva
 *
 * Parameter:
 *  ui32Base: I2C's base address. The adress is one of the following value:
 *  	I2C0_BASE, I2C1_BASE, I2C2_BASE, I2C3_BASE.
 *  bFast	:
 *    true : I2C will run in fast mode (400kbps)
 *    false: I2C will run in stardard mode (100kbps)
 *
 * Return: none
 *****************************************************************************/
void I2C_Init(uint32_t ui32Base, bool bFast)
{
	switch(ui32Base)
	{
	case I2C0_BASE:
		// Enable the peripherals
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
		ROM_GPIOPinConfigure(GPIO_PB2_I2C0SCL);
		ROM_GPIOPinConfigure(GPIO_PB3_I2C0SDA);
		// Initialize the I2C master - Transfer data is define depend on variable bFast
		ROM_I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), bFast);
		// Configure the appropriate pins to be I2C instead of GPIO.
		ROM_GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
		ROM_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
		ROM_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);
		break;

	case I2C1_BASE:
		// Enable the peripherals
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
		ROM_GPIOPinConfigure(GPIO_PA6_I2C1SCL);
		ROM_GPIOPinConfigure(GPIO_PA7_I2C1SDA);
		// Initialize the I2C master - Transfer data is define depend on variable bFast
		ROM_I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), bFast);
		// Configure the appropriate pins to be I2C instead of GPIO.
		ROM_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
		ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
		ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);
		break;

	case I2C2_BASE:
		// Enable the peripherals
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
		ROM_GPIOPinConfigure(GPIO_PE4_I2C2SCL);
		ROM_GPIOPinConfigure(GPIO_PE5_I2C2SDA);
		// Initialize the I2C master - Transfer data is define depend on variable bFast
		ROM_I2CMasterInitExpClk(I2C2_BASE, SysCtlClockGet(), bFast);
		// Configure the appropriate pins to be I2C instead of GPIO.
		ROM_GPIOPinTypeI2C(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
		ROM_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
		ROM_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);
		break;

	case I2C3_BASE:
		// Enable the peripherals
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		ROM_GPIOPinConfigure(GPIO_PD0_I2C3SCL);
		ROM_GPIOPinConfigure(GPIO_PD1_I2C3SDA);
		// Initialize the I2C master - Transfer data is define depend on variable bFast
		ROM_I2CMasterInitExpClk(I2C3_BASE, SysCtlClockGet(), bFast);
		// Configure the appropriate pins to be I2C instead of GPIO.
		ROM_GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
		ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
		ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);
		break;
	}

	ROM_I2CMasterEnable(ui32Base);		//I2C is ready to use
}

/******************************************************************************
 * Write multiple bytes to a slave device
 *
 * Parameter:
 * 	uiSlave_add: the slave address
 * 	*ucData    : the data that are going to be sent
 * 	uiCount	   : number of byte will be sent
 * 	ucStart_add: register address you want to write or a control byte
 *
 * Return: none
 *****************************************************************************/
void I2C_Write(uint32_t ui32Base, unsigned char uiSlave_add, unsigned char *ucData, uint16_t uiCount, unsigned char ucStart_add)
{
	unsigned int temp = 1;
	IntMasterDisable();
	// Set the slave address and setup for a transmit operation.
	I2CMasterSlaveAddrSet(ui32Base, uiSlave_add, false);
	// Place the address to be written in the data register.
	I2CMasterDataPut(ui32Base, ucStart_add);
	while (I2CMasterBusy(ui32Base));
	if (uiCount == 0)
		// Initiate send of character from Master to Slave
		I2CMasterControl(ui32Base, I2C_MASTER_CMD_SINGLE_SEND);
	else
	{
		// Start the burst cycle, writing the address as the first byte.
		I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_SEND_START);
		// Write the next byte to the data register.
		while (I2CMasterBusy(ui32Base));
		I2CMasterDataPut(ui32Base, ucData[0]);
		for( ; temp < uiCount; temp++)        //Loop to send data if not the last byte
		{
			// Continue the burst write.
			I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_SEND_CONT);
			// Write the next byte to the data register.
			while (I2CMasterBusy(ui32Base));
			I2CMasterDataPut(ui32Base, ucData[temp]);
		}
		// Finish the burst write.
		I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_SEND_FINISH);
	}
	while (I2CMasterBusy(ui32Base));
	IntMasterEnable();
}

/******************************************************************************
 * Read multiple bytes from slave device
 *
 * Parameter:
 * 	uiSlave_add: the slave address
 * 	*ucRec_Data: a variable to save the data received
 * 	uiCount	   : number of byte will be received
 * 	ucStart_add: register address you want to read or a control byte
 *
 * Return: none
 *****************************************************************************/
void I2C_Read(uint32_t ui32Base, unsigned char uiSlave_add, unsigned char *ucRec_Data, uint16_t uiCount, unsigned char ucStart_add, bool bDummyRead)
{
	unsigned int uitemp = 0;
		// Set the slave address and setup for a transmit operation.
	IntMasterDisable();
		I2CMasterSlaveAddrSet(ui32Base, uiSlave_add, false);
		// Place the address to be written in the data register.
		I2CMasterDataPut(ui32Base, ucStart_add);
		// Start the burst cycle, writing the address as the first byte.
		I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_SEND_START);

		while (I2CMasterBusy(ui32Base));
		//while (!(I2CMasterErr(ui32Base) == I2C_MASTER_ERR_NONE));

	I2CMasterSlaveAddrSet(ui32Base, uiSlave_add, true);
	if (uiCount == 1)
	{
		I2CMasterControl(ui32Base, I2C_MASTER_CMD_SINGLE_RECEIVE);
		while (I2CMasterBusy(ui32Base));
		ucRec_Data[0]  = I2CMasterDataGet(ui32Base);
		if (bDummyRead)
		{
			I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
			while (I2CMasterBusy(ui32Base));
			ucRec_Data[0]  = I2CMasterDataGet(ui32Base);
		}
	}
	else
	{
		I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_RECEIVE_START);
		if (bDummyRead)
		{
			while (I2CMasterBusy(ui32Base));
			ucRec_Data[0]  = I2CMasterDataGet(ui32Base);
			I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
		}
		while (I2CMasterBusy(ui32Base));
		ucRec_Data[uitemp++]  = I2CMasterDataGet(ui32Base);
		for ( ; uitemp < (uiCount - 1); uitemp++)
		{
			I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
			while (I2CMasterBusy(ui32Base));
			ucRec_Data[uitemp]  = I2CMasterDataGet(ui32Base);
		}
		I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
		while (I2CMasterBusy(ui32Base));
		ucRec_Data[uitemp]  = I2CMasterDataGet(ui32Base);
	}
//	while(I2CMasterIntStatus(ui32Base, false) == 0)
//	{
//	}
	while (I2CMasterBusy(ui32Base));
	IntMasterEnable();
}
