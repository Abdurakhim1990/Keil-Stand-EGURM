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

//********************************************//
void Period200ms(void)
{
	//Period1MsForAkip();
}

//**-- Таймер 5 настроен на период 10 мкс --**//
void TIMER5_DAC_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_UP)){
		/* clear channel 0 interrupt bit */
		timer_interrupt_flag_clear(TIMER5, TIMER_INT_UP);
		
		Period10us();
	}
}

//**-- Таймер 6 настроен на период 100 мкс --**//
void TIMER6_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_UP)){
		/* clear channel 0 interrupt bit */
		timer_interrupt_flag_clear(TIMER6, TIMER_INT_UP);
		
		Period100us();
	}
}
