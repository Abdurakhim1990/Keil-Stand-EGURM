#include "main.h"

char str[] = {"Assalam!"};
int16_t dpr_voltage[4] = {0, 0, 0, 0};

//*******************************************************//
int main(void)
  {
	systick_config();
	GeneralInitEgurm();
	
	GetClockFreq();
	
	uint8_t temp = 0;
	while(1)
	{
		Test();
		Led1_4Display(temp);
		dpr_voltage[0] = GetVoltageDM_1();
		dpr_voltage[1] = GetVoltageDM_2();
		dpr_voltage[2] = GetVoltageDM_3();
		dpr_voltage[3] = GetVoltageDM_4();
		//usbSendResponse();
		//DebugUart(str);
		ClearTestUSB();
		ClearTestEncoder();
		temp++;
		delay_1ms(1000);
	}
}

void GeneralInitEgurm(void)
{
#ifdef DEBUG
		DebugInit();
#endif
	LedInit();
	InitTimerTo10us();
	InitTimerTo100us();
	AdcEgurnInit();
	UsartAkip1148aInit();
	ServoInit();
	EncoderInInit();
	MUFTA_PWMEgurmInit();
	UsbdEgurmInit();
	VoltageReversInit();
	IngitionInit();
	UsartcanInit();
	SwitchingCurrentInit();
	ValeoTimersInit();
	
}
