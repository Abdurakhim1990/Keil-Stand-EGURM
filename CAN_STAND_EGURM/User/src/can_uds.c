#include "can_uds.h"


uint8_t uds_cf_count = 0;/////////////


void (*pUdsResponse[])(UdsMessage* UdsResp, uint8_t* recieve_data) =
{
	[SINGLE_FRAME] = 				UdsResponseSingleFrame,
	[FIRST_FRAME] = 				UdsResponseFirstFrame,
	[CONSECUTIVE_FRAME] = 	UdsResponseConsecutiveFrame,
	[FLOW_CONTROL_FRAME] = 	UdsResponseFlowControlFrame
};

//***************************************************************//
void UdsResponse(UdsMessage* UdsResp, uint8_t* recieve_data)
{
	UdsResp->type_frame = ((recieve_data[0] & MASK_TYPE_FRAME) >> SHIFT_TYPE_FRAME);
	pUdsResponse[UdsResp->type_frame](UdsResp, recieve_data);
}

//***************************************************************//
void ResetUdsFrame(UdsMessage* UdsFrame)
{
	UdsFrame->data_length = 0;
	UdsFrame->serial_number = 0;
	UdsFrame->flow_status = FLOW_READY;
	UdsFrame->block_size = 0;
	UdsFrame->min_separation_time = 0;
	UdsFrame->counter_data_block = 0;
	UdsFrame->counter_data = 0;
	UdsFrame->UdsFrame.counter_uds_data = 0;
	UdsFrame->UdsFrame.err_id = 0;
	volatile uint16_t data_size = sizeof(UdsFrame->UdsFrame.data)/sizeof(UdsFrame->UdsFrame.data[0]);
	for(uint16_t i = 0; i < data_size; ++i){
		UdsFrame->UdsFrame.data[i] = 0;
		__ASM("NOP");
	}
}

//***************************************************************//
void UdsResponseSingleFrame(UdsMessage* UdsResp, uint8_t* recieve_data)
{
	ResetUdsFrame(UdsResp);
	UdsResp->data_length = recieve_data[0] & MASK_DATA_LENGTH;
	if(recieve_data[1] == ERROR_ID){
		UdsResp->UdsFrame.err_id = ERROR_ID;
		++UdsResp->counter_data;
		UdsResponseFrame(UdsResp, recieve_data + 1, 6);
	} else {
		UdsResponseFrame(UdsResp, recieve_data, 7);
	}
	// Выполнение данной команды
}

//***************************************************************//
void UdsResponseFirstFrame(UdsMessage* UdsResp, uint8_t* recieve_data)
{
	ResetUdsFrame(UdsResp);
	UdsResp->data_length = ((recieve_data[0] & MASK_DATA_LENGTH) << SHIFT_DATA_LENGTH);
	UdsResp->data_length += recieve_data[1];
	UdsResponseFrame(UdsResp, recieve_data + 1, 6);
	UdsRequestFlowControlFrame(UdsResp);
}

//***************************************************************//
void UdsResponseConsecutiveFrame(UdsMessage* UdsResp, uint8_t* recieve_data)
{
	UdsResp->serial_number = recieve_data[0] & MASK_SERIAL_NUMBER;
	UdsResponseFrame(UdsResp, recieve_data, 7);
	if(UdsResp->counter_data == UdsResp->data_length){
		// Выполнение данной команды
	}
}

//***************************************************************//
void UdsResponseFlowControlFrame(UdsMessage* UdsResp, uint8_t* recieve_data)
{
	
	// CanTransmitUdsRequest(recieve_data);
}

//***************************************************************//
void UdsResponseFrame(UdsMessage* UdsResp, uint8_t* recieve_data, uint8_t len)
{
	for(uint8_t i = 1; i <= len && UdsResp->counter_data < UdsResp->data_length; ++i, ++UdsResp->counter_data){
		if(UdsResp->counter_data < 2 && i == 1){
			if(UdsResp->UdsFrame.err_id == ERROR_ID){
				UdsResp->UdsFrame.sid = recieve_data[i];
			} else {
				UdsResp->UdsFrame.sid = recieve_data[i] - SID_REQUEST;
			}
		} else if(UdsResp->counter_data < 4 && i == 2){
			UdsResp->UdsFrame.did = recieve_data[i] << 8;
		} else if(UdsResp->counter_data < 4 && i == 3){
			UdsResp->UdsFrame.did += recieve_data[i];
		} else {
			UdsResp->UdsFrame.data[UdsResp->UdsFrame.counter_uds_data] = recieve_data[i];
			++UdsResp->UdsFrame.counter_uds_data;
		}
	}
}

