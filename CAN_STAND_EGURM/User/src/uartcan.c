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
	//uint16_t len = GetCommandLength(uartcan_tx_buffer);
	
	dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	
	dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_TX, (uint32_t)uartcan_tx_buffer);
//	dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_TX, UARTCAN_SIZE_PACKET);
	
	/* enable DMA channel1 */
	dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	/* USART DMA enable for transmission and reception */
	usart_dma_transmit_config(UARTCAN_USART, USART_TRANSMIT_DMA_ENABLE);
	/* wait DMA Channel transfer complete */
	//while(RESET == dma_flag_get(UARTCAN_DMA, UARTCAN_DMA_CH_TX, DMA_FLAG_FTF));
}

//*******************************************************************//
void UartcanDmaRxData(void)
{
    dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
    dma_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_FLAG_G);
    
//    dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, UARTCAN_SIZE_PACKET);
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

//*******************************************************************//
void UARTCAN_USART_IRQHandler_Rx(void)
{
	volatile uint8_t code_error = 0;
	volatile char rx_data;
	
	if(RESET != usart_interrupt_flag_get(UARTCAN_USART, USART_INT_FLAG_IDLE)){
		/* clear IDLE flag */
		rx_data = usart_data_receive(UARTCAN_USART);
		
		//dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
		////dma_transfer_number_config(AKIP_DMA, AKIP_DMA_CH_RX, UARTCAN_SIZE_PACKET);
		//dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, (uint32_t)uartcan_rx_buffer);
		//dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
	}
}

//***********************---UARTCAN RS232_Tx---**********************//
void UARTCAN_DMA_Channel_IRQHandler_Tx(void)
{
	if(RESET != dma_interrupt_flag_get(UARTCAN_DMA, UARTCAN_DMA_CH_TX, DMA_INT_FLAG_FTF)){
		dma_interrupt_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_TX, DMA_INT_FLAG_G);
		
	}
}

//***********************---UARTCAN RS232_Rx---**********************//
void UARTCAN_DMA_Channel_IRQHandler_Rx(void)
{
	if(RESET != dma_interrupt_flag_get(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_INT_FLAG_FTF)){
		dma_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_FLAG_G);
		
		if(uartcan_rx_buffer[SER_NUM_HEADER] < TYPE_ADD_OTHER){
			uint8_t length = uartcan_rx_buffer[SER_NUM_HEADER];
			
			dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
			dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, (uint32_t)uartcan_rx_buffer);
			dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, length);
			dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
		} else {
			dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
			dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, (uint32_t)uartcan_rx_buffer);
			dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, UARTCAN_HEARDER_SIZE);
			dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
			
			
		}
		dma_interrupt_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_INT_FLAG_G);
	}
}


