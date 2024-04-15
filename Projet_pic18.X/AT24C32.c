/*
 * File:   AT24C32.c
 * Author: Nathanaël BLAVO BALLARIN
 *
 * Created on March 22, 2024, 17:42
 */

#include "i2c_soft.h"
#include "AT24C32.h"

/******************* Function Definitions *******************/

/**
 * @brief Writes a single byte to the EEPROM at the specified register address.
 * 
 * @param c The byte to write.
 * @param register_address The register address to write to.
 */
void write_one_byte_in_eeprom(unsigned char c, uint16_t register_address)
{
    unsigned char address_upper_byte = (register_address & 0xF00)>>8;
    unsigned char address_lower_byte = (register_address & 0xFF);
    
    // Start I2C communication and send the address and data
    I2C_Start();
    I2C_WriteToAddress(AT24C32_ADDRESS);
    I2C_WriteByte(address_upper_byte);
    I2C_WriteByte(address_lower_byte);
    I2C_WriteByte(c);
    I2C_Stop();
}

/**
 * @brief Writes a page of bytes to the EEPROM starting at the specified register address.
 * 
 * @param c Pointer to the data array.
 * @param register_address The starting register address to write to.
 */
void write_one_page_in_eeprom(unsigned char* c, uint16_t register_address)
{
    unsigned char address_upper_byte = (register_address & 0xF00)>>8;
    unsigned char address_lower_byte = (register_address & 0xFF);

    // Start I2C communication and send the address and data
    I2C_Start();
    I2C_WriteToAddress(AT24C32_ADDRESS);
    I2C_WriteByte(address_upper_byte);
    I2C_WriteByte(address_lower_byte);
    for(int i=0;i<8;i++)
    {
      I2C_WriteByte(c[i]);
    }
    I2C_Stop();
}

/**
 * @brief Reads a single byte from the EEPROM at the specified register address.
 * 
 * @param register_address The register address to read from.
 * @return The byte read from the EEPROM.
 */
unsigned char read_one_byte_in_eeprom(uint16_t register_address)
{
    unsigned char c;
    unsigned char address_upper_byte = (register_address & 0xF00)>>8;
    unsigned char address_lower_byte = (register_address & 0xFF);
    
    // Start I2C communication and send the address, then read the data
    I2C_Start();
    I2C_WriteToAddress(AT24C32_ADDRESS);
    I2C_WriteByte(address_upper_byte);
    I2C_WriteByte(address_lower_byte);
    I2C_Start();
    I2C_ReadFromAddress(AT24C32_ADDRESS);
    c = I2C_ReadResult();
    I2C_Stop();
    return c;
}

/**
 * @brief Reads a page of bytes from the EEPROM starting at the specified register address.
 * 
 * @param register_address The starting register address to read from.
 * @param data Pointer to the array where the data will be stored.
 */
void read_one_page_in_eeprom(uint16_t register_address, unsigned char* data)
{
    unsigned char c;
    unsigned char address_upper_byte = (register_address & 0xF00)>>8;
    unsigned char address_lower_byte = (register_address & 0xFF);
    
    // Start I2C communication and send the address, then read the data
    I2C_Start();
    I2C_WriteToAddress(AT24C32_ADDRESS);
    I2C_WriteByte(address_upper_byte);
    I2C_WriteByte(address_lower_byte);
    I2C_Start();
    I2C_ReadFromAddress(AT24C32_ADDRESS);
    for(int i=0;i<7;i++)
    {
        data[i] = I2C_ReadResult_withAck();
    }
    data[7] = I2C_ReadResult();
    I2C_Stop();
}
