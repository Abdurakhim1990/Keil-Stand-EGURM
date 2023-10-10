#ifndef UARTCAN_H
#define UARTCAN_H

#include "uartcan_init.h"
#include "main.h"

#define UARTCAN_DMA_TX_SIZE				30
#define UARTCAN_DMA_RX_SIZE				30


uint8_t* GetUartcanTxBuff(void);
void SetEngineSpeed(int16_t speed);
void SetCarSpeed(int16_t speed);
int16_t GetAngleEgurm(void);

#endif
