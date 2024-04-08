#ifndef PWM_H
#define	PWM_H

#include "common.h" // include processor files - each processor file is guarded.  

void start_pwm(void);
void set_pwm_duty(float duty);

#endif	/* XC_HEADER_TEMPLATE_H */

