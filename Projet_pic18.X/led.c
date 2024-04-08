/*
 * File:   led.c
 * Author: Nathanael
 *
 * Created on 24 mars 2024, 16:55
 */


#include "led.h"

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