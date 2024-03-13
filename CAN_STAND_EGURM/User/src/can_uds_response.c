#include "can_uds_response_request.h"

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
	if((recieve_data[0] & MASK_TYPE_FRAME) == FLOW_CONTROL_FRAME){
		UdsResponseFlowControlFrame(GetUdsRequest(), recieve_data);
	} else {
		UdsResp->type_frame = ((recieve_data[0] & MASK_TYPE_FRAME) >> SHIFT_TYPE_FRAME);
		pUdsResponse[UdsResp->type_frame](UdsResp, recieve_data);
	}
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
	UdsFrame->err_id = ERROR_NO;
	uint16_t data_size = LENGTH_BUFF_DATA;
	for(uint16_t i = 0; i < data_size; ++i){
		UdsFrame->data[i] = 0;
	}
}

//***************************************************************//
void UdsResponseSingleFrame(UdsMessage* UdsResp, uint8_t* recieve_data)
{
	ResetUdsFrame(UdsResp);
	UdsResp->data_length = recieve_data[0] & MASK_DATA_LENGTH - 1;
	volatile uint8_t len = 7;
	if(recieve_data[1] == ERROR_YES){
		UdsResp->err_id = ERROR_YES;
		--len;
		UdsResp->sid = recieve_data[SIZE_CAN_FRAME - len];
		--UdsResp->data_length;
	} else {
		UdsResp->sid = recieve_data[SIZE_CAN_FRAME - len] - DIF_SID_RESP;
	}
	--len;
	UdsResponseFrame(UdsResp, recieve_data, len);
	UdsResponseService(UdsResp);
}

//***************************************************************//
void UdsResponseFirstFrame(UdsMessage* UdsResp, uint8_t* recieve_data)
{
	ResetUdsFrame(UdsResp);
	UdsResp->data_length = ((recieve_data[0] & MASK_DATA_LENGTH) << SHIFT_DATA_LENGTH);
	UdsResp->data_length += recieve_data[1] - 1;
	uint8_t len = 6;
	UdsResp->sid = recieve_data[SIZE_CAN_FRAME - len] - DIF_SID_RESP;
	--len;
	UdsResponseFrame(UdsResp, recieve_data, len);
	UdsRequestFlowControlFrame(UdsResp);
}

//***************************************************************//
void UdsResponseConsecutiveFrame(UdsMessage* UdsResp, uint8_t* recieve_data)
{
	UdsResp->serial_number = recieve_data[0] & MASK_SERIAL_NUMBER;
	UdsResponseFrame(UdsResp, recieve_data, 7);
	if(UdsResp->counter_data == UdsResp->data_length){
		UdsResponseService(UdsResp);
	}
}

//***************************************************************//
void UdsResponseFlowControlFrame(UdsMessage* UdsReq, uint8_t* recieve_data)
{
	UdsReq->type_frame = (recieve_data[0] & MASK_TYPE_FRAME) >> SHIFT_TYPE_FRAME;
	UdsReq->flow_status = recieve_data[0] & MASK_FLOW_STATUS;
	UdsReq->block_size = recieve_data[1];
	UdsReq->min_separation_time = recieve_data[2];
	if(UdsReq->min_separation_time > 240 && UdsReq->min_separation_time < 250){
		UdsReq->counter_min_separ = (UdsReq->min_separation_time - 240) * 100;
	} else if(UdsReq->min_separation_time < 128){
		UdsReq->counter_min_separ = UdsReq->min_separation_time;
	} else {
		UdsReq->counter_min_separ = 0;
	}
	UdsReq->counter_block_size = 0;
	if(UdsReq->min_separation_time){
		UdsReq->mode_separat = SEPARAT_MODE_ON;
	} else {
		UdsReq->mode_separat = SEPARAT_MODE_OFF;
		UdsRequestConsecutiveFrameSend();
	}
}

//***************************************************************//
void UdsResponseFrame(UdsMessage* UdsResp, uint8_t* recieve_data, uint8_t len)
{
	for(uint8_t i = SIZE_CAN_FRAME - len; i < 8 && UdsResp->counter_data < UdsResp->data_length; ++i, ++UdsResp->counter_data){
		UdsResp->data[UdsResp->counter_data] = recieve_data[i];
	}
}

//***************************************************************//
void UdsResponseService(UdsMessage* UdsResp)
{
	switch(UdsResp->sid){
		case DIAGNOSTIC_SESSION_CONTROL:	ResponseDaignosticSessionControl(UdsResp->data, UdsResp->err_id);											break;
		case ECU_RESET:										ResponseEcuReset(UdsResp->data, UdsResp->err_id);																			break;
		case SECURITY_ACCESS:							ResponseSecurityAccess(UdsResp->counter_data, UdsResp->data, UdsResp->err_id);				break;
		case COMMUNICATION_CONTROL:				ResponseCommunicationControl(UdsResp->data, UdsResp->err_id);													break;
		case TESTER_PRESENT:							ResponseTesterPresent(UdsResp->data, UdsResp->err_id);																break;
		case CONTROL_DTC_SETTING:					ResponseControlDtcSetting(UdsResp->data, UdsResp->err_id);														break;
		case READ_DATA_BY_IDENTIFIER:			ResponseReadDataByIdentifier(UdsResp->counter_data, UdsResp->data, UdsResp->err_id);	break;
		case WRITE_DATA_BY_IDENTIFIER:		ResponseWriteDataByIdentifier(UdsResp->data, UdsResp->err_id);												break;
		case CLEAR_DIAGN_INFO:						ResponseClearDiagnosticInformation(UdsResp->data, UdsResp->err_id);										break;
		case READ_DTC_INFO:								ResponseReadDtcInformation(UdsResp->counter_data, UdsResp->data, UdsResp->err_id);		break;
		case ROUTINE_CONTROL:							ResponseRoutineControl(UdsResp->counter_data, UdsResp->data, UdsResp->err_id);				break;
		case REQUEST_DOWNLOAD:						ResponseRequestDownload(UdsResp->counter_data, UdsResp->data, UdsResp->err_id);				break;
		case TRANSFER_DATA:								ResponseTransferData(UdsResp->counter_data, UdsResp->data, UdsResp->err_id);					break;
		case REQUEST_TRANSFER_EXIT:				ResponseRequestTransferExit(UdsResp->data, UdsResp->err_id);													break;
		default : break;
	}
	UdsResp->counter_data = 0;
}
