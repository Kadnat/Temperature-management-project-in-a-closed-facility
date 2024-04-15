#ifndef HEATER_H
#define	HEATER_H

#include "common.h"  

/******************* Macros and Definitions *******************/

#define HEATER_RELAY PORTDbits.RD4
#define TRIS_HEATER_RELAY TRISDbits.TRISD4

/***************************************************************/

/******************* Function Prototypes *******************/

/**
 * @brief Sets the mode of the heater relay.
 * 
 * This function sets the mode of the heater relay (ON or OFF) based on the
 * state specified.
 * 
 * @param state The state to set the heater relay to (ON or OFF).
 */
void heater_set_mode(BooleanState state);

/**************************************************************/

#endif	/* HEATER_H */
