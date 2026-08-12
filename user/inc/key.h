#ifndef KEY_H
#define KEY_H

#include "stm32f4xx_hal.h"

#define KEY_PORT GPIOC
#define KEY_PIN  GPIO_PIN_11


uint8_t key_read(void);

uint8_t key_short_press(void);

uint8_t key_long_press(void);


#endif