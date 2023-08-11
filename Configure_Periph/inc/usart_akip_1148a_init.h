#ifndef USART_AKIP_1148A_INIT_H
#define USART_AKIP_1148A_INIT_H

#include "usart_akip_1148a.h"


#define	AKIP_RCU_USART							RCU_USART2
#define	AKIP_RCU_GPIO								RCU_GPIOB
#define	AKIP_RCU_DMA								RCU_DMA0
			
#define	AKIP_USART									USART2
			
#define	AKIP_GPIO										GPIOB
#define	AKIP_GPIO_PIN_TX						GPIO_PIN_10
#define	AKIP_GPIO_PIN_RX						GPIO_PIN_11
			
#define	AKIP_BAUD_RATE							19200U
#define	AKIP_DMA										DMA0
#define	AKIP_DMA_CH_TX							DMA_CH1
#define	AKIP_DMA_CH_RX							DMA_CH2

#define	AKIP_DMA_Channel_IRQn_Tx 		DMA0_Channel1_IRQn
#define	AKIP_DMA_Channel_IRQn_Rx 		DMA0_Channel2_IRQn
#define	AKIP_USART_IRQn_Rx 					USART2_IRQn


#define	AKIP_DMA_Channel_IRQHandler_Tx		DMA0_Channel1_IRQHandler
#define	AKIP_DMA_Channel_IRQHandler_Rx		DMA0_Channel2_IRQHandler
#define	AKIP_USART_IRQHandler_Rx					USART2_IRQHandler


void UsartAkip1148aInit(void);
void DmaAkip1148aTxInit(void);
void DmaAkip1148aRxInit(void);
void AkipNvicConfig(void);

#endif
