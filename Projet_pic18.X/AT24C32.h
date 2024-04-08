
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef AT24C32_H
#define	AT24C32_H

#include "common.h" // include processor files - each processor file is guarded.  

#define AT24C32_ADDRESS 0x50

void write_one_byte_in_eeprom(unsigned char c, uint16_t register_address);
void write_one_page_in_eeprom(unsigned char* c, uint16_t register_address);
unsigned char read_one_byte_in_eeprom(uint16_t register_address);
void read_one_page_in_eeprom(uint16_t register_address, unsigned char* data);

#endif	/* XC_HEADER_TEMPLATE_H */

