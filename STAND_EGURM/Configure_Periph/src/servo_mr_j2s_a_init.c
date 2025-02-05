#include "servo_mr_j2s_a_init.h"

extern uint16_t servo_rx_size;
extern char servo_rxbuffer[];
extern uint16_t servo_tx_size;


void ServoInit(void/*servo_baudrate _baudrate*/)
{
	ServoUsartInit();
	ServoUsartDmaTxInit((char*)GetServoTxBuff(), servo_tx_size);
	ServoUsartDmaRxInit(servo_rxbuffer, servo_rx_size);

	/*configure SERVO_DMA_CH_TX interrupt*/
	nvic_irq_enable(SERVO_DMA_Channel_IRQn_Tx, 0U, 0U);
	/*configure SERVO_DMA_CH_RX interrupt*/
	nvic_irq_enable(SERVO_DMA_Channel_IRQn_Rx, 1U, 0U);
	InitServoTimer();
}

//************************************************//
void ServoUsartInit(void)
{
  /* enable COM GPIO clock */
  rcu_periph_clock_enable(SERVO_RCU_GPIO);
  
  /* enable USART clock */
  rcu_periph_clock_enable(SERVO_RCU_USART);
	
  /* configure USART Tx as alternate function push-pull */
	gpio_init(SERVO_GPIO, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, SERVO_GPIO_PIN_TX);
  
  /* configure USART Rx as alternate function push-pull */
	gpio_init(SERVO_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, SERVO_GPIO_PIN_RX);

  /* USART configure */
  usart_deinit(SERVO_USART);
  usart_baudrate_set(SERVO_USART, BAUDRATE_57600);
  usart_receive_config(SERVO_USART, USART_RECEIVE_ENABLE);
  usart_transmit_config(SERVO_USART, USART_TRANSMIT_ENABLE);
	usart_parity_config(SERVO_USART, USART_PM_EVEN);
	usart_stop_bit_set(SERVO_USART, USART_STB_1BIT);
	usart_word_length_set(SERVO_USART, USART_WL_9BIT);
  usart_enable(SERVO_USART);
}

void ServoUsartDmaTxInit(char* txbuffer, uint16_t len)
{
	dma_parameter_struct dma_init_struct;
	/* enable DMA clock */
	rcu_periph_clock_enable(SERVO_RCU_DMA);
	
	/* deinitialize DMA channel1 */
	dma_deinit(SERVO_DMA, SERVO_DMA_CH_TX);
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_addr = (uint32_t)GetServoTxBuff();
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = len;
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(SERVO_USART);
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(SERVO_DMA, SERVO_DMA_CH_TX, &dma_init_struct);
	
	/* configure DMA mode */
	//dma_interrupt_enable(SERVO_DMA, SERVO_DMA_CH_TX, DMA_INT_FTF);
	dma_circulation_disable(SERVO_DMA, SERVO_DMA_CH_TX);
	dma_memory_to_memory_disable(SERVO_DMA, SERVO_DMA_CH_TX);
}

void ServoUsartDmaRxInit(char* rxbuffer, uint16_t len)
{
    dma_parameter_struct dma_init_struct;
    /* deinitialize DMA channel2 */
    dma_deinit(SERVO_DMA, SERVO_DMA_CH_RX);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rxbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = len;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(SERVO_USART);;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(SERVO_DMA, SERVO_DMA_CH_RX, &dma_init_struct);
    
    dma_circulation_disable(SERVO_DMA, SERVO_DMA_CH_RX);
    dma_memory_to_memory_disable(SERVO_DMA, SERVO_DMA_CH_RX);

    /* enable DMA channel2 transfer complete interrupt */
    dma_interrupt_enable(SERVO_DMA, SERVO_DMA_CH_RX, DMA_INT_FTF);
    usart_dma_receive_config(SERVO_USART, USART_RECEIVE_DMA_ENABLE);
    dma_channel_enable(SERVO_DMA, SERVO_DMA_CH_RX);
}

//*********************************************************************//
/*!
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void InitServoTimer(void)
{
    /* ----------------------------------------------------------------------------
    SERVO_TIMER Configuration: 
    SERVO_TIMERCLK = SystemCoreClock/10 = 16.8MHz, the period is 1s(16800000/1680 = 100us).
    ---------------------------------------------------------------------------- */
		timer_oc_parameter_struct timer_ocinit_struct;
		timer_parameter_struct timer_init_struct;

    /* enable the peripherals clock */
    rcu_periph_clock_enable(SERVO_RCU_TIMER);

    /* deinit a TIMER */
    timer_deinit(SERVO_TIMER);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_init_struct);
    /* SERVO_TIMER configuration */
    timer_init_struct.prescaler					= 16799;
		timer_init_struct.alignedmode				= TIMER_COUNTER_EDGE;
		timer_init_struct.counterdirection	= TIMER_COUNTER_UP;
		timer_init_struct.period						= 2000;
		timer_init_struct.clockdivision			= TIMER_CKDIV_DIV1;
		timer_init(SERVO_TIMER, &timer_init_struct);

    /* enable the TIMER interrupt */
    //timer_interrupt_enable(SERVO_TIMER, TIMER_INT_UP);
    /* enable a TIMER */
    timer_enable(SERVO_TIMER);
		
		nvic_irq_enable(TIMER0_BRK_TIMER8_IRQn, 0, 0);
}

