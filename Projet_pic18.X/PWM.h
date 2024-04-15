/*
 * File:   PWM.h
 * Author: Nathanaël BLAVO BALLARIN
 *
 * Created on March 23, 2024, 10:59
 */

#ifndef PWM_H
#define	PWM_H

/******************* Includes Section ********************/
#include "common.h"
/***********************************************************/

/******************* Function Prototypes *******************/

/**
 * @brief Starts the PWM module.
 */
void start_pwm(void);

/**
 * @brief Sets the PWM duty cycle.
 * 
 * @param duty The PWM duty cycle (in percentage).
 */
void set_pwm_duty(float duty);

/***********************************************************/

#endif	/* PWM_H */
