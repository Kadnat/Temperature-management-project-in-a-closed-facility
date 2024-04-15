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
/*
 * @Brief              : To save data in eeprom AT24C32.
 * @Param pSystem_data : Pointer to the data structure.
 * @Return None.        
 */
void save_in_eeprom(SystemData* pSystem_data);

/*
 * @Brief              : To update system data.
 * @Param pSystem_data : Pointer to the data structure.
 * @Return None.        
 */
void update_system_data(SystemData* pSystem_data);

/*
 * @Brief              : To read the various addresses in the eeprom.
 * @Param  None.
 * @Return None.        
 */
void read_eep_address_in_eeprom(void);

/*
 * @Brief              : To save the various addresses in the eeprom.
 * @Param  None.
 * @Return None.        
 */
void save_eep_address_in_eeprom(void);

/*
 * @Brief              : To write data in SD Card.
 * @Param pSystem_data : Pointer to the data structure.
 * @Return None.        
 */
void update_SD_tab(SystemData* pSystem_data);

/*
 * @Brief              : To save SD last write address in eeprom AT24C32.
 * @Param  None.
 * @Return None.        
 */
void save_sd_address_in_eeprom(void);

/*
 * @Brief              : To read SD last write address in eeprom AT24C32.
 * @Param  None.
 * @Return None.        
 */
void read_sd_address_in_eeprom(void);

/*
 * @Brief              : To reset the various addresses in the eeprom.
 * @Param  None.
 * @Return None.        
 */
void reset_eep_address_in_eeprom(void);

/*
 * @Brief  : Extracts all alarms stored in the eeprom
 * @Param  None.
 * @Return None.        
 */
void extract_all_alarms(void);

/*
 * @Brief              : To reset SD last write address in eeprom AT24C32.
 * @Param  None.
 * @Return None.        
 */
void reset_sd_address_in_eeprom(void);

/*
 * @Brief              : Send data for X days to HMI.
 * @Param  number_days : Number of days to extract.
 * @Return None.        
 */
void extract_data_for_days(int number_days);

/*
 * @Brief               : Temperature management function .
 * @Param  pSystem_data : Pointer to the data structure.
 * @Return None.        
 */
void temp_management(SystemData* pSystem_data);

/*
 * @Brief              : Send current system status to HMI.
 * @Param  None.
 * @Return None.        
 */
void log_system(SystemData* pSystem_data);

/*
 * @Brief              : Launch screen.
 * @Param  None.
 * @Return None.        
 */
void launch_screen(void);

/**
 * @brief Reads the temperature from the DS18B20 sensor and prints it.
 * 
 * @param pSystem_data Pointer to the system data structure.
 * @return The temperature in Celsius.
 */
float print_temperature(SystemData* pSystem_data);
/**********************************************************************/

#endif	/* TEMP_MONITORING_H */
