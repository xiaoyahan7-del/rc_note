#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "stdint.h"


typedef enum
{

    STATE_IDLE = 0,     // 初始状态，全部熄灭

    STATE_FLOW,         // 流水灯状态

    STATE_BREATH,       // 呼吸灯状态

    STATE_COUNT

}state_t;



void state_machine_init(void);


void state_machine_run(void);


#endif