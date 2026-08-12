#include "key.h"


static uint32_t press_tick = 0;

static uint8_t last_state = 0;

static uint8_t long_sent = 0;



uint8_t key_read(void)
{

    if(HAL_GPIO_ReadPin(KEY_PORT,KEY_PIN)
        == GPIO_PIN_SET)
    {
        return 1;
    }


    return 0;
}



uint8_t key_scan(void)
{

    uint8_t now;


    now = key_read();



    // 按下瞬间
    if(now && !last_state)
    {
        press_tick = HAL_GetTick();

        long_sent = 0;
    }



    // 长按
    if(now)
    {

        if(!long_sent &&
           HAL_GetTick()-press_tick > 1000)
        {

            long_sent = 1;

            last_state = now;

            return KEY_LONG;
        }

    }



    // 松开
    if(!now && last_state)
    {

        uint32_t t;

        t = HAL_GetTick()-press_tick;



        last_state = now;



        if(t < 1000)
        {
            return KEY_SHORT;
        }

    }


    last_state = now;


    return KEY_NONE;

}