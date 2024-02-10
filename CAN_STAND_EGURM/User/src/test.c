#include "test.h"

struct clock_freq_get{
	uint32_t CK_SYS;
	uint32_t CK_AHB;
	uint32_t CK_APB1;
	uint32_t CK_APB2;
	uint32_t CK_USART;
} clock_freq;

/*!
    \brief      get the system clock, bus and peripheral clock frequency
    \param[in]  clock: the clock frequency which to get
                only one parameter can be selected which is shown as below:
      \arg        CK_SYS: system clock frequency
      \arg        CK_AHB: AHB clock frequency
      \arg        CK_APB1: APB1 clock frequency
      \arg        CK_APB2: APB2 clock frequency
      \arg        CK_USART: USART5 clock frequency
    \param[out] none
    \retval     clock frequency of system, AHB, APB1, APB2
*/	
void GetClockFreq(void)
{
	clock_freq.CK_SYS = rcu_clock_freq_get(CK_SYS);
	clock_freq.CK_AHB = rcu_clock_freq_get(CK_AHB);
	clock_freq.CK_APB1 = rcu_clock_freq_get(CK_APB1);
	clock_freq.CK_APB2 = rcu_clock_freq_get(CK_APB2);
	clock_freq.CK_USART = rcu_clock_freq_get(CK_USART);
}

uint8_t speed_can = 0;

int16_t n_m_in = 0, n_m_out = 0, voltage = 0, current = 0, temper = 0;
int16_t setting_voltage = 0;
int16_t setting_current = 0;

uint8_t flag_set_volt = 0;
uint8_t flag_set_curr = 0;

uint16_t value_encoder = 0;
int16_t angle_encoder_in = 0, angle_encoder_out = 0;

uint16_t pwm_mufta = 200;

uint8_t revers_volt = 0;

uint8_t usbd_can_mode = 0;

uint8_t can_recieve_test[8];
uint8_t count_can_test = 0;


