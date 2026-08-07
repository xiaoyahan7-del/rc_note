#include "alarm_sys.h"
#include "led.h"
#include "buzzer.h"


#define LED_COUNT 4U

#define RUN_DELAY 500U
#define ALARM_DELAY 100U
#define BEEP_MS 300U


void blink_led(uint8_t led_num,uint16_t times,uint32_t delay_ms)
{
    uint16_t i;


    if(led_num > LED_COUNT)
    {
        return;
    }


    for(i=0;i<times;i++)
    {
        led_on(led_num);

        HAL_Delay(delay_ms);

        led_off(led_num);

        HAL_Delay(delay_ms);
    }
}



void beep(uint32_t beep_ms)
{
    buzzer_on();

    HAL_Delay(beep_ms);

    buzzer_off();
}



void alarm_sys_func(void)
{

    uint8_t led;


    /*
       当前设置为报警模式
    */


    for(led=1;led<=LED_COUNT;led++)
    {

        blink_led(led,3,ALARM_DELAY);

    }


    beep(BEEP_MS);

}