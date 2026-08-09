#ifndef LED_FLOW_H
#define LED_FLOW_H

#include "stm32h7xx_hal.h"


#define LED_FLOW_STEP_COUNT 4U


typedef struct
{
    uint8_t led_num;
    uint16_t on_ms;
    uint16_t off_ms;

} led_step_config;



typedef struct
{
    led_step_config steps[LED_FLOW_STEP_COUNT];

} led_flow_config;



void config_apply(led_flow_config config);
/*
 * 流水灯模式
 */
typedef enum
{

FLOW_IDLE,

FLOW_ONE,

FLOW_TWO,

FLOW_ALL

}flow_mode_t;


void flow_mode_update(void);
void flow_mode_set(flow_mode_t mode);
void led_flow_run(void);



#endif