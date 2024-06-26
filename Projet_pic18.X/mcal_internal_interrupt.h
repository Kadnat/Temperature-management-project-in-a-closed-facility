/* 
 * File     : mcal_internal_interrupt.h
 * Author   : Mohamed Ahmed Abdel Wahab
 * LinkedIn : https://www.linkedin.com/in/mohamed-abdel-wahab-162413253/
 * Github   : https://github.com/moabdelwahab6611
 * Created on June 2, 2023, 9:32 PM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H

/**************************Includes-Section*****************************/
/***********************************************************************/

/**********************Macro Declarations-Section***********************/
#define INTERRUPT_FEATURE_ENABLE 1U
//#define INTERRUPT_PRIORITY_LEVELS_ENABLE            INTERRUPT_FEATURE_ENABLE

#define EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE      INTERRUPT_FEATURE_ENABLE
#define EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE  INTERRUPT_FEATURE_ENABLE

#define ADC_INTERRUPT_FEATURE_ENABLE                INTERRUPT_FEATURE_ENABLE

#define TIMER0_INTERRUPT_FEATURE_ENABLE             INTERRUPT_FEATURE_ENABLE
#define TIMER1_INTERRUPT_FEATURE_ENABLE             INTERRUPT_FEATURE_ENABLE
#define TIMER2_INTERRUPT_FEATURE_ENABLE             INTERRUPT_FEATURE_ENABLE
#define TIMER3_INTERRUPT_FEATURE_ENABLE             INTERRUPT_FEATURE_ENABLE

#define USART_TX_INTERRUPT_FEATURE_ENABLE          INTERRUPT_FEATURE_ENABLE
#define USART_RX_INTERRUPT_FEATURE_ENABLE          INTERRUPT_FEATURE_ENABLE

#define MSSP_I2C_INTERRUPT_FEATURE_ENABLE           INTERRUPT_FEATURE_ENABLE
#define MSSP_I2C_BUS_COLL_INTERRUPT_FEATURE_ENABLE  INTERRUPT_FEATURE_ENABLE

#define  CCP1_INTERRUPT_FEATURE_ENABLE              INTERRUPT_FEATURE_ENABLE
#define  CCP2_INTERRUPT_FEATURE_ENABLE              INTERRUPT_FEATURE_ENABLE
/***********************************************************************/

/**********************Macro Declarations-Section***********************/
/* @Brief : Macro to enable interrupts. */
#define INTERRUPT_ENABLE    1
/* @Brief : Macro to disable interrupts. */
#define INTERRUPT_DISABLE   0

/* @Brief : Macro for interrupts Occur. */
#define INTERRUPT_OCCUR     1
/* @Brief : Macro for interrupts to Not OCcur. */
#define INTERRUPT_NOT_OCCUR 0

/* @Brief : Macro to enable interrupts priority. */
#define INTERRUPT_PRIORITY_ENABLE    1
/* @Brief : Macro to disable interrupts priority. */
#define INTERRUPT_PRIORITY_DISABLE   0
/***********************************************************************/

/******************Macro Function Declarations-Section******************/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : Macro function to enable priority levels on interrupts. */
#define INTERRUPT_PriorityLevelsEnable()  (RCONbits.IPEN = 1)
/* @Brief : Macro function to disable priority levels on interrupts. */
#define INTERRUPT_PriorityLevelsDisable() (RCONbits.IPEN = 0)

/* @Brief : Macro function to enable high priority global interrupts. */
#define INTERRUPT_GlobalInterruptHighEnable()  (INTCONbits.GIEH = 1)
/* @Brief : Macro function to disable high priority global interrupts. */
#define INTERRUPT_GlobalInterruptHighDisable() (INTCONbits.GIEH = 0)

/* @Brief : Macro function to enable low priority global interrupts. */ 
#define INTERRUPT_GlobalInterruptLowEnable()   (INTCONbits.GIEL = 1)
/* @Brief : Macro function to disable low priority global interrupts. */
#define INTERRUPT_GlobalInterruptLowDisable()  (INTCONbits.GIEL = 0)
#else
/* @Brief : Macro function to enable global interrupts. */ 
#define INTERRUPT_GlobalInterruptEnable()  (INTCONbits.GIE = 1)
/* @Brief : Macro function to disable global interrupts. */
#define INTERRUPT_GlobalInterruptDisable() (INTCONbits.GIE = 0)

/* @Brief : Macro function to enable peripheral interrupts. */ 
#define INTERRUPT_PeripheralInterruptEnable()  (INTCONbits.PEIE = 1)
/* @Brief : Macro function to disable peripheral interrupts. */
#define INTERRUPT_PeripheralInterruptDisable() (INTCONbits.PEIE = 0)
#endif
/***********************************************************************/

