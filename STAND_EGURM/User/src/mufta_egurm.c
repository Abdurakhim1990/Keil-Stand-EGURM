#include "mufta_egurm.h"

int16_t set_moment_out = 0;
int16_t set_pwm_mufta = 0;
int8_t step_pwm = 0;
int16_t out_moment = 0;
int16_t in_moment = 0;

void SetPwmMufta(void)
{
	in_moment = GetMomentIn();
	out_moment = GetMomentOut();
	set_pwm_mufta = set_moment_out;
	//if(set_moment_out == 0){
	//	set_pwm_mufta = 0;
	//} else if(set_moment_out > out_moment){
	//	set_pwm_mufta += step_pwm;
	//	set_pwm_mufta = set_pwm_mufta > MUFTA_PWM_PERIOD ? MUFTA_PWM_PERIOD : set_pwm_mufta;
	//} else if(set_moment_out < out_moment){
	//	set_pwm_mufta -= step_pwm;
	//	set_pwm_mufta = set_pwm_mufta < 0 ? 0 : set_pwm_mufta;
	//}
	timer_channel_output_pulse_value_config(MUFTA_PWM_TIMER, MUFTA_PWM_TIMER_CH, set_pwm_mufta);
}
//******************************************************//
void SetMomentOut(int16_t val_moment)
{
	set_moment_out = val_moment;
	
	if(set_moment_out == 0){
		set_pwm_mufta = 0;
		timer_channel_output_pulse_value_config(MUFTA_TIMER, MUFTA_PWM_TIMER_CH, set_pwm_mufta);
	} else if(set_moment_out == 1000){
		set_pwm_mufta = MUFTA_PWM_PERIOD;
		timer_channel_output_pulse_value_config(MUFTA_TIMER, MUFTA_PWM_TIMER_CH, set_pwm_mufta);
	} else{
		//step_pwm = 10;
		SetPwmMufta();
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

