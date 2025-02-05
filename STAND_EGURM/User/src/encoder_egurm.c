#include "encoder_egurm.h"


uint16_t angle_0_in_encoder = 0;
uint8_t count_zero = 0;
int16_t steering_speed = 0;

//*********************************************************//
int16_t GetAngleEncoderIn(void)
{
	uint16_t current_angle = timer_counter_read(ENCODER_IN_A_B_TIMER);
	AddTestInEncoder(current_angle);
	return current_angle;
}

//*********************************************************//
int16_t GetAngleEncoderOut(void)
{
	int16_t current_angle = timer_counter_read(ENCODER_OUT_A_B_TIMER);
	return current_angle;
}

//*********************************************************//
int16_t GetSteeringSpeed(void)
{
	return steering_speed;
}

//*********************************************************//
void ENCODER_IN_A_B_TIMER_IRQHandler(void)
{
	static uint8_t perehod = 0;
	if(SET == timer_interrupt_flag_get(ENCODER_IN_A_B_TIMER, TIMER_INT_FLAG_UP)){
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(ENCODER_IN_A_B_TIMER, TIMER_INT_FLAG_UP);
		if(perehod == 0){
			perehod = 1;
			++count_zero;
		}
  }
	
	if(SET == timer_interrupt_flag_get(ENCODER_IN_A_B_TIMER, TIMER_INT_CH3)){
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(ENCODER_IN_A_B_TIMER, TIMER_INT_CH3);
		if(perehod == 1){
			perehod = 0;
		}
  }
}

//*********************************************************//
void ENCODER_OUT_A_B_TIMER_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(ENCODER_OUT_A_B_TIMER, TIMER_INT_FLAG_UP)){
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(ENCODER_OUT_A_B_TIMER, TIMER_INT_FLAG_UP);
  }
}


//uint32_t my_angle = 0;
//uint16_t my_pre_val_angle = 0;
//uint16_t my_val_angle = 0;
//uint8_t fixed = 0;
////*********************************************************//
//void TIMER0_TRG_CMT_TIMER10_IRQHandler(void)
//{
//	static uint16_t pre_val_angle = 0;
//	uint16_t val_angle = 0;
//	uint32_t angle = 0;
//	
//	if(SET == timer_interrupt_flag_get(PERIOD_FOR_SPEED_TIMER, TIMER_INT_FLAG_UP)){
//    /* clear channel 0 interrupt bit */
//    timer_interrupt_flag_clear(PERIOD_FOR_SPEED_TIMER, TIMER_INT_FLAG_UP);
//		
//		val_angle = timer_counter_read(ENCODER_IN_A_B_TIMER);
//		if(fixed){
//			my_val_angle = val_angle;
//			my_pre_val_angle = pre_val_angle;
//		}
//		
//		while(count_zero > 1){
//			angle += 36000;
//			--count_zero;
//		}
//		if(count_zero){
//			if(pre_val_angle < val_angle){
//				angle += (pre_val_angle + 36000 - val_angle);
//			} else if(pre_val_angle > val_angle){
//				angle += (val_angle + 36000 - pre_val_angle);
//			}
//		} else{
//			if(pre_val_angle < val_angle){
//				angle += (val_angle - pre_val_angle);
//			} else if(pre_val_angle > val_angle){
//				angle += (pre_val_angle - val_angle);
//			}
//		}
//		if(fixed){
//			my_angle  = angle;
//			fixed = 0;
//		}
//		count_zero = 0;
//		steering_speed = angle * 5;
//		pre_val_angle = val_angle;
//  }
//}
