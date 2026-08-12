#ifndef KEY_H
#define KEY_H

#include "main.h"


#define KEY_PORT GPIOC
#define KEY_PIN  GPIO_PIN_11


typedef enum
{
    KEY_NONE = 0,

    KEY_SHORT,

    KEY_LONG

}key_event_t;


uint8_t key_read(void);


uint8_t key_scan(void);


#endif