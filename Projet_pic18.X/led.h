/************************** LED Header File**************************/
/*
 * File:   led.h
 * Author: Nathanaël BLAVO BALLARIN
 *
 * Created on March 24, 2024, 16:55
 */

#ifndef LED_H
#define	LED_H

/******************* Includes Section ********************/
#include "common.h" 
#include "temp_monitoring.h"
/***********************************************************/

/******************* Macros and Definitions *******************/

#define RED_LED PORTDbits.RD3
#define GREEN_LED PORTDbits.RD2
#define BLUE_LED PORTDbits.RD5
#define TRIS_RED_LED TRISDbits.TRISD3
#define TRIS_GREEN_LED TRISDbits.TRISD2
#define TRIS_BLUE_LED TRISDbits.TRISD5

/***************************************************************/

/******************* Function Prototypes *******************/

/**
 * @brief Sets the mode of the LEDs based on the system state.
 * 
 * This function sets the mode of the LEDs (red, green, and blue) based on the
 * error type specified in the system state.
 * 
 * @param psystem_state A pointer to the system state containing the error type.
 */
void led_set_mode(SystemData *psystem_state);

/**************************************************************/

#endif	/* LED_H */
