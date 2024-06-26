/* 
 * File     : hal_timer1.c
 * Author   : Mohamed Ahmed Abdel Wahab
 * LinkedIn : https://www.linkedin.com/in/mohamed-abdel-wahab-162413253/
 * Github   : https://github.com/moabdelwahab6611
 * Created on June 8, 2023, 3:24 PM
 */

/**************************Includes-Section*****************************/
#include "hal_timer1.h"
/***********************************************************************/

/********************Data Types Declarations-Section********************/
static uint16_t timer1_preload = 0;
/***********************************************************************/

/*****************Helper Functions Declarations-Section*****************/
/*
 * @Brief : Callback pointer to function.
 */
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static void (*TMR1_InterruptHandler)(void) = NULL;    /* @Brief : Timer1 Interrupt Handler. */
#endif
       
/*
 * @Brief        : Timer1 Timer or Counter mode selection configuration.
 * @Param _timer : Pointer to the Timer1 module configurations.
 */
static inline void Timer1_Mode_Selection(const timer1_t *_timer);
/***********************************************************************/

/*****************Software Interfaces Functions-Section*****************/
/*
 * @Brief              : To initialize the Timer1.
 * @Param _timer       : Pointer to the Timer1 module configurations.
 * @Return Status of the function.
 *          (1)     : The function done successfully.
 *          (0) : The function has issue while performing this action.
 */
uint8_t Timer1_Init(const timer1_t *_timer)
{
    uint8_t ret = 0;
    if(NULL == _timer)
    {
        ret = 0;
    }
    else
    { 
        /* @Brief : Disable Timer1. */
        TIMER1_DISABLE_MODULE();
        /* @Brief : Timer1 Pre-scaler configuration. */
        TIMER1_PRESCALER_SELECTION(_timer->timer1_prescaler_value);
        /* @Brief : Selection of Timer1 Timer or Counter modes. */
        Timer1_Mode_Selection(_timer);
        /* @Brief : Timer1 Pre-load configuration. */
        TMR1H = (_timer->timer1_preload_value) >> 8;
        TMR1L = (uint8_t)(_timer->timer1_preload_value);  
        timer1_preload = _timer->timer1_preload_value;
        /* @Brief : Timer1 Interrupt configuration. */ 
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptEnable();
        TIMER1_InterruptFlagClear();
        TMR1_InterruptHandler = _timer->TMR1_InterruptHandler;
        /* @Brief : Timer1 Priority configuration. */ 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable(); 
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER1_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER1_LowPrioritySet();
        }
        else{/*****Nothing*****/}
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif        
#endif
        /* @Brief : Enable Timer1. */       
        TIMER1_ENABLE_MODULE();       
        ret = 1;
    }
    return ret;
}

/*
 * @Brief              : To de-initialize the Timer1.
 * @Param _timer       : Pointer to the Timer1 module configurations.
 * @Return Status of the function.
 *          (1)     : The function done successfully.
 *          (0) : The function has issue while performing this action.
 */
uint8_t Timer1_DeInit(const timer1_t *_timer)
{
    uint8_t ret = 0;
    if(NULL == _timer)
    {
        ret = 0;
    }
    else
    { 
        /* @Brief : Disable Timer1. */
        TIMER1_DISABLE_MODULE();
        /* @Brief : Timer1 Interrupt configuration. */ 
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptDisable();
#endif
        ret = 1;
    }
    return ret;  
}

/*
 * @Brief              : To write value Timer1 counter register.
 * @Param _timer       : Pointer to the Timer1 module configurations.
 * @Param _value
 * @Return Status of the function.
 *          (1)     : The function done successfully.
 *          (0) : The function has issue while performing this action.
 */
uint8_t Timer1_Write_Value(const timer1_t *_timer, uint16_t _value)
{
  uint8_t ret = 0;
    if(NULL == _timer)
    {
        ret = 0;
    }
    else
    { 
        /* @Brief : Timer1 Pre-load configuration. */
        TMR1H = (_value) >> 8;
        TMR1L = (uint8_t)(_value);
        ret = 1;
    }
    return ret;   
}

/*
 * @Brief              : To read value Timer1 counter register.
 * @Param _timer       : Pointer to the Timer1 module configurations.
 * @Param _value       : Pointer to access value to read it from the register. 
 * @Return Status of the function.
 *          (1)     : The function done successfully.
 *          (0) : The function has issue while performing this action.
 */
uint8_t Timer1_Read_Value(const timer1_t *_timer, uint16_t *_value)
{
    uint8_t ret = 0;
    uint8_t l_tmr1l = 0;
    uint8_t l_tmr1h = 0;
    if((NULL == _timer) || (NULL == _value))
    {
        ret = 0;
    }
    else
    { 
        l_tmr1l = TMR1L;
        l_tmr1h = TMR1H;
        *_value = (uint16_t)((l_tmr1h << 8) + l_tmr1l);
        ret = 1;
    }
    return ret; 
}

/*
 * @Brief : Callback pointer to function to Timer1 interrupt service routine.
 */
void TMR1_ISR(void)
{
    TIMER1_InterruptFlagClear();
    TMR1H = (timer1_preload) >> 8;
    TMR1L = (uint8_t)(timer1_preload);  
    if(TMR1_InterruptHandler)
    {
        TMR1_InterruptHandler();
    }
    else{/*****Nothing*****/}
}

/*
 * @Brief        : Timer1 Timer or Counter mode selection configuration.
 * @Param _timer : Pointer to the Timer1 module configurations.
 */
static inline void Timer1_Mode_Selection(const timer1_t *_timer)
{
    if(TIMER1_TIMER_MODE == _timer->timer1_mode)
    {
        TIMER1_ENABLE_TIMER_MODE();
    }
    else if(TIMER1_COUNTER_MODE == _timer->timer1_mode)
    {
        TIMER1_ENABLE_COUNTER_MODE();
        if(TIMER1_ASYNCHRONOUS_COUNTER_MODE == _timer->timer1_counter_mode)
        {
            TIMER1_ENABLE_ASYNCHRONOUS_COUNTER_MODE();
        }
        else if(TIMER1_SYNCHRONOUS_COUNTER_MODE == _timer->timer1_counter_mode)
        {
            TIMER1_ENABLE_SYNCHRONOUS_COUNTER_MODE();
        }
        else{/*****Nothing*****/}
    }
    else{/*****Nothing*****/}
}
/***********************************************************************/