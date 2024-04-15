/************************** lcd.c **************************/
/*
 * File:   lcd.c
 * Author: https://microdigisoft.com
 *
 * Created on 9 August, 2020, 5:11 PM
 * Modified by Nathanaël BLAVO BALLARIN, 22 March 2024
 */

#include "common.h"
#include "lcd.h"
#include "i2c_soft.h"


/******************* Global Variables *******************/
unsigned char RS, i2c_add, BackLight_State = LCD_BACKLIGHT;
/*********************************************************/

//======================================================

//---------------[ LCD Routines ]----------------
//------------------------------------------------------

/**
 * @brief Initializes the LCD module.
 * 
 * @param I2C_Add The I2C address of the LCD module.
 */
void LCD_Init(unsigned char I2C_Add) 
{
  i2c_add = I2C_Add;
    I2C_Start();
  I2C_WriteToAddress(i2c_add);
  I2C_WriteByte(0 | BackLight_State);
  I2C_Stop();
  __delay_ms(30);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(LCD_RETURN_HOME);
  __delay_ms(5);
  LCD_CMD(0x20 | (LCD_TYPE << 2));
  __delay_ms(50);
  LCD_CMD(LCD_TURN_ON);
  __delay_ms(50);
  LCD_CMD(LCD_CLEAR);
  __delay_ms(50);
  LCD_CMD(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
  __delay_ms(50);
}

/**
 * @brief Writes a 4-bit value to the LCD.
 * 
 * @param Nibble The 4-bit value to write.
 */
void LCD_Write_4Bit(unsigned char Nibble) 
{
  // Get The RS Value To LSB OF Data  
  Nibble |= RS;
  I2C_Start();
  I2C_WriteToAddress(i2c_add);
  I2C_WriteByte((Nibble | 0x04) | BackLight_State);
  I2C_Stop();
  

  I2C_Start();
  I2C_WriteToAddress(i2c_add);
  I2C_WriteByte((Nibble & 0xFB) | BackLight_State);
  I2C_Stop();
  __delay_us(50);
}

/**
 * @brief Sends a command to the LCD.
 * 
 * @param CMD The command to send.
 */
void LCD_CMD(unsigned char CMD) 
{
  RS = 0; // Command Register Select
  LCD_Write_4Bit(CMD & 0xF0);
  LCD_Write_4Bit((CMD << 4) & 0xF0);
}

/**
 * @brief Writes a character to the LCD.
 * 
 * @param Data The character to write.
 */
void LCD_Write_Char(char Data)
{
  RS = 1;  // Data Register Select
  LCD_Write_4Bit(Data & 0xF0);
  LCD_Write_4Bit((Data << 4) & 0xF0);
}

/**
 * @brief Writes a string to the LCD.
 * 
 * @param Str The string to write.
 */
void LCD_Write_String(char* Str)
{
    for(int i=0; Str[i]!='\0'; i++)
       LCD_Write_Char(Str[i]); 
}

/**
 * @brief Sets the cursor position on the LCD.
 * 
 * @param ROW The row number (1 to 4).
 * @param COL The column number (1 to 20).
 */
void LCD_Set_Cursor(unsigned char ROW, unsigned char COL) 
{    
  switch(ROW) 
  {
    case 2:
      LCD_CMD(0xC0 + COL-1);
      break;
    case 3:
      LCD_CMD(0x94 + COL-1);
      break;
    case 4:
      LCD_CMD(0xD4 + COL-1);
      break;
    // Case 1  
    default:
      LCD_CMD(0x80 + COL-1);
  }
}

/**
 * @brief Turns on the backlight of the LCD.
 */
void Backlight() 
{
  BackLight_State = LCD_BACKLIGHT;
    I2C_Start();
  I2C_WriteToAddress(i2c_add);
  I2C_WriteByte(0 | BackLight_State);
  I2C_Stop();
}

/**
 * @brief Turns off the backlight of the LCD.
 */
void noBacklight() 
{
  BackLight_State = LCD_NOBACKLIGHT;
    I2C_Start();
  I2C_WriteToAddress(i2c_add);
  I2C_WriteByte(0 | BackLight_State);
  I2C_Stop();
}

/**
 * @brief Shifts the display to the left.
 */
void LCD_SL()
{
  LCD_CMD(0x18);
  __delay_us(40);
}

/**
 * @brief Shifts the display to the right.
 */
void LCD_SR()
{
  LCD_CMD(0x1C);
  __delay_us(40);
}

/**
 * @brief Clears the display of the LCD.
 */
void LCD_Clear()
{
  LCD_CMD(0x01); 
  __delay_us(40);
}