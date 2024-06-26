/* 
 * File:   RTC.c
 * Author: microdigisoft
 *
 * Created on 30 August, 2020, 11:34 AM
 * 
 * Modified by Nathana�l BLAVO BALLARIN, 25 March 2024
 */
/********************************* Includes **********************************/
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "common.h"
#include "i2c_soft.h"
#include"RTC.h"
#include "temp_monitoring.h"
/**********************************************************/

/******************** Global Variables Declarations Section ********************/
unsigned char pRTCArray[4];
unsigned char Temp;
/**********************************************************/
/*
 * 
 */
//Function related to RTC
// Function Purpose: Write_Byte_To_DS1307_RTC writes a single byte on given address
// Address can have any value fromm 0 to 0xFF, and DataByte can have a value of 0 to 0xFF.
void Write_Byte_To_DS1307_RTC(unsigned char Address, unsigned char DataByte)
{
     I2C_Start();                    // Start i2c communication
    // Send i2c address of DS1307 with write command
    I2C_WriteToAddress(Device_Address_DS1307_EEPROM + 0);
    I2C_WriteByte(Address);
    I2C_WriteByte(DataByte);              // Write data byte
     I2C_Stop();                      // Stop i2c communication
}

// Function Purpose: Read_Byte_From_DS1307_RTC reads a single byte from given address
// Address can have any value fromm 0 to 0xFF.
unsigned char Read_Byte_From_DS1307_RTC(unsigned char Address)
{
    unsigned char Byte = 0;                // Variable to store Received byte
     I2C_Start();                    // Start i2c communication
    // Send i2c address of DS1307 with write command
    I2C_WriteToAddress(Device_Address_DS1307_EEPROM + 0);
    I2C_WriteByte(Address);
    I2C_Stop();                   // Restart i2c
    I2C_Start();
    // Send i2c address of DS1307 RTC with read command
    I2C_ReadFromAddress(Device_Address_DS1307_EEPROM + 0);
    Byte = I2C_ReadResult();              // Read byte from EEPROM
    I2C_Stop();                      // Stop i2c communication
    return Byte;        // Return the byte received from 24LC64 EEPROM
}

// Function Purpose: Write_Bytes_To_DS1307_RTC writes mulitple bytes from given starting address.
// Address can have any value from 0 to 0xFF and pData is pointer to the array
// containing NoOfBytes bytes in it. NoOfBytes is the number of bytes to write.
void Write_Bytes_To_DS1307_RTC(unsigned char Address,unsigned char* pData,unsigned char NoOfBytes)
{
    unsigned int i;
     I2C_Start();                    // Start i2c communication
    // Send i2c address of DS1307 with write command    
    I2C_WriteToAddress(Device_Address_DS1307_EEPROM + 0);   
    I2C_WriteByte(Address);
    
    for(i=0; i<NoOfBytes; i++)              // Write NoOfBytes
        I2C_WriteByte(pData[i]);            // Write data byte
     I2C_Stop();                      // Stop i2c communication
}

// Function Purpose: Read_Bytes_From_DS1307_RTC reads a NoOfBytes bytes from given starting address.
// Address can have any value fromm 0 to 0xFF. NoOfBytes is the number of bytes to write.
// Read bytes are returned in pData array.
void Read_Bytes_From_DS1307_RTC(unsigned char Address, unsigned char* pData, unsigned int NoOfBytes)
{
    unsigned int i;
     
    pData[0] = I2C_ReadRegister(Device_Address_DS1307_EEPROM,Address);
    for(i=1; i<NoOfBytes; i++)              // Read NoOfBytes
    {
        pData[i] = I2C_ReadRegister(Device_Address_DS1307_EEPROM,Address+i);    // Read next byte from EEPROM
    }

}

