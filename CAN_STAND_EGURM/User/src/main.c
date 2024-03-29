#include "main.h"


//*******************************************************//
void LedInit(void)
{
	/* enable the LED1 clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	
	/* configure LED1 GPIO port */ 
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
	
	gpio_bit_reset(GPIOA, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
}

void Led1_2Display(uint8_t temp)
{
	if(temp & 0x01) gpio_bit_set(GPIOA, GPIO_PIN_7);
	else gpio_bit_reset(GPIOA, GPIO_PIN_7);
	if(temp & 0x02) gpio_bit_set(GPIOA, GPIO_PIN_8);
	else gpio_bit_reset(GPIOA, GPIO_PIN_8);
}

void ToggleLed(uint32_t pin_gpio)
{
	if(gpio_input_bit_get(GPIOA, pin_gpio)) gpio_bit_reset(GPIOA, pin_gpio);
	else gpio_bit_set(GPIOA, pin_gpio);
}

void DisplayLed_3(void)
{
	ToggleLed(GPIO_PIN_9);
}

void DisplayLed_4(void)
{
	ToggleLed(GPIO_PIN_10);
}

//*******************************************************//
int main(void)
{
	systick_config();
	GeneralInitEgurm();
	
	GetClockFreq();
	LedInit();
	
	uint8_t temp = 0;
	while(1)
	{
		Test();
		Led1_2Display(temp);
		
		temp++;
		delay_1ms(1000);
	}
}

//*******************************************************//
void GeneralInitEgurm(void)
{
	InitTimerTo10us();
	InitTimerTo100us();
	CanEgurmInit();
	UsartcanInit();
}
