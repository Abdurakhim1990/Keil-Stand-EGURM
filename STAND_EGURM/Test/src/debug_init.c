#include "debug_init.h"


//**********************************************//
void DebugInit(void)
{
	/* enable USART clock */
	rcu_periph_clock_enable(DEBUG_RCU_USART);

	/* enable GPIO clock */
	rcu_periph_clock_enable(DEBUG_RCU_GPIO_TX);
	rcu_periph_clock_enable(DEBUG_RCU_GPIO_RX);
	/* configure the USART0 Tx pin and USART0 Rx pin */
	gpio_init(DEBUG_GPIO_TX, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, DEBUG_GPIO_PIN_TX);
	gpio_init(DEBUG_GPIO_RX, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, DEBUG_GPIO_PIN_RX);

	/* USART configure */
	usart_deinit(DEBUG_USART);
	usart_baudrate_set(DEBUG_USART, DEBUG_BAUD_RATE);
	
	usart_receive_config(DEBUG_USART, USART_RECEIVE_ENABLE);
	usart_transmit_config(DEBUG_USART, USART_TRANSMIT_ENABLE);
	
	usart_enable(DEBUG_USART);
	
	DebugNvicConfig();
}

//**********************************************//
void DebugNvicConfig(void)
{
	nvic_irq_enable(DEBUG_USART_IRQn_Rx, 0U, 0U);

	/* enable USART TBE interrupt */
	usart_interrupt_enable(DEBUG_USART, USART_INT_RBNE);
}

