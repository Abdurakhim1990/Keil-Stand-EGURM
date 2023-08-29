#include "voltage_revers.h"


//**********************************************************//
void VoltageReversInit(void)
{
	/* enable the LED1 clock */
	rcu_periph_clock_enable(VOLTAGE_REVERS_RCU_GPIO);
	
	/* configure LED1 GPIO port */ 
	gpio_init(VOLTAGE_REVERS_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, VOLTAGE_REVERS_GPIO_PIN);
	
	gpio_bit_reset(VOLTAGE_REVERS_GPIO, VOLTAGE_REVERS_GPIO_PIN);
}

//**********************************************************//
void VoltageReversOn(void)
{
	gpio_bit_set(VOLTAGE_REVERS_GPIO, VOLTAGE_REVERS_GPIO_PIN);
}

//**********************************************************//
void VoltageReversOff(void)
{
	gpio_bit_reset(VOLTAGE_REVERS_GPIO, VOLTAGE_REVERS_GPIO_PIN);
}
