#include "can_uds_response_request.h"


//***************************************************************//
void UdsRequest(UdsMessage* UdsReq)
{
	if(UdsReq->data_length <= 7){
		UdsRequestSingleFrame(UdsReq);
	} else {
		UdsRequestFirstFrame(UdsReq);
	}
}

//***************************************************************//
void UdsRequestSingleFrame(UdsMessage* UdsReq)
{
	uint8_t buff[8] = {0};
	UdsReq->type_frame = SINGLE_FRAME;
	buff[0] = (UdsReq->type_frame << SHIFT_TYPE_FRAME) + (UdsReq->data_length & MASK_DATA_LENGTH);
	UdsRequestCreateOneFrame(UdsReq, buff, 7);
	CanTransmitUdsRequest(buff);
}

//***************************************************************//
void UdsRequestFirstFrame(UdsMessage* UdsReq)
{
	uint8_t buff[8] = {0};
	UdsReq->type_frame = FIRST_FRAME;
	buff[0] = ((UdsReq->type_frame << SHIFT_TYPE_FRAME) & MASK_TYPE_FRAME) + ((UdsReq->data_length >> SHIFT_DATA_LENGTH) & MASK_DATA_LENGTH);
	buff[1] = UdsReq->data_length & 0x00FF;
	UdsRequestCreateOneFrame(UdsReq, buff, 6);
	CanTransmitUdsRequest(buff);
}

//***************************************************************//
void UdsRequestConsecutiveFrame(UdsMessage* UdsReq)
{
	uint8_t buff[8] = {0};
	++UdsReq->serial_number;
	UdsReq->type_frame = CONSECUTIVE_FRAME;
	buff[0] = ((UdsReq->type_frame << SHIFT_TYPE_FRAME) & MASK_TYPE_FRAME) + (UdsReq->serial_number & MASK_SERIAL_NUMBER);
	UdsRequestCreateOneFrame(UdsReq, buff, 7);
	CanTransmitUdsRequest(buff);
}

//***************************************************************//
void UdsRequestConsecutiveFrameSend(void)
{
	UdsMessage *UdsReq = GetUdsRequest();
	if(UdsReq->flow_status == FLOW_READY && (UdsReq->block_size == 0 || UdsReq->counter_block_size < UdsReq->block_size)){
		if(UdsReq->block_size && (UdsReq->counter_data < UdsReq->data_length)){
			UdsRequestConsecutiveFrame(UdsReq);
		} else {
			while(UdsReq->counter_data < UdsReq->data_length){
				UdsRequestConsecutiveFrame(UdsReq);
			}
		}
		if(UdsReq->block_size){
			++UdsReq->counter_block_size;
			if(UdsReq->counter_block_size == UdsReq->block_size){
				UdsReq->mode_separat = SEPARAT_MODE_OFF;
			}
			UdsReq->counter_block_size %= UdsReq->block_size;
		}
	}
}

//***************************************************************//
void UdsRequestFlowControlFrame(UdsMessage* UdsResp)
{
	uint8_t mess_frame[8];
	UdsResp->type_frame = FLOW_CONTROL_FRAME;
	UdsResp->flow_status = FLOW_READY;
	UdsResp->block_size = 0;
	UdsResp->min_separation_time = 0;
	
	mess_frame[0] = ((UdsResp->type_frame << SHIFT_TYPE_FRAME) & MASK_TYPE_FRAME) + (UdsResp->flow_status & MASK_FLOW_STATUS);
	mess_frame[1] = UdsResp->block_size;
	mess_frame[2] = UdsResp->min_separation_time;
	CanTransmitUdsRequest(mess_frame);
}

//***************************************************************//
void UdsRequestCreateOneFrame(UdsMessage* UdsReq, uint8_t* buf_for_send, uint8_t len)
{
	for(uint8_t i = SIZE_CAN_FRAME - len; i < SIZE_CAN_FRAME; ++i){
		if(UdsReq->counter_data < UdsReq->data_length){
			buf_for_send[i] = UdsReq->data[UdsReq->counter_data];
			++UdsReq->counter_data;
		} else {
			buf_for_send[i] = 0;
		}
	}
}

//***************************************************************//
void UdsDelayMs(void)
{
	UdsMessage* UdsReq = GetUdsRequest();
	if(UdsReq->mode_separat == SEPARAT_MODE_ON){
		if(UdsReq->counter_min_separ){
			--UdsReq->counter_min_separ;
		} else {
			if(UdsReq->min_separation_time > 240 && UdsReq->min_separation_time < 250){
				UdsReq->counter_min_separ = (UdsReq->min_separation_time - 240) * 100;
			} else if(UdsReq->min_separation_time < 128){
				UdsReq->counter_min_separ = UdsReq->min_separation_time;
			} else {
				UdsReq->counter_min_separ = 0;
			}
			UdsRequestConsecutiveFrameSend();
		}
	}
}

//***************************************************************//
void UdsRequestResetFrameUds(UdsMessage* UdsReq)
{
	UdsReq->counter_data = 0;
	UdsReq->serial_number = 0;
	UdsReq->counter_block_size = 0;
	UdsReq->counter_min_separ = 0;
}

