#include "uartcan.h"

char uartcan_rx_buffer[UARTCAN_DMA_RX_SIZE];
uint8_t data_rx[UARTCAN_DMA_RX_SIZE];
uint16_t number_byte = 0;


struct struct_uartcan_rx {
	type_to_can can_type;
	character charac;
	uint16_t len_data;
	uint16_t count_data;
	uint8_t *data;
} uartcan_rx_par = {.charac = 0, .count_data = 0};

struct struct_can_par {
	uint16_t speed_engine;
	uint16_t speed_car;
	uint16_t angle_wheel;
	uint16_t zero_angle;
	uint16_t left_angle;
	uint16_t right_angle;
} val_param_can_tx = {.speed_engine = 0xFFFF, .speed_car = 0xFFFF};

////////////*****************//////////////************////////////
void CanErrorRecieve(uint8_t *data);


//*******************************************************************//
void UartcanDmaRxData(void)
{
	uint16_t uartcan_transf_num = dma_transfer_number_get(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
	if(uartcan_transf_num != UARTCAN_DMA_RX_SIZE){
		uartcan_rx_par.data = data_rx;
		number_byte = UARTCAN_DMA_RX_SIZE - uartcan_transf_num;
		for(uint16_t i = 0; i < number_byte; ++i){
			if(uartcan_rx_par.charac == CHARAC_TYPE){
				uartcan_rx_par.can_type = uartcan_rx_buffer[i];
				uartcan_rx_par.count_data = 0;
				uartcan_rx_par.charac = CHARAC_LEN;
			} else if(uartcan_rx_par.charac == CHARAC_LEN){
				uartcan_rx_par.len_data = uartcan_rx_buffer[i];
				uartcan_rx_par.charac = CHARAC_DATA;
			} else if(uartcan_rx_par.charac == CHARAC_DATA){
				uartcan_rx_par.data[uartcan_rx_par.count_data] = uartcan_rx_buffer[i];
				++uartcan_rx_par.count_data;
				if(uartcan_rx_par.count_data == uartcan_rx_par.len_data){
					SetUartcanType(uartcan_rx_par.can_type, uartcan_rx_par.count_data, uartcan_rx_par.data);
					uartcan_rx_par.charac = CHARAC_TYPE;
				}
			}
		}
		dma_channel_disable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
		dma_memory_address_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, (uint32_t)uartcan_rx_buffer);
		dma_transfer_number_config(UARTCAN_DMA, UARTCAN_DMA_CH_RX, UARTCAN_DMA_RX_SIZE);
		dma_channel_enable(UARTCAN_DMA, UARTCAN_DMA_CH_RX);
	}
}

//*******************************************************************//
void SetUartcanType(type_to_can type, uint16_t len, uint8_t *data)
{
	uint16_t id = (uint16_t)(data[0] << 8);
	id += data[1];
	switch(type){
		case TYPE_PARAMETR:				SetUartcanParametrs(data);										break;
		case TYPE_WRITE_ID:				SendWriteDataByIdentifier(id, &data[2], len);	break;
		case TYPE_READ_ID:				SendReadDataByIdentifier(id);									break;
		case TYPE_WRITE_FIRMWARE:			break;
		default: 	break;
	}
}

#define SIZE_CAN_ERRORS		20
uint8_t can_errors[SIZE_CAN_ERRORS];////////

//*******************************************************************//
void SetUartcanParametrs(uint8_t *data)
{
	uint16_t param = (uint16_t)(data[1] << 8);
	param += data[2];
	
	switch(data[0]){
		case CAR_SPEED:							val_param_can_tx.speed_car = param;																					break;
		case ENGINE_SPEED:					val_param_can_tx.speed_engine = param;																			break;
		case STEERING_WHEEL_ZERO:		SendRoutineControl(START_ROUTINE, SET_STEERING_WHEEL_ZERO, data, 0);				break;
		case STEERING_WHEEL_LEFT:		SendRoutineControl(START_ROUTINE, SET_STEERING_WHEEL_MOST_LEFT, data, 0);		break;
		case STEERING_WHEEL_RIGHT:	SendRoutineControl(START_ROUTINE, SET_STEERING_WHEEL_MOST_RIGHT, data, 0);	break;
		///////*********/////////*********/////////
		case CAN_ERROR:							CanErrorRecieve(&data[1]);			break;
		default: 	break;
	}
}

//*******************************************************************//
void SendEngineSpeedToCan(void)
{
	static uint16_t time = 0;
	if(time == 0){
		if(val_param_can_tx.speed_engine < 0xFFFF){
			CanTransmitTachographSpeed(val_param_can_tx.speed_engine);
			time = 50;
			//time = 500;
			DisplayLed_3();
		}
	} else {
		--time;
	}
}

//*******************************************************************//
void SendCarSpeedToCan(void)
{
	static uint16_t time = 0;
	if(time == 0){
		if(val_param_can_tx.speed_car < 0xFFFF){
			CanTransmitEngineSpeed(val_param_can_tx.speed_car);
			time = 10;
			//time = 100;
			DisplayLed_4();
		}
	} else {
		--time;
	}
}

//*******************************************************************//
void UARTCAN_USART_IRQHandler_Rx(void)
{
	volatile char rx_data;
	if(RESET != usart_interrupt_flag_get(UARTCAN_USART, USART_INT_FLAG_IDLE)){
		rx_data = usart_data_receive(UARTCAN_USART);
		UartcanDmaRxData();
	}
}

//***********************---UARTCAN RS232_Rx---**********************//
void UARTCAN_DMA_Channel_IRQHandler_Rx(void)
{
	if(RESET != dma_interrupt_flag_get(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_INT_FLAG_FTF)){
		dma_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_FLAG_G);
		
		uartcan_rx_par.charac = CHARAC_TYPE;
		UartcanDmaRxData();
		
		dma_interrupt_flag_clear(UARTCAN_DMA, UARTCAN_DMA_CH_RX, DMA_INT_FLAG_G);
	}
}

////////////*****************//////////////************////////////
//*******************************************************************//
void CanErrorRecieve(uint8_t *data)
{
	if(!data[0]){
		for(uint8_t i = 0; i < SIZE_CAN_ERRORS; ++i){
			can_errors[i] = 0;
		}
	}
	can_errors[data[0]] = data[1];
}
