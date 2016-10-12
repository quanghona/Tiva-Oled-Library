/*
 * global_include.h - The include file for the entire project
 *
 * This file contain some lib that almost any function in the
 * project is use - Tivaware API
 * Every library and main file should include this file to the code
 *
 * 		Revision: 2.00
 * 		Author: QUANG
 */

#ifndef GLOBAL_INCLUDE_H_
#define GLOBAL_INCLUDE_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/timer.h"
//#include "inc/tm4c123gh6pm.h"
#include "driverlib/i2c.h"
#include "driverlib/ssi.h"
#include "global_define.h"

#endif /* GLOBAL_INCLUDE_H_ */
