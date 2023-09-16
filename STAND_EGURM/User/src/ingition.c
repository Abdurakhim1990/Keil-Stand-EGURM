#include "ingition.h"

//**********************************************************//
void IngitionInit(void)
{
	/* enable the LED1 clock */
	rcu_periph_clock_enable(INGITION_RCU_GPIO);
	
	/* configure LED1 GPIO port */ 
	gpio_init(INGITION_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, INGITION_GPIO_PIN);
	
	gpio_bit_reset(INGITION_GPIO, INGITION_GPIO_PIN);
}

//**********************************************************//
void IngitionOn(void)
{
	gpio_bit_set(INGITION_GPIO, INGITION_GPIO_PIN);
}

//**********************************************************//
void IngitionOff(void)
{
	gpio_bit_reset(INGITION_GPIO, INGITION_GPIO_PIN);
}
