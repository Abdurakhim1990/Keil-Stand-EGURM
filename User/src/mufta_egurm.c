#include "mufta_egurm.h"

void SetPwmMufta(uint16_t val_pwm)
{
	if(val_pwm > MUFTA_PWM_PERIOD){
		val_pwm = MUFTA_PWM_PERIOD;
	}
	timer_channel_output_pulse_value_config(MUFTA_TIMER, MUFTA_TIMER_CH, val_pwm);
}
