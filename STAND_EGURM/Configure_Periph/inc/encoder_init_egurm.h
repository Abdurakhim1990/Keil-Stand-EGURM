#ifndef ENCODER_INIT_EGURM_H
#define ENCODER_INIT_EGURM_H

#include "encoder_egurm.h"

#define ENCODER_IN_A_B_RCU_GPIO							RCU_GPIOB
#define ENCODER_IN_R_RCU_GPIO								RCU_GPIOB
#define ENCODER_IN_A_B_RCU_TIMER						RCU_TIMER2
#define ENCODER_IN_R_RCU_TIMER							RCU_TIMER9

#define ENCODER_IN_A_B_GPIO									GPIOB
#define ENCODER_IN_R_GPIO										GPIOB
#define ENCODER_IN_A_GPIO_PIN								GPIO_PIN_4
#define ENCODER_IN_B_GPIO_PIN								GPIO_PIN_5
#define ENCODER_IN_R_GPIO_PIN								GPIO_PIN_8
#define ENCODER_IN_A_B_TIMER								TIMER2
#define ENCODER_IN_R_TIMER									TIMER9
#define ENCODER_IN_A_B_GPIO_TIMER_PARTIAL_REMAP			GPIO_TIMER2_PARTIAL_REMAP
#define ENCODER_IN_A_B_TIMER_IRQHandler			TIMER2_IRQHandler
#define ENCODER_IN_A_B_TIMER_IRQn						TIMER2_IRQn


#define ENCODER_OUT_A_B_RCU_GPIO							RCU_GPIOB
#define ENCODER_OUT_R_RCU_GPIO								RCU_GPIOB
#define ENCODER_OUT_A_B_RCU_TIMER							RCU_TIMER3
#define ENCODER_OUT_R_RCU_TIMER								RCU_TIMER10

#define ENCODER_OUT_A_B_GPIO									GPIOB
#define ENCODER_OUT_R_GPIO										GPIOB
#define ENCODER_OUT_A_GPIO_PIN								GPIO_PIN_6
#define ENCODER_OUT_B_GPIO_PIN								GPIO_PIN_7
#define ENCODER_OUT_R_GPIO_PIN								GPIO_PIN_9
#define ENCODER_OUT_A_B_TIMER									TIMER3
#define ENCODER_OUT_R_TIMER										TIMER10
#define ENCODER_OUT_A_B_TIMER_IRQHandler			TIMER3_IRQHandler
#define ENCODER_OUT_A_B_TIMER_IRQn						TIMER3_IRQn




void EncoderInGpioInit(void);
void EncoderIn_A_B_Init(void);
void EncoderIn_R_Init(void);
void EncoderOut_A_B_Init(void);
void EncoderOut_R_Init(void);
void EncoderInInit(void);

#endif
