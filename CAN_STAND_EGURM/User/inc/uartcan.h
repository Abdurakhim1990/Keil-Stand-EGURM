#ifndef UARTCAN_H
#define UARTCAN_H

#include "uartcan_init.h"
#include "main.h"

typedef enum {
	UARTCAN_DMA_TX_SIZE			= 260,
	UARTCAN_DMA_RX_SIZE			= 260,
	UARTCAN_HEARDER_SIZE		= 2,
} uartcan_size;

typedef enum {
	SER_NUM_HEADER					= 0,
	SER_NUM_LEN							= 1,
} serial_number;

typedef enum{
	TYPE_PARAMETR					= 1,
	TYPE_WRITE_ID					= 2,
	TYPE_READ_ID					= 3,
	TYPE_WRITE_FIRMWARE		= 4,
	TYPE_ADD_OTHER				= 0xF0,
} type_to_can;

typedef enum{
	CAR_SPEED								= 1,
	ENGINE_SPEED						= 2,
	STEERING_WHEEL_ZERO			= 3,
	STEERING_WHEEL_LEFT			= 4,
	STEERING_WHEEL_RIGHT		= 5,
	ANGLE_STEERING_WHEEL		= 6,
} parametrs;


uint8_t* GetUartcanTxBuff(void);
uint8_t GetCommandLength(char* str);

#endif
