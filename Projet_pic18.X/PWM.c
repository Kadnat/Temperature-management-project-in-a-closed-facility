/*
 * File:   PWM.c
 * Author: Nathanael
 *
 * Created on 23 mars 2024, 10:59
 */
#include "PWM.h"

void start_pwm(void)
{
    T2CONbits.TMR2ON=1;  /* Turn ON Timer2 */
}


 void set_pwm_duty(float duty)
{
    
    float calcul=0, duty1 = 100- duty;
    TRISCbits.TRISC2=0;  /* Set CCP2 pin as output for PWM out */
    CCP1CON=0x0F;        /* set PWM mode and no decimal value for PWM */
    PR2=254;             /* load period value in PR2 register */ //4kHz
    calcul = (PR2 + 1)*((float)duty1/100);           /* load duty cycle */
    CCPR1L= (char)calcul;
    //T2CON = 0b00000110; // Timer2 on, prescaler is 1
    T2CON = 0b00000100; // Timer2 on, prescaler is 1

    TMR2=0;
    
}
