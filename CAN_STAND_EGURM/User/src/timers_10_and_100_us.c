#include "timers_10_and_100_us.h"

uint16_t count_10p = 0, count_100p = 0;

//********************************************//
void Period10us(void)
{
	
}

//********************************************//
void Period100us(void)
{
	
}

//********************************************//
void Period1ms(void)
{
	UdsDelayMs();
	PeriodTesterPresent();
	StartRequiredTimeDelay(0);
	SendEngineSpeedToCan();
	SendCarSpeedToCan();
}

//********************************************//
void Period200ms(void)
{
	//Period1MsForAkip();
}

/*!
    \brief      this function handles TIMER5 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER5_DAC_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_UP)){
			/* clear channel 0 interrupt bit */
			timer_interrupt_flag_clear(TIMER5, TIMER_INT_UP);
			
			Period10us();
	}
}

/*!
    \brief      this function handles TIMER5 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER6_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_UP)){
			/* clear channel 0 interrupt bit */
			timer_interrupt_flag_clear(TIMER6, TIMER_INT_UP);
			
			Period100us();
	}
}
