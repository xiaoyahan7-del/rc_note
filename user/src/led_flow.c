#include "led_flow.h"
#include "blink.h"


static led_flow_config current_config;


static uint8_t current_step = 0;

static flow_mode_t current_mode = FLOW_ONE;



void config_apply(led_flow_config config)
{
    current_config = config;

    current_step = 0;
}



void led_flow_run(void)
{

    switch(current_mode)
    {

        case FLOW_ONE:

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


            break;

        }



        case FLOW_TWO:

        {

            led_on(1);
            led_on(2);

            HAL_Delay(500);

            led_off(1);
            led_off(2);


            led_on(3);
            led_on(4);

            HAL_Delay(500);

            led_off(3);
            led_off(4);


            break;

        }



        case FLOW_ALL:

        {

            led_on(1);
            led_on(2);
            led_on(3);
            led_on(4);


            HAL_Delay(500);


            led_off(1);
            led_off(2);
            led_off(3);
            led_off(4);


            HAL_Delay(500);


            break;

        }



        default:

            break;

    }

}


void flow_mode_set(flow_mode_t mode)
{

    current_mode = mode;

    current_step = 0;

}