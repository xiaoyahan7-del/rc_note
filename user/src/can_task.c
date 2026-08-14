#include "can_task.h"
#include "can.h"

void can_task_init(void)
{
    CAN_FilterTypeDef filter={0};
    filter.FilterBank = 0U;
    /* 创建过滤器配置变量; */
    /* 填写过滤器; */
    /* 应用过滤器; */

    /* 启动CAN; */
    /* 开启FIFO0接收通知; */
}


void can_task_run(void)
{

}