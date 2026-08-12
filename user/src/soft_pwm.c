#include "soft_pwm.h"

#include "led.h"


/*
    软件PWM周期

    100次
*/
#define PWM_PERIOD 100



static uint8_t pwm_counter = 0;



static uint8_t led3_duty = 0;

static uint8_t led4_duty = 0;




void soft_pwm_init(void)
{

    pwm_counter = 0;

    led3_duty = 0;

    led4_duty = 0;

}





/*
 设置占空比

 duty:
 0~100

*/
void soft_pwm_set(uint8_t led,
                  uint8_t duty)
{

    if(duty >100)
        duty =100;



    if(led == 3)
    {
        led3_duty=duty;
    }


    if(led == 4)
    {
        led4_duty=duty;
    }

}





/*
 软件PWM核心

 每次循环调用
*/
void soft_pwm_update(void)
{

    pwm_counter++;


    if(pwm_counter >= PWM_PERIOD)
    {
        pwm_counter=0;
    }



    /*
     LED3
    */

    if(pwm_counter < led3_duty)
    {
        led_on(LED3_PIN);
    }
    else
    {
        led_off(LED3_PIN);
    }



    /*
     LED4
    */

    if(pwm_counter < led4_duty)
    {
        led_on(LED4_PIN);
    }
    else
    {
        led_off(LED4_PIN);
    }

}




/*
 呼吸效果
*/
void breath_run(void)
{

    static uint8_t duty=0;

    static int8_t step=1;



    duty += step;



    if(duty >=100)
    {
        duty=100;

        step=-1;
    }



    if(duty ==0)
    {
        step=1;
    }



    soft_pwm_set(3,duty);

    soft_pwm_set(4,duty);


}