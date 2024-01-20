#ifndef CAN_EGURM_H
#define CAN_EGURM_H

#include "main.h"
#include "can_init_egurm.h"
#include "can_dm.h"
#include "can_sas_message.h"

typedef enum {
	CAN_ID_SAS_CS						= 0x18F01D13,
	CAN_ID_DM1SINGLE				= 0x18FECA13,
	CAN_ID_DM1BAM						= 0x1CECFF13,			//0x18ECFF13
	CAN_ID_DM1PACK					= 0x1CEBFF13,			//0x18EBFF13
	CAN_ID_DM2SINGLE				= 0x18FECB13,
	CAN_ID_DM2BAM						= 0x18EC0013,
	CAN_ID_DM2PACK					= 0x18EB0013,
	CAN_ID_UDS_RESPONSE			= 0x18DAF113
} can_tx_id;

typedef enum {
	CAN_ID_EEC1							= 0x0CF00400,
	CAN_ID_TCO1							= 0x0CFE6CEE,
	CAN_ID_DM3							= 0x18FECC13,
	CAN_ID_UDS_REQEST				= 0x18DA13F1
} can_rx_id;


#define LEN_QUEUE_CAN_TX			15
typedef enum {
	CAN_TX_LOCK			= 1,
	CAN_TX_UNLOCK		= 0
} can_data_tx_lock;

typedef struct{
	can_trasnmit_message_struct can_transmit_queue[LEN_QUEUE_CAN_TX];
	uint8_t begin_queue;
	uint8_t end_queue;
	uint8_t num_active_queue;
	can_data_tx_lock data_tx_lock;
}	QueueCan;

can_trasnmit_message_struct* pGetTransmitMessage(void);
can_receive_message_struct* pGetReceiveMessage(void);

void CanReceive(void);
void CanTransmitEngineSpeed(uint16_t engine_speed);
void CanTransmitTachographSpeed(uint16_t tachograph_speed);
void CanTransmitDM3(void);
void CanTransmitUdsRequest(uint8_t* frame);

void CanEgurmReceive(void);

void QueueCanTransmitStand(can_trasnmit_message_struct* transmit_message);
void QueueCanTransmitSend(void);

//////////////
//void ExecutionFunc(void (*Func)(DiagnosticMess*, uint8_t*), uint8_t* buff);
void FuncSelectDM(uint8_t sel_dm);
#endif
