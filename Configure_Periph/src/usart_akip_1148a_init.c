#include "usart_akip_1148a_init.h"

extern uint8_t akip_rx_buffer[20];

//***********************************************************//
void UsartAkip1148aInit(void)
{
	/* enable USART clock */
	rcu_periph_clock_enable(AKIP_RCU_USART);

	/* enable GPIO clock */
	rcu_periph_clock_enable(AKIP_RCU_GPIO);
	/* configure the USART0 Tx pin and USART0 Rx pin */
	gpio_init(AKIP_GPIO, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, AKIP_GPIO_PIN_TX);
	gpio_init(AKIP_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, AKIP_GPIO_PIN_RX);

	/* USART configure */
	usart_deinit(AKIP_USART);
	usart_baudrate_set(AKIP_USART, AKIP_BAUD_RATE);
	
	usart_receive_config(AKIP_USART, USART_RECEIVE_ENABLE);
	usart_transmit_config(AKIP_USART, USART_TRANSMIT_ENABLE);
	
	usart_enable(AKIP_USART);
	
	DmaAkip1148aTxInit();
	DmaAkip1148aRxInit();
	AkipNvicConfig();
	
	usart_interrupt_enable(AKIP_USART, USART_INT_IDLE);
}

//********************************************************//
void DmaAkip1148aTxInit(void)
{
	dma_parameter_struct dma_init_struct;
	/* enable AKIP_DMA clock */
	rcu_periph_clock_enable(AKIP_RCU_DMA);
	
	/* initialize DMA channel3 Tx*/
	dma_deinit(AKIP_DMA, AKIP_DMA_CH_TX);
	dma_struct_para_init(&dma_init_struct);
	
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_addr = (uint32_t)GetAkipTxBuff;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = AKIP_DMA_TX_SIZE;
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(AKIP_USART);
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(AKIP_DMA, AKIP_DMA_CH_TX, &dma_init_struct);
	
	/* configure DMA mode */
	dma_circulation_disable(AKIP_DMA, AKIP_DMA_CH_TX);
	dma_memory_to_memory_disable(AKIP_DMA, AKIP_DMA_CH_TX);
	
	/* enable DMA channel6 transfer complete interrupt */
	dma_interrupt_enable(AKIP_DMA, AKIP_DMA_CH_TX, DMA_INT_FTF);
}

//**************************************************************//
void DmaAkip1148aRxInit(void)
{
	dma_parameter_struct dma_init_struct;
	
	/* initialize DMA channel2 Rx*/
	dma_deinit(AKIP_DMA, AKIP_DMA_CH_RX);
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_addr = (uint32_t)akip_rx_buffer;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = AKIP_DMA_RX_SIZE;
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(AKIP_USART);
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(AKIP_DMA, AKIP_DMA_CH_RX, &dma_init_struct);
	
	/* configure DMA mode */
	dma_circulation_disable(AKIP_DMA, AKIP_DMA_CH_RX);
	dma_memory_to_memory_disable(AKIP_DMA, AKIP_DMA_CH_RX);
	
	/* USART DMA enable for reception */
	usart_dma_receive_config(AKIP_USART, USART_RECEIVE_DMA_ENABLE);
	/* enable DMA channel3 transfer complete interrupt */
	dma_interrupt_enable(AKIP_DMA, AKIP_DMA_CH_RX, DMA_INT_FTF);
	/* enable DMA channel3 */
	dma_channel_enable(AKIP_DMA, AKIP_DMA_CH_RX);
}

//**********************************************//
void AkipNvicConfig(void)
{
    /*configure AKIP_DMA_CH1 interrupt*/
    nvic_irq_enable(AKIP_DMA_Channel_IRQn_Tx, 0U, 0U);
    /*configure DMA0_CH2 interrupt*/
    nvic_irq_enable(AKIP_DMA_Channel_IRQn_Rx, 2U, 0U);
		nvic_irq_enable(AKIP_USART_IRQn_Rx, 0U, 0U);
}
