#include "uartcan.h"

char uartcan_rx_buffer[UARTCAN_DMA_TX_SIZE];


//*******************************************************************//
void UartcanDmaRxData(void)
{
    dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
    dma_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_FLAG_G);
    
//    dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, UARTCAN_SIZE_PACKET);
    dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
}

//*******************************************************************//
void UARTCAN_USART_IRQHandler_Rx(void)
{
	//volatile uint8_t code_error = 0;
	volatile char rx_data;
	
	if(RESET != usart_interrupt_flag_get(UARTCAN_USART, USART_INT_FLAG_IDLE)){
		/* clear IDLE flag */
		rx_data = usart_data_receive(UARTCAN_USART);
		
		dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
		//dma_transfer_number_config(UARTCAN_DMA, AKIP_DMA_CH_RX, UARTCAN_SIZE_PACKET);
		dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, (uint32_t)uartcan_rx_buffer);
		dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
	}
}

//***********************---UARTCAN RS232_Rx---**********************//
void UARTCAN_DMA_Channel_IRQHandler_Rx(void)
{
	if(RESET != dma_interrupt_flag_get(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_INT_FLAG_FTF)){
		dma_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_FLAG_G);
		
		dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
		dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, (uint32_t)uartcan_rx_buffer);
//		dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, UARTCAN_SIZE_PACKET);
		dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
		
		dma_interrupt_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_INT_FLAG_G);
	}
}


