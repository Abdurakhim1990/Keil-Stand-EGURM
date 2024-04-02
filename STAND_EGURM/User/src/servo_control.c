#include "servo_control.h"

int16_t servo_freq = 0;
uint32_t servo_acceleration_time = 1000;
int32_t  servo_path_length = 0;
uint32_t servo_torque = 0;

uint8_t servo_rd_on = 0;


uint16_t servo_rx_size = 30;
char servo_rxbuffer[30]; 

uint16_t servo_tx_size = 30;
char servo_txbuffer[30];

const uint8_t ascii[16] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46};

//--------------------------------------------------------SERVO FUNCTIONS---------------------------------------------------

//*******************************************************************//
void (*servo_funcs[SERVO_FUNC_CNT]) (void) = {
	[SERVO_SET_TORQUE]		=	ServoSetTorque_m,
	[SERVO_POS_ON]				=	ServoPosModeOn_m,
	[SERVO_JOG_ON]				=	ServoJogModeOn_m,
	[SERVO_OFF]						=	ServoModeOff_m,
	[SERVO_POS_CHK_RD_ON]	=	ServoCheckRdOn,
	[SERVO_JOG_CHK_RD_ON]	=	ServoCheckRdOn
};
uint8_t servo_mode = SERVO_NONE;

//*********************** SERVO CONFIG *********************************//
/*!
\brief      Ограничение крутящего момента в процентах
		\param[in]  
		\param[in]
\arg					0 - (0 - 100)percents
    \param[out] none
		\retval     none
*/
void SetServoTorque(int16_t torque_in_percents)
{
	servo_torque = torque_in_percents;
	ServoSetTorque_m();
}

void ServoSetTorque_m(void)
{
	static uint8_t counter = 0;
	if (counter == 0) {
		servo_mode = SERVO_SET_TORQUE;
		counter = 1;
		ServoSendWriteCommand8(WRITE_PARAMS, 0x1C, (uint32_t)servo_torque, 0);
	} else {
		counter = 0;
		servo_mode = SERVO_NONE;
	}
}


//********************************** POS MODE *********************************//
/*!
\brief      Вход в режим позиционирования и начало вращения
		\param[in]  grad - угол поворота
    \param[out] none
		\retval     none
*/
void SetServoPosition(int16_t grad)
{
	servo_freq = 50;
	servo_acceleration_time = 500;
	if (grad != 0) {
		servo_path_length = (131072 * grad)/360;
		ServoPosModeOn_m();
	} else {
		ServoModeOff_m();
	}
}

void ServoPosModeOn_m(void)
{
	static int8_t counter = -1;
	switch (counter) {
		//************** OFF MODE ON **************//
		case -1:
			ServoTimerDisable();
			if (servo_mode == SERVO_JOG_CHK_RD_ON)
				counter = 0;
			else if (servo_mode == SERVO_POS_CHK_RD_ON)
				counter = 1;
			else
				counter = 5;
			servo_mode = SERVO_POS_ON;
			break;
		case 0:
			counter = 2;
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, JOG_MODE_STOP_ROTATION, 0);
			break;
		case 1:
			counter = 2;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_STOP, TEST_MODE_BREAK_DATA, 0);
			break;
		case 2:
			counter = 3;
			ServoSendWriteCommand4(TEST_MODE, JOG_MODE_STOP, TEST_MODE_BREAK_DATA, 0);
			break;
		case 3:
			counter = 4;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_BREAK, 0);
			break;
		case 4:
			counter = 5;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_UNLOCK, TEST_MODE_BREAK_DATA, 0);
			break;
		
		//************** POS MODE ON **************//
		case 5:
			counter = 6;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_BLOCK, TEST_MODE_BREAK_DATA, 0);
			break;
		case 6:
			counter = 7;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_POSITIONING, 0);
			break;
		case 7:
			counter = 8;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_FREQUENCY, servo_freq, 0);
			break;
		case 8:
			counter = 9;
			ServoSendWriteCommand8(TEST_MODE, POS_MODE_ACCELERATION_TIME, servo_acceleration_time, 0);
			break;
		case 9:
			counter = 10;
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, POS_MODE_SON_LSP_LSN_ON_DATA, 0);
			break;
		//************** POSITIONING ON **************//
		case 10:
			counter = 11;
			ServoSendWriteCommand8(TEST_MODE, POS_MODE_SET_PATH_LENGTH, servo_path_length, 0);
			break;
		case 11:
			counter = -1;
			servo_mode = SERVO_POS_CHK_RD_ON;
			ServoTimerEnable();
			break;
		default:
			break;
	};
}


//********************************** JOG MODE *********************************//
/*!
\brief      Вход в режим JOG и начало вращения
		\param[in]  grad - угол поворота
    \param[out] none
		\retval     none
*/
void SetServoSpeed(int16_t grad)
{
	int16_t koef = 4;
	servo_freq = grad/60;
	if (servo_freq != 0) {
		servo_acceleration_time = servo_freq * koef;
		ServoJogModeOn_m();
	} else {
		ServoModeOff_m();
	}
}

