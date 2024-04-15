/************************** AT24C32 header file **************************/
/* 
 * File:   AT24C32.h
 * Author: Nathanaël BLAVO BALLARIN
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef AT24C32_H
#define	AT24C32_H

/******************** Includes Section ********************/
#include "common.h" 
/**********************************************************/

/******************* Macros and Definitions *******************/

#define AT24C32_ADDRESS 0x50

/***************************************************************/

/******************* Function Prototypes *******************/

/**
 * @brief Writes a single byte to the AT24C32 EEPROM at the specified register address.
 * 
 * @param c The byte to write.
 * @param register_address The register address to write to.
 */
void write_one_byte_in_eeprom(unsigned char c, uint16_t register_address);

/**
 * @brief Writes a page of bytes to the AT24C32 EEPROM starting at the specified register address.
 * 
 * @param c Pointer to the data array.
 * @param register_address The starting register address to write to.
 */
void write_one_page_in_eeprom(unsigned char* c, uint16_t register_address);

/**
 * @brief Reads a single byte from the AT24C32 EEPROM at the specified register address.
 * 
 * @param register_address The register address to read from.
 * @return The byte read from the EEPROM.
 */
unsigned char read_one_byte_in_eeprom(uint16_t register_address);

/**
 * @brief Reads a page of bytes from the AT24C32 EEPROM starting at the specified register address.
 * 
 * @param register_address The starting register address to read from.
 * @param data Pointer to the array where the data will be stored.
 */
void read_one_page_in_eeprom(uint16_t register_address, unsigned char* data);

/***************************************************************/

#endif	/* AT24C32_H */