// Function Purpose: Set_DS1307_RTC_Time sets given time in RTC registers.
// Mode can have a value AM_Time or PM_Time	or TwentyFourHoursMode only.
// Hours can have value from 0 to 23 only.
// Mins can have value from 0 to 59 only.
// Secs can have value from 0 to 59 only.
void Set_DS1307_RTC_Time(unsigned char Mode, unsigned char Hours, unsigned char Mins, unsigned char Secs)
{
	// Convert Hours, Mins, Secs into BCD
	pRTCArray[0] = (((unsigned char)(Secs/10))<<4)|((unsigned char)(Secs%10));
	pRTCArray[1] = (((unsigned char)(Mins/10))<<4)|((unsigned char)(Mins%10));
	pRTCArray[2] = (((unsigned char)(Hours/10))<<4)|((unsigned char)(Hours%10));

	switch(Mode)	// Set mode bits
	{
	case AM_Time: 	pRTCArray[2] |= 0x40;	break;
	case PM_Time: 	pRTCArray[2] |= 0x60;	break;
	
	default:	break;	// do nothing for 24HoursMode
	}

	// WritepRTCArray to DS1307
	Write_Bytes_To_DS1307_RTC(0x00, pRTCArray, 3);
}

/**
 * @brief Read Time in RTC
 * @param None
 * @return Time : returns a data structure of type Time 
 */
Time Get_DS1307_RTC_Time(void) {
    // D�claration d'une variable pour stocker l'heure
    Time currentTime;

    // Tableau temporaire pour stocker les donn�es lues depuis le DS1307 RTC
    unsigned char pRTCArray[3];

    // Lire les heures, minutes et secondes depuis le RTC DS1307
    Read_Bytes_From_DS1307_RTC(0x00, pRTCArray, 3);

    // Convertir les secondes de BCD en nombre
    unsigned char Temp = pRTCArray[0];
    currentTime.second = ((Temp & 0x7F) >> 4) * 10 + (Temp & 0x0F);

    // Convertir les minutes de BCD en nombre
    Temp = pRTCArray[1];
    currentTime.minute = (Temp >> 4) * 10 + (Temp & 0x0F);

    // Convertir les heures de BCD en nombre
    if (pRTCArray[2] & 0x40) { // si mode 12 heures
        if (pRTCArray[2] & 0x20)
            currentTime.mode = PM_Time; // si c'est l'heure de PM
        else
            currentTime.mode = AM_Time; // si c'est l'heure de AM

        Temp = pRTCArray[2];
        currentTime.hour = ((Temp & 0x1F) >> 4) * 10 + (Temp & 0x0F);
    } else { // si mode 24 heures
        Temp = pRTCArray[2];
        currentTime.hour = (Temp >> 4) * 10 + (Temp & 0x0F);
        currentTime.mode = TwentyFourHoursMode;
    }

    return currentTime;
}

// Function Purpose: Set_DS1307_RTC_Date sets given date in RTC registers.
// Year can have a value from 0 to 99 only.
// Month can have value from 1 to 12 only.
// Date can have value from 1 to 31 only.
// Day can have value from 1 to 7 only. Where 1 means Monday, 2 means Tuesday etc.
void Set_DS1307_RTC_Date(unsigned char Date, unsigned char Month, unsigned char Year, unsigned char Day)
{
	// Convert Year, Month, Date, Day into BCD
	pRTCArray[0] = (((unsigned char)(Day/10))<<4)|((unsigned char)(Day%10));
	pRTCArray[1] = (((unsigned char)(Date/10))<<4)|((unsigned char)(Date%10));
	pRTCArray[2] = (((unsigned char)(Month/10))<<4)|((unsigned char)(Month%10));
	pRTCArray[3] = (((unsigned char)(Year/10))<<4)|((unsigned char)(Year%10));

	// WritepRTCArray to DS1307
	Write_Bytes_To_DS1307_RTC(0x03, pRTCArray, 4);
}

/**
 * @brief Read Date in RTC
 * @param None
 * @return Date : returns a data structure of type Date 
 */
