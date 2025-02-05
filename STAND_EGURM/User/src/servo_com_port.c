#include "servo_com_port.h"

uint16_t servo_rx_size = BUFF_SIZE_RX;
char servo_rxbuffer[BUFF_SIZE_RX]; 
uint16_t servo_tx_size = BUFF_SIZE_TX;
char servo_txbuffer[BUFF_SIZE_TX];


void ServoSetDmaRxLength(uint16_t response_size);
void ServoUsartDmaSend(char* buffer, uint16_t len);
uint8_t GetChecksum(char* data, uint8_t from_byte, uint8_t end_byte);
uint16_t GetServoDataLength(const char* data);
void ServoTimerInterrupt(void);
void ServoRxInterrupt(void);


//******************************** SERVO CHECK RD_ON ***********************************//
/*!
		\brief      Проверка готовности
*/
void ServoCheckRdOn(void)
{ 
	ServoSendReadCommand(READ_IO_SIGNALS, OUTPUT_SIGNALS, RESPONSE_SIZE_IO_SIGNALS, 0);
	//servo_rd_on = (servo_rxbuffer[10] - '0') & 1;
}

//***************************--- Установка размера принимаемого пакета DMA ---********************************//
uint8_t* GetServoTxBuff(void)
{
	return (uint8_t*)servo_txbuffer;
}

//***************************--- Установка размера принимаемого пакета DMA ---********************************//
void ServoSetDmaRxLength(uint16_t response_size)
{
	timer_counter_value_config(SERVO_TIMER, 0);
	
	dma_channel_disable(SERVO_DMA, SERVO_DMA_CH_RX);
	dma_flag_clear(SERVO_DMA, SERVO_DMA_CH_RX, DMA_FLAG_G);
	dma_transfer_number_config(SERVO_DMA, SERVO_DMA_CH_RX, response_size);
	dma_channel_enable(SERVO_DMA, SERVO_DMA_CH_RX);
}

//*************************--- Отправить буффер в Com Port ---********************************//
void ServoUsartDmaSend(char* buffer, uint16_t len)
{
    dma_channel_disable(SERVO_DMA, SERVO_DMA_CH_TX);
    
    dma_memory_address_config(SERVO_DMA, SERVO_DMA_CH_TX, (uint32_t)buffer);
    dma_transfer_number_config(SERVO_DMA, SERVO_DMA_CH_TX, len);
    
    dma_channel_enable(SERVO_DMA, SERVO_DMA_CH_TX);
    usart_dma_transmit_config(USART1, USART_TRANSMIT_DMA_ENABLE);
}

//**********************--- Расчет контрольной суммы ---*************************//
uint8_t GetChecksum(char* data, uint8_t from_byte, uint8_t end_byte)
{
	volatile uint16_t checksum = 0;
	for(int8_t i = from_byte; i < end_byte; ++i){
		checksum += data[i];
	}
	return (uint8_t)checksum;
}

//*********************--- Отправка пакета для чтения данных ---*************************//
void ServoSendReadCommand(uint16_t command, uint16_t data, uint16_t response_size, uint8_t servo_number)
{
	ServoSetDmaRxLength(response_size);
	sprintf(servo_txbuffer, "%c%c%X%02X%c%02X%c", EOT, SOH, servo_number, command, STX, data, ETX);
	
	volatile uint8_t checksum = GetChecksum(servo_txbuffer, 2, GetServoDataLength(servo_txbuffer));
	sprintf(servo_txbuffer, "%s%02X", servo_txbuffer, checksum & 0xFF);

	ServoUsartDmaSend(servo_txbuffer, GetServoDataLength(servo_txbuffer));
}

//*******************--- Отправка пакета для записи данных с размером 4 разряда ---******************//
void ServoSendWriteCommand4(uint16_t write_command, uint16_t data_number, uint16_t data_to_write, uint8_t servo_number)
{
	ServoSetDmaRxLength(WRITE_COMMAND_RESPONSE_SIZE);
	sprintf(servo_txbuffer, "%c%c%X%02X%c%02X%04X%c", EOT, SOH, servo_number, write_command, STX, data_number, data_to_write, ETX);
	
	volatile uint8_t checksum = GetChecksum(servo_txbuffer, 2, GetServoDataLength(servo_txbuffer));
	sprintf(servo_txbuffer, "%s%02X", servo_txbuffer, checksum & 0xFF);
	
	ServoUsartDmaSend(servo_txbuffer, GetServoDataLength(servo_txbuffer));
}

