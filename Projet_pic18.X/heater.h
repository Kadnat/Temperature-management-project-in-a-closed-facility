
#ifndef HEATER_H
#define	HEATER_H

#include "common.h"  

#define HEATER_RELAY PORTDbits.RD4
#define TRIS_HEATER_RELAY TRISDbits.TRISD4

void heater_set_mode(BooleanState state);

#endif	/* XC_HEADER_TEMPLATE_H */

