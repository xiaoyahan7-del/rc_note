#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"


#define BUZZER_PORT GPIOB
#define BUZZER_PIN GPIO_PIN_0


void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_beep(void);


#endif