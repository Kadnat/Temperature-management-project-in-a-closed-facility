/*
 * File:   buzzer.c
 * Author: Nathanael
 *
 * Created on 23 mars 2024, 13:16
 */
#include "buzzer.h"

void buzzer(int second) {
    TRISEbits.TRISE0=0;
    PORTE |=0x1;
    for(int i =0;i<second;i++)
    {
     __delay_ms(1000);   
    }
    
    PORTE &=0xFE; 
}