uint16_t count_command8 = 0;
//********************--- Отправка пакета для записи данных с размером 8 разрядов ---*********************//
void ServoSendWriteCommand8(uint16_t write_command, uint16_t data_number, uint32_t data_to_write, uint8_t servo_number)
{
	++count_command8;
	ServoSetDmaRxLength(WRITE_COMMAND_RESPONSE_SIZE);
	sprintf(servo_txbuffer, "%c%c%X%02X%c%02X%08X%c", EOT, SOH, servo_number, write_command, STX, data_number, data_to_write, ETX);
	
	volatile uint8_t checksum = GetChecksum(servo_txbuffer, 2, GetServoDataLength(servo_txbuffer));
	sprintf(servo_txbuffer, "%s%02X", servo_txbuffer, checksum & 0xFF);
	
	ServoUsartDmaSend(servo_txbuffer, GetServoDataLength(servo_txbuffer));
}

//********************--- Отправка пакета для записи данных с размером 8 разрядов ---*********************//
void ServoSendWriteCommand8WithPoint(uint16_t write_command, uint16_t data_number, uint32_t data_to_write, uint8_t servo_number, uint8_t point)
{
	++count_command8;
	ServoSetDmaRxLength(WRITE_COMMAND_RESPONSE_SIZE);
	sprintf(servo_txbuffer, "%c%c%X%02X%c%02X%02X%06X%c", EOT, SOH, servo_number, write_command, STX, data_number, point, data_to_write, ETX);
	
	volatile uint8_t checksum = GetChecksum(servo_txbuffer, 2, GetServoDataLength(servo_txbuffer));
	sprintf(servo_txbuffer, "%s%02X", servo_txbuffer, checksum & 0xFF);
	
	ServoUsartDmaSend(servo_txbuffer, GetServoDataLength(servo_txbuffer));
}

//**********************--- Проверка ошибки ---**********************//
uint8_t ServoHandleError(void)
{
	if (servo_rxbuffer[2] != 'A') {
		//gpio_bit_set(GPIOA, GPIO_PIN_7);
	}
	return servo_rxbuffer[2];
}

//***********************--- Получить размер буффера ---*****************************//
uint16_t GetServoDataLength(const char* data)
{
	uint16_t length = 0;
	while (data[length] != '\0') {
		++length;
	}
	return length;
}

//**********************--- Включить таймер ---***********************//
void ServoTimerEnable(void)
{
	timer_interrupt_enable(SERVO_TIMER, TIMER_INT_UP);
}

//*********************--- Выключить таймер ---************************//
void ServoTimerDisable(void)
{
	timer_interrupt_disable(SERVO_TIMER, TIMER_INT_UP);
}

//*********************--- Функция прерывания таймера ---************************//
void ServoTimerInterrupt(void)
{
	static uint16_t test_out_count = 0;
	Servo_motor_parametr* Servo_param = GetServoParam();
	
	if (Servo_param->mode == SERVO_POS_CHK_RD_ON || Servo_param->mode == SERVO_JOG_CHK_RD_ON) {
		StartServoQueue();
	}
	if(Servo_param->speed == 0){
		++test_out_count;
		if(test_out_count > Servo_param->acceleration/1000){
			SetServoTestOff();
		}
	} else {
		test_out_count = 0;
	}
}

//*********************--- Функция прерывания Rx DMA Uart ---************************//
void ServoRxInterrupt(void)
{
		ServoHandleError();
		NextModeServoQueue();
}


//******************************** TIMER HANDLER ***********************************//
/*!
    \brief      this function handles TIMER5 interrupt request.
*/
void SERVO_TIMER_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(SERVO_TIMER, TIMER_INT_UP)){	
		timer_interrupt_flag_clear(SERVO_TIMER, TIMER_INT_UP);
		
		ServoTimerInterrupt();
	}
}

//******************************** DMA RX HANDLER ***********************************//
void SERVO_DMA_Channel_IRQHandler_Rx(void)
{
	if(RESET != dma_interrupt_flag_get(SERVO_DMA, SERVO_DMA_CH_RX, DMA_INT_FLAG_FTF)) {
		dma_interrupt_flag_clear(SERVO_DMA, SERVO_DMA_CH_RX, DMA_INT_FLAG_G);
		
		ServoRxInterrupt();
	}
}
