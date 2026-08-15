#ifndef SLAVE_PROTOCOL_H
#define SLAVE_PROTOCOL_H

#include <stdint.h>

/* CAN报文ID */
#define CAN_ID_LED_CONTROL 0x0FFU
#define CAN_ID_DUTY_FEEDBACK 0x0AAU
#define CAN_ID_BEEP_COMMAND 0x01020101U
#define CAN_ID_MASTER_NOISE 0x012U
#define CAN_ID_SLAVE_NOISE 0x02010101U

/* DLC */
#define CAN_DLC_LED_CONTROL 3U
#define CAN_DLC_DUTY_FEEDBACK 4U
#define CAN_DLC_BEEP_COMMAND 8U
#define CAN_DLC_MASTER_NOISE 2U
#define CAN_DLC_SLAVE_NOISE 2U

void slave_protocol_init(void);
void slave_protocol_run(void);

#endif
