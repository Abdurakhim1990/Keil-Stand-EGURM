#ifndef SERVO_MR_J2S_A_INIT_H
#define SERVO_MR_J2S_A_INIT_H

#include "servo_control.h"

#define	SERVO_RCU_USART							RCU_USART1
#define	SERVO_RCU_GPIO							RCU_GPIOA
#define	SERVO_RCU_DMA								RCU_DMA0
			
#define	SERVO_USART									USART1
			
#define	SERVO_GPIO									GPIOA
#define	SERVO_GPIO_PIN_TX						GPIO_PIN_2
#define	SERVO_GPIO_PIN_RX						GPIO_PIN_3
			
#define	SERVO_BAUD_RATE							19200U
#define	SERVO_DMA										DMA0
#define	SERVO_DMA_CH_TX							DMA_CH6
#define	SERVO_DMA_CH_RX							DMA_CH5

#define	SERVO_DMA_Channel_IRQn_Tx 		DMA0_Channel6_IRQn
#define	SERVO_DMA_Channel_IRQn_Rx 		DMA0_Channel5_IRQn
#define	SERVO_USART_IRQn_Rx 					USART1_IRQn


#define	SERVO_DMA_Channel_IRQHandler_Tx		DMA0_Channel6_IRQHandler
#define	SERVO_DMA_Channel_IRQHandler_Rx		DMA0_Channel5_IRQHandler
#define	SERVO_USART_IRQHandler_Rx					USART1_IRQHandler

#define	SERVO_RCU_TIMER							RCU_TIMER8
#define SERVO_TIMER									TIMER8
#define SERVO_TIMER_IRQHandler			TIMER0_BRK_TIMER8_IRQHandler

typedef enum {
	BAUDRATE_9600 = 9600,
	BAUDRATE_19200 = 19200,
	BAUDRATE_38400 = 38400,
	BAUDRATE_57600 = 57600
} servo_baudrate;

//__IO FlagStatus g_transfer_complete = SET;

#define ARRAYNUM(arr_nanme)       (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))


void ServoInit(void);
void ServoUsartInit(void);
void ServoUsartDmaTxInit(char* txbuffer, uint16_t len);
void ServoUsartDmaRxInit(char* rxbuffer, uint16_t len);

void InitServoTimer(void);

#endif /* SERVO_MR_J2S_A_INIT_H */
