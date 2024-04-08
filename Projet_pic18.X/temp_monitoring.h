/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */


#ifndef TEMP_MONITORING_H
#define	TEMP_MONITORING_H

#include "common.h" // include processor files - each processor file is guarded.  



typedef struct{
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t temp_decimal;
    uint8_t temp_fraction;
    uint8_t command_decimal;
    uint8_t command_fraction;
    float temperature;
    unsigned char mode;
    unsigned char weekday;
    
} TemperatureData;

void save_in_eeprom(TemperatureData* pSystem_data);
void update_system_data(TemperatureData* pSystem_data);
void read_address_in_eeprom(void);
void save_address_in_eeprom(void);
void extract_one_day_of_data(void);
void SD_control(void);
#endif	/* XC_HEADER_TEMPLATE_H */

