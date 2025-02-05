#include "test.h"

uint16_t milli_amper = 0;
uint16_t inj_curr = 0;

void Test(void)
{
	//TestSendUartCan();
	//MotorTest();
	//IngitionOn();
	//inj_curr = GetCurrentInjition();
	//milli_amper = GetCurrentMilliAmpere();
	MainTest();
	//TestAkip();
}

int16_t n_m_in = 0, n_m_out = 0, voltage = 0, current = 0, temper = 0;
int16_t setting_voltage = 0;
int16_t setting_current = 0;

uint8_t flag_set_volt = 0;
uint8_t flag_set_curr = 0;
uint16_t angle_encoder_in = 0, angle_encoder_out = 0;

uint8_t revers_volt = 0;


void MainTest(void)
{
	static uint8_t temp = 0;
//	if(flag_set_volt == 1){
//		SetVoltageEgurm(setting_voltage);
//	}
//	if(flag_set_curr == 1){
//		SetCurrentEgurm(setting_current);
//	}
	
	//n_m_in = GetMomentIn();
	//n_m_out = GetMomentOut();
	//voltage = GetVoltage();
	//current = GetCurrent();
	//temper = GetTemperature();
	
	angle_encoder_in = GetAngleEncoderIn();
	angle_encoder_out = GetAngleEncoderOut();
	//SetVoltageEgurm(temp);
	
	//SetPwmMufta();
	//
	//if(revers_volt){
	//	IngitionOn();
	//		VoltageReversOn();
	//} else{
	//	IngitionOff();
	//		VoltageReversOff();
	//}
	//ServoMotor();
}

char test_write_ident[] = "Assalam 1111";
char test_read_ident[] = "My Parametr";
char test_write_firmware[] = "Firmware EGURM 2024";
uint8_t mode_uart_can = 0;
uint16_t test_speed_car = 8888;
uint16_t test_speed_engine = 1111;

void TestSendUartCan(void)
{
	char * test_send_canuart;
	
	if(mode_uart_can == 1){
		test_send_canuart = test_write_ident;
		volatile uint8_t len = sizeof(test_write_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 0;
	} else if(mode_uart_can == 2){
		test_send_canuart = test_read_ident;
		volatile uint8_t len = sizeof(test_read_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_READ_ID);
		mode_uart_can = 0;
	} else if(mode_uart_can == 3){
		test_send_canuart = test_write_firmware;
		volatile uint8_t len = sizeof(test_write_firmware) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 0;
	} else if(mode_uart_can == 4){
		test_send_canuart = test_write_firmware;
		volatile uint8_t len = 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 0;
	} else if(mode_uart_can == 5){
		SetEngineSpeed(test_speed_engine);
		//mode_uart_can = 0;
	} else if(mode_uart_can == 6){
		SetCarSpeed(test_speed_car);
		//mode_uart_can = 0;
	} else if(mode_uart_can == 7){
		SetZeroWheel(555);
		mode_uart_can = 8;
	} else if(mode_uart_can == 8){
		SetLeftWheel(1111);
		mode_uart_can = 9;
	} else if(mode_uart_can == 9){
		SetRightWheel(5656);
		mode_uart_can = 10;
	} else if(mode_uart_can == 10){
		SetAngleWheel(3546);
		mode_uart_can = 0;
	} else if(mode_uart_can == 11){
		test_send_canuart = test_write_ident;
		volatile uint8_t len = sizeof(test_write_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 12;
	} else if(mode_uart_can == 12){
		test_send_canuart = test_read_ident;
		volatile uint8_t len = sizeof(test_read_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 13;
	} else if(mode_uart_can == 13){
		test_send_canuart = test_write_firmware;
		volatile uint8_t len = sizeof(test_write_firmware) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 0;
	}
}

uint32_t timer_ms[3] = {0};
status_timer status_buf[3] = {0};


void ConfigTimerCounter(uint8_t num, status_timer st)
{
	status_buf[num] = st;
	if(st == START_TIM){
		timer_ms[num] = 0;
	}
}

void CounterTimerUsb(void)
{
	for(uint8_t i = 0; i < sizeof(status_buf); ++i){
		if(status_buf[i] == START_TIM)
			++timer_ms[i];
	}
}


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


typedef struct {
	#define TEST_SIZE_BUFF_USB		150
	uint8_t clear;
	uint16_t count;
	uint16_t index;
	uint16_t val[TEST_SIZE_BUFF_USB];
} TestCounter;

TestCounter TestUsb;

void AddTestUSB(uint16_t par, uint16_t val)
{
	TestUsb.count++;
	TestUsb.val[TestUsb.index++] = par;
	TestUsb.val[TestUsb.index++] = val;
	TestUsb.index %= TEST_SIZE_BUFF_USB;
}

void ClearTestUSB(void)
{
	if(TestUsb.clear){
		TestUsb.count = 0;
		TestUsb.index = 0;
		for(uint16_t i = 0; i < TEST_SIZE_BUFF_USB; ++i){
			TestUsb.val[i] = 0;
		}
		TestUsb.clear = 0;
	}
}


TestCounter TestEncod;
void AddTestInEncoder(uint16_t val)
{
	TestEncod.count++;
	TestEncod.val[TestEncod.index++] = val;
	TestEncod.index %= TEST_SIZE_BUFF_USB;
}

void ClearTestEncoder(void)
{
	if(TestEncod.clear){
		TestEncod.count = 0;
		TestEncod.index = 0;
		for(uint16_t i = 0; i < TEST_SIZE_BUFF_USB; ++i){
			TestEncod.val[i] = 0;
		}
		TestEncod.clear = 0;
	}
}

uint16_t akip_volt = 222;
void TestAkip(void)
{
	SetVoltageEgurm(akip_volt);
}
