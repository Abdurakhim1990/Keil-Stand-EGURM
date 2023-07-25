#ifndef CAN_EGURM_H
#define CAN_EGURM_H

#include "main.h"
#include "can_init_egurm.h"

#define CAN_ID_TX_1					0xaabbU
#define CAN_ID_TX_2					0xaabbU
#define CAN_ID_RX_1					0x1aabbU
#define CAN_ID_RX_2					0x2aabbU


can_trasnmit_message_struct* pGetTransmitMessage(void);
can_receive_message_struct* pGetReceiveMessage(void);
void CanReceive(void);
void CanTransmit(void);

#endif
