/**************************LED.c**************************/
/*
 * File:   led.c
 * Author: Nathanael
 *
 * Created on March 24, 2024, 16:55
 */

#include "led.h"

/**
 * @brief Sets the mode of the LEDs based on the system state.
 * 
 * This function sets the mode of the LEDs (red, green, and blue) based on the
 * error type specified in the system state.
 * 
 * @param psystem_state A pointer to the system state containing the error type.
 */
void led_set_mode(SystemData *psystem_state)
{
    TRIS_RED_LED = 0;
    TRIS_GREEN_LED = 0;
    TRIS_BLUE_LED = 0;
    switch(psystem_state->error_type)
    {
        case NO_ERROR:
            GREEN_LED = 1;
            RED_LED = 0;
            BLUE_LED = 0;
            break;
            
        case TOO_COLD:
            GREEN_LED = 0;
            RED_LED = 0;
            BLUE_LED = 1;
            break;
            
        case TOO_HOT:
            GREEN_LED = 0;
            RED_LED = 1;
            BLUE_LED = 0;
            break;
            
        default:
            GREEN_LED = 0;
            RED_LED = 0;
            BLUE_LED = 0;
            break;
    }
}
