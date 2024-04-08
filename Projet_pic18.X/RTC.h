/* 
 * File:   RTC.h
 * Author: microdigisoft
 *
 * Created on 30 August, 2020, 11:33 AM
 */

#ifndef RTC_H
#define	RTC_H

#include "temp_monitoring.h"

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

    
// RTC Function Declarations
void Write_Byte_To_DS1307_RTC(unsigned char, unsigned char);
unsigned char Read_Byte_From_DS1307_RTC(unsigned char);
void Write_Bytes_To_DS1307_RTC(unsigned char,unsigned char*,unsigned char);
void Read_Bytes_From_DS1307_RTC(unsigned char,unsigned char*,unsigned int);
void Set_DS1307_RTC_Time(unsigned char,unsigned char,unsigned char,unsigned char);
//unsigned char* Get_DS1307_RTC_Time(void);
void Set_DS1307_RTC_Date(unsigned char,unsigned char,unsigned char,unsigned char);
//unsigned char* Get_DS1307_RTC_Date(void);
Date Get_DS1307_RTC_Date(void);
Time Get_DS1307_RTC_Time(void); 
// Global RTC Array and temp variable
unsigned char pRTCArray[4];
unsigned char Temp;
//void DisplayTimeToLCD(unsigned char*) ;
//void DisplayDateOnLCD( unsigned char*);
void DisplayDateOnLCD(TemperatureData *pDate);
void DisplayTimeToLCD(TemperatureData *pTime);

#endif	/* RTC_H */