/********************Data Types Declarations-Section********************/
/* @Brief : Priority configuration. */
typedef enum
{
    INTERRUPT_LOW_PRIORITY = 0,     /* @Brief : Low Priority Selection. */
    INTERRUPT_HIGH_PRIORITY         /* @Brief : High Priority Selection. */
}interrupt_priority_cfg;
/***********************************************************************/

/******************Macro Function Declarations-Section******************/
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* Brief : This routine clears the interrupt enable for the (ADC) module. */
#define ADC_InterruptDisable()         (PIE1bits.ADIE = 0)
/* Brief : This routine sets enable interrupt for the (ADC) module. */
#define ADC_InterruptEnable()          (PIE1bits.ADIE = 1)
/* Brief : This routine clears the interrupt flag for the (ADC) module. */
#define ADC_InterruptFlagClear()       (PIR1bits.ADIF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* Brief : This routine set high priority for the (ADC) module interrupt. */
#define ADC_HighPrioritySet()          (IPR1bits.ADIP = 1)
/* Brief : This routine set low priority for the (ADC) module interrupt. */
#define ADC_LowPrioritySet()           (IPR1bits.ADIP = 0)
#endif
#endif

#if USART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* @Brief : This routine clears the interrupt enable for the (USART) module. */
#define USART_TX_InterruptDisable()         (PIE1bits.TXIE = 0)
/* @Brief : This routine sets enable interrupt for the (USART) module. */
#define USART_TX_InterruptEnable()          (PIE1bits.TXIE = 1)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : This routine set high priority for the (USART) module interrupt. */
#define USART_TX_HighPrioritySet()          (IPR1bits.TXIP = 1)
/* @Brief : This routine set low priority for the (USART) module interrupt. */
#define USART_TX_LowPrioritySet()           (IPR1bits.TXIP = 0)
#endif
#endif

#if USART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* @Brief : This routine clears the interrupt enable for the (USART) module. */
#define USART_RX_InterruptDisable()         (PIE1bits.RCIE = 0)
/* @Brief : This routine sets enable interrupt for the (USART) module. */
#define USART_RX_InterruptEnable()          (PIE1bits.RCIE = 1)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : This routine set high priority for the (USART) module interrupt. */
#define USART_RX_HighPrioritySet()          (IPR1bits.RCIP = 1)
/* @Brief : This routine set low priority for the (USART) module interrupt.*/
#define USART_RX_LowPrioritySet()           (IPR1bits.RCIP = 0)
#endif
#endif

#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* @Brief : This routine clears the interrupt enable for the (TIMER0) module.*/
#define TIMER0_InterruptDisable()         (INTCONbits.TMR0IE = 0)
/* @Brief : This routine sets enable interrupt for the (TIMER0) module. */
#define TIMER0_InterruptEnable()          (INTCONbits.TMR0IE = 1)
/* @Brief : This routine clears the interrupt flag for the (TIMER0) module. */
#define TIMER0_InterruptFlagClear()       (INTCONbits.TMR0IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : This routine set high priority for the (TIMER0) module interrupt. */
#define TIMER0_HighPrioritySet()          (INTCON2bits.TMR0IP = 1)
/* @Brief : This routine set low priority for the (TIMER0) module interrupt. */
#define TIMER0_LowPrioritySet()           (INTCON2bits.TMR0IP = 0)
#endif
#endif

#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* @Brief : This routine clears the interrupt enable for the (TIMER1) module. */
#define TIMER1_InterruptDisable()         (PIE1bits.TMR1IE = 0)
/* @Brief : This routine sets enable interrupt for the (TIMER1) module. */
#define TIMER1_InterruptEnable()          (PIE1bits.TMR1IE = 1)
/* @Brief : This routine clears the interrupt flag for the (TIMER1) module. */
#define TIMER1_InterruptFlagClear()       (PIR1bits.TMR1IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : This routine set high priority for the (TIMER1) module interrupt. */
#define TIMER1_HighPrioritySet()          (IPR1bits.TMR1IP = 1)
/* @Brief : This routine set low priority for the (TIMER1) module interrupt. */
#define TIMER1_LowPrioritySet()           (IPR1bits.TMR1IP = 0)
#endif
#endif