Date Get_DS1307_RTC_Date(void) {
    // D�claration d'une variable pour stocker la date
    Date currentDate;

    // Tableau temporaire pour stocker les donn�es lues depuis le DS1307 RTC
    unsigned char pRTCArray[4];

    // Lire la date depuis le DS1307 RTC
    Read_Bytes_From_DS1307_RTC(0x03, pRTCArray, 4);

    // Convertir la date de BCD en nombre
    unsigned char Temp;

    // Convertir le jour de BCD en nombre
    Temp = pRTCArray[1];
    currentDate.day = (Temp >> 4) * 10 + (Temp & 0x0F);
    

    // Convertir le mois de BCD en nombre
    Temp = pRTCArray[2];
    currentDate.month = (Temp >> 4) * 10 + (Temp & 0x0F);

    // Convertir l'ann�e de BCD en nombre
    Temp = pRTCArray[3];
    currentDate.year = (Temp >> 4) * 10 + (Temp & 0x0F);

    return currentDate;
}

/**
 * @brief Display read date on LCD.
 * @param pDate : Pointer to the data structure.
 * @return None.
 */
void DisplayDateOnLCD(SystemData *pDate) {
    // D�claration des variables locales
    unsigned char tens, units;

    // D�placement du curseur � la position de d�but
    LCD_Set_Cursor(2, 1);

    // Affichage du jour (tens)
    tens = pDate->day / 10;
    LCD_Write_Char(tens + 0x30);

    // Affichage du jour (unit�s)
    units = pDate->day % 10;
    LCD_Set_Cursor(2, 2);
    LCD_Write_Char(units + 0x30);

    // Affichage du s�parateur '/'
    LCD_Set_Cursor(2, 3);
    LCD_Write_Char('/');

    // Affichage du mois (tens)
    tens = pDate->month / 10;
    LCD_Set_Cursor(2, 4);
    LCD_Write_Char(tens + 0x30);

    // Affichage du mois (unit�s)
    units = pDate->month % 10;
    LCD_Set_Cursor(2, 5);
    LCD_Write_Char(units + 0x30);

    // Affichage du s�parateur '/'
    LCD_Set_Cursor(2, 6);
    LCD_Write_Char('/');

    // Affichage de l'ann�e (dizaines)
    tens = pDate->year / 10;
    LCD_Set_Cursor(2, 7);
    LCD_Write_Char(tens + 0x30);

    // Affichage de l'ann�e (unit�s)
    units = pDate->year % 10;
    LCD_Set_Cursor(2, 8);
    LCD_Write_Char(units + 0x30);

    // Affichage de l'espace
    LCD_Set_Cursor(2, 9);
    LCD_Write_Char(' ');
   
}

/**
 * @brief Display read Time on LCD.
 * @param pTime : Pointer to the data structure.
 * @return None.
 */
void DisplayTimeToLCD(SystemData *pTime) {
    // D�claration des variables locales
    unsigned char tens, units;

    // D�placement du curseur � la position de d�but
    LCD_Clear(); // Clear the screen
    LCD_Set_Cursor(1, 1);

    // Affichage de l'heure (dizaines)
    tens = pTime->hour / 10;
    LCD_Write_Char(tens + 0x30);

    // Affichage de l'heure (unit�s)
    units = pTime->hour % 10;
    LCD_Set_Cursor(1, 2);
    LCD_Write_Char(units + 0x30);

    // Affichage du s�parateur ':'
    LCD_Set_Cursor(1, 3);
    LCD_Write_Char(':');

    // Affichage des minutes (dizaines)
    tens = pTime->minute / 10;
    LCD_Set_Cursor(1, 4);
    LCD_Write_Char(tens + 0x30);

    // Affichage des minutes (unit�s)
    units = pTime->minute % 10;
    LCD_Set_Cursor(1, 5);
    LCD_Write_Char(units + 0x30);

    // Affichage du s�parateur ':'
    LCD_Set_Cursor(1, 6);
    LCD_Write_Char(':');

    // Affichage des secondes (dizaines)
    tens = pTime->second / 10;
    LCD_Set_Cursor(1, 7);
    LCD_Write_Char(tens + 0x30);

    // Affichage des secondes (unit�s)
    units = pTime->second % 10;
    LCD_Set_Cursor(1, 8);
    LCD_Write_Char(units + 0x30);

    // Affichage de l'espace
    LCD_Set_Cursor(1, 9);
    LCD_Write_Char(' ');

}