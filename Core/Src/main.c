/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 */
/* USER CODE END Header */

#include "main.h"

#include "gpio.h"
#include "tim.h"

#include "led.h"
#include "buzzer.h"
#include "key.h"
#include "soft_pwm.h"
#include "state_machine.h"

void SystemClock_Config(void);

int main(void)
{

    /*
        HAL初始化
    */
    HAL_Init();

    /*
        系统时钟
    */
    SystemClock_Config();

    /*
        外设初始化
    */
    MX_GPIO_Init();

    MX_TIM3_Init();

    /*
        TIM3 PWM启动
        用于软件/硬件PWM相关功能
    */
    HAL_TIM_PWM_Start(
        &htim3,
        TIM_CHANNEL_1);

    /*
        用户模块初始化
    */

    buzzer_init();

    soft_pwm_init();

    state_machine_init();

    /*
        主循环

        状态机统一入口
    */
    uint8_t current_state = 0U; // 待机状态0，流水灯1，呼吸灯2

    uint8_t key_now = 0U;
    uint8_t key_last = 0U;
    uint8_t long_sent = 0U;

    uint32_t press_tick = 0U;

    // 流水灯变量
    uint32_t last_led_time = 0U;
    uint8_t current_led = 1U;

    /* 当前呼吸亮度：0～100 */
    uint8_t breath_level = 0U;

    /* 亮度变化方向：1表示变亮，-1表示变暗 */
    int8_t breath_direction = 1;

    /* 上一次改变呼吸亮度的时间 */
    uint32_t last_breath_time = 0U;

    /* 软件PWM计数器：0～99 */
    uint8_t pwm_count = 0U;

    while (1)
    {
        /*
         * 第一部分：判断按键事件，修改状态
         */
        key_now = ((GPIOC->IDR & GPIO_PIN_11) != 0U);

        /* 刚刚按下 = 现在按下 + 上一次没按下  */
        if ((key_now == 1U) && (key_last == 0U))
        {
            press_tick = uwTick;
            long_sent = 0U;
        }

        /* 长按 = 现在按着 + 本次还没报过长按 + 已经按了至少1s */
        if ((key_now == 1U) && (long_sent == 0U) && ((uwTick - press_tick) >= 1000U))
        {
            current_state = 1U;
            long_sent = 1U;
            buzzer_beep();
        }

        /* 刚刚松开 = 现在松开 + 上一轮按下 */
        if ((key_now == 0U) && (key_last == 1U))
        {
            /* 没有触发过长按，说明这是一次短按 */
            if (long_sent == 0U)
            {
                /* 在这里处理短按 */
                /* 呼吸灯切换流水灯 */
                if (current_state == 2U)
                {
                    current_state = 1U;
                }

                /* 流水灯切换呼吸灯 */
                else
                {
                    current_state = 2U;
                }
                buzzer_beep();
            }
        }

        /*
         * 第二部分：根据当前状态运行灯效
         */
        if (current_state == 1U)
        {
            /* 流水灯 */
            if ((uwTick - last_led_time) >= 300U)
            {
                last_led_time = uwTick;

                GPIOA->BSRR =
                    ((uint32_t)(GPIO_PIN_4 |
                                GPIO_PIN_5 |
                                GPIO_PIN_6 |
                                GPIO_PIN_7)
                     << 16U);
                if (current_led == 1U)
                {
                    GPIOA->BSRR = GPIO_PIN_4;
                }

                else if (current_led == 2U)
                {
                    GPIOA->BSRR = GPIO_PIN_5;
                }

                else if (current_led == 3U)
                {
                    GPIOA->BSRR = GPIO_PIN_6;
                }

                else if (current_led == 4U)
                {
                    GPIOA->BSRR = GPIO_PIN_7;
                }

                current_led++;
                if (current_led > 4U)
                {
                    current_led = 1U;
                }
            }
        }

        else if (current_state == 2U)
        {
            /* 呼吸灯：每10ms改变一次亮度 */
            if ((uwTick - last_breath_time) >= 10U)
            {
                last_breath_time = uwTick;

                if (breath_direction == 1)
                {
                    /* 当前正在变亮 */
                    if (breath_level < 100U)
                    {
                        breath_level++;
                    }
                    else
                    {
                        breath_direction = -1;
                    }
                }
                else
                {
                    /* 当前正在变暗 */
                    if (breath_level > 0U)
                    {
                        breath_level--;
                    }
                    else
                    {
                        breath_direction = 1;
                    }
                }
            }

            pwm_count++;

            if (pwm_count >= 100U)
            {
                pwm_count = 0U;
            }
            /* PWM计数小于目标亮度时，四盏灯点亮 */
            if (pwm_count < breath_level)
            {
                GPIOA->BSRR =
                    GPIO_PIN_4 |
                    GPIO_PIN_5 |
                    GPIO_PIN_6 |
                    GPIO_PIN_7;
            }
            else
            {
                /* PWM计数达到目标亮度后，四盏灯熄灭 */
                GPIOA->BSRR =
                    ((uint32_t)(GPIO_PIN_4 |
                                GPIO_PIN_5 |
                                GPIO_PIN_6 |
                                GPIO_PIN_7)
                     << 16U);
            }
        }
        else
        {
            /* 待机，关闭全部LED */
            GPIOA->BSRR =
                ((uint32_t)(GPIO_PIN_4 |
                            GPIO_PIN_5 |
                            GPIO_PIN_6 |
                            GPIO_PIN_7)
                 << 16U);
        }

        key_last = key_now;
    }
    // state_machine_run();
}

void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /*
        开启电源时钟
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(
        PWR_REGULATOR_VOLTAGE_SCALE1);

    /*
        使用HSI
    */
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState =
        RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState =
        RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /*
        时钟树
    */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_HSI;

    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
        RCC_HCLK_DIV1;

    RCC_ClkInitStruct.APB2CLKDivider =
        RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{

    __disable_irq();

    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
}

#endif
