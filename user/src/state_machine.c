
#include "state_machine.h"

#include "led.h"
#include "buzzer.h"
#include "key.h"
#include "soft_pwm.h"


/*
    当前状态
*/
static state_t current_state;


/*
    状态函数类型
*/
typedef void (*state_func_t)(void);



/*
================================================
状态1：待机
四灯全灭

长按 -> 流水灯
短按 -> 呼吸灯
================================================
*/
static void state_idle_run(void)
{

    led_all_off();


    uint8_t key_event;

    key_event = key_scan();



    if(key_event == KEY_LONG)
    {

        buzzer_beep();


        current_state = STATE_FLOW;

    }



    else if(key_event == KEY_SHORT)
    {

        buzzer_beep();


        current_state = STATE_BREATH;

    }

}



/*
================================================
状态2：流水灯

LED1 -> LED2 -> LED3 -> LED4

短按 -> 呼吸灯
================================================
*/
static void state_flow_run(void)
{

    static uint8_t led_index = 0;

    static uint32_t last_tick = 0;



    /*
        非阻塞流水灯
    */
    if(HAL_GetTick() - last_tick >= 300)
    {

        last_tick = HAL_GetTick();



        led_all_off();



        switch(led_index)
        {

            case 0:
                led_on(LED1_PIN);
                break;


            case 1:
                led_on(LED2_PIN);
                break;


            case 2:
                led_on(LED3_PIN);
                break;


            case 3:
                led_on(LED4_PIN);
                break;

        }



        led_index++;


        if(led_index >=4)
        {
            led_index=0;
        }

    }



    /*
        短按切换呼吸灯
    */
    if(key_scan()==KEY_SHORT)
    {

        buzzer_beep();


        led_all_off();


        current_state = STATE_BREATH;

    }


}



/*
================================================
状态3：呼吸灯

LED3 LED4软件PWM

短按 -> 流水灯
================================================
*/
static void state_breath_run(void)
{


    breath_run();


    soft_pwm_update();



    /*
        短按切回流水灯
    */
    if(key_scan()==KEY_SHORT)
    {

        buzzer_beep();


        current_state = STATE_FLOW;

    }


}



/*
================================================
状态表

状态编号 -> 对应运行函数

================================================
*/
static const state_func_t state_table[STATE_COUNT]=
{

    [STATE_IDLE] = state_idle_run,


    [STATE_FLOW] = state_flow_run,


    [STATE_BREATH] = state_breath_run

};




/*
================================================
状态机初始化

上电必须进入IDLE

================================================
*/
void state_machine_init(void)
{

    current_state = STATE_IDLE;


    led_all_off();

}



/*
================================================
状态机运行

main循环只调用这里

================================================
*/
void state_machine_run(void)
{


    if(current_state < STATE_COUNT)
    {

        state_table[current_state]();

    }


}