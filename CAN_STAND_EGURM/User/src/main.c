#include "main.h"

uint8_t can_unlock = 0;
uint16_t engine_speed = 0;
uint16_t tachograph_speed = 0;
uint8_t select_dm = 0;
uint8_t uds_send_ensble = 0;

//*******************************************************//
int main(void)
{
	systick_config();
	GeneralInitEgurm();
	
	GetClockFreq();
	
	uint8_t temp = 0;
	while(1)
	{
		if(uds_send_ensble){
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
		delay_1ms(1000);
	}
}

//*******************************************************//
void GeneralInitEgurm(void)
{
	InitTimerTo10us();
	InitTimerTo100us();
	CanEgurmInit();
}
