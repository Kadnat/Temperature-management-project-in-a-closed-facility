/*
 * File:   heater.c
 * Author: Nathanael
 *
 * Created on April 8, 2024, 9:32 PM
 */

#include "heater.h"

/**
 * @brief Sets the mode of the heater relay.
 * 
 * This function sets the mode of the heater relay (ON or OFF) based on the
 * state specified.
 * 
 * @param state The state to set the heater relay to (ON or OFF).
 */
void heater_set_mode(BooleanState state)
{
    TRIS_HEATER_RELAY = 0;
    switch(state)
    {
        case OFF:
            HEATER_RELAY = 1;
            break;
            
        case ON:
            HEATER_RELAY = 0;
            break;
            
        default:
            HEATER_RELAY = 1;
            break;
    }
}
