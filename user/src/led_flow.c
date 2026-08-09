#include "led_flow.h"
#include "led.h"
#include "signal.h"

static led_flow_config current_config;

static uint8_t current_step = 0;

static flow_mode_t current_mode = FLOW_ONE;

static int last_signal = -1;

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

            uint8_t led_num;


            led_num =
            current_config.steps[current_step].led_num;


            led_on(led_num);

            HAL_Delay(
                current_config.steps[current_step].on_ms
            );


            led_off(led_num);

            HAL_Delay(
                current_config.steps[current_step].off_ms
            );


            current_step++;


            if(current_step >= LED_FLOW_STEP_COUNT)
            {
                current_step=0;
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


void flow_mode_update(void)
{

    if(signal == last_signal)
    {
        return;
    }


    last_signal = signal;


    switch(signal)
    {

        case 0:

            flow_mode_set(FLOW_IDLE);

            break;


        case 1:

            flow_mode_set(FLOW_ONE);

            break;


        case 2:

            flow_mode_set(FLOW_TWO);

            break;


        case 3:

            flow_mode_set(FLOW_ALL);

            break;


        default:

            flow_mode_set(FLOW_IDLE);

            break;

    }

}


void flow_mode_set(flow_mode_t mode)
{

    if(current_mode != mode)
    {

        current_mode = mode;

        current_step = 0;
        
        buzzer_mode_set(mode);

    }

}

