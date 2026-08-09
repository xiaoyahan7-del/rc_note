#ifndef BUZZER_CTRL_H
#define BUZZER_CTRL_H

#include "stdint.h"


typedef struct
{

    uint16_t on_time;

    uint16_t off_time;

}beep_config_t;



void buzzer_ctrl_init(void);

void buzzer_ctrl_run(void);

void buzzer_mode_set(uint8_t mode);



#endif