#include "servo_mr_j2s_a.h"

servo_func_mode servo_mode = nothing_mode;
servo_jog_functions jog_func = jog_on;
servo_pos_functions pos_func = pos_on;

uint8_t servo_jog_functions_cnt[5] = {0, 0, 0, 0, 0};
uint8_t servo_pos_functions_cnt[5] = {0, 0, 0, 0, 0};

uint16_t servo_rx_size = 50;
char servo_rxbuffer[50];

uint16_t servo_tx_size = 50;
char servo_txbuffer[50];

servo_state servo_status = ready;

uint16_t servo_freq = 100;
uint32_t servo_acceleration_time = 1000;
int32_t pos_mode_path_length = 1000;

uint8_t servo_is_ready_cnt = 0;
uint8_t servo_rd_on = 0;

const uint8_t ascii[16] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46};




//*******************************************************************//
uint8_t* GetServoTxBuff(void)
{
	return (uint8_t*)servo_txbuffer;
}

//*******************************************************************//
void ServoUsartDmaRxData(uint16_t response_size)
{
    dma_channel_disable(SERVO_DMA, SERVO_DMA_CH_RX);
    dma_flag_clear(SERVO_DMA, SERVO_DMA_CH_RX, DMA_FLAG_G);
    
    dma_transfer_number_config(SERVO_DMA, SERVO_DMA_CH_RX, response_size);
    dma_channel_enable(SERVO_DMA, SERVO_DMA_CH_RX);
}

//*******************************************************************//
void ServoUsartDmaSend(char* buffer, uint16_t len)
{
    dma_channel_disable(SERVO_DMA, SERVO_DMA_CH_TX);
    
    dma_memory_address_config(SERVO_DMA, SERVO_DMA_CH_TX, (uint32_t)buffer);
    dma_transfer_number_config(SERVO_DMA, SERVO_DMA_CH_TX, len);
    
    /* enable DMA channel1 */
    dma_channel_enable(SERVO_DMA, SERVO_DMA_CH_TX);
    /* USART DMA enable for transmission and reception */
    usart_dma_transmit_config(USART1, USART_TRANSMIT_DMA_ENABLE);
}

//*******************************************************************//
void ServoTimerEnable(void)
{
	timer_interrupt_enable(SERVO_TIMER, TIMER_INT_UP);
}

//*******************************************************************//
void ServoTimerDisable(void)
{
	timer_interrupt_disable(SERVO_TIMER, TIMER_INT_UP);
}

//*******************************************************************//
void ServoSendReadCommand(uint16_t command, uint16_t data, uint16_t response_size, uint8_t servo_number)
{
	ServoUsartDmaRxData(response_size);

	uint16_t sum = ascii[servo_number] + ascii[(command >> 4)] + ascii[(command & 0x0f)] + STX 
	             + ascii[(data >> 4)] + ascii[(data & 0x0f)] + ETX;
	
  sprintf(servo_txbuffer, "%c%c%X%02X%c%02X%c%02X", EOT, SOH, servo_number, command, STX, data, ETX, sum & 0xff);
	ServoUsartDmaSend(servo_txbuffer, COMMAND_SIZE + 1);
}

//*******************************************************************//
void ServoSendWriteCommand4(uint16_t write_command, uint16_t data_number, uint16_t data_to_write, uint8_t servo_number)
{
	ServoUsartDmaRxData(WRITE_COMMAND_RESPONSE_SIZE);
	
	uint16_t sum = 0;
	for (uint16_t i = 0; i < 4; i++) {
		sum += ascii[(uint8_t)((data_to_write >> i * 4) & 0x0F)];
	}
	sum += ascii[servo_number] + ascii[(write_command >> 4)] + ascii[(write_command & 0x0F)]
				+ ascii[(data_number >> 4)] + ascii[(data_number & 0x0f)] + STX + ETX;
	
	sprintf(servo_txbuffer, "%c%c%X%02X%c%02X%04X%c%02X", EOT, SOH, servo_number, write_command, STX, data_number, data_to_write, ETX, sum & 0xff);
	
	ServoUsartDmaSend(servo_txbuffer, COMMAND_SIZE + 4 + 1);
}

