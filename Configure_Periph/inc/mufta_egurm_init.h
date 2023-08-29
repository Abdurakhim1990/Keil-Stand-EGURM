#ifndef MUFTA_EGURM_INIT_H
#define MUFTA_EGURM_INIT_H

#include "mufta_egurm.h"

#define MUFTA_RCU_TIMER				RCU_TIMER4
#define MUFTA_RCU_GPIO				RCU_GPIOA
#define MUFTA_GPIO						GPIOA
#define MUFTA_GPIO_PIN				GPIO_PIN_1
#define MUFTA_TIMER						TIMER4
#define MUFTA_TIMER_CH				TIMER_CH_1
#define MUFTA_PWM_PERIOD			1000


void MuftaEgurmInit(void);

#endif
