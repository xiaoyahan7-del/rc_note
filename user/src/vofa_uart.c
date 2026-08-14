#include "vofa_uart.h"

#include <math.h>
#include <string.h>

#include "main.h"
#include "usart.h"
#include "buzzer.h"

#define VOFA_SEND_INTERVAL_MS  10U
#define SINE_PERIOD_MS         2000U
#define TWO_PI                 6.28318530718f

/* 保存 USART1 最近收到的一个字节。 */
static uint8_t rx_data = 0U;
static uint32_t last_send_time = 0U;

void vofa_uart_init(void)
{
    rx_data = 0U;
    last_send_time = HAL_GetTick();
    buzzer_init();
}

void vofa_uart_run(void)
{
    uint32_t now = HAL_GetTick();

    /* 尝试接收一个字节；只等待 1 ms，避免阻塞正弦波发送。 */
    if (HAL_UART_Receive(&huart1, &rx_data, 1U, 1U) == HAL_OK)
    {
        /* VOFA+ 发送大写 B 时，让蜂鸣器响一次。 */
        if (rx_data == 'B')
        {
            buzzer_beep();
        }
    }

    /* 每 10 ms 向 VOFA+ 发送一个 JustFloat 正弦波采样点。 */
    if ((uint32_t)(now - last_send_time) >= VOFA_SEND_INTERVAL_MS)
    {
        uint8_t frame[8];
        float phase;
        float sine_value;

        last_send_time = now;
        phase = TWO_PI * (float)(now % SINE_PERIOD_MS) / (float)SINE_PERIOD_MS;
        sine_value = sinf(phase);

        /* JustFloat：4 字节 float 数据 + 帧尾 00 00 80 7F。 */
        memcpy(frame, &sine_value, sizeof(sine_value));
        frame[4] = 0x00U;
        frame[5] = 0x00U;
        frame[6] = 0x80U;
        frame[7] = 0x7FU;

        (void)HAL_UART_Transmit(&huart1, frame, sizeof(frame), 100U);
    }
}
