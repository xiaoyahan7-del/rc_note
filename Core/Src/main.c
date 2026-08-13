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
  while (1)
  {

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_SET)
    {

      HAL_GPIO_WritePin(
          GPIOA,
          GPIO_PIN_5,
          GPIO_PIN_SET);
    }
    else
    {

      HAL_GPIO_WritePin(
          GPIOA,
          GPIO_PIN_5,
          GPIO_PIN_RESET);
    }
  }
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