#include "led.h"


static void ToggleLed(uint32_t pin_gpio);

//*******************************************************//
void LedInit(void)
{
	/* enable the LED1 clock */
	rcu_periph_clock_enable(LED_RCU_GPIO);
	
	/* configure LED1 GPIO port */ 
	gpio_init(LED_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_1_PIN|LED_2_PIN|LED_3_PIN|LED_4_PIN);
	
	gpio_bit_reset(LED_GPIO, LED_1_PIN|LED_2_PIN|LED_3_PIN|LED_4_PIN);
}

void Led1_2Display(uint8_t temp)
{
	if(temp & 0x01) gpio_bit_set(LED_GPIO, LED_1_PIN);
	else gpio_bit_reset(LED_GPIO, LED_1_PIN);
	if(temp & 0x02) gpio_bit_set(LED_GPIO, LED_2_PIN);
	else gpio_bit_reset(LED_GPIO, LED_2_PIN);
}

void Led1_4Display(uint8_t temp)
{
	if(temp & 0x01) gpio_bit_set(LED_GPIO, LED_1_PIN);
	else gpio_bit_reset(LED_GPIO, LED_1_PIN);
	if(temp & 0x02) gpio_bit_set(LED_GPIO, LED_2_PIN);
	else gpio_bit_reset(LED_GPIO, LED_2_PIN);
	if(temp & 0x04) gpio_bit_set(LED_GPIO, LED_3_PIN);
	else gpio_bit_reset(LED_GPIO, LED_3_PIN);
	if(temp & 0x08) gpio_bit_set(LED_GPIO, LED_4_PIN);
	else gpio_bit_reset(LED_GPIO, LED_4_PIN);
	__ASM("NOP");
}

void ToggleLed(uint32_t pin_gpio)
{
	if(gpio_input_bit_get(LED_GPIO, pin_gpio)) gpio_bit_reset(LED_GPIO, pin_gpio);
	else gpio_bit_set(LED_GPIO, pin_gpio);
}

void ToggleLed_3(void)
{
	ToggleLed(LED_3_PIN);
}

void ToggleLed_4(void)
{
	ToggleLed(LED_4_PIN);
}
