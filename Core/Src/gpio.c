#include "gpio.h"


void MX_GPIO_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /*
     * GPIO时钟开启
     */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();



    /*
     * 初始化输出默认状态
     *
     * LED灭
     * 蜂鸣器关闭
     */
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7 |
        GPIO_PIN_8,
        GPIO_PIN_RESET
    );



    /*
     * PA4 PA5 PA6 PA7
     * LED
     *
     * PA8
     * 蜂鸣器
     *
     * 全部普通GPIO输出
     */
    GPIO_InitStruct.Pin =
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7 |
        GPIO_PIN_8;


    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;


    GPIO_InitStruct.Pull =
        GPIO_NOPULL;


    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(
        GPIOA,
        &GPIO_InitStruct
    );



    /*
     * PC11 按键输入
     *
     * 根据你的硬件：
     * 按下 = 高电平
     *
     */
    GPIO_InitStruct.Pin =
        GPIO_PIN_11;


    GPIO_InitStruct.Mode =
        GPIO_MODE_INPUT;


    GPIO_InitStruct.Pull =
        GPIO_PULLDOWN;


    HAL_GPIO_Init(
        GPIOC,
        &GPIO_InitStruct
    );

}