//*******************************************************************//
void ServoSendWriteCommand8(uint16_t write_command, uint16_t data_number, uint32_t data_to_write, uint8_t servo_number)
{
	ServoUsartDmaRxData(WRITE_COMMAND_RESPONSE_SIZE);
	
	volatile uint16_t sum = 0;
	for (uint16_t i = 0; i < 8; i++) {
		sum += ascii[(uint8_t)((data_to_write >> i * 4) & 0x0F)];
	}
	sum += ascii[servo_number] + ascii[(write_command >> 4)] + ascii[(write_command & 0x0F)]
				+ ascii[(data_number >> 4)] + ascii[(data_number & 0x0f)] + STX + ETX;
	
	sprintf(servo_txbuffer, "%c%c%X%02X%c%02X%08X%c%02X", EOT, SOH, servo_number, write_command, STX, data_number, data_to_write, ETX, sum & 0xff);
	
	ServoUsartDmaSend(servo_txbuffer, COMMAND_SIZE + 8 + 1);
}

//******************** Остановка мотора*******************************//
void ServoEmgStop(void)
{
	switch (servo_status) {
		case jog:
			ServoTimerDisable();
			ServoJogModeOff();
			break;
		case positioning:
			ServoPositioningModeOff();
			break;
		case ready:
			break;
	}
}


//*******************************************************************//
/*!
    \brief      Частота вращения
		\param[in]  can_parameter: pointer to a can_parameter_struct structure
		\param[in]	freq:
		\arg					0000 - 7FFF (0 - 32767)rot/m
    \param[out] none
		\retval     none
*/
void ServoSetFreq(uint16_t freq)
{
	servo_freq = freq;
}

//*******************************************************************//
/*!
		\brief      Врема разгона
		\param[in]	acceleration_time:
		\arg					00000000 - 7FFFFFFF (0 - 2147483647)ms
    \param[out] none
		\retval     none
*/
void ServoSetAccelerationTime(uint32_t acceleration_time)
{
	servo_acceleration_time = acceleration_time;
}

//*********************** *********************************//
/*!
		\brief      Путь прохождения в импульсах
		\param[in]  can_parameter: pointer to a can_parameter_struct structure
		\param[in]	path:
		\arg					FF676981 - 98967F (-9999999 - 9999999)pulse
								1 об = 131072 имп
    \param[out] none
		\retval     none
*/
void ServoSetPathLength(int32_t path)
{
	pos_mode_path_length = path;
}

