#ifndef USART_AKIP_1148A_H
#define USART_AKIP_1148A_H

#include "main.h"
#include "usart_akip_1148a_init.h"

#define AKIP_DMA_TX_SIZE				30
#define AKIP_DMA_RX_SIZE				30

enum MAX_PARARAMETRS{
	MAX_VOLTAGE = 500,		// Umax = 50.0 B
	MAX_CURRENT = 600,		// Imax = 60.0 A
};

enum MODE_AKIP{
	MODE_CURRENT_FLAG	= 0,
	MODE_VOLTAGE_FLAG	= 1,
};

void AkipSend(void);
uint8_t* GetAkipTxBuff(void);
void Period1MsForAkip(void);
uint8_t GetAkipCommandLength(char* str);
void SetParametr(void);
void SetVoltageEgurm(uint16_t voltage);
void SetVoltageAkip(void);
void SetCurrentEgurm(uint16_t current);
void SetCurrentAkip(void);
void AKIP_USART_IRQHandler_Rx(void);
void AKIP_DMA_Channel_IRQHandler_Tx(void);
void AKIP_DMA_Channel_IRQHandler_Rx(void);


#endif
