#include "vofa_uart.h"

#include "main.h"
#include "usart.h"
#include "buzzer.h"

/* 保存USART1最近收到的一个字节。 */
static uint8_t rx_data = 0U;

void vofa_uart_init(void)
{
    /* 上电时先清空接收变量。 */
    rx_data = 0U;
}

void vofa_uart_run(void)
{
    /*
     * 尝试从USART1接收1个字节，最多等待10ms。
     *
     * &huart1  ：使用USART1；
     * &rx_data ：收到的字节存放在rx_data中；
     * 1U       ：本次只接收1个字节；
     * 10U      ：最多等待10ms，避免程序一直卡在接收处。
     */
    if (HAL_UART_Receive(
            &huart1,
            &rx_data,
            1U,
            10U) == HAL_OK)
    {
        /* 收到大写字符B时，让蜂鸣器响一次。 */
        if (rx_data == 'B')
        {
            buzzer_beep();
        }
    }
}
