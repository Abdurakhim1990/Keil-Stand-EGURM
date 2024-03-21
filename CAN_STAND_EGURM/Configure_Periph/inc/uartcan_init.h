#ifndef UARTCAN_INIT_H
#define UARTCAN_INIT_H

#include "uartcan.h"

#define	UARTCAN_RCU_USART							RCU_UART3
#define	UARTCAN_RCU_GPIO							RCU_GPIOC
#define	UARTCAN_RCU_DMA								RCU_DMA1
			
#define	UARTCAN_USART									UART3
			
#define	UARTCAN_GPIO									GPIOC
#define	UARTCAN_GPIO_PIN_TX						GPIO_PIN_10
#define	UARTCAN_GPIO_PIN_RX						GPIO_PIN_11
			
#define	UARTCAN_BAUD_RATE							115200U
#define	UARTCAN_DMA										DMA1
#define	UARTCAN_DMA_CH_TX							DMA_CH4
#define	UARTCAN_DMA_CH_RX							DMA_CH2

#define	UARTCAN_DMA_Channel_IRQn_Tx 		DMA1_Channel3_Channel4_IRQn
#define	UARTCAN_DMA_Channel_IRQn_Rx 		DMA1_Channel2_IRQn
#define	UARTCAN_USART_IRQn_Rx 					UART3_IRQn


#define	UARTCAN_DMA_Channel_IRQHandler_Tx		DMA1_Channel3_4_IRQHandler
#define	UARTCAN_DMA_Channel_IRQHandler_Rx		DMA1_Channel2_IRQHandler
#define	UARTCAN_USART_IRQHandler_Rx					UART3_IRQHandler

void DmaUartcanTxInit(void);
void DmaUartcanRxInit(void);
void UartcanNvicConfig(void);
void UsartcanInit(void);

#endif
