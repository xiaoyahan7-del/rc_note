#ifndef SOFT_PWM_H
#define SOFT_PWM_H


#include "stdint.h"



void soft_pwm_init(void);



void soft_pwm_set(uint8_t led,
                  uint8_t duty);



void soft_pwm_update(void);



void breath_run(void);



#endif