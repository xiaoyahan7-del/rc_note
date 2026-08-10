#include "state_machine.h"
#include "led.h"


static state_t current_state;



typedef void (*state_func_t)(void);


static void state_idle_run(void)
{
    led_off(0xF8);

    HAL_Delay(1000);


    current_state = STATE_RUNNING;
}

static void state_running_run(void)
{

    led_on(LED1_PIN);
    HAL_Delay(200);

    led_off(LED1_PIN);


    led_on(LED2_PIN);
    HAL_Delay(200);

    led_off(LED2_PIN);


    led_on(LED3_PIN);
    HAL_Delay(200);

    led_off(LED3_PIN);


    led_on(LED4_PIN);
    HAL_Delay(200);

    led_off(LED4_PIN);



    current_state = STATE_ALARM;

}

static void state_alarm_run(void)
{

    led_on(LED1_PIN|
           LED2_PIN|
           LED3_PIN|
           LED4_PIN);


    HAL_Delay(100);


    led_off(LED1_PIN|
            LED2_PIN|
            LED3_PIN|
            LED4_PIN);


    HAL_Delay(100);



    current_state = STATE_IDLE;

}

static const state_func_t state_table[STATE_COUNT] =
{
    [STATE_IDLE] = state_idle_run,
    [STATE_RUNNING] = state_running_run,
    [STATE_ALARM] = state_alarm_run
};



void state_machine_init(void)
{
    current_state = STATE_IDLE;
}



void state_machine_run(void)
{
    if(current_state < STATE_COUNT)
    {
        state_table[current_state]();
    }
}