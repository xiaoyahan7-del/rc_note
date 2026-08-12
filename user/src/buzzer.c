#include "buzzer.h"


void buzzer_init(void)
{
    buzzer_off();
}


void buzzer_on(void)
{
    HAL_GPIO_WritePin(
        BUZZER_PORT,
        BUZZER_PIN,
        GPIO_PIN_SET
    );
}


void buzzer_off(void)
{
    HAL_GPIO_WritePin(
        BUZZER_PORT,
        BUZZER_PIN,
        GPIO_PIN_RESET
    );
}


void buzzer_beep(void)
{
    buzzer_on();

    HAL_Delay(100);

    buzzer_off();
}