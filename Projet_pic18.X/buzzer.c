/*
 * File:   buzzer.c
 * Author: Nathanael
 *
 * Created on 23 mars 2024, 13:16
 */
#include "buzzer.h"

void buzzer(uint8_t activate) {
    TRISEbits.TRISE0=0;
    
    if(activate == 1)
    {
        PORTE |=0x1;
    }
    else
    {
        PORTE &=0xFE;
    }
    
     
}
