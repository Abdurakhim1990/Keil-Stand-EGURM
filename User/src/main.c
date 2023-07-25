#include "main.h"

struct clock_freq_get{
	uint32_t CK_SYS;
	uint32_t CK_AHB;
	uint32_t CK_APB1;
	uint32_t CK_APB2;
	uint32_t CK_USART;
} clock_freq;

/*!
    \brief      get the system clock, bus and peripheral clock frequency
    \param[in]  clock: the clock frequency which to get
                only one parameter can be selected which is shown as below:
      \arg        CK_SYS: system clock frequency
      \arg        CK_AHB: AHB clock frequency
      \arg        CK_APB1: APB1 clock frequency
      \arg        CK_APB2: APB2 clock frequency
      \arg        CK_USART: USART5 clock frequency
    \param[out] none
    \retval     clock frequency of system, AHB, APB1, APB2
*/	
void GetClockFreq(void)
{
	clock_freq.CK_SYS = rcu_clock_freq_get(CK_SYS);
	clock_freq.CK_AHB = rcu_clock_freq_get(CK_AHB);
	clock_freq.CK_APB1 = rcu_clock_freq_get(CK_APB1);
	clock_freq.CK_APB2 = rcu_clock_freq_get(CK_APB2);
	clock_freq.CK_USART = rcu_clock_freq_get(CK_USART);
}


uint8_t speed_can = 0;

int16_t n_m_in = 0, n_m_out = 0, voltage = 0, current = 0;

//*******************************************************//
int main(void)
{
	systick_config();
	GeneralInitEgurm();
	
	GetClockFreq();
	
	/* enable the LED1 clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	
	/* configure LED1 GPIO port */ 
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
	
	gpio_bit_reset(GPIOA, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
	
	uint16_t temp = 0;
	
	
	while(1)
	{
		gpio_port_write(GPIOA, (uint8_t)temp << 7);
		//usbSend();
		if(speed_can == 1){
			CanConfig(BAUD_RATE_250);
			speed_can = 0;
		} else if(speed_can == 2){
			CanConfig(BAUD_RATE_500);
			speed_can = 0;
		}
		CanTransmit();
		
		
		n_m_in = GetMomentIn();
		n_m_out = GetMomentOut();
		voltage = GetVoltage();
		current = GetCurrent();
		
		temp++;
		delay_1ms(100);
	}
}

void GeneralInitEgurm(void)
{
	InitTimerTo10us();
	InitTimerTo100us();
	UsbdEgurmInit();
	//CanEgurmInit();
	AdcEgurnInit();
}
