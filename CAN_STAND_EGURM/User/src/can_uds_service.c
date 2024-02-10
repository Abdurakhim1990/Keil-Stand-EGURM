#include "can_uds_service.h"

UdsMessage UdsReqMess;

//***************************************************************//
UdsMessage* GetUdsRequest(void)
{
	return &UdsReqMess;
}

//***************************************************************//
void UdsRequestResetFrameUds(UdsMessage* UdsReq)
{
	UdsReq->counter_data = 0;
	UdsReq->serial_number = 0;
	UdsReq->counter_block_size = 0;
	UdsReq->counter_min_separ = 0;
}

//***************************************************************//
void WriteDataToStructure(const uint8_t* data, const uint8_t len, const uds_subfunct_did did)
{
	UdsRequestResetFrameUds(&UdsReqMess);
	UdsReqMess.sid  = WRITE_DATA_BY_IDENTIFIER;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.sid;
	++UdsReqMess.counter_data;
	UdsReqMess.SubFunc.did = did;
	UdsReqMess.data[UdsReqMess.counter_data] = (uint8_t)(did >> 8);
	++UdsReqMess.counter_data;
	UdsReqMess.data[UdsReqMess.counter_data] = (uint8_t)(did);
	++UdsReqMess.counter_data;
	for(uint8_t i = 0; i < len; ++i, ++UdsReqMess.counter_data){
		UdsReqMess.data[UdsReqMess.counter_data] = data[i];
	}
	UdsReqMess.data_length = UdsReqMess.counter_data;
	UdsReqMess.counter_data = 0;
	UdsRequest(&UdsReqMess);
}

//***************************************************************//
void ReadDataByIdentifier(const uint8_t* data, const uint8_t len, const uds_subfunct_did did)
{
	UdsRequestResetFrameUds(&UdsReqMess);
	UdsReqMess.sid  = READ_DATA_BY_IDENTIFIER;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.sid;
	++UdsReqMess.counter_data;
	UdsReqMess.SubFunc.did = did;
	UdsReqMess.data[UdsReqMess.counter_data] = (uint8_t)(did >> 8);
	++UdsReqMess.counter_data;
	UdsReqMess.data[UdsReqMess.counter_data] = (uint8_t)(did);
	++UdsReqMess.counter_data;
	for(uint8_t i = 0; i < len; ++i, ++UdsReqMess.counter_data){
		UdsReqMess.data[UdsReqMess.counter_data] = data[i];
	}
	UdsReqMess.data_length = UdsReqMess.counter_data;
	UdsReqMess.counter_data = 0;
	UdsRequest(&UdsReqMess);
}

//***************************************************************//
void DaignosticSessionControl(uds_subfunct_session_control session_control)
{
	UdsRequestResetFrameUds(&UdsReqMess);
	UdsReqMess.sid  = DIAGNOSTIC_SESSION_CONTROL;
	UdsReqMess.SubFunc.session_control = session_control;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.sid;
	++UdsReqMess.counter_data;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.SubFunc.session_control;
	++UdsReqMess.counter_data;
	UdsReqMess.data_length = UdsReqMess.counter_data;
	UdsReqMess.counter_data = 0;
	UdsRequest(&UdsReqMess);
}

//***************************************************************//
void EquReset(uds_subfunct_ecu_reset ecu_reset)
{
	UdsRequestResetFrameUds(&UdsReqMess);
	UdsReqMess.sid  = ECU_RESET;
	UdsReqMess.SubFunc.ecu_reset = ecu_reset;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.sid;
	++UdsReqMess.counter_data;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.SubFunc.ecu_reset;
	++UdsReqMess.counter_data;
	UdsReqMess.data_length = UdsReqMess.counter_data;
	UdsReqMess.counter_data = 0;
	UdsRequest(&UdsReqMess);
}

//***************************************************************//
void SecurityAccess(uds_subfunct_security_access security_access)
{
	UdsRequestResetFrameUds(&UdsReqMess);
	UdsReqMess.sid  = SECURITY_ACCESS;
	UdsReqMess.SubFunc.security_access = security_access;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.sid;
	++UdsReqMess.counter_data;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.SubFunc.security_access;
	++UdsReqMess.counter_data;
	UdsReqMess.data_length = UdsReqMess.counter_data;
	UdsReqMess.counter_data = 0;
	UdsRequest(&UdsReqMess);
}

//***************************************************************//
void SecurityAccessKey(uds_subfunct_security_access security_access, uint32_t key)
{
	UdsRequestResetFrameUds(&UdsReqMess);
	UdsReqMess.sid  = SECURITY_ACCESS;
	UdsReqMess.SubFunc.security_access = security_access;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.sid;
	++UdsReqMess.counter_data;
	UdsReqMess.data[UdsReqMess.counter_data] = UdsReqMess.SubFunc.security_access;
	++UdsReqMess.counter_data;
	for(int8_t i = 3; i >= 0; --i, ++UdsReqMess.counter_data){
		UdsReqMess.data[UdsReqMess.counter_data] = (uint8_t)(key >> (8 * i));
	}
	UdsReqMess.data_length = UdsReqMess.counter_data;
	UdsReqMess.counter_data = 0;
	UdsRequest(&UdsReqMess);
}

//***************************************************************//
void TesterPresent(void)
{
	UdsRequestResetFrameUds(&UdsReqMess);
	
}