void CanRecieveTest(void)
{
	if(count_can_test == 1){		// Single red = 0, amber = 0, spn = 0, fmi = 0, count = 0, spn method = 0
		for(uint8_t i = 0; i < 8; ++i){
			can_recieve_test[i] = 0;
		}
		ExecutionFunc(DignosticDmSingle, can_recieve_test);
		
		//******************************************************************//
	} else if(count_can_test == 2){		// Single red = 0, amber = 1, spn = 0x03FACB, fmi = 0x04, count = 0x1B, spn method = 1
		can_recieve_test[0] = 0x04;
		can_recieve_test[1] = 0xff;
		can_recieve_test[2] = 0xCB;
		can_recieve_test[3] = 0xFA;
		can_recieve_test[4] = 0x64;
		can_recieve_test[5] = 0x9B;
		can_recieve_test[6] = 0xff;
		can_recieve_test[7] = 0xff;
		ExecutionFunc(DignosticDmSingle, can_recieve_test);
		
		//******************************************************************//
	} else if(count_can_test == 3){		// Single red = 1, amber = 0, spn = 0x063524, fmi = 0x0D, count = 0x0A, spn method = 0
		can_recieve_test[0] = 0x10;
		can_recieve_test[1] = 0xff;
		can_recieve_test[2] = 0x24;
		can_recieve_test[3] = 0x35;
		can_recieve_test[4] = 0xCD;
		can_recieve_test[5] = 0x0A;
		can_recieve_test[6] = 0xff;
		can_recieve_test[7] = 0xff;
		ExecutionFunc(DignosticDmSingle, can_recieve_test);
		
		//******************************************************************//
	} else if(count_can_test == 4){
		can_recieve_test[0] = 0x20;		// BAM control = 32(0x20), total bytes = 10(0x0E), packets = 0x02, PGN = 0x00FECB
		can_recieve_test[1] = 0x0E;
		can_recieve_test[2] = 0x00;
		can_recieve_test[3] = 0x02;
		can_recieve_test[4] = 0xFF;
		can_recieve_test[5] = 0xCB;
		can_recieve_test[6] = 0xFE;
		can_recieve_test[7] = 0x00;
		ExecutionFunc(DignosticDmBam, can_recieve_test);
		
		can_recieve_test[0] = 0x01;		// Pack 1 red = 1, amber = 0
		can_recieve_test[1] = 0x10;
		can_recieve_test[2] = 0x00;
		
		can_recieve_test[3] = 0x67;		// Pack 1 spn = 0x05EF67, fmi = 0x1C, count = 0x1F, spn method = 1
		can_recieve_test[4] = 0xEF;
		can_recieve_test[5] = 0xBC;
		can_recieve_test[6] = 0x9F;
		
		can_recieve_test[7] = 0x54;
		ExecutionFunc(DignosticDmPack, can_recieve_test);
		can_recieve_test[0] = 0x02;		// Pack 2 spn = 0x050B54, fmi = 0x14, count = 0x67, spn method = 0
		can_recieve_test[1] = 0x0B;
		can_recieve_test[2] = 0xB4;
		can_recieve_test[3] = 0x67;
		
		can_recieve_test[4] = 0xFF;		// Pack 2 spn = 0x07FFFF, fmi = 0x1F, count = 0x7F, spn method = 1
		can_recieve_test[5] = 0xFF;
		can_recieve_test[6] = 0xFF;
		can_recieve_test[7] = 0xFF;
		ExecutionFunc(DignosticDmPack, can_recieve_test);
		
		//******************************************************************//
	} else if(count_can_test == 5){
		can_recieve_test[0] = 0x20;		// BAM control = 32(0x20), total bytes = 10(0x0E), packets = 0x02, PGN = 0x00FECB
		can_recieve_test[1] = 0x0E;
		can_recieve_test[2] = 0x00;
		can_recieve_test[3] = 0x02;
		can_recieve_test[4] = 0xFF;
		can_recieve_test[5] = 0xCB;
		can_recieve_test[6] = 0xFE;
		can_recieve_test[7] = 0x00;
		ExecutionFunc(DignosticDmBam, can_recieve_test);
		
		can_recieve_test[0] = 0x01;		// Pack 1 red = 0, amber = 1
		can_recieve_test[1] = 0x04;
		can_recieve_test[2] = 0x00;
		
		can_recieve_test[3] = 0xFF;		// Pack 1 spn = 0x07FFFF, fmi = 0x1F, count = 0x7F, spn method = 1
		can_recieve_test[4] = 0xFF;
		can_recieve_test[5] = 0xFF;
		can_recieve_test[6] = 0xFF;
		
		can_recieve_test[7] = 0x55;
		ExecutionFunc(DignosticDmPack, can_recieve_test);
		can_recieve_test[0] = 0x02;		// Pack 2 spn = 0x051255, fmi = 0x07, count = 0x24, spn method = 1
		can_recieve_test[1] = 0x12;
		can_recieve_test[2] = 0x87;
		can_recieve_test[3] = 0xA4;
		 
		can_recieve_test[4] = 0xC6;		// Pack 2 spn = 0x078DC6, fmi = 0x1A, count = 0x69, spn method = 0
		can_recieve_test[5] = 0x8D;
		can_recieve_test[6] = 0xFA;
		can_recieve_test[7] = 0x69;
		ExecutionFunc(DignosticDmPack, can_recieve_test);
		
		//******************************************************************//
	} else if(count_can_test == 6){
		can_recieve_test[0] = 0x20;		// BAM control = 32(0x20), total bytes = 10(0x0E), packets = 0x02, PGN = 0x00FECB
		can_recieve_test[1] = 0x1A;
		can_recieve_test[2] = 0x00;
		can_recieve_test[3] = 0x02;
		can_recieve_test[4] = 0xFF;
		can_recieve_test[5] = 0xCB;
		can_recieve_test[6] = 0xFE;
		can_recieve_test[7] = 0x00;
		ExecutionFunc(DignosticDmBam, can_recieve_test);
		
		can_recieve_test[0] = 0x01;		// Pack 1 red = 0, amber = 1
		can_recieve_test[1] = 0x04;
		can_recieve_test[2] = 0x00;
		
		can_recieve_test[3] = 0x67;		// Pack 1 spn = 0x05EF67, fmi = 0x1C, count = 0x1F, spn method = 1
		can_recieve_test[4] = 0xEF;
		can_recieve_test[5] = 0xBC;
		can_recieve_test[6] = 0x9F;
		
		can_recieve_test[7] = 0x55;
		ExecutionFunc(DignosticDmPack, can_recieve_test);
		can_recieve_test[0] = 0x02;		// Pack 2 spn = 0x041255, fmi = 0x07, count = 0x24, spn method = 1
		can_recieve_test[1] = 0x12;
		can_recieve_test[2] = 0x87;
		can_recieve_test[3] = 0xA4;
		 
		can_recieve_test[4] = 0xC6;		// Pack 2 spn = 0x078DC6, fmi = 0x1A, count = 0x69, spn method = 0
		can_recieve_test[5] = 0x8D;
		can_recieve_test[6] = 0xFA;
		can_recieve_test[7] = 0x69;
		ExecutionFunc(DignosticDmPack, can_recieve_test);
		
		can_recieve_test[0] = 0x03;		// Pack 3 spn = 0x058712, fmi = 0x04, count = 0x46, spn method = 1
		can_recieve_test[1] = 0x12;
		can_recieve_test[2] = 0x87;
		can_recieve_test[3] = 0xA4;
		can_recieve_test[4] = 0xC6;
		
		can_recieve_test[5] = 0x8D;		// Pack 3 spn = 0x03FA8D, fmi = 0x09
		can_recieve_test[6] = 0xFA;
		can_recieve_test[7] = 0x69;
		ExecutionFunc(DignosticDmPack, can_recieve_test);
		
		can_recieve_test[0] = 0x04;		// Pack 4 count = 0x12, spn method = 0
		can_recieve_test[1] = 0x12;
		
		can_recieve_test[2] = 0x87;		// Pack 4 spn = 0x06A487, fmi = 0x06, count = 0x0D, spn method = 1
		can_recieve_test[3] = 0xA4;
		can_recieve_test[4] = 0xC6;
		can_recieve_test[5] = 0x8D;
		
		can_recieve_test[6] = 0xFF;
		can_recieve_test[7] = 0xFF;
		ExecutionFunc(DignosticDmPack, can_recieve_test);
		
	} else if(count_can_test == 7){
		
	} else if(count_can_test == 8){
		
	}
	//count_can_test = 0;
}
const char uds_serial_number_test[] = "EGURM N17635 AO \"Koncern KEMZ\" KB-E 2024 Y";
const char uds_version_po_test[] = "Version 1.0.1 2024";
char key_test[4];
//uint8_t uds_test_len = 20;
uint8_t select_uds_did = 0;
uds_subfunct_did did_test = SYSTEM_SUPPLIER_ECU_SOFTWARE_VERSION_NUMBER;
void UdsReqTest(void)
{
	if(select_uds_did == 1){
		did_test = SYSTEM_SUPPLIER_ECU_SOFTWARE_VERSION_NUMBER;
		select_uds_did = 0;
		WriteDataToStructure((uint8_t*)uds_serial_number_test, sizeof(uds_serial_number_test), did_test);
	} else if(select_uds_did == 2){
		did_test = ECU_SERIAL_NUMBER;
		select_uds_did = 0;
//		WriteDataToStructure((uint8_t*)uds_serial_number_test, 2, did_test);
		WriteDataToStructure((uint8_t*)uds_serial_number_test, sizeof(uds_serial_number_test), did_test);
	} else if(select_uds_did == 3){
		did_test = ECU_SERIAL_NUMBER;
		select_uds_did = 0;
		ReadDataByIdentifier((uint8_t*)uds_serial_number_test, 0, did_test);
	} else if(select_uds_did == 4){
		did_test = SYSTEM_SUPPLIER_ECU_SOFTWARE_VERSION_NUMBER;
		select_uds_did = 0;
		ReadDataByIdentifier((uint8_t*)uds_serial_number_test, 0, did_test);
	} else if(select_uds_did == 5){
		//did_test = EXTENDED_DIAGNOSTIC_SESSION;
		select_uds_did = 0;
		//UdsRequest((uint8_t*)uds_version_po_test, 0, DIAGNOSTIC_SESSION_CONTROL, did_test);
	} else if(select_uds_did == 6){
		uds_subfunct_security_access security_access = SEED_3;
		select_uds_did = 0;
		SecurityAccess(security_access);
	} else if(select_uds_did == 7){
		//did_test = SEED_3 + 1;
		select_uds_did = 0;
		//UdsRequest((uint8_t*)key_test, sizeof(key_test), SECURITY_ACCESS, did_test);
	}
}

