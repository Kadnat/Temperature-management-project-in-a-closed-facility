/* 
 * File     : mcal_interrupt_manager.c
 * Author   : Mohamed Ahmed Abdel Wahab
 * LinkedIn : https://www.linkedin.com/in/mohamed-abdel-wahab-162413253/
 * Github   : https://github.com/moabdelwahab6611
 * Created on June 2, 2023, 9:36 PM
 * Modified by Nathanaël BLAVO BALLARIN, 26 March 2024
 */

/**************************Includes-Section*****************************/
#include "mcal_interrupt_manager.h"
#include "common.h"
/***********************************************************************/

/********************Data Types Declarations-Section********************/


/***********************************************************************/

/*****************Software Interfaces Functions-Section*****************/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
void __interrupt() InterruptManagerHigh(void)
{
    if((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCUR == INTCON3bits.INT2IF))
    {
        INT2_ISR();
    }
    else{/*****Nothing*****/}
    if((INTERRUPT_ENABLE == INTCONbits.INT0IE) && (INTERRUPT_OCCUR == INTCONbits.INT0IF))
    {
        INT0_ISR();
    }
    else{/*****Nothing*****/}
}

void __interrupt(low_priority) InterruptManagerLow(void)
{
    if((INTERRUPT_ENABLE == INTCON3bits.INT1E) && (INTERRUPT_OCCUR == INTCON3bits.INT1F))
    {
        INT1_ISR();
    }
    else{/*****Nothing*****/}
}

#else
void __interrupt() InterruptManager(void)
{

 
    /**********************Timers modules interrupt*************************/
    if((INTERRUPT_ENABLE == INTCONbits.TMR0IE) && (INTERRUPT_OCCUR == INTCONbits.TMR0IF))
    {
        TMR0_ISR();
    }
    else{/*****Nothing*****/}
    if((INTERRUPT_ENABLE == PIE1bits.TMR1IE) && (INTERRUPT_OCCUR == PIR1bits.TMR1IF))
    {
        TMR1_ISR();
    }
    else{/*****Nothing*****/}
    /***********************************************************************/
    
    /***********************USART module interrupt**************************/
    if((INTERRUPT_ENABLE == PIE1bits.TXIE) && (INTERRUPT_OCCUR == PIR1bits.TXIF))
    {
        USART_TX_ISR();
    }
    else{/*****Nothing*****/}
    if((INTERRUPT_ENABLE == PIE1bits.RCIE) && (INTERRUPT_OCCUR == PIR1bits.RCIF))
    {
        USART_RX_ISR();
    }
    else{/*****Nothing*****/}
   

}
#endif
/***********************************************************************/