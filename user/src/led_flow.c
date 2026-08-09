#include "led_flow.h"
#include "led.h"


static led_flow_config current_config;


static uint8_t current_step = 0;



void config_apply(led_flow_config config)
{
    current_config = config;

    current_step = 0;
}



void led_flow_run(void)
{

    led_on(
        current_config.steps[current_step].led_num
    );


    HAL_Delay(
        current_config.steps[current_step].on_ms
    );


    led_off(
        current_config.steps[current_step].led_num
    );


    HAL_Delay(
        current_config.steps[current_step].off_ms
    );


    current_step++;


    if(current_step >= LED_FLOW_STEP_COUNT)
    {
        current_step = 0;
    }

}