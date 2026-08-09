/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led_flow.h"
#include "buzzer.h"
#include "led.h"
#include "user_beep.h"
#include "alarm_sys.h"

/* USER CODE END Includes */


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */


/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */


/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */


/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void MPU_Config(void);


/* USER CODE BEGIN PFP */


/* USER CODE END PFP */


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */


  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();


  /* MCU Configuration--------------------------------------------------------*/


  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();


  /* USER CODE BEGIN Init */


  /* USER CODE END Init */


  /* Configure the system clock */
  SystemClock_Config();


  /* USER CODE BEGIN SysInit */


  /* USER CODE END SysInit */


  /* Initialize all configured peripherals */
  MX_GPIO_Init();


  /* USER CODE BEGIN 2 */


/*
 * 初始化蜂鸣器
 * 防止上电误响
 */
buzzer_init();



led_flow_config flow =
{
    {
        {1,500,100},
        {2,500,100},
        {3,500,100},
        {4,500,100}
    }
};


config_apply(flow);



/* USER CODE END 2 */


  /* Infinite loop */


  /* USER CODE BEGIN WHILE */

  while (1)
  {

    /*
     * 把报警系统交给 alarm_sys.c 管理
     */
    led_flow_run();


  }

  /* USER CODE END WHILE */


}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);


  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);


  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {

  }


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;

  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;

  RCC_OscInitStruct.HSICalibrationValue = 64;


  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;

  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;

  RCC_OscInitStruct.PLL.PLLM = 4;

  RCC_OscInitStruct.PLL.PLLN = 34;

  RCC_OscInitStruct.PLL.PLLP = 1;

  RCC_OscInitStruct.PLL.PLLQ = 2;

  RCC_OscInitStruct.PLL.PLLR = 2;

  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;

  RCC_OscInitStruct.PLL.PLLFRACN = 3072;


  if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK)
  {
    Error_Handler();
  }


  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_SYSCLK |
      RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2 |
      RCC_CLOCKTYPE_D3PCLK1 |
      RCC_CLOCKTYPE_D1PCLK1;


  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;

  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;

  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;

  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;

  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;


  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_3)!=HAL_OK)
  {
    Error_Handler();
  }

}



/**
  * @brief MPU Configuration
  */
void MPU_Config(void)
{

  MPU_Region_InitTypeDef MPU_InitStruct={0};


  HAL_MPU_Disable();


  MPU_InitStruct.Enable = MPU_REGION_ENABLE;

  MPU_InitStruct.Number = MPU_REGION_NUMBER0;

  MPU_InitStruct.BaseAddress = 0x0;

  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;

  MPU_InitStruct.SubRegionDisable = 0x87;

  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;

  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;

  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;

  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;

  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;


  HAL_MPU_ConfigRegion(&MPU_InitStruct);


  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}



/**
  * @brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{

  __disable_irq();

  while(1)
  {

  }

}



#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file,uint32_t line)
{

}

#endif