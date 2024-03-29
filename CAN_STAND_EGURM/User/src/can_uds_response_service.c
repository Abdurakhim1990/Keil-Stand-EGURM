#include "can_uds_response_request_service.h"

static uint32_t CalcExpectedKey(uint32_t seed, uds_access_level access_level);

struct_session_control			resp_session_control;
struct_ecu_reset						resp_ecu_reset;
struct_security_access			resp_security_access;
struct_communic_control			resp_communic_control;
struct_tester_present				resp_tester_present;
struct_control_dtc_setting	resp_control_dtc_setting;
struct_read_write_data			resp_read_data_by_ident;
struct_read_write_data			resp_write_data_by_ident;
struct_clear_diag_inform		resp_clear_diag_inform;
struct_read_dtc_inform			resp_read_dtc_inform;
struct_routine_control			resp_routine_control;
struct_request_dowload			resp_request_dowload;
struct_transfer_data				resp_transfer_data;
struct_request_transf_exit	resp_request_transf_exit;


//***************************************************************//
void ResponseDaignosticSessionControl(uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_session_control.nrc_codes = data[0];
		NegativeResponseDaignosticSessionControl(&resp_session_control);
	} else {
		uint8_t count = 0;
		resp_session_control.nrc_codes = POSITIVE_RESPONSE;
		resp_session_control.subfunct = data[count++];
		resp_session_control.P2_server = (uint16_t)(data[count++] << 8);
		resp_session_control.P2_server += data[count++];
		resp_session_control.P_2_server = (uint16_t)(data[count++] << 8);
		resp_session_control.P_2_server += data[count++];
		if(resp_session_control.subfunct == EXTENDED_DIAGNOSTIC_SESSION || resp_session_control.subfunct == PROGRAMMING_SESSION){
			resp_tester_present.state_tester = TESTER_ENABLE;
			//pUdsNextRequest();
		} else {
			resp_tester_present.state_tester = TESTER_DISABLE;
		}
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

//***************************************************************//
void ResponseEcuReset(uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_ecu_reset.nrc_codes = data[0];
		NegativeResponseEcuReset(&resp_ecu_reset);
	} else {
		uint8_t count = 0;
		resp_ecu_reset.nrc_codes = POSITIVE_RESPONSE;
		resp_ecu_reset.subfunct = data[count++];
		resp_ecu_reset.time_power_off = data[count++];
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

//***************************************************************//
void ResponseSecurityAccess(uint16_t length, uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_security_access.nrc_codes = data[0];
		NegativeResponseSecurityAccess(&resp_security_access);
	} else {
		uint8_t count = 0;
		resp_security_access.nrc_codes = POSITIVE_RESPONSE;
		resp_security_access.subfunct = data[count++];
		if(count < length && resp_security_access.subfunct % 2){
			resp_security_access.secur_access = ACCESS_DISABLE;
			resp_security_access.seed = 0;
			for(int8_t i = 3; i >= 0; --i, ++count){
				resp_security_access.seed += (uint32_t)(data[count] << (8 * i));
			}
			SendSecurAccessKey(&resp_security_access);
		} else {
			resp_security_access.secur_access = ACCESS_ENABLE;
			StandInQueueRequest(GetNegativeSession());
		}
	}
	SendRequestQueue();
}

//***************************************************************//
void ResponseCommunicationControl(uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_communic_control.nrc_codes = data[0];
		NegativeResponseCommunicationControl(&resp_communic_control);
	} else {
		uint8_t count = 0;
		resp_communic_control.nrc_codes = POSITIVE_RESPONSE;
		resp_communic_control.subfunct = data[count++];
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

//***************************************************************//
void ResponseTesterPresent(uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_tester_present.nrc_codes = data[0];
		NegativeResponseTesterPresent(&resp_tester_present);
	} else {
		uint8_t count = 0;
		resp_tester_present.nrc_codes = POSITIVE_RESPONSE;
		resp_tester_present.subfunct = data[count++];
	}
	SendRequestQueue();
}

//***************************************************************//
void ResponseControlDtcSetting(uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_control_dtc_setting.nrc_codes = data[0];
		NegativeResponseControlDtcSetting(&resp_control_dtc_setting);
	} else {
		uint8_t count = 0;
		resp_control_dtc_setting.nrc_codes = POSITIVE_RESPONSE;
		resp_control_dtc_setting.subfunct = data[count++];
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

uint8_t buff_read_data_by_ident[100];
//***************************************************************//
void ResponseReadDataByIdentifier(uint16_t length, uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_read_data_by_ident.nrc_codes = data[0];
		NegativeResponseReadDataByIdentifier(&resp_read_data_by_ident);
	} else {
		uint8_t count = 0;
		resp_read_data_by_ident.data = buff_read_data_by_ident;
		resp_read_data_by_ident.nrc_codes = POSITIVE_RESPONSE;
		resp_read_data_by_ident.did = (uint16_t)(data[count++] << 8);
		resp_read_data_by_ident.did += data[count++];
		resp_read_data_by_ident.len = length - count;
		for(int8_t i = 0; i < resp_read_data_by_ident.len; ++i){
			resp_read_data_by_ident.data[i] = data[count++];
		}
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

//***************************************************************//
void ResponseWriteDataByIdentifier(uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_write_data_by_ident.nrc_codes = data[0];
		NegativeResponseWriteDataByIdentifier(&resp_write_data_by_ident);
	} else {
		uint8_t count = 0;
		resp_write_data_by_ident.nrc_codes = POSITIVE_RESPONSE;
		resp_write_data_by_ident.did = (uint16_t)(data[count++] << 8);
		resp_write_data_by_ident.did += data[count++];
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

//***************************************************************//
void ResponseClearDiagnosticInformation(uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_clear_diag_inform.nrc_codes = data[0];
		NegativeResponseClearDiagnosticInformation(&resp_clear_diag_inform);
	} else {
		resp_clear_diag_inform.nrc_codes = POSITIVE_RESPONSE;
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

uds_dtc_format resp_dtc_inform[10];
//***************************************************************//
void ResponseReadDtcInformation(uint16_t length, uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_read_dtc_inform.nrc_codes = data[0];
		NegativeResponseReadDtcInformation(&resp_read_dtc_inform);
	} else {
		uint8_t count = 0;
		resp_read_dtc_inform.dtc = resp_dtc_inform;
		resp_read_dtc_inform.nrc_codes = POSITIVE_RESPONSE;
		resp_read_dtc_inform.subfunct = data[count++];
		resp_read_dtc_inform.dtc_status_mask = data[count++];
		for(uint8_t i = 0; (length - count) >= 4; ++i){
			resp_read_dtc_inform.dtc[i].spn = ((uint16_t)data[count++] << 8) & 0x00FF;
			resp_read_dtc_inform.dtc[i].spn += data[count++];
			resp_read_dtc_inform.dtc[i].fmi = data[count++];
			resp_read_dtc_inform.dtc[i].status_dtc = data[count++];
		}
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

uint8_t buff_routine_status[10];
//***************************************************************//
void ResponseRoutineControl(uint16_t length, uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_routine_control.nrc_codes = data[0];
		NegativeResponseRoutineControl(&resp_routine_control);
	} else {
		uint8_t count = 0;
		resp_routine_control.routine_status = buff_routine_status;
		resp_routine_control.nrc_codes = POSITIVE_RESPONSE;
		resp_routine_control.subfunct = data[count++];
		resp_routine_control.id_routine = (uint16_t)(data[count++] << 8);
		resp_routine_control.id_routine += data[count++];
		resp_routine_control.routine_info = data[count++];
		for(uint8_t i = 0; count < length; ++i){
			resp_routine_control.routine_status[i] = data[count++];
		}
		if(resp_routine_control.subfunct == STOP_ROUTINE){
			DecrReqRoutineControl();
		} else if(resp_routine_control.id_routine == STEERING_WHEEL_CALIBRATION){
			IncReqRoutineControl();
			AddQueueNegativeSession(ROUTINE_CONTROL);
			StandInQueueRequest(GetNegativeSession());
		} else {
			switch(resp_routine_control.id_routine){
				case SET_STEERING_WHEEL_ZERO:				SendZeroWheel(1);		break;
				case SET_STEERING_WHEEL_MOST_LEFT:	SendLeftWheel(1);		break;
				case SET_STEERING_WHEEL_MOST_RIGHT:	SendRightWheel(1);	break;
				case ERASE_PROGRAM_FLASH:						break;
				default: break;
			}
			//StandInQueueRequest(GetNegativeSession());
		}
	}
	SendRequestQueue();
}

uint8_t buff_data_blocks[10];
//***************************************************************//
void ResponseRequestDownload(uint16_t length, uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_request_dowload.nrc_codes = data[0];
		NegativeResponseRequestDownload(&resp_request_dowload);
	} else {
		uint8_t count = 0;
		resp_request_dowload.data_blocks = buff_data_blocks;
		resp_request_dowload.nrc_codes = POSITIVE_RESPONSE;
		resp_request_dowload.id_format_addr_len = data[count++];
		for(uint8_t i = 0; count < length; ++i){
			resp_request_dowload.data_blocks[i] = data[count++];
		}
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

uint8_t buff_send_param[10];
//***************************************************************//
void ResponseTransferData(uint16_t length, uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_transfer_data.nrc_codes = data[0];
		NegativeResponseTransferData(&resp_transfer_data);
	} else {
		uint8_t count = 0;
		resp_transfer_data.transfer_data = buff_send_param;
		resp_transfer_data.nrc_codes = POSITIVE_RESPONSE;
		resp_transfer_data.count_blocks = data[count++];
		for(uint8_t i = 0; count < length; ++i){
			resp_transfer_data.transfer_data[i] = data[count++];
		}
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

//***************************************************************//
void ResponseRequestTransferExit(uint8_t *data, uint8_t error)
{
	if(error == ERROR_YES){
		resp_request_transf_exit.nrc_codes = data[0];
		NegativeResponseRequestTransferExit(&resp_request_transf_exit);
	} else {
		resp_request_transf_exit.nrc_codes = POSITIVE_RESPONSE;
		StandInQueueRequest(GetNegativeSession());
	}
	SendRequestQueue();
}

//***************************************************************//
static uint32_t CalcExpectedKey(uint32_t seed, uds_access_level access_level)
{
    uint32_t key = seed;
    
    switch (access_level) {
        case UDS_ACCESS_LEVEL_MAINTENANCE_SHOP:
            key ^= 0xAEAEAEAE;
            key = (key << 7) | (key >> 25);
            key ^= 0x55522233;
            break;
            
        case UDS_ACCESS_LEVEL_VEHICLE_MANUF:
            key ^= 0xDCDCDCDC;
            key = (key << 11) | (key >> 21);
            key ^= 0x444EEE22;
            break;

        case UDS_ACCESS_LEVEL_SYSTEM_SUPPLIER:
            key ^= 0x46464646;
            key = (key << 17) | (key >> 15);
            key ^= 0xACDCACDC;
            break;
        
        default:
            key = 0;
    }
    return key;
}

//***************************************************************//
void SendSecurAccessKey(struct_security_access *resp_security_access)
{
	++resp_security_access->subfunct;
	resp_security_access->key = CalcExpectedKey(resp_security_access->seed, UDS_ACCESS_LEVEL_SYSTEM_SUPPLIER);
	RequestSecurityAccessKey(resp_security_access);
}

//***************************************************************//
void PeriodTesterPresent(void)
{
	static uint16_t counter = 800;
	if(resp_tester_present.state_tester == TESTER_ENABLE){
		if(counter){
			--counter;
		} else {
			SendTesterPresent(ZERO_FUNCTION);
			counter = 800;
		}
	} else {
		counter = 800;
		resp_session_control.subfunct = DEFAULT_SESSION;
	}
		
}

//***************************************************************//
uds_subfunct_session_control GetSessionMode(void)
{
	return resp_session_control.subfunct;
}

//***************************************************************//
access_secur GetSecurityAccess(void)
{
	return resp_security_access.secur_access;
}

//*******************************************************************//