//***************************************************************//
/*!
\brief      Включает режим позиционирования
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoPositioningModeOn(void)
{
	switch (servo_pos_functions_cnt[POS_ON]) {
		case 0:
			servo_status = positioning;
			servo_mode = pos_mode;
			pos_func = pos_on;
			servo_pos_functions_cnt[POS_ON] = 1;
			ServoSendWriteCommand8(WRITE_PARAMS, 0x00, 0x30000000, 0);
			break;
		case 1:
			servo_pos_functions_cnt[POS_ON] = 2;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_BLOCK, TEST_MODE_BREAK_DATA, 0);
			break;
		case 2:
			servo_pos_functions_cnt[POS_ON] = 3;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_POSITIONING, 0);
		case 3:
			servo_pos_functions_cnt[POS_ON] = 0;
			servo_mode = nothing_mode;
			break;
		default:
			break;
	};
}

//*******************************************************************//
/*!
\brief      Отключает режим позиционирования
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoPositioningModeOff(void)
{
	switch (servo_pos_functions_cnt[POS_OFF]) {
		case 0:
			servo_mode = pos_mode;
			pos_func = pos_off;
			servo_pos_functions_cnt[POS_OFF] = 1;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_STOP, TEST_MODE_BREAK_DATA, 0);
			break;
		case 1:
			servo_pos_functions_cnt[POS_OFF] = 2;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_STOP, TEST_MODE_BREAK_DATA, 0);
			break;
		case 2:
			servo_pos_functions_cnt[POS_OFF] = 3;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_BREAK, 0);
			break;
		case 3:
			servo_pos_functions_cnt[POS_OFF] = 0;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_UNLOCK, TEST_MODE_BREAK_DATA, 0);
			servo_mode = nothing_mode;
			servo_status = ready;
			break;
		default:
			break;
	};
}

//*******************************************************************//
/*!
\brief      Настройка режима позиционирования (частота вращения, время разгона)
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoPositioningModeConfig(void)
{
	switch (servo_pos_functions_cnt[POS_CONFIG]) {
		case 0:
			servo_mode = pos_mode;
			pos_func = pos_config;
			servo_pos_functions_cnt[POS_CONFIG] = 1;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_FREQUENCY, servo_freq, 0);
			break;
		case 1:
			servo_pos_functions_cnt[POS_CONFIG] = 2;
			ServoSendWriteCommand8(TEST_MODE, POS_MODE_ACCELERATION_TIME, servo_acceleration_time, 0);
			break;
		case 2:
			servo_pos_functions_cnt[POS_CONFIG] = 0;
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, POS_MODE_SON_LSP_LSN_ON_DATA, 0);
			servo_mode = nothing_mode;
			break;
		default:
			break;
	}
}

//*******************************************************************//
/*!
\brief      Запуск позиционирования
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoPositioningModePathLength(void)
{
	if (servo_pos_functions_cnt[POS_PATH_LENGTH] == 0) {
		servo_mode = pos_mode;
		pos_func = pos_path_length;
		servo_pos_functions_cnt[POS_PATH_LENGTH] = 1;
		ServoSendWriteCommand8(TEST_MODE, POS_MODE_SET_PATH_LENGTH, pos_mode_path_length, 0);
	} else if (servo_pos_functions_cnt[POS_PATH_LENGTH] == 1) {
		servo_pos_functions_cnt[POS_PATH_LENGTH] = 0;
		servo_mode = nothing_mode;
	}
}

//*******************************************************************//
/*!
\brief      Остановка позиционирования
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoPositioningModeBreak(void)
{
	if (servo_pos_functions_cnt[POS_BREAK] == 0) {
		servo_mode = pos_mode;
		pos_func = pos_break;
		servo_pos_functions_cnt[POS_BREAK] = 1;
		ServoSendWriteCommand4(TEST_MODE, POS_MODE_BREAK, TEST_MODE_BREAK_DATA, 0);
	} else if (servo_pos_functions_cnt[POS_BREAK] == 1) {
		servo_pos_functions_cnt[POS_BREAK] = 0;
		servo_mode = nothing_mode;
	}
}

//**********************************JOG mode*********************************//
/*!
\brief      Ввод в режим JOG (режим вращения)
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoJogModeOn(void)
{
	switch (servo_jog_functions_cnt[JOG_ON]) {
		case 0:
			servo_status = jog;
			servo_mode = jog_mode;
			jog_func = jog_on;
			servo_jog_functions_cnt[JOG_ON] = 1;
			ServoSendWriteCommand8(WRITE_PARAMS, 0x00, 0x30000002, 0);
			break;
		case 1:
			servo_jog_functions_cnt[JOG_ON] = 2;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_BLOCK, TEST_MODE_BREAK_DATA, 0);
			break;
		case 2:
			servo_jog_functions_cnt[JOG_ON] = 3;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_JOG, 0);
			break;
		case 3:
			servo_jog_functions_cnt[JOG_ON] = 0;
			servo_mode = timer_mode;
			ServoTimerEnable();
			break;	
		default:
			break;
	};
}

//***********************************************************************//
/*!
		\brief      Выход из режима JOG
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoJogModeOff(void)
{
	switch (servo_jog_functions_cnt[JOG_OFF]) {
		case 0:
			ServoTimerDisable();
			servo_mode = jog_mode;
			jog_func = jog_off;
			servo_jog_functions_cnt[JOG_OFF] = 1;
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, JOG_MODE_STOP_ROTATION, 0);
			break;
		case 1:
			servo_jog_functions_cnt[JOG_OFF] = 2;
			ServoSendWriteCommand4(TEST_MODE, JOG_MODE_STOP, TEST_MODE_BREAK_DATA, 0);
			break;
		case 2:
			servo_jog_functions_cnt[JOG_OFF] = 3;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_BREAK, 0);
			break;
		case 3:
			servo_jog_functions_cnt[JOG_OFF] = 0;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_UNLOCK, TEST_MODE_BREAK_DATA, 0);
			servo_mode = nothing_mode;
			servo_status = ready;
			break;
		default:
			break;
	};
}

//**************************************************************************//
/*!
		\brief      Установка частоты вращения, времени разгона/торможения, включение прямого вращения
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoJogModeDirectRotation(void) 
{
	switch (servo_jog_functions_cnt[JOG_DIRECT_ROT]) {
		case 0:
			servo_mode = jog_mode;
			jog_func = jog_direct_rotation;
			servo_jog_functions_cnt[JOG_DIRECT_ROT] = 1;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_FREQUENCY, servo_freq, 0);
			break;
		case 1:
			servo_jog_functions_cnt[JOG_DIRECT_ROT] = 2;
			ServoSendWriteCommand8(TEST_MODE, POS_MODE_ACCELERATION_TIME, servo_acceleration_time, 0);
			break;
		case 2:
			servo_jog_functions_cnt[JOG_DIRECT_ROT] = 0;
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, JOG_MODE_DIRECT_ROTATION, 0);
			servo_mode = timer_mode;
			break;
		default:
			break;
	};
}

//************************************************************************//
/*!
		\brief      Установка частоты вращения, времени разгона/торможения, включение обратного вращения
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoJogModeReversRotation(void)
{
	switch (servo_jog_functions_cnt[JOG_REVERSE_ROT]) {
		case 0:
			servo_mode = jog_mode;
			jog_func = jog_reverse_rotation;
			servo_jog_functions_cnt[JOG_REVERSE_ROT] = 1;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_FREQUENCY, servo_freq, 0);
			break;
		case 1:
			servo_jog_functions_cnt[JOG_REVERSE_ROT] = 2;
			ServoSendWriteCommand8(TEST_MODE, POS_MODE_ACCELERATION_TIME, servo_acceleration_time, 0);
			break;
		case 2:
			servo_jog_functions_cnt[JOG_REVERSE_ROT] = 0;
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, JOG_MODE_REVERSE_ROTATION, 0);
			servo_mode = timer_mode;
			break;
		default:
			break;
	};
}

//****************************************************//
/*!
		\brief      Остановка двигателя в режиме JOG
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoJogModeStopRotation(void)
{
	if (servo_jog_functions_cnt[JOG_STOP] == 0) {
		servo_mode = jog_mode;
		jog_func = jog_stop;
		servo_jog_functions_cnt[JOG_STOP] = 1;
		ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, JOG_MODE_STOP_ROTATION, 0);
	} else if (servo_jog_functions_cnt[JOG_STOP] == 1) {
		servo_jog_functions_cnt[JOG_STOP] = 0;
		servo_mode = timer_mode;
	}
}

//*******************************************************************//
/*!
\brief      Проверка ошибки
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
uint8_t ServoHandleError(void)
{
	if (servo_rxbuffer[2] != 'A') {
		gpio_bit_set(GPIOA, GPIO_PIN_7);
		ServoEmgStop();
	}
	return servo_rxbuffer[2];
}

//*******************************************************************//
/*!
		\brief      Получить размер буффера
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
uint16_t GetServoDataLength(const char* data)
{
	uint16_t length = 0;
	while (data[length] != '\0') {
		++length;
	}
	return length;
}

//*******************************************************************//
/*!
		\brief      Проверка готовности
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoCheckRdOn(void)
{ 
	if (servo_is_ready_cnt == 0) {
		servo_mode = is_ready;
		servo_is_ready_cnt = 1;
		ServoSendReadCommand(READ_IO_SIGNALS, OUTPUT_SIGNALS, RESPONSE_SIZE_IO_SIGNALS, 0);
	} else {
		servo_is_ready_cnt = 0;
	if ((servo_rxbuffer[10] - '0') & 1)
		servo_rd_on = 1;
	else
		servo_rd_on = 0;
		servo_mode = nothing_mode;
	}
}

/*!
    \brief      this function handles TIMER5 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER0_BRK_TIMER8_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER8, TIMER_INT_UP)){
			/* clear channel 0 interrupt bit */
			timer_interrupt_flag_clear(TIMER8, TIMER_INT_UP);
			
			if (servo_mode == timer_mode) {
			//gpio_bit_set(GPIOA, GPIO_PIN_8);
			if (servo_status == jog)
				ServoSendReadCommand(READ_STATE, DATA_FEEDBACK_IMPULSES, RESPONSE_SIZE_STATE, 0);
			else if (servo_status == positioning)
				ServoSendReadCommand(READ_STATE, DATA_FEEDBACK_IMPULSES, RESPONSE_SIZE_STATE, 0);
		}
	}
}
	
