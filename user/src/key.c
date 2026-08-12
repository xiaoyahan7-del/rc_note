#include "key.h"


static uint32_t press_time = 0;
static uint8_t key_state = 0;


/*
按键读取
默认：
松开 = 0
按下 = 1
*/
uint8_t key_read(void)
{
    if(HAL_GPIO_ReadPin(KEY_PORT,KEY_PIN)
        == GPIO_PIN_SET)
    {
        return 1;
    }

    return 0;
}


/*
短按检测
*/
uint8_t key_short_press(void)
{
    static uint8_t last_state = 0;


    uint8_t now = key_read();


    if(now && !last_state)
    {
        press_time = HAL_GetTick();
    }


    if(!now && last_state)
    {
        uint32_t time =
        HAL_GetTick()-press_time;


        last_state = now;


        if(time < 1000)
        {
            return 1;
        }
    }


    last_state = now;

    return 0;
}



/*
长按检测
超过1秒
*/
uint8_t key_long_press(void)
{
    static uint8_t triggered = 0;


    if(key_read())
    {
        if(!triggered &&
          HAL_GetTick()-press_time >1000)
        {
            triggered = 1;
            return 1;
        }
    }
    else
    {
        triggered = 0;
    }


    return 0;
}