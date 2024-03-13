#include "uartcan_init.h"

extern uint8_t uartcan_rx_buffer[UARTCAN_DMA_RX_SIZE];

void UsartcanInit(void)
{
	/* enable USART clock */
	rcu_periph_clock_enable(UARTCAN_RCU_USART);

	/* enable GPIO clock */
	rcu_periph_clock_enable(UARTCAN_RCU_GPIO);
	/* configure the USART0 Tx pin and USART0 Rx pin */
	gpio_init(UARTCAN_GPIO, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, UARTCAN_GPIO_PIN_TX);
	gpio_init(UARTCAN_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, UARTCAN_GPIO_PIN_RX);

	/* USART configure */
	usart_deinit(UARTCAN_USART);
	usart_baudrate_set(UARTCAN_USART, UARTCAN_BAUD_RATE);
	
	usart_receive_config(UARTCAN_USART, USART_RECEIVE_ENABLE);
	usart_transmit_config(UARTCAN_USART, USART_TRANSMIT_ENABLE);
	
	usart_enable(UARTCAN_USART);
	
	DmaUartcanTxInit();
	DmaUartcanRxInit();
	UartcanNvicConfig();
	
	usart_interrupt_enable(UARTCAN_USART, USART_INT_IDLE);
}

//********************************************************//
void DmaUartcanTxInit(void)
{
	dma_parameter_struct dma_init_struct;
	/* enable UARTCAN_DMA clock */
	rcu_periph_clock_enable(UARTCAN_RCU_DMA);
	
	/* initialize DMA channel3 Tx*/
	dma_deinit(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	dma_struct_para_init(&dma_init_struct);
	
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_addr = (uint32_t)GetUartcanTxBuff;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = UARTCAN_DMA_TX_SIZE;
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(UARTCAN_USART);
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(UARTCAN_DMA, UARTCAN_DMA_CH_TX, &dma_init_struct);
	
	/* configure DMA mode */
	dma_circulation_disable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	dma_memory_to_memory_disable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	
	/* enable DMA channel6 transfer complete interrupt */
	dma_interrupt_enable(UARTCAN_DMA, UARTCAN_DMA_CH_TX, DMA_INT_FTF);
}

//**************************************************************//
void DmaUartcanRxInit(void)
{
	dma_parameter_struct dma_init_struct;
	
	/* initialize DMA channel2 Rx*/
	dma_deinit(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_addr = (uint32_t)uartcan_rx_buffer;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = UARTCAN_HEARDER_SIZE;
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(UARTCAN_USART);
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(UARTCAN_DMA, UARTCAN_DMA_CH_RX, &dma_init_struct);
	
	/* configure DMA mode */
	dma_circulation_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
	dma_memory_to_memory_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
	
	/* USART DMA enable for reception */
	usart_dma_receive_config(UARTCAN_USART, USART_RECEIVE_DMA_ENABLE);
	/* enable DMA channel3 transfer complete interrupt */
	dma_interrupt_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_INT_FTF);
	/* enable DMA channel3 */
	dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
}

//**********************************************//
void UartcanNvicConfig(void)
{
    /*configure UARTCAN_DMA_CHx interrupt*/
    nvic_irq_enable(UARTCAN_DMA_Channel_IRQn_Tx, 0U, 0U);
    /*configure UARTCAN_DMA_CHx interrupt*/
    nvic_irq_enable(UARTCAN_DMA_Channel_IRQn_Rx, 2U, 0U);
		//nvic_irq_enable(UARTCAN_USART_IRQn_Rx, 0U, 0U);
}
