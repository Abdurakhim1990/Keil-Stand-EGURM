#include "main.h"

uint8_t can_unlock = 0;
uint16_t engine_speed = 0;
uint16_t tachograph_speed = 0;
uint8_t select_dm = 0;
uint8_t uds_send_enable = 0;


int8_t abc = -100;
uint8_t cba = 0;
uint8_t bca = 0;

//*******************************************************//
int main(void)
{
	systick_config();
	GeneralInitEgurm();
	
	GetClockFreq();
	
	cba = abc;
	bca = (uint8_t)abc;
	
	uint8_t temp = 0;
	while(1)
	{
		UdsRespTest();
		if(uds_send_enable){
			UdsReqTest();
		}
		if(select_dm){
			FuncSelectDM(select_dm);
			CanRecieveTest();
		}
		if(can_unlock){
			if(!(temp % 5)){
				CanTransmitTachographSpeed(tachograph_speed);
			} 
			CanTransmitEngineSpeed(engine_speed);
		}
		
		temp++;
		delay_1ms(10);
	}
}

//*******************************************************//
void GeneralInitEgurm(void)
{
	InitTimerTo10us();
	InitTimerTo100us();
	CanEgurmInit();
}
