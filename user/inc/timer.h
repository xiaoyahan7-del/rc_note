#ifndef TIMER_H
#define TIMER_H

#include "stm32h7xx_hal.h"


typedef struct
{
    uint32_t start_time;
    uint32_t delay_time;
    uint8_t active;

}timer_t;


/*
 * 单次触发定时器：
 *
 * timer_start() 后开始计时；
 * timer_expired() 查询是否到期；
 * 到期后自动失效；
 * 如果不重新启动，不会再次触发。
 */


void timer_start(timer_t *timer, uint32_t ms);


uint8_t timer_expired(timer_t *timer);


#endif