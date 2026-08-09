#include "led_flow.h"
#include "blink.h"


static led_flow_config current_config;


static uint8_t current_step = 0;



void config_apply(led_flow_config config)
{
    current_config = config;

    current_step = 0;
}



void led_flow_run(void)
{

    blink_cfg_t cfg;


    cfg.led_num =
        current_config.steps[current_step].led_num;


    cfg.on_time =
        current_config.steps[current_step].on_ms;


    cfg.off_time =
        current_config.steps[current_step].off_ms;



    blink(&cfg);



    current_step++;


    if(current_step >= LED_FLOW_STEP_COUNT)
    {
        current_step = 0;
    }

}
