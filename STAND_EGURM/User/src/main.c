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

void ServoMotor(void);

uint8_t speed_can = 0;

int16_t n_m_in = 0, n_m_out = 0, voltage = 0, current = 0, temper = 0;
int16_t setting_voltage = 0;
int16_t setting_current = 0;

uint8_t flag_set_volt = 0;
uint8_t flag_set_curr = 0;

uint16_t value_encoder = 0;
int16_t angle_encoder_in = 0, angle_encoder_out = 0;

uint16_t pwm_mufta = 0;

uint8_t revers_volt = 0;

uint8_t usbd_can_mode = 0;

int16_t dpr_voltage[4] = {0, 0, 0, 0};


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
	
	/* enable the LED1 clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	
	/* configure LED1 GPIO port */ 
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
	
	gpio_bit_reset(GPIOA, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
	
	uint8_t temp = 0;
	
	
	while(1)
	{
		Test();
		gpio_port_write(GPIOA, (uint8_t)temp << 7);
		dpr_voltage[0] = GetVoltageDM_1();
		dpr_voltage[1] = GetVoltageDM_2();
		dpr_voltage[2] = GetVoltageDM_3();
		dpr_voltage[3] = GetVoltageDM_4();
	//usbSendResponse();
//	if(flag_set_volt == 1){
//		SetVoltageEgurm(setting_voltage);
//	}
//	if(flag_set_curr == 1){
//		SetCurrentEgurm(setting_current);
//	}
//	
//	n_m_in = GetMomentIn();
//	n_m_out = GetMomentOut();
//	voltage = GetVoltage();
//	current = GetCurrent();
//	temper = GetTemperature();
//	
//	angle_encoder_in = GetAngleEncoderIn();
//	SetVoltageEgurm(temp);
//	
//	SetPwmMufta(pwm_mufta);
//	
//	if(revers_volt){
//		IngitionOn();
//			VoltageReversOn();
//	} else{
//		IngitionOff();
//			VoltageReversOff();
//	}
	ServoMotor();
		temp++;
		delay_1ms(1000);
	}
}

int32_t pos_angle = 360;			// угол поворота
int16_t speed_motor = 50;		// частота вращения
uint32_t torque = 10;					// крутящий момент
uint8_t main_servo_mode = 0;	

void ServoMotor(void)
{
	switch (main_servo_mode) {
		/*--------------------------------- SET TORQUE -----------------------------------*/
		case 1:
			SetServoTorque(torque); 									// (1) set torque
			break;
		/*------------------------------------ JOG ---------------------------------------*/
		case 2:
			SetServoSpeed(speed_motor);								// (2) jog mode - on and rotation
			break;
		case 3:
			SetServoSpeed(0);													// (3) jog mode - stop rotation
			break;
		/*-------------------------------- POSITIONING -----------------------------------*/
		case 4:
			SetServoPosition(pos_angle);									// (4) pos mode - on and direct rotation
			break;
		case 5:
			SetServoPosition(0);													// (5) pos mode - set path
			break;
		default:
			break;
	}
	main_servo_mode = 0;
}

void GeneralInitEgurm(void)
{
	InitTimerTo10us();
	InitTimerTo100us();
	AdcEgurnInit();
	UsartAkip1148aInit();
	ServoInit();
	ServoInit();
	EncoderInInit();
	MUFTA_PWMEgurmInit();
	//UsbdEgurmInit();
	PeriodUsbReturnInit();
	VoltageReversInit();
	IngitionInit();
	UsartcanInit();
	SwitchingCurrentInit();
}