UdsMessage UdsMessageFlowResp;
uint8_t uds_resp_test[8];
uint8_t select_uds_resp = 0;

void UdsRespTest(void)
{
	if(select_uds_resp == 1){
		uds_resp_test[0] = 0x30;
		uds_resp_test[1] = 0x00;
		uds_resp_test[2] = 0x00;
		UdsResponse(&UdsMessageFlowResp, uds_resp_test);
		select_uds_resp = 0;
	} else if(select_uds_resp == 2){
		uds_resp_test[0] = 0x30;
		uds_resp_test[1] = 0x02;
		uds_resp_test[2] = 0x32;
		UdsResponse(&UdsMessageFlowResp, uds_resp_test);
		select_uds_resp = 0;
	} else if(select_uds_resp == 3){
		uds_resp_test[0] = 0x30;
		uds_resp_test[1] = 0x05;
		uds_resp_test[2] = 0x64;
		UdsResponse(&UdsMessageFlowResp, uds_resp_test);
		select_uds_resp = 0;
		
		
	} else if(select_uds_resp == 4){
		volatile static uint8_t count_frame = 0;
		static uint8_t count_byte = 0;
		static uint8_t length;
		
		if(count_frame == 0){
			length = sizeof(uds_serial_number_test);
			uds_resp_test[0] = FIRST_FRAME << SHIFT_TYPE_FRAME;
			uds_resp_test[1] = length + 3;
			uds_resp_test[2] = READ_DATA_BY_IDENTIFIER + DIF_SID_RESP;
			uds_resp_test[3] = (uint8_t)ECU_SERIAL_NUMBER >> 8;
			uds_resp_test[4] += (uint8_t)ECU_SERIAL_NUMBER;
			for(uint8_t i = 5; i < 8; ++i){
				if(count_byte < length){
					uds_resp_test[i] = uds_serial_number_test[count_byte];
				} else {
					uds_resp_test[i] = 0;
				}
				++count_byte;
			}
			++count_frame;
		} else {
			uds_resp_test[0] = (CONSECUTIVE_FRAME << SHIFT_TYPE_FRAME) + (count_frame & MASK_SERIAL_NUMBER);
			for(uint8_t i = 1; i < 8; ++i){
				if(count_byte < length){
					uds_resp_test[i] = uds_serial_number_test[count_byte];
				} else {
					uds_resp_test[i] = 0;
				}
				++count_byte;
			}
			++count_frame;
			__ASM("NOP");
		}
		UdsResponse(&UdsMessageFlowResp, uds_resp_test);
		if(count_byte >= length){
			count_frame = 0;
			select_uds_resp = 0;
			count_byte = 0;
		}
	} else if(select_uds_resp == 5){
		uds_resp_test[0] = 0x06;
		uds_resp_test[1] = 0x67;
		uds_resp_test[2] = 0x05;
		uds_resp_test[3] = 0x60;
		uds_resp_test[4] = 0x49;
		uds_resp_test[5] = 0xe0;
		uds_resp_test[6] = 0x43;
		uds_resp_test[7] = 0x00;
		UdsResponse(&UdsMessageFlowResp, uds_resp_test);
		select_uds_resp = 0;
		
		
	} else if(select_uds_resp == 6){
		uds_resp_test[0] = 0x03;
		uds_resp_test[1] = 0x7f;
		uds_resp_test[2] = 0x2e;
		uds_resp_test[3] = 0x31;
		uds_resp_test[4] = 0x00;
		uds_resp_test[5] = 0x00;
		uds_resp_test[6] = 0x00;
		uds_resp_test[7] = 0x00;
		UdsResponse(&UdsMessageFlowResp, uds_resp_test);
		select_uds_resp = 0;
	} else if(select_uds_resp == 7){
		CanClearResetPreviouslyActiveDTCs();
		select_uds_resp = 0;
	} else if(select_uds_resp == 8){
		CanPreviouslyActiveDTCsCodes();
		select_uds_resp = 0;
	}
}
