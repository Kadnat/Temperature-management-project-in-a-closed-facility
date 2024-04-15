/* 
 * File     : mcal_interrupt_manager.h
 * Author   : Mohamed Ahmed Abdel Wahab
 * LinkedIn : https://www.linkedin.com/in/mohamed-abdel-wahab-162413253/
 * Github   : https://github.com/moabdelwahab6611
 * Created on June 2, 2023, 9:36 PM
 * Modified by Nathanaël BLAVO BALLARIN, 26 March 2024
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H

/**************************Includes-Section*****************************/
#include "mcal_internal_interrupt.h"
/***********************************************************************/

/**********************Macro Declarations-Section***********************/

/***********************************************************************/

/******************Macro Function Declarations-Section******************/

/***********************************************************************/

/********************Data Types Declarations-Section********************/

/***********************************************************************/

/**********Software Interfaces Functions Declarations-Section***********/
/*
 * @Brief : Callback pointer to function to Timer0 interrupt service routine.
 */
void TMR0_ISR(void);
/*
 * @Brief : Callback pointer to function to Timer1 interrupt service routine.
 */
void TMR1_ISR(void);
/*
 * @Brief : Callback pointer to function to USART Transmit interrupt service routine.
 */
void USART_TX_ISR(void);
/*
 * @Brief : Callback pointer to function to USART Receive interrupt service routine.
 */
void USART_RX_ISR(void);
/***********************************************************************/
#endif	/* MCAL_INTERRUPT_MANAGER_H */

