#include "can_dm.h"


//*******************************************************************//
void DignosticDmSingle(DiagnosticMess* Dm, uint8_t* recieve_data)
{
	CleanDiagMessActiveDtc(Dm);
	Dm->seq_number = 0;
	Dm->num_dtc = 0;
	Dm->tot_numb_bytes = 6;
	Dm->tot_numb_packets = 1;
	
	Dm->num_dtc = 0;
	Dm->byte_num_dtc = 0;
	
	for(size_t i = 0; i < 8; ++i){
		
		if(i == 0){
			Dm->protect_lamp_status = recieve_data[i] & 0x03;
			Dm->amber_warn_lamp_status = (recieve_data[i] & 0x0C) >> 2;
			Dm->red_stop_lamp_status = (recieve_data[i] & 0x30) >> 4;
			Dm->malfunc_indic_lamp_status = (recieve_data[i] & 0xC0) >> 6;
		} else if(i == 1){
			Dm->flash_mil = recieve_data[i];
		} else if(i >= 2 && i <= 5){
			DtcMessages(Dm, recieve_data[i]);
		}
	}
	SendUartcanError(DmDtcErrors(Dm->DTCsActive[0].spn, Dm->DTCsActive[0].fmi));
}

//*******************************************************************//
void DignosticDmBam(DiagnosticMess* Dm, uint8_t* recieve_data)
{
	CleanDiagMessActiveDtc(Dm);
	Dm->control_byte = recieve_data[0];
	Dm->tot_numb_bytes = recieve_data[1] + (recieve_data[2] << 8);
	Dm->tot_numb_packets = recieve_data[3];
	Dm->pgn_pack_mess = recieve_data[5] + (recieve_data[6] << 8) + (recieve_data[7] << 16);
	
	Dm->num_dtc = 0;
	Dm->byte_num_dtc = 0;
}

//*******************************************************************//
void DignosticDmPack(DiagnosticMess* Dm, uint8_t* recieve_data)
{
	uint8_t count_numb_packets = recieve_data[0];
	
	Dm->seq_number = count_numb_packets;
	
	for(size_t i = 1; i < 8; ++i){
		if(count_numb_packets == 1){
			if(i == 1){
				Dm->protect_lamp_status = recieve_data[i] & 0x03;
				Dm->amber_warn_lamp_status = (recieve_data[i] & 0x0C) >> 2;
				Dm->red_stop_lamp_status = (recieve_data[i] & 0x30) >> 4;
				Dm->malfunc_indic_lamp_status = (recieve_data[i] & 0xC0) >> 6;
			} else if(i == 2){
				Dm->flash_protect_lamp = recieve_data[i] & 0x03;
				Dm->flash_amber_warn_lamp = (recieve_data[i] & 0x0C) >> 2;
				Dm->flash_red_stop_lamp = (recieve_data[i] & 0x30) >> 4;
				Dm->flash_malfunc_indic_lamp = (recieve_data[i] & 0xC0) >> 6;
			} else if(i > 2){
				DtcMessages(Dm, recieve_data[i]);
			}
		} else if(i != 0){
			DtcMessages(Dm, recieve_data[i]);
		}
		++Dm->count_numb_bytes;
		if(Dm->count_numb_bytes == Dm->tot_numb_bytes){
			for(uint8_t i = 0; i < Dm->num_dtc; ++i){
				SendUartcanError((i << 8) +	DmDtcErrors(Dm->DTCsActive[i].spn, Dm->DTCsActive[i].fmi));
			}
			Dm->count_numb_bytes = 0;
			break;
		}
	}
}

//*******************************************************************//
void DtcMessages(DiagnosticMess* Dm, uint8_t data)
{
	if(Dm->byte_num_dtc == 0){
		Dm->DTCsActive[Dm->num_dtc].spn = data;
		
	} else if(Dm->byte_num_dtc == 1){
		Dm->DTCsActive[Dm->num_dtc].spn += (data << 8);
		
	} else if(Dm->byte_num_dtc == 2){
		Dm->DTCsActive[Dm->num_dtc].fmi = (data & 0x1F);
		Dm->DTCsActive[Dm->num_dtc].spn += ((data & 0xE0) << 11);
		
	} else if(Dm->byte_num_dtc == 3){
		Dm->DTCsActive[Dm->num_dtc].occur_count = (data & 0x7F);
		Dm->DTCsActive[Dm->num_dtc].spn_method = ((data & 0x80) >> 7);
	} else {
		__ASM("NOP");
	}
	++Dm->byte_num_dtc;
	Dm->num_dtc += Dm->byte_num_dtc/4;
	Dm->byte_num_dtc %= 4;
}

//*******************************************************************//
void CleanDiagMessActiveDtc(DiagnosticMess* Dm)
{
	for(uint8_t i = 0; i < 10; ++i){
		Dm->DTCsActive[i].spn = 0xFF;
		Dm->DTCsActive[i].fmi = 0xFF;
		Dm->DTCsActive[i].occur_count = 0xFF;
		Dm->DTCsActive[i].spn_method = 0xFF;
	}
}

//*******************************************************************//
uint8_t DmDtcErrors(uint32_t spn, uint8_t fmi)
{
	uint8_t error = 0;
	if(spn > 19000) spn -= 19000;
	
	if(spn == 1 && fmi == 2){
		error = ERROR_PARAMETR_CHECKSUM;
	} else if(spn == 1 && fmi == 12){
		error = ERROR_CONTROL_AND_COMMUN_CONTROLLER;
	} else if(spn == 2 && fmi == 2){
		error = ERROR_SELF_TEST_CONTROLLER;
	} else if(spn == 3 && fmi == 2){
		error = ERROR_POWER_ELECTRONICS;
	} else if(spn == 3 && fmi == 19){
		error = ERROR_HEAD;
	} else if(spn == 4 && fmi == 2){
		error = ERROR_INPUT_POWER_MONITORING;
	} else if(spn == 5 && fmi == 2){
		error = ERROR_SENSOR_TORQUE;
	} else if(spn == 3683 && fmi == 2){
		error = ERROR_STEERING_ANGLE_SENS;
	} else if(spn == 3683 && fmi == 13){
		error = ERROR_STEERING_CALIBRATION;
	} else if(spn == 6 && fmi == 2){
		error = ERROR_MOTOR_ROTOR_POSITION;
	} else if(spn == 7 && fmi == 6){
		error = ERROR_EXCEEDING_CURRENT_IN_MOTOR;
	} else if(spn == 7 && fmi == 5){
		error = ERROR_BROKEN_MOTOR_WINDING;
	} else if(spn == 7 && fmi == 20){
		error = ERROR_SHORT_CIRCUIT_WINDING;
	} else if(spn == 8 && fmi == 11){
		error = ERROR_GENERAL_FAIL;
	} else if(spn == 1624 && fmi == 9){
		error = ERROR_NO_VEHICLE_SPEED_DATA;
	} else if(spn == 190 && fmi == 9){
		error = ERROR_NO_ENGINE_SPEED_DATA;
	}
	__ASM("NOP");
	return error;
}
