/*
 * File:   led.c
 * Author: Nathanael
 *
 * Created on 24 mars 2024, 16:55
 */


#include "led.h"

void set_mode(enum System_state system_state)
{
    TRIS_RED_LED = 0;
    TRIS_GREEN_LED = 0;
    switch(system_state)
    {
        case ALARM:
            GREEN_LED = 0;
            RED_LED = 1;
            break;
            
        case NORMAL:
            GREEN_LED = 1;
            RED_LED = 0;
            break;
            
        default:
            GREEN_LED = 0;
            RED_LED = 0;
            break;
    }
}