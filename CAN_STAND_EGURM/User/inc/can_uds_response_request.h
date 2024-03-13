#ifndef CAN_UDS_RESPONSE_REQUEST_H
#define CAN_UDS_RESPONSE_REQUEST_H

#include "main.h"

enum {
	SIZE_CAN_FRAME			= 8,
	LENGTH_BUFF_DATA		= 256,
};

typedef enum {
	SINGLE_FRAME = 0,
	FIRST_FRAME = 1,
	CONSECUTIVE_FRAME = 2,
	FLOW_CONTROL_FRAME = 3
} type_uds_frame;

typedef enum {
	SHIFT_TYPE_FRAME		= 4,
	SHIFT_DATA_LENGTH		= 8
} uds_shift;

typedef enum {
	MASK_TYPE_FRAME					= 0xF0,
	MASK_DATA_LENGTH				= 0x0F,
	MASK_SERIAL_NUMBER			= 0x0F,
	MASK_FLOW_STATUS				= 0x0F,
} uds_mask;

typedef enum {
	FLOW_READY = 0,
	FLOW_WAIT = 1,
	FLOW_OVERFLOW = 2
} status_flow;

typedef enum {
	SEPARAT_MODE_ON = 1,
	SEPARAT_MODE_OFF = 0
} separat_mode;

typedef enum {
	ERROR_YES										= 0x7F,
	ERROR_NO										= 0x00,
} error_id;

typedef enum {
	DIF_SID_RESP								= 0x40,
	DIF_KEY_RESP								= 0x01
} uds_difference;

//********************************************************************//
typedef enum {
	READ_DATA_BY_IDENTIFIER			= 0x22,
	WRITE_DATA_BY_IDENTIFIER		= 0x2E,
	ROUTINE_CONTROL							= 0X31,
	DIAGNOSTIC_SESSION_CONTROL	= 0x10,
	ECU_RESET										= 0x11,
	COMMUNICATION_CONTROL				= 0x28,
	CONTROL_DTC_SETTING					= 0x85,
	CLEAR_DIAGN_INFO						= 0x14,
	READ_DTC_INFO								= 0x19,
	SECURITY_ACCESS							= 0x27,
	TESTER_PRESENT							= 0x3E,
	REQUEST_DOWNLOAD						= 0x34,
	TRANSFER_DATA								= 0x36,
	REQUEST_TRANSFER_EXIT				= 0x37,
	
	P_UDS_NEXT_REQUEST					= 0xF1,
} uds_sid;

//********************************************************************//
typedef struct{
	type_uds_frame	type_frame;
	uint16_t 				data_length;
	uint8_t 				serial_number;
	status_flow 		flow_status;
	uint8_t 				block_size;
	uint8_t 				min_separation_time;
	
	uds_sid					sid;
	error_id				err_id;
	uint8_t					data[LENGTH_BUFF_DATA];
	
	uint16_t				counter_data;
	uint8_t					counter_block_size;
	uint8_t					counter_data_block;
	uint16_t				counter_min_separ;
	
	uint16_t				frames_number;
	separat_mode		mode_separat;
} UdsMessage;


//------------------ Headers from can_uds_response.c ----------------//
void ResetUdsFrame(UdsMessage* UdsFrame);
void UdsResponse(UdsMessage* UdsResp, uint8_t* recieve_data);
void UdsResponseSingleFrame(UdsMessage* UdsResp, uint8_t* recieve_data);
void UdsResponseFirstFrame(UdsMessage* UdsResp, uint8_t* recieve_data);
void UdsResponseConsecutiveFrame(UdsMessage* UdsResp, uint8_t* recieve_data);
void UdsResponseFlowControlFrame(UdsMessage* UdsReq, uint8_t* recieve_data);
void UdsResponseFrame(UdsMessage* UdsResp, uint8_t* recieve_data, uint8_t len);
void UdsResponseService(UdsMessage* UdsResp);

//------------------ Headers from can_uds_request.c ----------------//
void UdsRequest(UdsMessage* UdsReq);
void UdsRequestSingleFrame(UdsMessage* UdsReq);
void UdsRequestFirstFrame(UdsMessage* UdsReq);
void UdsRequestConsecutiveFrame(UdsMessage* UdsReq);
void UdsRequestConsecutiveFrameSend(void);
void UdsRequestFlowControlFrame(UdsMessage* UdsResp);
void UdsRequestCreateOneFrame(UdsMessage* UdsReq, uint8_t* buf_for_send, uint8_t len);
void UdsRequestResetFrameUds(UdsMessage* UdsReq);
void UdsDelayMs(void);

//------------------ Headers from can_uds_request_service.c ----------------//
UdsMessage* GetUdsRequest(void);

#endif
