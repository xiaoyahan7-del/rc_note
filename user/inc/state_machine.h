#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


#include "stdint.h"



typedef enum
{

    STATE_IDLE = 0,

    STATE_FLOW,

    STATE_BREATH,

    STATE_COUNT


}state_t;



void state_machine_init(void);


void state_machine_run(void);



#endif