UdsMessage UdsReqMess;
//***************************************************************//
void UdsRequest(uint8_t* data, uint8_t len, uds_did did)
{
	UdsRequestFillingFrameUds(&UdsReqMess, data, len, did);
	if(UdsReqMess.data_length <= 7){
		UdsRequestSingleFrame(&UdsReqMess);
	} else {
		while(UdsReqMess.UdsFrame.counter_uds_data < UdsReqMess.UdsFrame.data_length){
			if(UdsReqMess.counter_data < 6){
				UdsRequestFirstFrame(&UdsReqMess);
			} else {
				UdsRequestConsecutiveFrame(&UdsReqMess);
			}
		}
	}
}

//***************************************************************//
void UdsRequestFillingFrameUds(UdsMessage* UdsReq, uint8_t* data, uint8_t len, uds_did did)
{
	UdsReq->UdsFrame.did = did;
	if(UdsReq->UdsFrame.did == SYSTEM_SUPPLIER_ECU_SOFTWARE_VERSION_NUMBER_DATA_IDENTIFIER ||
						UdsReq->UdsFrame.did == ECU_SERIAL_NUMBER_DATA_IDENTIFIER){
		UdsReq->UdsFrame.sid = WRITE_DATA_BY_IDENTIFIER;
	}
	UdsReq->UdsFrame.data_length = len;
	for(uint8_t i = 0; i < len; ++i){
		UdsReqMess.UdsFrame.data[i] = data[i];
	}
	UdsReq->data_length = UdsReq->UdsFrame.data_length + sizeof(UdsReq->UdsFrame.sid)/sizeof(uint8_t)
																								+ sizeof(UdsReq->UdsFrame.did)/sizeof(uint8_t);
	UdsReq->UdsFrame.counter_uds_data = 0;
	UdsReq->counter_data = 0;
	UdsReq->serial_number = 0;
}

//***************************************************************//
void UdsRequestSingleFrame(UdsMessage* UdsReq)
{
	uint8_t buff[8] = {0};
	UdsReq->type_frame = SINGLE_FRAME;
	buff[0] = (UdsReq->type_frame << SHIFT_TYPE_FRAME) + (UdsReq->data_length & MASK_DATA_LENGTH);
	UdsRequestCreateOneFrame(UdsReq, buff + 1, 7);
	CanTransmitUdsRequest(buff);////
}

//***************************************************************//
void UdsRequestFirstFrame(UdsMessage* UdsReq)
{
	++uds_cf_count;
	uint8_t buff[8] = {0};
	UdsReq->type_frame = FIRST_FRAME;
	buff[0] = ((UdsReq->type_frame << SHIFT_TYPE_FRAME) & MASK_TYPE_FRAME) + ((UdsReq->data_length >> SHIFT_DATA_LENGTH) & MASK_DATA_LENGTH);
	buff[1] = UdsReq->data_length & 0x00FF;
	UdsRequestCreateOneFrame(UdsReq, buff + 2, 6);
	CanTransmitUdsRequest(buff);////
}

//***************************************************************//
void UdsRequestConsecutiveFrame(UdsMessage* UdsReq)
{
	++uds_cf_count;
	uint8_t buff[8] = {0};
	++UdsReq->serial_number;
	UdsReq->type_frame = CONSECUTIVE_FRAME;
	buff[0] = ((UdsReq->type_frame << SHIFT_TYPE_FRAME) & MASK_TYPE_FRAME) + (UdsReq->serial_number & MASK_SERIAL_NUMBER);
	buff[1] = UdsReq->data_length & 0x00FF;
	UdsRequestCreateOneFrame(UdsReq, buff + 1, 7);
	CanTransmitUdsRequest(buff);////
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
	for(uint8_t i = 0; i < len; ++i){
		if(UdsReq->counter_data < 6)
		{
			if(i == 0){
				buf_for_send[i] = UdsReq->UdsFrame.sid;
			} else if(sizeof(UdsReq->UdsFrame.did) == 2 && i == 1){
				buf_for_send[i] = UdsReq->UdsFrame.did >> 8;
			}	else if(sizeof(UdsReq->UdsFrame.did) == 2 && i == 2){
				buf_for_send[i] = UdsReq->UdsFrame.did & 0x00FF;
			}	else if(sizeof(UdsReq->UdsFrame.did) == 2 && i == 1){
				buf_for_send[i] = UdsReq->UdsFrame.did & 0x00FF;
			} else if(UdsReq->UdsFrame.counter_uds_data < UdsReq->UdsFrame.data_length){
				buf_for_send[i] = UdsReq->UdsFrame.data[UdsReq->UdsFrame.counter_uds_data];
				++UdsReq->UdsFrame.counter_uds_data;
			} else {
				buf_for_send[i] = 0;
			}
		} else {
			if(UdsReq->UdsFrame.counter_uds_data < UdsReq->UdsFrame.data_length){
				buf_for_send[i] = UdsReq->UdsFrame.data[UdsReq->UdsFrame.counter_uds_data];
				++UdsReq->UdsFrame.counter_uds_data;
			} else {
				buf_for_send[i] = 0;
			}
		}
		++UdsReq->counter_data;
	}
}

