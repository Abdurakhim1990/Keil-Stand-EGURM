#include "can_uds_response_request_service.h"

uds_request_queue send_req_queue = {.queue_begin = 0, .queue_end = 0};

struct_session_control			req_session_control;
struct_ecu_reset						req_ecu_reset;
struct_security_access			req_security_access;
struct_communic_control			req_communic_control;
struct_tester_present				req_tester_present;
struct_control_dtc_setting	req_control_dtc_setting;
struct_read_write_data			req_read_data;
struct_read_write_data			req_write_data;
struct_clear_diag_inform		req_clear_diag_inform;
struct_read_dtc_inform			req_read_dtc_inform;
struct_full_routine_control	req_routine_control = {
	.count_rout = 0,
	.routine_control[0].id_routine = STEERING_WHEEL_CALIBRATION,
	.routine_control[0].len_memory = 0
};
struct_routine_control			*p_req_routine_control;
struct_request_dowload			req_request_dowload;
struct_transfer_data				req_transfer_data;
//struct_request_transf_exit	req_request_transf_exit;

void (*pUdsNextRequest)(void);


//***************************************************************//
//***************************************************************//
void SendDaignosticSessionControl(uds_subfunct_session_control subfunct)
{
	req_session_control.subfunct = subfunct;
	RequestDaignosticSessionControl(&req_session_control);
}

//***************************************************************//
void SendEcuReset(uds_subfunct_ecu_reset subfunct)
{
	req_ecu_reset.subfunct = subfunct;
	RequestEcuReset(&req_ecu_reset);
}

//***************************************************************//
void SendSecurityAccess(uds_subfunct_security_access subfunct)
{
	req_security_access.subfunct = subfunct;
	RequestSecurityAccess(&req_security_access);
}

//***************************************************************//
void SendCommunicationControl(uds_subfunct_communic_control	subfunct, uint8_t type_communic, uint16_t id_node)
{
	req_communic_control.subfunct = subfunct;
	req_communic_control.type_communic = type_communic;
	req_communic_control.id_node = id_node;
	RequestCommunicationControl(&req_communic_control);
}

//***************************************************************//
void SendTesterPresent(uds_subfunct_tester_present subfunct)
{
	req_tester_present.subfunct = subfunct;
	RequestTesterPresent(&req_tester_present);
}

//***************************************************************//
void SendControlDtcSetting(uds_subfunct_control_dtc_setting subfunct)
{
	req_control_dtc_setting.subfunct = subfunct;
	RequestControlDtcSetting(&req_control_dtc_setting);
}

//***************************************************************//
void SendReadDataByIdentifier(uds_did did)
{
	req_read_data.did = did;
	RequestReadDataByIdentifier(&req_read_data);
}

//***************************************************************//
void SendWriteDataByIdentifier(uds_did did, uint8_t* data, const uint8_t len)
{
	req_write_data.did = did;
	req_write_data.data = data;
	req_write_data.len = len;
	
	StandInQueueRequest(WRITE_DATA_BY_IDENTIFIER);
	SendRequestQueue();
}

//***************************************************************//
void SendClearDiagnosticInformation(uint8_t *data, uint8_t len)
{
	uds_dtc_format dtc[60];
	req_clear_diag_inform.len = len / 3;
	for(uint8_t i = 0, k = 0; i < len; ++i){
		switch(i % 3){
			case 0:
				dtc[k].spn = (uint16_t)(data[i] << 8);
				break;
			case 1:
				dtc[k].spn += data[i];
				break;
			case 2:
				dtc[k].fmi = data[i];
				++k;
				break;
		}
	}
	req_clear_diag_inform.dtc = dtc;
	RequestClearDiagnosticInformation(&req_clear_diag_inform);
}

//***************************************************************//
void SendReadDtcInformation(uint8_t dtc_status_mask)
{
	req_read_dtc_inform.subfunct = REP_DTC_BY_STATUS_MASK;
	req_read_dtc_inform.dtc_status_mask = dtc_status_mask;
	RequestReadDtcInformation(&req_read_dtc_inform);
}

//***************************************************************//
void SendRoutineControl(uds_subfunct_routine_control subfunct, roitine_identifier id_routine, uint8_t *data, uint8_t len)
{
	req_routine_control.routine_control[0].subfunct = subfunct;
	req_routine_control.routine_control[0].option_control_routine = data;
	req_routine_control.routine_control[1].subfunct = subfunct;
	req_routine_control.routine_control[1].id_routine = id_routine;
	req_routine_control.routine_control[1].option_control_routine = data;
	req_routine_control.routine_control[1].len_memory = len;
	if(subfunct == STOP_ROUTINE){
		req_routine_control.routine_control[1].id_routine = STEERING_WHEEL_CALIBRATION;
		p_req_routine_control = &req_routine_control.routine_control[1];
		RequestRoutineControl(p_req_routine_control);
	} else if(req_routine_control.count_rout == 0){
		p_req_routine_control = &req_routine_control.routine_control[0];
		RequestRoutineControl(p_req_routine_control);
	} else {
		p_req_routine_control = &req_routine_control.routine_control[1];
		RequestRoutineControl(p_req_routine_control);
	}
	
}