/*!
    \brief      this function handles DMA_Channel5_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SERVO_DMA_Channel_IRQHandler_Rx(void)
{
	if(RESET != dma_interrupt_flag_get(SERVO_DMA, SERVO_DMA_CH_RX, DMA_INT_FLAG_FTF)) {
		dma_channel_disable(SERVO_DMA, SERVO_DMA_CH_RX);
		dma_flag_clear(SERVO_DMA, SERVO_DMA_CH_RX, DMA_FLAG_G);				
		dma_transfer_number_config(SERVO_DMA, SERVO_DMA_CH_RX, servo_rx_size);
		dma_channel_enable(SERVO_DMA, SERVO_DMA_CH_RX);
		dma_interrupt_flag_clear(SERVO_DMA, SERVO_DMA_CH_RX, DMA_INT_FLAG_G); 
		
		ServoHandleError();
		
		switch (servo_mode) {
			case jog_mode:
				switch (jog_func) {
					case jog_on:
						if (servo_jog_functions_cnt[JOG_ON] != 0) {
							ServoJogModeOn();
						}
						break;
					case jog_off:
						if (servo_jog_functions_cnt[JOG_OFF] != 0) {
							ServoJogModeOff();
						}
						break;
					case jog_direct_rotation:
						if (servo_jog_functions_cnt[JOG_DIRECT_ROT] != 0) {
							ServoJogModeDirectRotation();
						}
						break;
					case jog_reverse_rotation:
						if (servo_jog_functions_cnt[JOG_REVERSE_ROT] != 0) {
							ServoJogModeReversRotation();
						}
						break;
					case jog_stop:
						if (servo_jog_functions_cnt[JOG_STOP] != 0) {
							ServoJogModeStopRotation();
						}
						break;
				}
				break;
			case pos_mode:
				switch (pos_func) {
					case pos_on:
						if (servo_pos_functions_cnt[POS_ON] != 0) {
							ServoPositioningModeOn();
						}
						break;
					case pos_off:
						if (servo_pos_functions_cnt[POS_OFF] != 0) {
							ServoPositioningModeOff();
						}
						break;
					case pos_config:
						if (servo_pos_functions_cnt[POS_CONFIG] != 0) {
							ServoPositioningModeConfig();
						}
						break;
					case pos_path_length:
						if (servo_pos_functions_cnt[POS_PATH_LENGTH] != 0) {
							ServoPositioningModePathLength();
						}
						break;
					case pos_break:
						if (servo_pos_functions_cnt[POS_BREAK] != 0) {
							ServoPositioningModeBreak();
						}
						break;
				}
				break;
			case timer_mode:
				//gpio_bit_reset(GPIOA, GPIO_PIN_8);
				break;
			case nothing_mode:
				break;
			default:
				break;
		}
	}
}
