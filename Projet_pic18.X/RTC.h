/************************** RTC Header File **************************/
/*
 * File:   RTC.h
 * Author: microdigisoft
 *
 * Created on 30 August, 2020, 11:33 AM
 * 
 * Modified by Nathanaël BLAVO BALLARIN, 25 March 2024
 */

#ifndef RTC_H
#define	RTC_H

/******************* Includes Section ********************/
#include "temp_monitoring.h"
/**********************************************************/

/******************* Definitions Section ********************/
// Define DS1307 i2c device address
#define Device_Address_DS1307_EEPROM  0x68

// Define Time Modes
#define AM_Time          0
#define PM_Time          1
#define TwentyFourHoursMode    2

// Define days
#define Monday    1
#define Tuesday    2
#define Wednesday  3
#define Thursday  4
#define Friday    5
#define Saturday  6
#define Sunday    7
/**********************************************************/

/******************* Data Types Declarations Section ********************/
typedef struct {
    unsigned char day;   // Jour
    unsigned char month; // Mois
    unsigned char year;  // Année
    unsigned char weekday; // Jour de la semaine
} Date;

typedef struct {
    unsigned char hour;   // Heure
    unsigned char minute; // Minute
    unsigned char second; // Seconde
    unsigned char mode;   // Mode (AM/PM)
} Time;
/**********************************************************/

/******************* Function Declarations Section ********************/
// RTC Function Declarations
void Write_Byte_To_DS1307_RTC(unsigned char, unsigned char);
unsigned char Read_Byte_From_DS1307_RTC(unsigned char);
void Write_Bytes_To_DS1307_RTC(unsigned char, unsigned char*, unsigned char);
void Read_Bytes_From_DS1307_RTC(unsigned char, unsigned char*, unsigned int);
void Set_DS1307_RTC_Time(unsigned char, unsigned char, unsigned char, unsigned char);
void Set_DS1307_RTC_Date(unsigned char, unsigned char, unsigned char, unsigned char);
Date Get_DS1307_RTC_Date(void);
Time Get_DS1307_RTC_Time(void);
/**********************************************************/

/******************* Global Variables Declarations Section ********************/
// Global RTC Array and temp variable
extern unsigned char pRTCArray[4];
extern unsigned char Temp;
/**********************************************************/

/******************* Function Prototypes for Displaying Date and Time on LCD ********************/
void DisplayDateOnLCD(SystemData *pDate);
void DisplayTimeToLCD(SystemData *pTime);
/**********************************************************/

#endif	/* RTC_H */
