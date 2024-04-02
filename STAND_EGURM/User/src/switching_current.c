#include "switching_current.h"


//**********************************************************//
void SwitchingCurrentInit(void)
{
	/* enable the LED1 clock */
	rcu_periph_clock_enable(SWITCHING_CURRENT_RCU_GPIO);
	
	/* configure LED1 GPIO port */ 
	gpio_init(SWITCHING_CURRENT_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SWITCHING_CURRENT_GPIO_PIN);
	
	gpio_bit_reset(SWITCHING_CURRENT_GPIO, SWITCHING_CURRENT_GPIO_PIN);
}

//**********************************************************//
void SelectMilliAmpere(void)
{
	gpio_bit_set(SWITCHING_CURRENT_GPIO, SWITCHING_CURRENT_GPIO_PIN);
}

//**********************************************************//
void SelectAmpere(void)
{
	gpio_bit_reset(SWITCHING_CURRENT_GPIO, SWITCHING_CURRENT_GPIO_PIN);
}

