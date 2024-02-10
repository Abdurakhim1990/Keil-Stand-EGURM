#include "can_egurm.h"

can_trasnmit_message_struct can_transmit_message;
can_receive_message_struct can_receive_message;

QueueCan QueueCanTx;

DiagnosticMess DiagMessDm1;
DiagnosticMess DiagMessDm2;
UdsMessage UdsResponseMess;

typedef struct {
	uint8_t can_transm_uds;
	uint8_t queue_can_tran;
	uint8_t queue_can_send;
	uint8_t interrupt;
} Count;///////////////////

Count count_queue = {.can_transm_uds = 0, .queue_can_tran = 0, .queue_can_send = 0, .interrupt = 0};

//*******************************************************//
can_trasnmit_message_struct* pGetTransmitMessage(void)
{
	return &can_transmit_message;
}

//*******************************************************//
can_receive_message_struct* pGetReceiveMessage(void)
{
	return &can_receive_message;
}

//*******************************************************//
void CanTransmitEngineSpeed(uint16_t engine_speed)
{
	can_trasnmit_message_struct transmit_message;
	engine_speed = (engine_speed % 8031) * 8;
	
	transmit_message.tx_efid = CAN_ID_EEC1;
	transmit_message.tx_sfid = 0x00;
	transmit_message.tx_ft = CAN_FT_DATA;
	transmit_message.tx_ff = CAN_FF_EXTENDED;
	transmit_message.tx_dlen = 8;
	for(size_t i = 0; i < 8; ++i){
		transmit_message.tx_data[i] = 0xFF;
	}
	
	transmit_message.tx_data[3] = (uint8_t)(0x00FF & engine_speed);
	transmit_message.tx_data[4] = (uint8_t)(0x00FF & (engine_speed >> 8));
	
	QueueCanTransmitStand(&transmit_message);
}

//*******************************************************//
void CanTransmitTachographSpeed(uint16_t tachograph_speed)
{
	can_trasnmit_message_struct transmit_message;
	tachograph_speed = (tachograph_speed % 251) * 256;
	
	transmit_message.tx_efid = CAN_ID_TCO1;
	transmit_message.tx_sfid = 0x00;
	transmit_message.tx_ft = CAN_FT_DATA;
	transmit_message.tx_ff = CAN_FF_EXTENDED;
	transmit_message.tx_dlen = 8;
	for(size_t i = 0; i < 6; ++i){
		transmit_message.tx_data[i] = 0xFF;
	}
	transmit_message.tx_data[6] = (uint8_t)(0x00FF & tachograph_speed);
	transmit_message.tx_data[7] = (uint8_t)(0x00FF & (tachograph_speed >> 8));
	
	QueueCanTransmitStand(&transmit_message);
}

//*******************************************************//
void CanTransmitDM2andDM3(mode_dm2_dm3 mode)
{
	can_trasnmit_message_struct transmit_message;
	
	transmit_message.tx_efid = CAN_ID_DM3;
	transmit_message.tx_sfid = 0x00;
	transmit_message.tx_ft = CAN_FT_DATA;
	transmit_message.tx_ff = CAN_FF_EXTENDED;
	transmit_message.tx_dlen = 8;
	if(mode == DM_RESET_ERROR){
		transmit_message.tx_data[0] = 0xCB;
		transmit_message.tx_data[1] = 0xFE;
		transmit_message.tx_data[2] = 0x00;
	} else if(mode == DM_READ_ERROR){
		transmit_message.tx_data[0] = 0xCC;
		transmit_message.tx_data[1] = 0xFE;
		transmit_message.tx_data[2] = 0x00;
	}
	for(size_t i = 3; i < 8; ++i){
		transmit_message.tx_data[i] = 0xFF;
	}
	QueueCanTransmitStand(&transmit_message);
}

//*******************************************************//
void CanTransmitUdsRequest(uint8_t* frame)
{
	++count_queue.can_transm_uds;
	can_trasnmit_message_struct transmit_message;
	transmit_message.tx_efid = CAN_ID_UDS_REQEST;
	transmit_message.tx_sfid = 0x00;
	transmit_message.tx_ft = CAN_FT_DATA;
	transmit_message.tx_ff = CAN_FF_EXTENDED;
	transmit_message.tx_dlen = 8;
	for(size_t i = 0; i < 8; ++i){
		transmit_message.tx_data[i] = frame[i];
	}
	QueueCanTransmitStand(&transmit_message);
}

//*******************************************************//
void TransmitMailboxEmpty(void)
{
	++count_queue.interrupt;
	QueueCanTx.data_tx_lock = CAN_TX_UNLOCK;
	if(QueueCanTx.num_active_queue){
		can_interrupt_enable(CAN1, CAN_INT_TME);
		QueueCanTransmitSend();
	} else {
		can_interrupt_disable(CAN1, CAN_INT_TME);
	}
}

//*******************************************************//
void CanClearResetPreviouslyActiveDTCs(void)
{
	CanTransmitDM2andDM3(DM_RESET_ERROR);
}

//*******************************************************//
void CanPreviouslyActiveDTCsCodes(void)
{
	CanTransmitDM2andDM3(DM_READ_ERROR);
}

