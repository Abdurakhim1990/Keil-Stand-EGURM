#include "uartcan.h"

char uartcan_tx_buffer[UARTCAN_DMA_TX_SIZE];


static void UartcanSendPacket(void);


//******************************************************//
uint8_t* GetUartcanTxBuff(void)
{
	return (uint8_t*)uartcan_tx_buffer;
}

uint16_t pos_rx_buff = 0;

//**************************************************//
void UartcanSend(uint8_t* buff, uint8_t len, type_to_can type)
{
	uartcan_tx_buffer[pos_rx_buff + SER_NUM_HEADER] = type;
	uartcan_tx_buffer[pos_rx_buff + SER_NUM_LEN] = len;
	for(uint8_t i = 0; i < len; ++i){
		uartcan_tx_buffer[pos_rx_buff + i + SER_NUM_LEN + 1] = buff[i];
	}
	UartcanSendPacket();
}

//******************************************************//
void SendAngleEgurm(uint16_t angle)
{
	uint8_t data[3];
	data[0] = ANGLE_STEERING_WHEEL;
	data[1] = (uint8_t)(angle >> 8);
	data[2] = (uint8_t)angle;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}

//******************************************************//
void SendUartcanError(uint16_t error)
{
	uint8_t data[3];
	data[0] = CAN_ERROR;
	data[1] = (uint8_t)(error >> 8);
	data[2] = (uint8_t)error;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
	__ASM("NOP");
}
//*******************************************************************//
void SendZeroWheel(int16_t status)
{
	uint8_t data[3];
	data[0] = STEERING_WHEEL_ZERO;
	data[1] = (uint8_t)(status >> 8);
	data[2] = (uint8_t)status;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}

//*******************************************************************//
void SendLeftWheel(int16_t status)
{
	uint8_t data[3];
	data[0] = STEERING_WHEEL_LEFT;
	data[1] = (uint8_t)(status >> 8);
	data[2] = (uint8_t)status;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}

//*******************************************************************//
void SendRightWheel(int16_t status)
{
	uint8_t data[3];
	data[0] = STEERING_WHEEL_RIGHT;
	data[1] = (uint8_t)(status >> 8);
	data[2] = (uint8_t)status;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}

//******************************************************//
static void UartcanSendPacket(void)
{
	uint8_t length = uartcan_tx_buffer[pos_rx_buff + SER_NUM_LEN] + UARTCAN_HEARDER_SIZE;
	volatile uint16_t uartcan_transf_num = dma_transfer_number_get(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	
	dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	//dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_TX, (uint32_t)uartcan_tx_buffer);
	dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_TX, length + uartcan_transf_num);
	dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
	
	/* USART DMA enable for transmission and reception */
	usart_dma_transmit_config(UARTCAN_USART, USART_TRANSMIT_DMA_ENABLE);
	pos_rx_buff += length;
	uint8_t count_nill_buf = 0;
	for(uint16_t i = pos_rx_buff; i < UARTCAN_DMA_TX_SIZE; ++i){
		if(uartcan_tx_buffer[i]){
			uartcan_tx_buffer[i] = 0;
			count_nill_buf = 0;
		} else {
			++count_nill_buf;
			if(count_nill_buf > 5) break;
		}
	}
}

//***********************---UARTCAN RS232_Tx---**********************//
void UARTCAN_DMA_Channel_IRQHandler_Tx(void)
{
	if(RESET != dma_interrupt_flag_get(UARTCAN_DMA, UARTCAN_DMA_CH_TX, DMA_INT_FLAG_FTF)){
		dma_interrupt_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_TX, DMA_INT_FLAG_G);
		
		dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
		dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_TX, (uint32_t)uartcan_tx_buffer);
		dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_TX);
		
		pos_rx_buff = 0;
		//TestSendUartCan();
	}
}


////////********///////////***********/////////**********/////////
//*******************************************************************//
void SetZeroWheel(int16_t speed)
{
	uint8_t data[3];
	data[0] = STEERING_WHEEL_ZERO;
	data[1] = (uint8_t)(speed >> 8);
	data[2] = (uint8_t)speed;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}

//*******************************************************************//
void SetLeftWheel(int16_t speed)
{
	uint8_t data[3];
	data[0] = STEERING_WHEEL_LEFT;
	data[1] = (uint8_t)(speed >> 8);
	data[2] = (uint8_t)speed;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}

//*******************************************************************//
void SetRightWheel(int16_t speed)
{
	uint8_t data[3];
	data[0] = STEERING_WHEEL_RIGHT;
	data[1] = (uint8_t)(speed >> 8);
	data[2] = (uint8_t)speed;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}

//*******************************************************************//
void SetAngleWheel(int16_t speed)
{
	uint8_t data[3];
	data[0] = ANGLE_STEERING_WHEEL;
	data[1] = (uint8_t)(speed >> 8);
	data[2] = (uint8_t)speed;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}

//*******************************************************************//
void SetEngineSpeed(int16_t speed)
{
	uint8_t data[3];
	data[0] = ENGINE_SPEED;
	data[1] = (uint8_t)(speed >> 8);
	data[2] = (uint8_t)speed;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}

//*******************************************************************//
void SetCarSpeed(int16_t speed)
{
	uint8_t data[3];
	data[0] = CAR_SPEED;
	data[1] = (uint8_t)(speed >> 8);
	data[2] = (uint8_t)speed;
	UartcanSend(data, UARTCAN_PARAMETR_SIZE, TYPE_PARAMETR);
}
