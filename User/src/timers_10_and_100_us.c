#include "timers_10_and_100_us.h"

uint16_t count_10p = 0, count_100p = 0;

//********************************************//
void Period10us(void)
{
	AverageValueAdcMoment();
}

//********************************************//
void Period100us(void)
{
	AverageValueAdcVoltAmper();
}

//********************************************//
void Period1ms(void)
{
	Period1MsForAkip();
}
