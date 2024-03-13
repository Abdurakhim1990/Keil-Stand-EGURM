#include "can_uds_response_request_service.h"

UdsMessage UdsReqDataMess;
UdsMessage UdsReqOfficialMess;

//***************************************************************//
UdsMessage* GetUdsRequest(void)
{
	return &UdsReqDataMess;
}

//***************************************************************//
void RequestDaignosticSessionControl(struct_session_control *session_control)
{
	UdsRequestResetFrameUds(&UdsReqOfficialMess);
	UdsReqOfficialMess.sid  = DIAGNOSTIC_SESSION_CONTROL;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = UdsReqOfficialMess.sid;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = session_control->subfunct;
	UdsReqOfficialMess.data_length = UdsReqOfficialMess.counter_data;
	UdsReqOfficialMess.counter_data = 0;
	UdsRequest(&UdsReqOfficialMess);
}

//***************************************************************//
void RequestEcuReset(struct_ecu_reset *ecu_reset)
{
	UdsRequestResetFrameUds(&UdsReqOfficialMess);
	UdsReqOfficialMess.sid  = ECU_RESET;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = UdsReqOfficialMess.sid;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = ecu_reset->subfunct;
	UdsReqOfficialMess.data_length = UdsReqOfficialMess.counter_data;
	UdsReqOfficialMess.counter_data = 0;
	UdsRequest(&UdsReqOfficialMess);
}

//***************************************************************//
void RequestSecurityAccess(struct_security_access *security_access)
{
	UdsRequestResetFrameUds(&UdsReqOfficialMess);
	UdsReqOfficialMess.sid  = SECURITY_ACCESS;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = UdsReqOfficialMess.sid;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = security_access->subfunct;
	UdsReqOfficialMess.data_length = UdsReqOfficialMess.counter_data;
	UdsReqOfficialMess.counter_data = 0;
	UdsRequest(&UdsReqOfficialMess);
}

//***************************************************************//
void RequestSecurityAccessKey(struct_security_access *security_access)
{
	UdsRequestResetFrameUds(&UdsReqOfficialMess);
	UdsReqOfficialMess.sid  = SECURITY_ACCESS;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = UdsReqOfficialMess.sid;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = security_access->subfunct;
	for(int8_t i = 3; i >= 0; --i){
		UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = (uint8_t)(security_access->key >> (8 * i));
	}
	UdsReqOfficialMess.data_length = UdsReqOfficialMess.counter_data;
	UdsReqOfficialMess.counter_data = 0;
	UdsRequest(&UdsReqOfficialMess);
}

//***************************************************************//
void RequestCommunicationControl(struct_communic_control *communic_control)
{
	UdsRequestResetFrameUds(&UdsReqOfficialMess);
	UdsReqOfficialMess.sid  = COMMUNICATION_CONTROL;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = UdsReqOfficialMess.sid;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = communic_control->subfunct;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = communic_control->type_communic;
	if(communic_control->subfunct == ENABLE_RX_AND_TX_WITH_EAI || communic_control->subfunct == DISABLE_RX_AND_TX_WITH_EAI){
		UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = (communic_control->id_node >> 8) & 0x00FF;
		UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = communic_control->id_node & 0x00FF;
	}
	UdsReqOfficialMess.data_length = UdsReqOfficialMess.counter_data;
	UdsReqOfficialMess.counter_data = 0;
	UdsRequest(&UdsReqOfficialMess);
}

//***************************************************************//
void RequestTesterPresent(struct_tester_present *tester_present)
{
	UdsRequestResetFrameUds(&UdsReqOfficialMess);
	UdsReqOfficialMess.sid  = TESTER_PRESENT;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = UdsReqOfficialMess.sid;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = tester_present->subfunct;
	UdsReqOfficialMess.data_length = UdsReqOfficialMess.counter_data;
	UdsReqOfficialMess.counter_data = 0;
	UdsRequest(&UdsReqOfficialMess);
}

//***************************************************************//
void RequestControlDtcSetting(struct_control_dtc_setting *control_dtc_setting)
{
	UdsRequestResetFrameUds(&UdsReqDataMess);
	UdsReqDataMess.sid  = CONTROL_DTC_SETTING;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = UdsReqDataMess.sid;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = control_dtc_setting->subfunct;
	UdsReqDataMess.data_length = UdsReqDataMess.counter_data;
	UdsReqDataMess.counter_data = 0;
	UdsRequest(&UdsReqDataMess);
}

//***************************************************************//
void RequestReadDataByIdentifier(struct_read_write_data *read_data)
{
	UdsRequestResetFrameUds(&UdsReqDataMess);
	UdsReqDataMess.sid  = READ_DATA_BY_IDENTIFIER;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = UdsReqDataMess.sid;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = (uint8_t)(read_data->did >> 8) & 0x00FF;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = read_data->did & 0x00FF;
	UdsReqDataMess.data_length = UdsReqDataMess.counter_data;
	UdsReqDataMess.counter_data = 0;
	UdsRequest(&UdsReqDataMess);
}

