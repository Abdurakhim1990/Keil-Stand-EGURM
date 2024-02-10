//#pragma once
#ifndef CAN_UDS_H
#define CAN_UDS_H

#include "main.h"
#include "can_uds_service.h"

enum {
	SIZE_CAN_FRAME			= 8,
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

#define LENGTH_BUFF_DATA		256
typedef enum {
	READ_DATA_BY_IDENTIFIER			= 0x22,
	WRITE_DATA_BY_IDENTIFIER		= 0x2E,
	ROUTINE_CONTROL							= 0X31,
	DIAGNOSTIC_SESSION_CONTROL	= 0x10,
	ECU_RESET										= 0x11,
	COMMUNICATION_CONTROL				= 0x28,
	CONTROL_DTC_SETTING					= 0x85,
	CLEAR_DTC_INFO							= 0x14,
	READ_DTC_INFO								= 0x19,
	SECURITY_ACCESS							= 0x27,
	TESTER_PRESENT							= 0x3E,
	REQUEST_DOWNLOAD						= 0x34,
	TRANSFER_DATA								= 0x36,
	REQUEST_TRANSFER_EXIT				= 0x37,
} uds_sid;

typedef enum {
	ERROR_YES										= 0x7F,
	ERROR_NO										= 0x00,
} error_id;

typedef enum {
	DIF_SID_RESP								= 0x40,
	DIF_KEY_RESP								= 0x01
} uds_difference;

typedef enum {
	UDS_ACCESS_LEVEL_MAINTENANCE_SHOP		= 1,
	UDS_ACCESS_LEVEL_VEHICLE_MANUF			= 2,
	UDS_ACCESS_LEVEL_SYSTEM_SUPPLIER		= 3,
} uds_access_level;

typedef enum {
	DEFAULT_SESSION									= 0x01,
	PROGRAMMING_SESSION							= 0x02,
	EXTENDED_DIAGNOSTIC_SESSION			= 0x03
} uds_subfunct_session_control;

typedef enum {
	HARD_RESET											= 0x01,
	KEY_OFF_ON_RESET								= 0x02,
	SOFT_RESET											= 0x03
} uds_subfunct_ecu_reset;

typedef enum {
	SEED_1													= 0x01,
	SEED_2													= 0x03,
	SEED_3													= 0x05,
	SEED_4													= 0x07
} uds_subfunct_security_access;

typedef enum {
	ENABLE_RX_AND_TX											= 0x00,
	ENABLE_RX_AND_TX_MORE									= 0x80,
	ENABLE_RX_AND_DISABLE_TX							= 0x01,
	ENABLE_RX_AND_DISABLE_TX_MORE					= 0x81,
	DISABLE_RX_AND_ENABLE_TX							= 0x02,
	DISABLE_RX_AND_ENABLE_TX_MORE					= 0x82,
	DISABLE_RX_AND_TX											= 0x03,
	DISABLE_RX_AND_TX_MORE								= 0x83,
	ENABLE_RX_AND_TX_WITH_EAI							= 0x04,
	ENABLE_RX_AND_TX_WITH_EAI_MORE				= 0x84,
	DISABLE_RX_AND_TX_WITH_EAI						= 0x05,
	DISABLE_RX_AND_TX_WITH_EAI_MORE				= 0x85,
//	VEHICLE_MANUFACTURER_SPECIFIC_FROM		= 0x40,
//	VEHICLE_MANUFACTURER_SPECIFIC_TO			= 0x5F,
//	SYSTEM_SUPPLIER_SPECIFIC_FROM					= 0x60,
//	SYSTEM_SUPPLIER_SPECIFIC_TO						= 0x7E,
} uds_subfunct_communic_control;

typedef enum {
	ZERO_FUNCTION										= 0x00,
	ZERO_FUNCTION_MORE							= 0x80,
} uds_subfunct_tester_present;

typedef enum {
	DTC_SETTING_ON												= 0x01,
	DTC_SETTING_ON_MORE										= 0x81,
	DTC_SETTING_OFF												= 0x02,
	DTC_SETTING_OFF_MORE									= 0x82,
//	VEHICLE_MANUFACTURER_SPECIFIC_FROM		= 0x40,
//	VEHICLE_MANUFACTURER_SPECIFIC_TO			= 0x5F,
//	SYSTEM_SUPPLIER_SPECIFIC_FROM					= 0x60,
//	SYSTEM_SUPPLIER_SPECIFIC_TO						= 0x7E,
} uds_subfunct_control_dtc_setting;

typedef enum {
	VEHICLE_MANUFACTURE_ECU_SOFTWARE_NUMBER						= 0xF188,
	VEHICLE_MANUFACTURE_ECU_SOFTWARE_VERSION_NUMBER		= 0xF189,
	SYSTEM_SUPPLIER_IDENTIFIER												= 0xF18A,
	ECU_MANUFACTURING_DATE														= 0xF18B,
	ECU_SERIAL_NUMBER																	= 0xF18C,
	VIN																								= 0xF190,
	VEHICLE_MANUFACTURE_ECU_HARDWARE_NUMBER						= 0xF191,
	SYSTEM_SUPPLIER_ECU_HARDWARE_NUMBER								= 0xF192,
	SYSTEM_SUPPLIER_ECU_HARDWARE_VERSION_NUMBER				= 0xF193,
	SYSTEM_SUPPLIER_ECU_SOFTWARE_NUMBER								= 0xF194,
	SYSTEM_SUPPLIER_ECU_SOFTWARE_VERSION_NUMBER				= 0xF195,
	EXHAUST_REGULATION_OR_TYPE_APPROVAL_NUMBER				= 0xF196,
	SYSTEM_NAME_OR_ENGINE_TYPE												= 0xF197,
	REPAIR_SHOP_CODE_OR_TESTER_SERIAL_NUMBER					= 0xF198,
	PROGRAMMING_DATE																	= 0xF199,
	ECU_INSTALLATION_DATE															= 0xF19D,
	POWER_VOLTAGE																			= 0xF1A2,
	STEERING_WHEEL_POSITION														= 0xF1A3,
	TORQUE_SENSOR_VALUE																= 0xF1A4,
	PHASE_A_POSITION																	= 0xF1A5,
	PHASE_B_POSITION																	= 0xF1A6,
	PHASE_C_POSITION																	= 0xF1A7,
	POWER_COMPONENTS_TEMPERATURE											= 0xF1AB,
	MOTOR_TYPE																				= 0xF1AC,
	STEERING_SHAFT_SENSOR_INSTALLED										= 0xF1AD,
	MOTOR_POSITION_SENSOR_INSTALLED										= 0xF1AE,
	ECU_TEMPERATURE_SENSOR_INSTALLED									= 0xF1AF,
	TORQUE_SENSOR_INSTALLED														= 0xF1B0,
	GEARBOX_INSTALLED																	= 0xF1B1,
	ENGINE_RPM																				= 0xF1B2,
	VEHICLE_SPEED																			= 0xF1B3,
	STEERING_WHEEL_CALIBRATED													= 0xF1B4,
	STEERING_WHEEL_CALIBRATED_PROGRESS								= 0xF1B5,
	STEERING_WHEEL_ZERO_COUNTER												= 0xF1B6,
	STEERING_WHEEL_MOST_LEFT_COUNTER									= 0xF1B7,
	STEERING_WHEEL_MOST_RIGHT_COUNTER									= 0xF1B8,
	ENGINE_RPM_VALID																	= 0xF1B9,
	VEHICLE_SPEED_VALID																= 0xF1BA,
	ENGINE_RPM_CONTROL																= 0xF1BB,
	VEHICLE_SPEED_CONTROL															= 0xF1BC,
	STEERING_WHEEL_ANGLE															= 0xF1BD,
	STEERING_WHEEL_NB_TURNS														= 0xF1BE,
	CAN_SPEED																					= 0xF1EC,
	CAN_SELF_ADDRESS																	= 0xF1ED,
	CAN_TESTER_ADDRESS																= 0xF1EE,
	FINGER_PRINT																			= 0xF1EF,
	LOADER_SOFTWARE_VERSION_NUMBER										= 0xFD03,
} uds_subfunct_did;

typedef enum {
	REP_DTC_BY_STATUS_MASK								= 0x02,
} uds_subfunct_read_dtc_inform;

typedef enum {
	START_ROUTINE													= 0x01,
	STOP_ROUTINE													= 0x02,
	REQUEST_ROUTINE_RESULT								= 0x03,
} uds_subfunct_routine_control;

typedef struct{
	uds_subfunct_did									did;
	
	uds_subfunct_ecu_reset						ecu_reset;
	uds_subfunct_security_access			security_access;
	uds_subfunct_communic_control			communic_control;
	uds_subfunct_tester_present				tester_present;
	uds_subfunct_control_dtc_setting	control_dtc_setting;
	uds_subfunct_read_dtc_inform			read_dtc_inform;
	uds_subfunct_routine_control			routine_control;
}	SubFunctions;

typedef struct{
	uds_subfunct_session_control			session_control;
	uint8_t P2_server_high;
	uint8_t P2_server_low;
	uint8_t P_2_server_high;
	uint8_t P_2_server_low;
} struct_session_control;

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
	
