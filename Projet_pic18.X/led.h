#ifndef LED_H
#define	LED_H

#include "common.h" 

#define RED_LED PORTDbits.RD3
#define GREEN_LED PORTDbits.RD2
#define TRIS_RED_LED TRISDbits.TRISD3
#define TRIS_GREEN_LED TRISDbits.TRISD2

enum System_state {ALARM, NORMAL};

void set_mode(enum System_state system_state);

#endif	/* XC_HEADER_TEMPLATE_H */

