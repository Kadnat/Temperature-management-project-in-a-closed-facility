/*
 * File:   AT24C32.c
 * Author: Nathanael
 *
 * Created on 22 mars 2024, 17:42
 */

#include "i2c_soft.h"
#include "AT24C32.h"

void write_one_byte_in_eeprom(unsigned char c, uint16_t register_address)
{
    unsigned char address_upper_byte = (register_address & 0xF00)>>8;
    unsigned char address_lower_byte = (register_address & 0xFF);
    
    I2C_Start();
	I2C_WriteToAddress(AT24C32_ADDRESS);
	I2C_WriteByte(address_upper_byte);
    I2C_WriteByte(address_lower_byte);
	I2C_WriteByte(c);
	I2C_Stop();
}

void write_one_page_in_eeprom(unsigned char* c, uint16_t register_address)
{
    unsigned char address_upper_byte = (register_address & 0xF00)>>8;
    unsigned char address_lower_byte = (register_address & 0xFF);

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

unsigned char read_one_byte_in_eeprom(uint16_t register_address)
{
    unsigned char c;
    unsigned char address_upper_byte = (register_address & 0xF00)>>8;
    unsigned char address_lower_byte = (register_address & 0xFF);
    
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

void read_one_page_in_eeprom(uint16_t register_address, unsigned char* data)
{
    unsigned char c;
    unsigned char address_upper_byte = (register_address & 0xF00)>>8;
    unsigned char address_lower_byte = (register_address & 0xFF);
    
  	I2C_Start();
  	I2C_WriteToAddress(AT24C32_ADDRESS);
	I2C_WriteByte(address_upper_byte);
    I2C_WriteByte(address_lower_byte);
  	I2C_Start();
  	I2C_ReadFromAddress(AT24C32_ADDRESS);
    for(int i=0;i<8;i++)
    {
        data[i] = I2C_ReadResult_withAck();
        
  	}
    I2C_Stop();

}