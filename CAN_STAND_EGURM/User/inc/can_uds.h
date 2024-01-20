#ifndef CAN_UDS_H
#define CAN_UDS_H

#include "main.h"


typedef enum {
	READ_DATA_BY_IDENTIFIER		= 0x22,
	WRITE_DATA_BY_IDENTIFIER	= 0x2E,
	ERROR_ID									= 0x7F,
	SID_REQUEST								= 0x40
} uds_sid;

typedef enum {
	ECU_SERIAL_NUMBER_DATA_IDENTIFIER														= 0xF18C,
	SYSTEM_SUPPLIER_ECU_SOFTWARE_VERSION_NUMBER_DATA_IDENTIFIER	= 0xF195
} uds_did;

typedef enum {
	SHIFT_TYPE_FRAME		= 4,
	SHIFT_DATA_LENGTH		= 8
} uds_shift;

typedef enum {
	MASK_TYPE_FRAME			= 0xF0,
	MASK_DATA_LENGTH		= 0x0F,
	MASK_SERIAL_NUMBER	= 0x0F,
	MASK_FLOW_STATUS		= 0x0F
} uds_mask;

typedef enum {
	SINGLE_FRAME = 0,
	FIRST_FRAME = 1,
	CONSECUTIVE_FRAME = 2,
	FLOW_CONTROL_FRAME = 3
} type_uds_frame;

typedef enum {
	FLOW_READY = 0,
	FLOW_WAIT = 1,
	FLOW_OVERFLOW = 2
} status_flow;

#define LENGTH_BUFF_DATA		256
typedef struct{
	uds_sid		sid;
	uds_did		did;
	uds_sid		err_id;
	uint8_t		data[LENGTH_BUFF_DATA];
	
	uint8_t		counter_uds_data;
	uint16_t 	data_length;
}	FrameUds;

typedef struct{
	type_uds_frame	type_frame;
	uint16_t 				data_length;
	uint8_t 				serial_number;
	status_flow 		flow_status;
	uint8_t 				block_size;
	uint8_t 				min_separation_time;
	FrameUds				UdsFrame;
	
	uint16_t				counter_data;
	uint8_t					counter_data_block;
	uint16_t				frames_number;
} UdsMessage;

void ResetUdsFrame(UdsMessage* UdsFrame);

void UdsResponse(UdsMessage* UdsResp, uint8_t* recieve_data);
void UdsResponseSingleFrame(UdsMessage* UdsResp, uint8_t* recieve_data);
void UdsResponseFirstFrame(UdsMessage* UdsResp, uint8_t* recieve_data);
void UdsResponseConsecutiveFrame(UdsMessage* UdsResp, uint8_t* recieve_data);
void UdsResponseFlowControlFrame(UdsMessage* UdsResp, uint8_t* recieve_data);
void UdsResponseFrame(UdsMessage* UdsResp, uint8_t* recieve_data, uint8_t len);

void UdsRequest(uint8_t* data, uint8_t len, uds_did did);
void UdsRequestFillingFrameUds(UdsMessage* UdsReq, uint8_t* data, uint8_t len, uds_did did);
void UdsRequestSingleFrame(UdsMessage* UdsReq);
void UdsRequestFirstFrame(UdsMessage* UdsReq);
void UdsRequestConsecutiveFrame(UdsMessage* UdsReq);
void UdsRequestFlowControlFrame(UdsMessage* UdsResp);
void UdsRequestCreateOneFrame(UdsMessage* UdsReq, uint8_t* buf_for_send, uint8_t len);

#endif