//***************************************************************//
void SendRequestDownload(uint8_t id_data, id_format_address_length	id_addr_len,
													uint8_t *data_addr, uint8_t len_addr_data, uint8_t *data_size, uint8_t len_size_data)
{
	req_request_dowload.id_data = id_data;
	req_request_dowload.id_format_addr_len = id_addr_len;
	req_request_dowload.data_address_memory = data_addr;
	req_request_dowload.len_addr = len_addr_data;
	req_request_dowload.data_size_memory = data_size;
	req_request_dowload.len_size = len_size_data;
	RequestRequestDownload(&req_request_dowload);
}

//***************************************************************//
void SendTransferData(uint8_t count_blocks, uint8_t *transfer_data, uint8_t len)
{
	req_transfer_data.count_blocks = count_blocks;
	req_transfer_data.transfer_data = transfer_data;
	req_transfer_data.len_data = len;
	RequestTransferData(&req_transfer_data);
}

//***************************************************************//
void SendRequestTransferExit()
{
	RequestRequestTransferExit();
}

//***************************************************************//
void ProcedyreWriteDataByIdentifier(void)
{
	SendWriteDataByIdentifier(req_write_data.did, req_write_data.data, req_write_data.len);
}

//***************************************************************//
void SendRequestQueue(void)
{
	if((send_req_queue.queue_begin != send_req_queue.queue_end) && (send_req_queue.queue_sid[send_req_queue.queue_begin] != 0)){
		switch(send_req_queue.queue_sid[send_req_queue.queue_begin]){
			case DIAGNOSTIC_SESSION_CONTROL:	RequestDaignosticSessionControl(&req_session_control);			break;
			case ECU_RESET:										RequestEcuReset(&req_ecu_reset);														break;
			case SECURITY_ACCESS:							RequestSecurityAccess(&req_security_access);								break;
			case COMMUNICATION_CONTROL:				RequestCommunicationControl(&req_communic_control);					break;
			case TESTER_PRESENT:							RequestTesterPresent(&req_tester_present);									break;
			case CONTROL_DTC_SETTING:					RequestControlDtcSetting(&req_control_dtc_setting);					break;
			case READ_DATA_BY_IDENTIFIER:			RequestReadDataByIdentifier(&req_read_data);								break;
			case WRITE_DATA_BY_IDENTIFIER:		RequestWriteDataByIdentifier(&req_write_data);							break;
			case CLEAR_DIAGN_INFO:						RequestClearDiagnosticInformation(&req_clear_diag_inform);	break;
			case READ_DTC_INFO:								RequestReadDtcInformation(&req_read_dtc_inform);						break;
			case ROUTINE_CONTROL:							RequestRoutineControl(p_req_routine_control);								break;
			case REQUEST_DOWNLOAD:						RequestRequestDownload(&req_request_dowload);								break;
			case TRANSFER_DATA:								RequestTransferData(&req_transfer_data);										break;
			case REQUEST_TRANSFER_EXIT:				RequestRequestTransferExit();																break;
			case P_UDS_NEXT_REQUEST:					pUdsNextRequest();																					break;
		}
		send_req_queue.queue_sid[send_req_queue.queue_begin++] = 0;
		send_req_queue.queue_begin %= LEN_SID_QUEUE;
	}
}

//***************************************************************//
void StandInQueueRequest(uint8_t sid)
{
	if(sid){
		send_req_queue.queue_sid[send_req_queue.queue_end++] = sid;
		send_req_queue.queue_end %= LEN_SID_QUEUE;
	}
}

//***************************************************************//
struct_session_control* GetReqSessionControl(){return &req_session_control;}
//***************************************************************//
struct_ecu_reset* GetReqEcuReset(){return &req_ecu_reset;}
//***************************************************************//
struct_security_access* GetReqSecurityAccess(){return &req_security_access;}
//***************************************************************//
struct_communic_control* GetReqCommunicControl(){return &req_communic_control;}
//***************************************************************//
struct_tester_present* GetReqTesterPresent(){return &req_tester_present;}
//***************************************************************//
struct_control_dtc_setting* GetReqControlDtcSetting(){return &req_control_dtc_setting;}
//***************************************************************//
struct_read_write_data* GetReqReadData(){return &req_read_data;}
//***************************************************************//
struct_read_write_data* GetReqWriteData(){return &req_write_data;}
//***************************************************************//
struct_clear_diag_inform* GetReqClearDiagInform(){return &req_clear_diag_inform;}
//***************************************************************//
struct_read_dtc_inform* GetReqReadDtcInfirm(){return &req_read_dtc_inform;}
//***************************************************************//
//struct_routine_control* GetReqRoutineControl(){return req_routine_control;}
//***************************************************************//
struct_request_dowload* GetReqRequestDowload(){return &req_request_dowload;}
//***************************************************************//
struct_transfer_data* GetReqTransferData(){return &req_transfer_data;}
//***************************************************************//
//struct_request_transf_exit	req_request_transf_exit;

//***************************************************************//
void IncReqRoutineControl(void)
{
	req_routine_control.count_rout = 1;
	p_req_routine_control = &req_routine_control.routine_control[req_routine_control.count_rout];
}

//***************************************************************//
void DecrReqRoutineControl(void)
{
	req_routine_control.count_rout = 0;
	p_req_routine_control = &req_routine_control.routine_control[req_routine_control.count_rout];
}