#include "led.h"


void led_on(uint16_t pin)
{
    HAL_GPIO_WritePin(LED_PORT,pin,GPIO_PIN_SET);
}


void led_off(uint16_t pin)
{
    HAL_GPIO_WritePin(LED_PORT,pin,GPIO_PIN_RESET);
}


void led_all_off(void)
{
    HAL_GPIO_WritePin(
        LED_PORT,
        LED1_PIN|
        LED2_PIN|
        LED3_PIN|
        LED4_PIN,
        GPIO_PIN_RESET
    );
}