	SubFunctions				SubFunc;
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
static uint32_t CalcExpectedKey(uint32_t seed, uds_access_level access_level);

//------------------ Headers from can_uds_request.c ----------------//
void UdsRequest(UdsMessage* UdsReq);
void UdsRequestSingleFrame(UdsMessage* UdsReq);
void UdsRequestFirstFrame(UdsMessage* UdsReq);
void UdsRequestConsecutiveFrame(UdsMessage* UdsReq);
void UdsRequestConsecutiveFrameSend(void);
void UdsRequestFlowControlFrame(UdsMessage* UdsResp);
void UdsRequestCreateOneFrame(UdsMessage* UdsReq, uint8_t* buf_for_send, uint8_t len);
void UdsDelayMs(void);

//------------------ Headers from can_uds_service.c ----------------//
UdsMessage* GetUdsRequest(void);
void UdsRequestResetFrameUds(UdsMessage* UdsReq);
void WriteDataToStructure(const uint8_t* data, const uint8_t len, const uds_subfunct_did did);
void ReadDataByIdentifier(const uint8_t* data, const uint8_t len, const uds_subfunct_did did);
void SecurityAccess(uds_subfunct_security_access security_access);
void SecurityAccessKey(uds_subfunct_security_access security_access, uint32_t key);

#endif