//********************************************//
void CanEgurmReceive(void)
{
	if(can_receive_message.rx_efid == CAN_ID_SAS_CS){
		CanSasCsMessage(can_receive_message.rx_data);
		__ASM("NOP");
		
	} else if(can_receive_message.rx_efid == CAN_ID_DM1SINGLE){
		DignosticDmSingle(&DiagMessDm1, can_receive_message.rx_data);
		
	} else if(can_receive_message.rx_efid == CAN_ID_DM1BAM){
		DignosticDmBam(&DiagMessDm1, can_receive_message.rx_data);
		
	} else if(can_receive_message.rx_efid == CAN_ID_DM1PACK){
		DignosticDmPack(&DiagMessDm1, can_receive_message.rx_data);
		
	} else if(can_receive_message.rx_efid == CAN_ID_DM2SINGLE){
		DignosticDmSingle(&DiagMessDm2, can_receive_message.rx_data);
		__ASM("NOP");
		
	} else if(can_receive_message.rx_efid == CAN_ID_DM2BAM){
		DignosticDmBam(&DiagMessDm2, can_receive_message.rx_data);
		__ASM("NOP");
		
	} else if(can_receive_message.rx_efid == CAN_ID_DM2PACK){
		DignosticDmPack(&DiagMessDm2, can_receive_message.rx_data);
		__ASM("NOP");
		
	} else if(can_receive_message.rx_efid == CAN_ID_UDS_RESPONSE){
		UdsResponse(&UdsResponseMess, can_receive_message.rx_data);
		__ASM("NOP");
	}
}

//*******************************************************//
void QueueCanTransmitStand(can_trasnmit_message_struct* transmit_message)
{
	uint8_t num_queue_tx = QueueCanTx.begin_queue + QueueCanTx.num_active_queue;
	num_queue_tx %= LEN_QUEUE_CAN_TX;
	if(num_queue_tx < LEN_QUEUE_CAN_TX){
		++count_queue.queue_can_tran;
		QueueCanTx.can_transmit_queue[num_queue_tx].tx_efid = transmit_message->tx_efid;
		QueueCanTx.can_transmit_queue[num_queue_tx].tx_sfid = transmit_message->tx_sfid;
		QueueCanTx.can_transmit_queue[num_queue_tx].tx_dlen = transmit_message->tx_dlen;
		QueueCanTx.can_transmit_queue[num_queue_tx].tx_ff = transmit_message->tx_ff;
		QueueCanTx.can_transmit_queue[num_queue_tx].tx_ft = transmit_message->tx_ft;
		for(uint8_t i = 0; i < 8; ++i){
			QueueCanTx.can_transmit_queue[num_queue_tx %= LEN_QUEUE_CAN_TX].tx_data[i] = transmit_message->tx_data[i];
		}
		++QueueCanTx.num_active_queue;
	}
	can_interrupt_enable(CAN1, CAN_INT_TME);
	QueueCanTransmitSend();
}

//*******************************************************//
void QueueCanTransmitSend(void)
{
	if(QueueCanTx.data_tx_lock == CAN_TX_UNLOCK)
	{
		++count_queue.queue_can_send;
		can_transmit_message.tx_efid = QueueCanTx.can_transmit_queue[QueueCanTx.begin_queue].tx_efid;
		can_transmit_message.tx_sfid = QueueCanTx.can_transmit_queue[QueueCanTx.begin_queue].tx_sfid;
		can_transmit_message.tx_dlen = QueueCanTx.can_transmit_queue[QueueCanTx.begin_queue].tx_dlen;
		can_transmit_message.tx_ff = QueueCanTx.can_transmit_queue[QueueCanTx.begin_queue].tx_ff;
		can_transmit_message.tx_ft = QueueCanTx.can_transmit_queue[QueueCanTx.begin_queue].tx_ft;
		for(uint8_t i = 0; i < 8; ++i){
			can_transmit_message.tx_data[i] = QueueCanTx.can_transmit_queue[QueueCanTx.begin_queue].tx_data[i];
		}
		can_message_transmit(CAN1, &can_transmit_message);
		++QueueCanTx.begin_queue;
		QueueCanTx.begin_queue %= LEN_QUEUE_CAN_TX;
		--QueueCanTx.num_active_queue;
		QueueCanTx.data_tx_lock = CAN_TX_LOCK;
	}
}

//*******************************************************//
void CAN1_TX_IRQHandler(void)
{
	if(can_interrupt_flag_get(CAN1, CAN_INT_FLAG_MTF0) == SET){
		can_interrupt_flag_clear(CAN1, CAN_INT_FLAG_MTF0);
		TransmitMailboxEmpty();
		__ASM("NOP");
	}
	if(can_interrupt_flag_get(CAN1, CAN_INT_FLAG_MTF1) == SET){
		can_interrupt_flag_clear(CAN1, CAN_INT_FLAG_MTF1);
		TransmitMailboxEmpty();
		__ASM("NOP");
	}
	if(can_interrupt_flag_get(CAN1, CAN_INT_FLAG_MTF2) == SET){
		can_interrupt_flag_clear(CAN1, CAN_INT_FLAG_MTF2);
		TransmitMailboxEmpty();
		__ASM("NOP");
	}

}

//*******************************************************//
void CAN1_RX0_IRQHandler(void)
{
	/* check the receive message */
	can_message_receive(CAN1, CAN_FIFO0, &can_receive_message);
	CanEgurmReceive();
}

//*******************************************************//
void CAN1_RX1_IRQHandler(void)
{
  /* check the receive message */
  can_message_receive(CAN1, CAN_FIFO1, &can_receive_message);
	CanEgurmReceive();
}


DiagnosticMess* SelectDM;
///////////////////////////
//*******************************************************************//
void ExecutionFunc(void (*Func)(DiagnosticMess*, uint8_t*), uint8_t* buff)
{
	Func(SelectDM, buff);
}

///////////////////////////
//*******************************************************************//
void FuncSelectDM(uint8_t sel_dm)
{
	if(sel_dm == 1){
		SelectDM = &DiagMessDm1;
	} else if(sel_dm == 2){
		SelectDM = &DiagMessDm2;
	}
}


