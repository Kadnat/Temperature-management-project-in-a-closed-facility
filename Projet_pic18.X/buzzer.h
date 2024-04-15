/*
 * File:   buzzer.h
 * Author: Nathanaël BLAVO BALLARIN
 *
 * Created on March 23, 2024, 13:16
 */

#ifndef BUZZER_H
#define	BUZZER_H

#include "common.h" 

/******************* Function Prototypes *******************/

/**
 * @brief Controls the activation of the buzzer.
 * 
 * This function controls the activation of the buzzer based on the input parameter.
 * 
 * @param activate 1 to activate the buzzer, 0 to deactivate.
 */
void buzzer(uint8_t activate);

/***********************************************************/

#endif	/* BUZZER_H */
