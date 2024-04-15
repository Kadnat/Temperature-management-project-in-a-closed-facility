/************************** buzzer.c **************************/
/*
 * File:   buzzer.c
 * Author: Nathanaël BLAVO BALLARIN
 *
 * Created on March 23, 2024, 13:16
 */

#include "buzzer.h"

/**
 * @brief Controls the buzzer activation.
 * 
 * This function controls the activation of the buzzer based on the input parameter.
 * 
 * @param activate 1 to activate the buzzer, 0 to deactivate.
 */
void buzzer(uint8_t activate) {
    TRISEbits.TRISE0=0; // Set pin as output
    
    if(activate == 1)
    {
        PORTE |=0x1; // Activate buzzer
    }
    else
    {
        PORTE &=0xFE; // Deactivate buzzer
    }  
}