#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* @Brief : This routine clears the interrupt enable for the (TIMER2) module. */
#define TIMER2_InterruptDisable()         (PIE1bits.TMR2IE = 0)
/* @Brief : This routine sets enable interrupt for the (TIMER2) module. */
#define TIMER2_InterruptEnable()          (PIE1bits.TMR2IE = 1)
/* @Brief : This routine clears the interrupt flag for the (TIMER2) module. */
#define TIMER2_InterruptFlagClear()       (PIR1bits.TMR2IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : This routine set high priority for the (TIMER2) module interrupt. */
#define TIMER2_HighPrioritySet()          (IPR1bits.TMR2IP = 1)
/* @Brief : This routine set low priority for the (TIMER2) module interrupt. */
#define TIMER2_LowPrioritySet()           (IPR1bits.TMR2IP = 0)
#endif
#endif

#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* @Brief : This routine clears the interrupt enable for the (TIMER3) module. */
#define TIMER3_InterruptDisable()         (PIE2bits.TMR3IE = 0)
/* @Brief : This routine sets enable interrupt for the (TIMER3) module. */
#define TIMER3_InterruptEnable()          (PIE2bits.TMR3IE = 1)
/* @Brief : This routine clears the interrupt flag for the (TIMER3) module. */
#define TIMER3_InterruptFlagClear()       (PIR2bits.TMR3IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : This routine set high priority for the (TIMER3) module interrupt. */
#define TIMER3_HighPrioritySet()          (IPR2bits.TMR3IP = 1)
/* @Brief : This routine set low priority for the (TIMER3) module interrupt. */
#define TIMER3_LowPrioritySet()           (IPR2bits.TMR3IP = 0)
#endif
#endif

#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* @Brief : This routine clears the interrupt enable for the MSSP I2C module. */
#define MSSP_I2C_InterruptDisable()         (PIE1bits.SSPIE = 0)
#define MSSP_I2C_BUS_COL_InterruptDisable() (PIE2bits.BCLIE = 0)
/* @Brief : This routine sets the interrupt enable for the MSSP I2C module. */
#define MSSP_I2C_InterruptEnable()          (PIE1bits.SSPIE = 1)
#define MSSP_I2C_BUS_COL_InterruptEnable()  (PIE2bits.BCLIE = 1)
/* @Brief : This routine clears the interrupt flag for the MSSP I2C module. */
#define MSSP_I2C_InterruptFlagClear()         (PIR1bits.SSPIF = 0)
#define MSSP_I2C_BUS_COL_InterruptFlagClear() (PIR2bits.BCLIF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : This routine set high priority for the MSSP I2C module Interrupt. */
#define MSSP_I2C_HighPrioritySet()          (IPR1bits.SSPIP = 1)
#define MSSP_I2C_BUS_COL_HighPrioritySet()  (IPR2bits.BCLIP = 1)
/* @Brief : This routine set low priority for the MSSP I2C module Interrupt. */
#define MSSP_I2C_LowPrioritySet()           (IPR1bits.SSPIP = 0)
#define MSSP_I2C_BUS_COL_LowPrioritySet()   (IPR2bits.BCLIP = 0)
#endif
#endif

#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* @Brief : This routine clears the interrupt enable for the CCP1 module. */
#define CCP1_InterruptDisable()         (PIE1bits.CCP1IE = 0)
/* @Brief : This routine sets the interrupt enable for the CCP1 module. */
#define CCP1_InterruptEnable()          (PIE1bits.CCP1IE = 1)
/* @Brief : This routine clears the interrupt flag for the CCP1 module. */
#define CCP1_InterruptFlagClear()       (PIR1bits.CCP1IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : This routine set high priority for the CCP1 module Interrupt. */
#define CCP1_HighPrioritySet()          (IPR1bits.CCP1IP = 1)
/* @Brief : This routine set low priority for the CCP1 module interrupt. */
#define CCP1_LowPrioritySet()           (IPR1bits.CCP1IP = 0)
#endif
#endif

#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/* @Brief : This routine clears the interrupt enable for the CCP2 module. */
#define CCP2_InterruptDisable()         (PIE2bits.CCP2IE = 0)
/* @Brief : This routine sets the interrupt enable for the CCP2 module. */
#define CCP2_InterruptEnable()          (PIE2bits.CCP2IE = 1)
/* @Brief : This routine clears the interrupt flag for the CCP2 module. */
#define CCP2_InterruptFlagClear()       (PIR2bits.CCP2IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
/* @Brief : This routine set high priority for the CCP2  module interrupt. */
#define CCP2_HighPrioritySet()          (IPR2bits.CCP2IP = 1)
/* @Brief : This routine set low priority for the CCP2 module interrupt. */
#define CCP2_LowPrioritySet()           (IPR2bits.CCP2IP = 0)
#endif
#endif
/***********************************************************************/

/********************Data Types Declarations-Section********************/

/***********************************************************************/

/**********Software Interfaces Functions Declarations-Section***********/

/***********************************************************************/
#endif	/* MCAL_INTERNAL_INTERRUPT_H */
