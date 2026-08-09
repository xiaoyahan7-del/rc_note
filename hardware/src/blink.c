#include "blink.h"
#include "led.h"
#include "stm32h7xx_hal.h"



void blink(blink_cfg_t *cfg)
{

    led_on(cfg->led_num);


    HAL_Delay(cfg->on_time);


    led_off(cfg->led_num);


    HAL_Delay(cfg->off_time);

}