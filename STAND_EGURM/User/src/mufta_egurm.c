#include "mufta_egurm.h"

int16_t set_moment_out = 0;
uint16_t set_pwm_mufta = 0;
int8_t step_pwm = 1;

void SetPwmMufta(void)
{
	if(set_moment_out > GetMomentOut()){
		set_pwm_mufta += step_pwm;
	} else if(set_moment_out < GetMomentOut()){
		step_pwm = 1;
		set_pwm_mufta -= step_pwm;
	}
	
	timer_channel_output_pulse_value_config(MUFTA_TIMER, MUFTA_PWM_TIMER_CH, set_pwm_mufta);
}
//******************************************************//
void SetMomentOut(int16_t val_moment)
{
	set_moment_out = val_moment;
	
	if(set_moment_out == 0){
		set_pwm_mufta = 0;
		timer_interrupt_disable(MUFTA_TIMER, TIMER_INT_UP);
		timer_channel_output_pulse_value_config(MUFTA_TIMER, MUFTA_PWM_TIMER_CH, set_pwm_mufta);
	} else if(set_moment_out == 1000){
		set_pwm_mufta = MUFTA_PWM_PERIOD;
		timer_interrupt_disable(MUFTA_TIMER, TIMER_INT_UP);
		timer_channel_output_pulse_value_config(MUFTA_TIMER, MUFTA_PWM_TIMER_CH, set_pwm_mufta);
	} else{
		step_pwm = 10;
		SetPwmMufta();
		timer_interrupt_enable(MUFTA_TIMER, TIMER_INT_UP);
	}
}

//******************************************************//
/*!
    \brief      this function handles MUFTA_TIMER interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MUFTA_TIMER_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(MUFTA_TIMER, TIMER_INT_UP)){
		/* clear channel 0 interrupt bit */
		timer_interrupt_flag_clear(MUFTA_TIMER, TIMER_INT_UP);
		
		SetPwmMufta();
	}
}

