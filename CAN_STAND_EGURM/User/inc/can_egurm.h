#ifndef CAN_EGURM_H
#define CAN_EGURM_H

#include "main.h"
#include "can_init_egurm.h"

#define CAN_ID_SAS_CS						0x18F01D13
#define CAN_ID_DM1SINGLE				0x18FECA13
#define CAN_ID_DM1BAM						0x18ECFF13
#define CAN_ID_DM1PACK					0x18EBFF13
#define CAN_ID_DM2SINGLE				0x18FECB13
#define CAN_ID_DM2BAM						0x18EC0013
#define CAN_ID_DM2PACK					0x18EB0013
#define CAN_ID_UDS_RESPONSE			0x18DAF113

#define CAN_ID_EEC1							0x0CF00400
#define CAN_ID_TCO							0x0CFE6CEE
#define CAN_ID_DM3							0x18FECC13
#define CAN_ID_UDS_REQEST				0x18DA13F1



can_trasnmit_message_struct* pGetTransmitMessage(void);
can_receive_message_struct* pGetReceiveMessage(void);
void CanReceive(void);
void CanTransmit(void);

#endif
