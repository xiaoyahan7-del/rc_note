#include "slave_protocol.h"

#include "buzzer.h"
#include "can.h"
#include "tim.h"

#define BREATH_PWM_MAX          999U
#define DUTY_FEEDBACK_PERIOD_MS 10U
#define NOISE_PERIOD_MS         2U
#define BEEP_ON_TIME_MS         100U
#define BEEP_OFF_TIME_MS        100U

static volatile uint8_t led_command_pending;
static volatile uint8_t led_command_data[CAN_DLC_LED_CONTROL];
static volatile uint8_t beep_command_pending;
static volatile uint8_t beep_command_data[CAN_DLC_BEEP_COMMAND];

static uint8_t breath_enabled;
static uint16_t breath_period_ms = 2000U;
static uint32_t breath_start_tick;
static float breath_duty;
static uint8_t beep_remaining;
static uint8_t beep_is_on;
static uint32_t beep_change_tick;
static uint16_t noise_counter;
static uint32_t feedback_tick;
static uint32_t noise_tick;

static void can_send(uint32_t id, uint32_t ide, uint8_t dlc,
                     const uint8_t data[8])
{
    CAN_TxHeaderTypeDef header = {0};
    uint32_t mailbox;

    header.IDE = ide;
    header.RTR = CAN_RTR_DATA;
    header.DLC = dlc;
    if (ide == CAN_ID_STD)
    {
        header.StdId = id;
    }
    else
    {
        header.ExtId = id;
    }
    (void)HAL_CAN_AddTxMessage(&hcan1, &header, (uint8_t *)data, &mailbox);
}

static void process_received_commands(uint32_t now)
{
    uint8_t i;
    uint8_t count;

    if (led_command_pending != 0U)
    {
        led_command_pending = 0U;
        breath_enabled = (led_command_data[0] != 0U) ? 1U : 0U;
        breath_period_ms = ((uint16_t)led_command_data[1] << 8U) |
                           (uint16_t)led_command_data[2];
        if (breath_period_ms < 20U)
        {
            breath_period_ms = 20U;
        }
        breath_start_tick = now;
    }

    if (beep_command_pending != 0U)
    {
        beep_command_pending = 0U;
        count = 0U;
        for (i = 0U; i < CAN_DLC_BEEP_COMMAND; i++)
        {
            if (beep_command_data[i] == 0x01U)
            {
                count++;
            }
        }
        beep_remaining = count;
        beep_is_on = 0U;
        buzzer_off();
        beep_change_tick = now;
    }
}

static void update_breath_leds(uint32_t now)
{
    uint32_t phase;
    uint32_t half_period;
    uint32_t compare;

    if (breath_enabled == 0U)
    {
        breath_duty = 0.0F;
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0U);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0U);
        return;
    }

    phase = (now - breath_start_tick) % breath_period_ms;
    half_period = breath_period_ms / 2U;
    if (phase < half_period)
    {
        breath_duty = (float)phase / (float)half_period;
    }
    else
    {
        breath_duty = (float)(breath_period_ms - phase) /
                      (float)(breath_period_ms - half_period);
    }

    compare = (uint32_t)(breath_duty * (float)BREATH_PWM_MAX);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, compare);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, compare);
}

static void update_buzzer(uint32_t now)
{
    if ((beep_is_on != 0U) &&
        ((now - beep_change_tick) >= BEEP_ON_TIME_MS))
    {
        buzzer_off();
        beep_is_on = 0U;
        beep_change_tick = now;
        if (beep_remaining > 0U)
        {
            beep_remaining--;
        }
    }
    else if ((beep_is_on == 0U) && (beep_remaining > 0U) &&
             ((now - beep_change_tick) >= BEEP_OFF_TIME_MS))
    {
        buzzer_on();
        beep_is_on = 1U;
        beep_change_tick = now;
    }
}

static void send_duty_feedback(uint32_t now)
{
    union
    {
        float value;
        uint8_t bytes[4];
    } payload;
    uint8_t data[8] = {0};
    uint8_t i;

    if ((now - feedback_tick) < DUTY_FEEDBACK_PERIOD_MS)
    {
        return;
    }
    feedback_tick += DUTY_FEEDBACK_PERIOD_MS;
    payload.value = breath_duty;
    for (i = 0U; i < CAN_DLC_DUTY_FEEDBACK; i++)
    {
        data[i] = payload.bytes[i];
    }
    can_send(CAN_ID_DUTY_FEEDBACK, CAN_ID_STD,
             CAN_DLC_DUTY_FEEDBACK, data);
}

static void send_slave_noise(uint32_t now)
{
    uint8_t data[8] = {0};

    if ((now - noise_tick) < NOISE_PERIOD_MS)
    {
        return;
    }
    noise_tick += NOISE_PERIOD_MS;
    data[0] = (uint8_t)(noise_counter >> 8U);
    data[1] = (uint8_t)noise_counter;
    noise_counter++;
    can_send(CAN_ID_SLAVE_NOISE, CAN_ID_EXT, CAN_DLC_SLAVE_NOISE, data);
}

void slave_protocol_init(void)
{
    CAN_FilterTypeDef filter = {0};
    uint32_t standard_filter = CAN_ID_LED_CONTROL << 21U;
    uint32_t extended_filter = (CAN_ID_BEEP_COMMAND << 3U) | 0x04U;
    uint32_t now = HAL_GetTick();

    /* One 32-bit list-mode bank accepts these two exact identifiers. */
    filter.FilterBank = 0U;
    filter.FilterMode = CAN_FILTERMODE_IDLIST;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14U;
    filter.FilterIdHigh = (standard_filter >> 16U) & 0xFFFFU;
    filter.FilterIdLow = standard_filter & 0xFFFFU;
    filter.FilterMaskIdHigh = (extended_filter >> 16U) & 0xFFFFU;
    filter.FilterMaskIdLow = extended_filter & 0xFFFFU;

    if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan1,
                                     CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }
    if ((HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) ||
        (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK))
    {
        Error_Handler();
    }

    buzzer_init();
    breath_start_tick = now;
    feedback_tick = now;
    noise_tick = now;
    beep_change_tick = now - BEEP_OFF_TIME_MS;
}

void slave_protocol_run(void)
{
    uint32_t now = HAL_GetTick();

    process_received_commands(now);
    update_breath_leds(now);
    update_buzzer(now);
    send_duty_feedback(now);
    send_slave_noise(now);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    uint8_t i;

    if ((hcan->Instance != CAN1) ||
        (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK))
    {
        return;
    }

    if ((header.IDE == CAN_ID_STD) &&
        (header.StdId == CAN_ID_LED_CONTROL) &&
        (header.DLC == CAN_DLC_LED_CONTROL))
    {
        for (i = 0U; i < CAN_DLC_LED_CONTROL; i++)
        {
            led_command_data[i] = data[i];
        }
        led_command_pending = 1U;
    }
    else if ((header.IDE == CAN_ID_EXT) &&
             (header.ExtId == CAN_ID_BEEP_COMMAND) &&
             (header.DLC == CAN_DLC_BEEP_COMMAND))
    {
        for (i = 0U; i < CAN_DLC_BEEP_COMMAND; i++)
        {
            beep_command_data[i] = data[i];
        }
        beep_command_pending = 1U;
    }
}
