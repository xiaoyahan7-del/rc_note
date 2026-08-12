#include "state_machine.h"

#include "led.h"

#include "buzzer.h"

#include "key.h"

#include "soft_pwm.h"

static state_t current_state;

typedef void (*state_func_t)(void);

/*
 * 初始状态
 * 四灯灭
 */
static void state_idle_run(void)
{

    led_all_off();

    uint8_t event;

    event = key_scan();

    if (event == KEY_LONG)
    {
        buzzer_beep();

        current_state = STATE_FLOW;
    }
}

/*
 * LED1 LED2 流水
 */
static void state_flow_run(void)
{

    led_all_off();

    led_on(LED1_PIN);
    HAL_Delay(300);


    led_all_off();

    led_on(LED2_PIN);
    HAL_Delay(300);


    led_all_off();

    led_on(LED3_PIN);
    HAL_Delay(300);


    led_all_off();

    led_on(LED4_PIN);
    HAL_Delay(300);

}


/*
 * LED3 LED4 呼吸
 */
static void state_breath_run(void)
{

    breath_run();

    soft_pwm_update();

    HAL_Delay(10);

    if (key_scan() == KEY_SHORT)
    {
        buzzer_beep();

        current_state = STATE_FLOW;
    }
}

/*
 * 状态表
 */
static const state_func_t state_table[STATE_COUNT] =
    {

        [STATE_IDLE] = state_idle_run,

        [STATE_FLOW] = state_flow_run,

        [STATE_BREATH] = state_breath_run

};

void state_machine_init(void)
{

current_state =
STATE_IDLE;

}

void state_machine_run(void)
{

    if (current_state < STATE_COUNT)
    {

        state_table[current_state]();
    }
}