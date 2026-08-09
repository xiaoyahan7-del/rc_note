#ifndef BLINK_H
#define BLINK_H


#include "stdint.h"


/*
 * LED闪烁配置
 *
 * led_num:
 *      LED编号
 *
 * on_time:
 *      点亮时间(ms)
 *
 * off_time:
 *      熄灭时间(ms)
 */
typedef struct
{

    uint8_t led_num;

    uint32_t on_time;

    uint32_t off_time;

} blink_cfg_t;



/*
 * 执行一次LED闪烁
 */
void blink(blink_cfg_t *cfg);


#endif