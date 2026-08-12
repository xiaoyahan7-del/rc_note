#ifndef __LED_H
#define __LED_H

#include "main.h"


#define LED1_PIN GPIO_PIN_5
#define LED2_PIN GPIO_PIN_7
#define LED3_PIN GPIO_PIN_4
#define LED4_PIN GPIO_PIN_6

#define LED_PORT GPIOA


void led_on(uint16_t pin);
void led_off(uint16_t pin);
void led_all_off(void);


#endif