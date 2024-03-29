#ifndef UARTCAN_H
#define UARTCAN_H

#include "uartcan_init.h"
#include "main.h"

typedef enum {
//	UARTCAN_DMA_TX_SIZE			= 260,
//	UARTCAN_DMA_RX_SIZE			= 260,
	UARTCAN_DMA_TX_SIZE			= 35,
	UARTCAN_DMA_RX_SIZE			= 35,
	UARTCAN_HEARDER_SIZE		= 2,
	UARTCAN_PARAMETR_SIZE		= 3,
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
} type_to_can;

typedef enum{
	CAR_SPEED								= 1,
	ENGINE_SPEED						= 2,
	STEERING_WHEEL_ZERO			= 3,
	STEERING_WHEEL_LEFT			= 4,
	STEERING_WHEEL_RIGHT		= 5,
	ANGLE_STEERING_WHEEL		= 6,
	CAN_ERROR								= 7,
} parametrs;

typedef enum{
	CHARAC_TYPE						= 0,
	CHARAC_LEN						= 1,
	CHARAC_DATA						= 2,
} character;


//---------------- Headers from uartcan_tx.c--------------------//
uint8_t* GetUartcanTxBuff(void);
void SetEngineSpeed(int16_t speed);
void SetCarSpeed(int16_t speed);
void UartcanSend(uint8_t* buff, uint8_t len, type_to_can type);
void SetZeroWheel(int16_t param);
void SetLeftWheel(int16_t param);
void SetRightWheel(int16_t param);

//---------------- Headers from uartcan_rx.c--------------------//
int16_t GetAngleEgurm(void);
void SetUartcanType(type_to_can type, uint16_t len, uint8_t *data);
void SetUartcanParametrs(uint8_t *data);
int16_t GetLeftAngle(void);
int16_t GetRightAngle(void);
int16_t GetZeroAngle(void);
int16_t GetCanError(uint8_t counter);



////////*********/////////////**********/////
void SetAngleWheel(int16_t speed);

#endif
