#include "uartcan.h"

char uartcan_rx_buffer[UARTCAN_DMA_TX_SIZE];
char uartcan_tx_buffer[UARTCAN_DMA_RX_SIZE];


//******************************************************//
uint8_t* GetUartcanTxBuff(void)
{
	return (uint8_t*)uartcan_tx_buffer;
}

//**************************************************//
void UartcanSend(void)
{
	uint16_t len = GetCommandLength(uartcan_tx_buffer);
	
	dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	
	dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_TX, (uint32_t)uartcan_tx_buffer);
	dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_TX, len);
	
	/* enable DMA channel1 */
	dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	/* USART DMA enable for transmission and reception */
	usart_dma_transmit_config(UARTCAN_USART, USART_TRANSMIT_DMA_ENABLE);
}

//*******************************************************************//
void UartcanDmaRxData(uint16_t response_size)
{
    dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
    dma_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_FLAG_G);
    
    dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, response_size);
    dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
}

//******************************************************//
uint8_t GetCommandLength(char* str)
{
	uint8_t len = 0;
	while(str[len] != '\0')
	{
		++len;
	}
	return len;
}

