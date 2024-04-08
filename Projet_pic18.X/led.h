#ifndef LED_H
#define	LED_H

#include "common.h" 
#include "temp_monitoring.h"

#define RED_LED PORTDbits.RD3
#define GREEN_LED PORTDbits.RD2
#define BLUE_LED PORTDbits.RD5
#define TRIS_RED_LED TRISDbits.TRISD3
#define TRIS_GREEN_LED TRISDbits.TRISD2
#define TRIS_BLUE_LED TRISDbits.TRISD5


void led_set_mode(SystemData *psystem_state);

#endif	/* XC_HEADER_TEMPLATE_H */

