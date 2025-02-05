#ifndef INCL_DEBUG_INIT_H
#define INCL_DEBUG_INIT_H

#include "main.h"


#define	DEBUG_RCU_USART							RCU_UART4
#define	DEBUG_RCU_GPIO_TX						RCU_GPIOC
#define	DEBUG_RCU_GPIO_RX						RCU_GPIOD

#define	DEBUG_USART									UART4

#define	DEBUG_GPIO_TX								GPIOC
#define	DEBUG_GPIO_RX								GPIOD
#define	DEBUG_GPIO_PIN_TX						GPIO_PIN_12
#define	DEBUG_GPIO_PIN_RX						GPIO_PIN_2

#define	DEBUG_BAUD_RATE							115200U

#define	DEBUG_USART_IRQn_Rx 					UART4_IRQn

#define	DEBUG_USART_IRQHandler_Rx					UART4_IRQHandler


void DebugInit(void);
void DebugNvicConfig(void);

#endif
