#include "timer.h"



void timer_start(timer_t *timer, uint32_t ms)
{

    timer->start_time = HAL_GetTick();

    timer->delay_time = ms;

    timer->active = 1;

}



uint8_t timer_expired(timer_t *timer)
{

    if(timer->active == 0)
    {
        return 0;
    }


    if(HAL_GetTick() - timer->start_time >= timer->delay_time)
    {

        timer->active = 0;

        return 1;

    }


    return 0;

}