//***************************************************************//
void RequestWriteDataByIdentifier(struct_read_write_data *write_data)
{
	UdsRequestResetFrameUds(&UdsReqDataMess);
	UdsReqDataMess.sid  = WRITE_DATA_BY_IDENTIFIER;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = UdsReqDataMess.sid;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = (uint8_t)(write_data->did >> 8) & 0x00FF;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = write_data->did & 0x00FF;
	for(uint8_t i = 0; i < write_data->len; ++i){
		UdsReqDataMess.data[UdsReqDataMess.counter_data++] = write_data->data[i];
	}
	UdsReqDataMess.data_length = UdsReqDataMess.counter_data;
	UdsReqDataMess.counter_data = 0;
	UdsRequest(&UdsReqDataMess);
}

//***************************************************************//
void RequestClearDiagnosticInformation(struct_clear_diag_inform *clear_diag_inform)
{
	UdsRequestResetFrameUds(&UdsReqDataMess);
	UdsReqDataMess.sid  = CLEAR_DIAGN_INFO;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = UdsReqDataMess.sid;
	for(uint8_t i = 0; i < clear_diag_inform->len; ++i){
		UdsReqDataMess.data[UdsReqDataMess.counter_data++] = (clear_diag_inform->dtc[i].spn >> 8) & 0xFF;
		UdsReqDataMess.data[UdsReqDataMess.counter_data++] = clear_diag_inform->dtc[i].spn & 0xFF;
		UdsReqDataMess.data[UdsReqDataMess.counter_data++] = clear_diag_inform->dtc[i].fmi & 0xFF;
	}
	UdsReqDataMess.data_length = UdsReqDataMess.counter_data;
	UdsReqDataMess.counter_data = 0;
	UdsRequest(&UdsReqDataMess);
}

//***************************************************************//
void RequestReadDtcInformation(struct_read_dtc_inform *read_dtc_inform)
{
	UdsRequestResetFrameUds(&UdsReqDataMess);
	UdsReqDataMess.sid  = READ_DTC_INFO;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = UdsReqDataMess.sid;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = read_dtc_inform->subfunct;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = read_dtc_inform->dtc_status_mask;
	UdsReqDataMess.data_length = UdsReqDataMess.counter_data;
	UdsReqDataMess.counter_data = 0;
	UdsRequest(&UdsReqDataMess);
}

//***************************************************************//
void RequestRoutineControl(struct_routine_control *routine_control)
{
	UdsRequestResetFrameUds(&UdsReqDataMess);
	UdsReqDataMess.sid  = ROUTINE_CONTROL;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = UdsReqDataMess.sid;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = routine_control->subfunct;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = (routine_control->id_routine >> 8) & 0xFF;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = routine_control->id_routine & 0xFF;
	for(uint8_t i = 0; i < routine_control->len_memory; ++i){
		UdsReqDataMess.data[UdsReqDataMess.counter_data++] = routine_control->option_control_routine[i];
	}
	UdsReqDataMess.data_length = UdsReqDataMess.counter_data;
	UdsReqDataMess.counter_data = 0;
	UdsRequest(&UdsReqDataMess);
}

//***************************************************************//
void RequestRequestDownload(struct_request_dowload *request_dowload)
{
	UdsRequestResetFrameUds(&UdsReqDataMess);
	UdsReqDataMess.sid  = ROUTINE_CONTROL;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = UdsReqDataMess.sid;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = request_dowload->id_data;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = request_dowload->id_format_addr_len;
	for(uint8_t i = 0; i < request_dowload->len_addr; ++i){
		UdsReqDataMess.data[UdsReqDataMess.counter_data++] = request_dowload->data_address_memory[i];
	}
	for(uint8_t i = 0; i < request_dowload->len_size; ++i){
		UdsReqDataMess.data[UdsReqDataMess.counter_data++] = request_dowload->data_size_memory[i];
	}
	UdsReqDataMess.data_length = UdsReqDataMess.counter_data;
	UdsReqDataMess.counter_data = 0;
	UdsRequest(&UdsReqDataMess);
}

//***************************************************************//
void RequestTransferData(struct_transfer_data *transfer_data)
{
	UdsRequestResetFrameUds(&UdsReqDataMess);
	UdsReqDataMess.sid  = TRANSFER_DATA;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = UdsReqDataMess.sid;
	UdsReqDataMess.data[UdsReqDataMess.counter_data++] = transfer_data->count_blocks;
	for(uint8_t i = 0; i < transfer_data->len_data; ++i){
		UdsReqDataMess.data[UdsReqDataMess.counter_data++] = transfer_data->transfer_data[i];
	}
	UdsReqDataMess.data_length = UdsReqDataMess.counter_data;
	UdsReqDataMess.counter_data = 0;
	UdsRequest(&UdsReqDataMess);
}

//***************************************************************//
void RequestRequestTransferExit()
{
	UdsRequestResetFrameUds(&UdsReqOfficialMess);
	UdsReqOfficialMess.sid  = REQUEST_TRANSFER_EXIT;
	UdsReqOfficialMess.data[UdsReqOfficialMess.counter_data++] = UdsReqOfficialMess.sid;
	UdsReqOfficialMess.data_length = UdsReqOfficialMess.counter_data;
	UdsReqOfficialMess.counter_data = 0;
	UdsRequest(&UdsReqOfficialMess);
}
