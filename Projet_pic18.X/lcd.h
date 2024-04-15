/************************** LCD Header file **************************/
/*
 * File:   LCD.h
 * Author: https://microdigisoft.com
 *
 * Created on August 9, 2020, 5:11 PM
 * Modified by Nathanaël BLAVO BALLARIN, March 22, 2024
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************* Macros and Definitions *******************/

#define FirstLine   0x80
#define SecondLine  0xC0

#define LCD_BACKLIGHT          0x08
#define LCD_NOBACKLIGHT        0x00
#define LCD_FIRST_ROW          0x80
#define LCD_SECOND_ROW         0xC0
#define LCD_THIRD_ROW          0x94
#define LCD_FOURTH_ROW         0xD4
#define LCD_CLEAR              0x01
#define LCD_RETURN_HOME        0x02
#define LCD_ENTRY_MODE_SET     0x04
#define LCD_CURSOR_OFF         0x0C
#define LCD_UNDERLINE_ON       0x0E
#define LCD_BLINK_CURSOR_ON    0x0F
#define LCD_MOVE_CURSOR_LEFT   0x10
#define LCD_MOVE_CURSOR_RIGHT  0x14
#define LCD_TURN_ON            0x0C
#define LCD_TURN_OFF           0x08
#define LCD_SHIFT_LEFT         0x18
#define LCD_SHIFT_RIGHT        0x1E
#define LCD_TYPE               2       // 0 -> 5x7 | 1 -> 5x10 | 2 -> 2 lines

/***************************************************************/

/******************* Function Prototypes *******************/

/**
 * @brief Initializes the LCD module.
 * 
 * @param I2C_Add The I2C address of the LCD module.
 */
void LCD_Init(unsigned char I2C_Add);

/**
 * @brief Writes a 4-bit value to the LCD.
 * 
 * @param Nibble The 4-bit value to write.
 */
void LCD_Write_4Bit(unsigned char Nibble);

/**
 * @brief Sends a command to the LCD.
 * 
 * @param CMD The command to send.
 */
void LCD_CMD(unsigned char CMD);

/**
 * @brief Sets the cursor position on the LCD.
 * 
 * @param ROW The row number (1 to 4).
 * @param COL The column number (1 to 20).
 */
void LCD_Set_Cursor(unsigned char ROW, unsigned char COL);

/**
 * @brief Writes a character to the LCD.
 * 
 * @param Data The character to write.
 */
void LCD_Write_Char(char Data);

/**
 * @brief Writes a string to the LCD.
 * 
 * @param Str The string to write.
 */
void LCD_Write_String(char* Str);

/**
 * @brief Turns on the backlight of the LCD.
 */
void Backlight();

/**
 * @brief Turns off the backlight of the LCD.
 */
void noBacklight();

/**
 * @brief Shifts the display to the left.
 */
void LCD_SL();

/**
 * @brief Shifts the display to the right.
 */
void LCD_SR();

/**
 * @brief Clears the display of the LCD.
 */
void LCD_Clear();

/**************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */
