#include "can_uds_response_request_service.h"

static void ServiceNotSupported(void);
static void SecurityAccessDenied(void);

typedef struct {
	uint8_t queue[5];
	uint8_t count;
} queue_negative_service;

queue_negative_service queue_neg_service = {.queue = {0}, .count = 0};

//***************************************************************//
void NegativeResponseDaignosticSessionControl(struct_session_control *resp_session_control)
{
	resp_session_control->P2_server = 0;
	resp_session_control->P_2_server = 0;
	struct_session_control* req_session_control = GetReqSessionControl();
	AddQueueNegativeSession(DIAGNOSTIC_SESSION_CONTROL);
	switch(resp_session_control->nrc_codes){
		case SUB_FUNCTION_NOT_SUPPORTED:		req_session_control->subfunct = DEFAULT_SESSION;		break;
		case INVALID_MESSAGE_LENGTH_FORMAT:	break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseEcuReset(struct_ecu_reset *resp_ecu_reset)
{
	AddQueueNegativeSession(ECU_RESET);
	switch(resp_ecu_reset->nrc_codes){
		case SERVICE_NOT_SUPPORTED:					ServiceNotSupported();					break;
		case SUB_FUNCTION_NOT_SUPPORTED:		break;
		case INVALID_MESSAGE_LENGTH_FORMAT:	break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseSecurityAccess(struct_security_access *resp_security_access)
{
	struct_security_access* req_security_access = GetReqSecurityAccess();
	AddQueueNegativeSession(SECURITY_ACCESS);
	switch(resp_security_access->nrc_codes){
		case SERVICE_NOT_SUPPORTED:				ServiceNotSupported();	break;
		case SUB_FUNCTION_NOT_SUPPORTED:	req_security_access->subfunct = SEED_3;		break;
		case INVALID_MESSAGE_LENGTH_FORMAT:				break;
		case REQUEST_SEQUENCE_ERROR:							break;
		case INVALID_KEY:	/*SendSecurAccessKey(resp_security_access);*/		break;
		case EXCEEDED_NUMBER_OF_ATTEMPTS:	StartRequiredTimeDelay(1);		break;
		case REQUIRED_TIME_DELAY_HAS_NOT_EXPIRED:	StartRequiredTimeDelay(1);		break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseCommunicationControl(struct_communic_control *resp_communic_control)
{
	resp_communic_control->subfunct = 0;
	resp_communic_control->type_communic = 0;
	resp_communic_control->id_node = 0;
	AddQueueNegativeSession(COMMUNICATION_CONTROL);
	switch(resp_communic_control->nrc_codes){
		case SERVICE_NOT_SUPPORTED:	ServiceNotSupported();	break;
		case SUB_FUNCTION_NOT_SUPPORTED:								break;
		case INVALID_MESSAGE_LENGTH_FORMAT:							break;
		case CONDITIONS_NOT_CORRECT:										break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseTesterPresent(struct_tester_present *resp_tester_present)
{
	resp_tester_present->subfunct = 0;
	switch(resp_tester_present->nrc_codes){
		case SUB_FUNCTION_NOT_SUPPORTED:		break;
		case INVALID_MESSAGE_LENGTH_FORMAT:	break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseControlDtcSetting(struct_control_dtc_setting *resp_control_dtc_setting)
{
	resp_control_dtc_setting->subfunct = 0;
	AddQueueNegativeSession(CONTROL_DTC_SETTING);
	switch(resp_control_dtc_setting->nrc_codes){
		case SERVICE_NOT_SUPPORTED:	ServiceNotSupported();	break;
		case SUB_FUNCTION_NOT_SUPPORTED:							break;
		case INVALID_MESSAGE_LENGTH_FORMAT:						break;
		case REQUEST_OUT_OF_RANGE: break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseReadDataByIdentifier(struct_read_write_data *resp_read_data)
{
	resp_read_data->did = 0;
	resp_read_data->len = 0;
	AddQueueNegativeSession(READ_DATA_BY_IDENTIFIER);
	switch(resp_read_data->nrc_codes){
		case SERVICE_NOT_SUPPORTED:	ServiceNotSupported();	break;
		case INVALID_MESSAGE_LENGTH_FORMAT:								break;
		case RESPONSE_TOO_LONG:														break;
		case REQUEST_OUT_OF_RANGE:												break;
		case SECURITY_ACCESS_DENIED:											break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseWriteDataByIdentifier(struct_read_write_data *resp_write_data)
{
	resp_write_data->len = 0;
	AddQueueNegativeSession(WRITE_DATA_BY_IDENTIFIER);
	switch(resp_write_data->nrc_codes){
		case SERVICE_NOT_SUPPORTED:		ServiceNotSupported();	break;
		case INVALID_MESSAGE_LENGTH_FORMAT:			break;
		case CONDITIONS_NOT_CORRECT:						break;
		case REQUEST_OUT_OF_RANGE:		ServiceNotSupported();	break;
		case SECURITY_ACCESS_DENIED:	SecurityAccessDenied();	break;
		case PROGRAMMING_FAILURE:								break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseClearDiagnosticInformation(struct_clear_diag_inform *resp_clear_diag_inform)
{
	resp_clear_diag_inform->len = 0;
	AddQueueNegativeSession(CLEAR_DIAGN_INFO);
	switch(resp_clear_diag_inform->nrc_codes){
		case GENERAL_REJECT:											break;
		case SERVICE_NOT_SUPPORTED:	ServiceNotSupported();		break;
		case INVALID_MESSAGE_LENGTH_FORMAT:				break;
		case SECURITY_ACCESS_DENIED:	SecurityAccessDenied();	break;
		case PROGRAMMING_FAILURE:									break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseReadDtcInformation(struct_read_dtc_inform *resp_read_dtc_inform)
{
	resp_read_dtc_inform->subfunct = 0;
	resp_read_dtc_inform->dtc_status_mask = 0;
	resp_read_dtc_inform->len = 0;
	resp_read_dtc_inform->dtc = 0;
	AddQueueNegativeSession(READ_DTC_INFO);
	switch(resp_read_dtc_inform->nrc_codes){
		case SERVICE_NOT_SUPPORTED:	ServiceNotSupported();	break;
		case SUB_FUNCTION_NOT_SUPPORTED:					break;
		case INVALID_MESSAGE_LENGTH_FORMAT:				break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseRoutineControl(struct_routine_control *resp_routine_control)
{
	resp_routine_control->subfunct = 0;
	resp_routine_control->id_routine = 0;
	resp_routine_control->routine_info = 0;
	AddQueueNegativeSession(ROUTINE_CONTROL);
	switch(resp_routine_control->nrc_codes){
		case SERVICE_NOT_SUPPORTED:					ServiceNotSupported();
								StandInQueueRequest(GetNegativeSession());		break;
		case SUB_FUNCTION_NOT_SUPPORTED:			break;
		case INVALID_MESSAGE_LENGTH_FORMAT:		break;
		case CONDITIONS_NOT_CORRECT:				ServiceNotSupported();
								StandInQueueRequest(GetNegativeSession());		break;
		case REQUEST_SEQUENCE_ERROR:				DecrReqRoutineControl();	break;
		case REQUEST_OUT_OF_RANGE:						break;
		case SECURITY_ACCESS_DENIED:				SecurityAccessDenied();		break;
		case PROGRAMMING_FAILURE:							break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseRequestDownload(struct_request_dowload *resp_request_dowload)
{
	resp_request_dowload->id_data = 0;
	resp_request_dowload->id_format_addr_len = 0;
	resp_request_dowload->len_addr = 0;
	resp_request_dowload->len_size = 0;
	AddQueueNegativeSession(REQUEST_DOWNLOAD);
	switch(resp_request_dowload->nrc_codes){
		case SERVICE_NOT_SUPPORTED:	ServiceNotSupported();	break;
		case INVALID_MESSAGE_LENGTH_FORMAT:				break;
		case CONDITIONS_NOT_CORRECT:							break;
		case REQUEST_OUT_OF_RANGE:								break;
		case SECURITY_ACCESS_DENIED:	SecurityAccessDenied();	break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseTransferData(struct_transfer_data *resp_transfer_data)
{
	resp_transfer_data->count_blocks = 0;
	resp_transfer_data->len_data = 0;
	AddQueueNegativeSession(TRANSFER_DATA);
	switch(resp_transfer_data->nrc_codes){
		case SERVICE_NOT_SUPPORTED:	ServiceNotSupported();	break;
		case INVALID_MESSAGE_LENGTH_FORMAT:				break;
		case REQUEST_SEQUENCE_ERROR:							break;
		case PROGRAMMING_FAILURE:									break;
		case WRONG_BLOCK_SEQUANCE_COUNTER:				break;
		default: break;
	}
}

//***************************************************************//
void NegativeResponseRequestTransferExit(struct_request_transf_exit *resp_request_transf_exit)
{
	AddQueueNegativeSession(REQUEST_TRANSFER_EXIT);
	switch(resp_request_transf_exit->nrc_codes){
		case SERVICE_NOT_SUPPORTED:	ServiceNotSupported();	break;
		case INVALID_MESSAGE_LENGTH_FORMAT:				break;
		case REQUEST_SEQUENCE_ERROR:							break;
		case PROGRAMMING_FAILURE:									break;
		default: break;
	}
}

//***************************************************************//
void StartRequiredTimeDelay(uint8_t start)
{
	static uint16_t time_delay = 0;
	if(start) time_delay = 11000;
	if(time_delay){
		--time_delay;
		if(time_delay == 100){
			StandInQueueRequest(GetNegativeSession());
			SendRequestQueue();
		}
	}
}

//***************************************************************//
static void ServiceNotSupported(void)
{
	struct_session_control* req_session_control = GetReqSessionControl();
	if(req_session_control->subfunct == DEFAULT_SESSION){
		req_session_control->subfunct = EXTENDED_DIAGNOSTIC_SESSION;
	} else if(req_session_control->subfunct == EXTENDED_DIAGNOSTIC_SESSION){
		req_session_control->subfunct = PROGRAMMING_SESSION;
	} else {
		req_session_control->subfunct = EXTENDED_DIAGNOSTIC_SESSION;
	}
	AddQueueNegativeSession(DIAGNOSTIC_SESSION_CONTROL);
	StandInQueueRequest(GetNegativeSession());
}

//***************************************************************//
static void SecurityAccessDenied(void)
{
	struct_security_access* req_security_access = GetReqSecurityAccess();
	req_security_access->subfunct = SEED_3;
	AddQueueNegativeSession(SECURITY_ACCESS);
	StandInQueueRequest(GetNegativeSession());
}

//***************************************************************//
uint8_t GetNegativeSession(void)
{
	if(queue_neg_service.count){
		return queue_neg_service.queue[--queue_neg_service.count];
	} else {
		return 0;
	}
}

//***************************************************************//
void AddQueueNegativeSession(uint8_t session)
{
	queue_neg_service.queue[queue_neg_service.count++] = session;
}
