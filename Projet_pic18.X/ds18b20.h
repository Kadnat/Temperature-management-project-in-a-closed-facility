
/* 
 * File:   DS18B20.h
 * Author: Jason McGuire (j.mcguire.2015@ieee.org)
 * Description: A library for the DS18B20 Temperature Sensor
 * Using the Dallas OneWire communication protocol.
 * See example.c for implementation of the library
 * Sample Product: https://www.sparkfun.com/products/11050
 * Created on April 20, 2016, 9:08 AM
 
License Information:
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 
 
 */

#ifndef DS18B20_H
#define DS18B20_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>
#include "temp_monitoring.h"

/******************* Macros and Definitions *******************/

// This configuration is required to make a PIC be in open drain 
#define BUSDIR LATAbits.LA1 // Set direction: input or output
#define BUSOUT TRISAbits.RA1 // Set output: high or low 
#define BUSIN PORTCbits.RC1 // Read input 

/***************************************************************/

/******************* Function Prototypes *******************/

/**
 * @brief Reads the temperature from the DS18B20 sensor.
 * 
 * @return The temperature in Celsius.
 */
float OneWireTemp(void);

/**
 * @brief Sends a reset pulse to the DS18B20 sensor.
 * 
 * @return 0 if the sensor responds with a presence pulse, 1 otherwise.
 */
unsigned int OneWireReset(void);

/**
 * @brief Writes a single bit to the OneWire bus.
 * 
 * @param bit The bit to write (0 or 1).
 */
void OneWireWriteBit(unsigned char bit);

/**
 * @brief Reads a single bit from the OneWire bus.
 * 
 * @return The bit read (0 or 1).
 */
unsigned char OneWireReadBit(void);

/**
 * @brief Writes a byte to the OneWire bus.
 * 
 * @param byte The byte to write.
 */
void OneWireWriteByte(unsigned char byte);

/**
 * @brief Reads a byte from the OneWire bus.
 * 
 * @return The byte read.
 */
unsigned char OneWireReadByte(void);

/**
 * @brief Reads the current status of the OneWire bus.
 * 
 * @return The status of the bus.
 */
unsigned char OneWireRead(void);

/**
 * @brief Sets the OneWire bus high.
 */
void OneWireHigh(void);

/**
 * @brief Releases the OneWire bus.
 */
void OneWireRelease(void);

/**
 * @brief Reads the temperature from the DS18B20 sensor and prints it.
 * 
 * @param pSystem_data Pointer to the system data structure.
 * @return The temperature in Celsius.
 */
float print_temperature(SystemData* pSystem_data);

/***************************************************************/

#endif // DS18B20_H



