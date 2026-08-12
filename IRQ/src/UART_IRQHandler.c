#include "UART_IRQHandler.h"

uint8_t rx_buffer[5] = {0};

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1){
        HAL_UART_Receive_IT(&huart1, rx_buffer, 5);
    }
}