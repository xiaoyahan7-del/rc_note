#include "buzzer_ctrl.h"
#include "buzzer.h"
#include "timer.h"
#include "led_flow.h"


static uint8_t buzzer_enable_flag = 0;

static uint32_t buzzer_tick = 0;


static beep_config_t current_beep;

static uint8_t beep_state = 0;

static timer_t beep_timer;

void buzzer_mode_set(flow_mode_t mode)
{

    buzzer_off();

    beep_state = 0;


    current_beep.on_time = 0;
    current_beep.off_time = 0;


    switch(mode)
    {

        case FLOW_TWO:

            current_beep.on_time = 200;
            current_beep.off_time = 800;

            break;


        case FLOW_ALL:

            current_beep.on_time = 50;
            current_beep.off_time = 100;

            break;


        default:

            break;

    }


    // 新模式重新开始计时
    timer_start(&beep_timer, current_beep.off_time);

}

void buzzer_ctrl_run(void)
{

    // 当前模式不需要蜂鸣器
    if(current_beep.on_time == 0)
    {

        buzzer_off();

        return;

    }


    // 定时器没有到期，不做任何事情
    if(timer_expired(&beep_timer) == 0)
    {
        return;
    }


    if(beep_state == 0)
    {

        // 开始鸣叫

        buzzer_on();

        beep_state = 1;


        // 响多少时间后关闭

        timer_start(&beep_timer, current_beep.off_time);
    }
    else
    {

        // 停止鸣叫

        buzzer_off();

        beep_state = 0;


        // 停多少时间后再次响

        timer_start(&beep_timer,
                    current_beep.off_time);

    }

}
