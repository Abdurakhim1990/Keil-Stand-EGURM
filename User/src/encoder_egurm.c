#include "encoder_egurm.h"

uint16_t angle_0_in_encoder = 0;

uint8_t count_zero = 0;


int16_t GetObsalutAngleEncoderIn(void)
{
	int16_t current_angle = timer_counter_read(ENCODER_IN_A_B_TIMER)/10;
	current_angle = current_angle - angle_0_in_encoder;
	return current_angle;
}

void ENCODER_IN_A_B_TIMER_IRQHandler(void)
{
		if(SET == timer_interrupt_flag_get(ENCODER_IN_A_B_TIMER, TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(ENCODER_IN_A_B_TIMER, TIMER_INT_FLAG_UP);
			++count_zero;
    }
}

void ENCODER_OUT_A_B_TIMER_IRQHandler(void)
{
		if(SET == timer_interrupt_flag_get(ENCODER_OUT_A_B_TIMER, TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(ENCODER_OUT_A_B_TIMER, TIMER_INT_FLAG_UP);
			++count_zero;
    }
}

