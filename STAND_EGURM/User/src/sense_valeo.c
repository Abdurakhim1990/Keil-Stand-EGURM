#include "sense_valeo.h"

typedef struct {
	uint16_t dutycycle;
	uint32_t ic1value;
	uint32_t ic2value;
	
	uint32_t upvalue;
} valeo_struct;

valeo_struct valeo_param[4];


//**-- Получить напряжение датчика момента ДПР 1 --**//
//**************************************************************//
int16_t GetVoltageDM_1(void)
{
	return valeo_param[0].dutycycle;
}

//**-- Получить напряжение датчика момента ДПР 2 --**//
//**************************************************************//
int16_t GetVoltageDM_2(void)
{
	return valeo_param[1].dutycycle;
}

//**-- Получить напряжение датчика момента ДПР 3 --**//
//**************************************************************//
int16_t GetVoltageDM_3(void)
{
	return valeo_param[2].dutycycle;
}

//**-- Получить напряжение датчика момента ДПР 4 --**//
//**************************************************************//
int16_t GetVoltageDM_4(void)
{
	return valeo_param[3].dutycycle;
}

//==============================================================================
void PwmCalculate(uint32_t timer, valeo_struct* valeo)
{
	valeo->ic1value = timer_channel_capture_value_register_read(timer, TIMER_CH_0);

	if(0 != valeo->ic1value){
		valeo->ic2value = timer_channel_capture_value_register_read(timer, TIMER_CH_1);
		valeo->dutycycle = (valeo->ic2value * 1000) / valeo->ic1value;
	}
}

//==============================================================================
void VALEO_IN_PWM_DM1_TIMER_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(VALEO_IN_PWM_DM1_TIMER, TIMER_INT_FLAG_CH0)){
		timer_interrupt_flag_clear(VALEO_IN_PWM_DM1_TIMER, TIMER_INT_FLAG_CH0);
		PwmCalculate(VALEO_IN_PWM_DM1_TIMER, &valeo_param[0]);
	}
	
	if(RESET != timer_interrupt_flag_get(VALEO_IN_PWM_DM1_TIMER, TIMER_INT_FLAG_UP)){
		timer_interrupt_flag_clear(VALEO_IN_PWM_DM1_TIMER, TIMER_INT_FLAG_UP);
		valeo_param[0].upvalue = timer_counter_read(VALEO_IN_PWM_DM1_TIMER);
	}
}

void VALEO_IN_PWM_DM2_TIMER_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(VALEO_IN_PWM_DM2_TIMER, TIMER_INT_FLAG_CH0)){
		timer_interrupt_flag_clear(VALEO_IN_PWM_DM2_TIMER, TIMER_INT_FLAG_CH0);
		PwmCalculate(VALEO_IN_PWM_DM2_TIMER, &valeo_param[1]);
	}
	
	if(SET == timer_interrupt_flag_get(VALEO_IN_PWM_DM2_TIMER, TIMER_INT_FLAG_UP)){
		timer_interrupt_flag_clear(VALEO_IN_PWM_DM2_TIMER, TIMER_INT_FLAG_UP);
		valeo_param[1].upvalue = timer_counter_read(VALEO_IN_PWM_DM2_TIMER);
	}
}

void VALEO_IN_PWM_DM3_TIMER_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(VALEO_IN_PWM_DM3_TIMER, TIMER_INT_FLAG_CH0)){
		timer_interrupt_flag_clear(VALEO_IN_PWM_DM3_TIMER, TIMER_INT_FLAG_CH0);
		PwmCalculate(VALEO_IN_PWM_DM3_TIMER, &valeo_param[2]);
	}
	
	if(SET == timer_interrupt_flag_get(VALEO_IN_PWM_DM3_TIMER, TIMER_INT_FLAG_UP)){
		timer_interrupt_flag_clear(VALEO_IN_PWM_DM3_TIMER, TIMER_INT_FLAG_UP);
		valeo_param[2].upvalue = timer_counter_read(VALEO_IN_PWM_DM3_TIMER);
	}
}

void VALEO_IN_PWM_DM4_TIMER_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(VALEO_IN_PWM_DM4_TIMER, TIMER_INT_FLAG_CH0)){
		timer_interrupt_flag_clear(VALEO_IN_PWM_DM4_TIMER, TIMER_INT_FLAG_CH0);
		PwmCalculate(VALEO_IN_PWM_DM4_TIMER, &valeo_param[3]);
	}
	
	if(SET == timer_interrupt_flag_get(VALEO_IN_PWM_DM4_TIMER, TIMER_INT_FLAG_UP)){
		timer_interrupt_flag_clear(VALEO_IN_PWM_DM4_TIMER, TIMER_INT_FLAG_UP);
		valeo_param[3].upvalue = timer_counter_read(VALEO_IN_PWM_DM4_TIMER);
	}
}
