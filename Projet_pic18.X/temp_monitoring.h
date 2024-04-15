/************************** Temp Monitoring Header File **************************/
/*
 * File:   temp_monitoring.h
 * Author: Nathanaël BLAVO BALLARIN
 *
 * Created on 1 avril 2024, 11:31
 */

#ifndef TEMP_MONITORING_H
#define	TEMP_MONITORING_H

/******************** Includes Section ********************/
#include "common.h"
/**********************************************************/

/******************** Data Types Declarations Section ********************/
typedef enum {
    NO_ERROR,
    TOO_HOT,
    TOO_COLD,
} ErrorType;

typedef struct {
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
    ErrorType error_type;
} SystemData;
/**********************************************************/

/******************** Global Variables Declarations Section ********************/
extern uint8_t activate_buzzer;
/**********************************************************************/

/******************** Function Prototypes Section ********************/
void save_in_eeprom(SystemData* pSystem_data);
void update_system_data(SystemData* pSystem_data);
void read_eep_address_in_eeprom(void);
void save_eep_address_in_eeprom(void);
void update_SD_tab(SystemData* pSystem_data);
void save_sd_address_in_eeprom(void);
void read_sd_address_in_eeprom(void);
void reset_eep_address_in_eeprom(void);
void extract_all_alarms(void);
void reset_sd_address_in_eeprom(void);
void extract_data_for_days(int number_days);
void temp_management(SystemData* pSystem_data);
void log_system(SystemData* pSystem_data);
/**********************************************************************/

#endif	/* TEMP_MONITORING_H */