void ServoJogModeOn_m(void) 
{
	static int8_t counter = -1;
	
	switch (counter) {
		//************** OFF MODE ON **************//
		case -1:
			ServoTimerDisable();
			if (servo_mode == SERVO_JOG_CHK_RD_ON)
				counter = 0;
			else if (servo_mode == SERVO_POS_CHK_RD_ON)
				counter = 1;
			else
				counter = 5;
			servo_mode = SERVO_JOG_ON;
			break;
		case 0:
			counter = 2;
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, JOG_MODE_STOP_ROTATION, 0);
			break;
		case 1:
			counter = 2;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_STOP, TEST_MODE_BREAK_DATA, 0);
			break;
		case 2:
			counter = 3;
			ServoSendWriteCommand4(TEST_MODE, JOG_MODE_STOP, TEST_MODE_BREAK_DATA, 0);
			break;
		case 3:
			counter = 4;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_BREAK, 0);
			break;
		case 4:
			counter = 5;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_UNLOCK, TEST_MODE_BREAK_DATA, 0);
			break;
		
		//************** JOG MODE ON **************//
		case 5:
			counter = 6;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_BLOCK, TEST_MODE_BREAK_DATA, 0);
			break;
		case 6:
			counter = 7;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_JOG, 0);
			break;
		//************** ROTATION ON **************//
		case 7:
			counter = 8;
			if (servo_freq > 0)
				ServoSendWriteCommand4(TEST_MODE, POS_MODE_FREQUENCY, servo_freq, 0);
			else
				ServoSendWriteCommand4(TEST_MODE, POS_MODE_FREQUENCY, -servo_freq, 0);
			break;
		case 8:
			counter = 9;
			ServoSendWriteCommand8(TEST_MODE, POS_MODE_ACCELERATION_TIME, servo_acceleration_time, 0);
			break;
		case 9:
			counter = 10;
			if (servo_freq > 0)
				ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, JOG_MODE_DIRECT_ROTATION, 0);
			else
				ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, JOG_MODE_REVERSE_ROTATION, 0);
			break;
		case 10:
			counter = -1;
			servo_mode = SERVO_JOG_CHK_RD_ON;
			ServoTimerEnable();
			break;	
		default:
			break;
	};
}


//***********************************************************************//
/*!
		\brief      Выход из режимов
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoModeOff_m(void)
{
	static uint8_t counter = 0;
	if (servo_mode == SERVO_JOG_CHK_RD_ON)
		counter = 0;
	else if (servo_mode == SERVO_POS_CHK_RD_ON)
		counter = 1;
	
	switch (counter) {
		case 0:
			ServoTimerDisable();
			servo_mode = SERVO_OFF;
			counter = 2;
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, JOG_MODE_STOP_ROTATION, 0);
			break;
		case 1:
			ServoTimerDisable();
			servo_mode = SERVO_OFF;
			counter = 2;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_STOP, TEST_MODE_BREAK_DATA, 0);
			break;
		case 2:
			counter = 3;
			ServoSendWriteCommand4(TEST_MODE, JOG_MODE_STOP, TEST_MODE_BREAK_DATA, 0);
			break;
		case 3:
			counter = 4;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_BREAK, 0);
			break;
		case 4:
			counter = 0;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_UNLOCK, TEST_MODE_BREAK_DATA, 0);
			servo_mode = SERVO_NONE;
			break;
		default:
			break;
	};
}


//******************************** SERVO CHECK RD_ON ***********************************//
/*!
		\brief      Проверка готовности
		\param[in]	none:
		\arg				
    \param[out] none
		\retval     none
*/
void ServoCheckRdOn(void)
{ 
	static uint8_t counter = 0;
	if (counter == 0) {
		counter = 1;
		ServoSendReadCommand(READ_IO_SIGNALS, OUTPUT_SIGNALS, RESPONSE_SIZE_IO_SIGNALS, 0);
	} else {
		counter = 0;
		servo_rd_on = (servo_rxbuffer[10] - '0') & 1;
	}
}


//******************************** TIMER HANDLER ***********************************//
/*!
    \brief      this function handles TIMER5 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SERVO_TIMER_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER8, TIMER_INT_UP)){	
		/* clear channel 0 interrupt bit */
			timer_interrupt_flag_clear(TIMER8, TIMER_INT_UP);
			
			if (servo_mode == SERVO_POS_CHK_RD_ON || servo_mode == SERVO_JOG_CHK_RD_ON) {
				ServoCheckRdOn();		// проверка сигнала RD_ON
		}
	}
}


//******************************** DMA TX HANDLER ***********************************//
/*!
    \brief      this function handles DMA_Channel5_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SERVO_DMA_Channel_IRQHandler_Tx(void)
{
	if(RESET != dma_interrupt_flag_get(SERVO_DMA, SERVO_DMA_CH_TX, DMA_INT_FLAG_FTF)) {
        dma_interrupt_flag_clear(SERVO_DMA, SERVO_DMA_CH_TX, DMA_INT_FLAG_G);
    }
}


//******************************** DMA RX HANDLER ***********************************//
void SERVO_DMA_Channel_IRQHandler_Rx(void)
{
	if(RESET != dma_interrupt_flag_get(SERVO_DMA, SERVO_DMA_CH_RX, DMA_INT_FLAG_FTF)) {
		dma_channel_disable(SERVO_DMA, SERVO_DMA_CH_RX);
		dma_flag_clear(SERVO_DMA, SERVO_DMA_CH_RX, DMA_FLAG_G);				
		dma_transfer_number_config(SERVO_DMA, SERVO_DMA_CH_RX, servo_rx_size);
		dma_channel_enable(SERVO_DMA, SERVO_DMA_CH_RX);
		dma_interrupt_flag_clear(SERVO_DMA, SERVO_DMA_CH_RX, DMA_INT_FLAG_G); 
		ServoHandleError();
		
		if (servo_mode != SERVO_NONE) {
			servo_funcs[servo_mode]();
		}
	}
}


//--------------------------------------------------------SERVO TOOLS---------------------------------------------------
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
		//gpio_bit_set(GPIOA, GPIO_PIN_7);
		// do something
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


