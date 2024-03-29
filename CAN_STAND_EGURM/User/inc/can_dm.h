#ifndef CAN_DM_H
#define CAN_DM_H

#include "main.h"

#include "can_egurm.h"

typedef struct{
	uint32_t spn;
	uint8_t fmi;
	uint8_t occur_count;
	uint8_t spn_method;
} DTCsFormat;

typedef struct{
	uint8_t seq_number;
	
	uint8_t protect_lamp_status;
	uint8_t amber_warn_lamp_status;
	uint8_t red_stop_lamp_status;
	uint8_t malfunc_indic_lamp_status;
	
	uint8_t flash_protect_lamp;
	uint8_t flash_amber_warn_lamp;
	uint8_t flash_red_stop_lamp;
	uint8_t flash_malfunc_indic_lamp;
	uint8_t flash_mil;
	
	uint8_t control_byte;
	uint16_t tot_numb_bytes;
	uint8_t tot_numb_packets;
	uint32_t pgn_pack_mess;
	
	
	uint16_t count_numb_bytes;		// Счетчик принятых байт данных
	uint8_t num_dtc;							// Порядковый номер DTC ошибки
	uint8_t byte_num_dtc;					// Счетчик байт DTC информации
	DTCsFormat DTCsActive[10];
} DiagnosticMess;


void DignosticDmPack(DiagnosticMess* Dm, uint8_t* recieve_data);
void DignosticDmBam(DiagnosticMess* Dm, uint8_t* recieve_data);
void DignosticDmSingle(DiagnosticMess* Dm, uint8_t* recieve_data);
void CleanDiagMessActiveDtc(DiagnosticMess* Dm);
void DtcMessages(DiagnosticMess* Dm, uint8_t data);
uint8_t DmDtcErrors(uint32_t spn, uint8_t fmi);

/////////////////////////////
void ExecutionFunc(void (*Func)(DiagnosticMess*, uint8_t*), uint8_t* buff);

#endif
