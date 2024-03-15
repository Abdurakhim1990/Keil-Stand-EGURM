#ifndef MUFTA_EGURM_INIT_H
#define MUFTA_EGURM_INIT_H

#include "mufta_egurm.h"

#define MUFTA_PWM_RCU_TIMER				RCU_TIMER4
#define MUFTA_PWM_RCU_GPIO				RCU_GPIOA
#define MUFTA_PWM_GPIO						GPIOA
#define MUFTA_PWM_GPIO_PIN				GPIO_PIN_1
#define MUFTA_PWM_TIMER						TIMER4
#define MUFTA_PWM_TIMER_CH				TIMER_CH_1
#define MUFTA_PWM_PERIOD			1000

#define	MUFTA_RCU_TIMER							RCU_TIMER11
#define MUFTA_TIMER									TIMER11
#define MUFTA_TIMER_IRQHandler			TIMER7_BRK_TIMER11_IRQHandler


void MUFTA_PWMEgurmInit(void);
void InitMuftaTimer(void);

#endif
