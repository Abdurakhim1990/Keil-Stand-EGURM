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

int16_t n_m_in = 0, n_m_out = 0, voltage = 0, current = 0, temper = 0;
int16_t setting_voltage = 0;
int16_t setting_current = 0;

uint8_t flag_set_volt = 0;
uint8_t flag_set_curr = 0;

uint16_t value_encoder = 0;
int16_t angle_encoder_in = 0, angle_encoder_out = 0;

uint16_t pwm_mufta = 200;

uint8_t revers_volt = 0;

uint8_t usbd_can_mode = 0;





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
	
	uint8_t temp = 0;
	
	
	while(1)
	{
		gpio_port_write(GPIOA, (uint8_t)temp << 7);
		usbSend();
		if(flag_set_volt == 1){
			SetVoltageEgurm(setting_voltage);
		}
		if(flag_set_curr == 1){
			SetCurrentEgurm(setting_current);
		}
		
		n_m_in = GetMomentIn();
		n_m_out = GetMomentOut();
		voltage = GetVoltage();
		current = GetCurrent();
		temper = GetTemperature();
		
		angle_encoder_in = GetObsalutAngleEncoderIn();
		SetVoltageEgurm(temp);
		
		SetPwmMufta(pwm_mufta);
		
		if(revers_volt){
			IngitionOn();
//			VoltageReversOn();
		} else{
			IngitionOff();
//			VoltageReversOff();
		}
		
		temp++;
		delay_1ms(1000);
	}
}

uint32_t time_speed = 500;
int32_t position = 655360;
uint16_t speed_motor = 50;
uint32_t torque = 50;

uint8_t temp = 0;

void ServoMotor(void)
{
	if (temp == 0) {
                        
		}
		else if (temp == 1){
										gpio_bit_set(GPIOA, GPIO_PIN_10);               // jog mode on
										ServoJogModeOn();
										temp = 0;
		}
		else if (temp == 2){
										gpio_bit_reset(GPIOA, GPIO_PIN_10);             // jog mode off
										ServoJogModeOff();
										temp = 0;
		}
		else if (temp == 3){
										ServoSetFreq(speed_motor);                                      // jog direct rot
										ServoSetAccelerationTime(time_speed);
										ServoSetTorque(torque);
										gpio_bit_set(GPIOA, GPIO_PIN_9);
										ServoJogModeDirectRotation();
										temp = 0;
		}
		else if (temp == 4){
										ServoSetFreq(speed_motor);                                      // jog reverse rot
										ServoSetAccelerationTime(time_speed);
										ServoSetTorque(torque);
										gpio_bit_set(GPIOA, GPIO_PIN_9);
										ServoJogModeReversRotation();
										temp = 0;
		}
		else if (temp == 5){
										ServoJogModeStopRotation();                             // jog mode stop
										gpio_bit_reset(GPIOA, GPIO_PIN_9);
										temp = 0;
		}
		else if (temp == 6){
										gpio_bit_set(GPIOA, GPIO_PIN_10);               // pos mode on
										ServoPositioningModeOn();
										temp = 0;
		}
		else if (temp == 7){
										gpio_bit_set(GPIOA, GPIO_PIN_10);               // pos mode off
										ServoPositioningModeOff();
										temp = 0;
		}
		else if (temp == 8){
										ServoSetFreq(speed_motor);
										ServoSetAccelerationTime(time_speed);                   // pos mode config
										ServoSetTorque(torque);
										ServoSetPathLength(position);
										ServoPositioningModeConfig();
										temp = 0;
		}
		else if (temp == 9){
										ServoPositioningModePathLength();               // pos mode set path
										temp = 0;
		}
}

void GeneralInitEgurm(void)
{
	InitTimerTo10us();
	InitTimerTo100us();
	AdcEgurnInit();
	UsartAkip1148aInit();
	EncoderInInit();
	MuftaEgurmInit();
	UsbdEgurmInit();
	VoltageReversInit();
	IngitionInit();
}
