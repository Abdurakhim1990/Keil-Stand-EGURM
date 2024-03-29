#ifndef UARTCAN_H
#define UARTCAN_H

#include "uartcan_init.h"
#include "main.h"

typedef enum {
	UARTCAN_DMA_TX_SIZE			= 260,
	UARTCAN_DMA_RX_SIZE			= 260,
//	UARTCAN_DMA_TX_SIZE			= 35,
//	UARTCAN_DMA_RX_SIZE			= 35,
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

enum _egurm_error{
	ERROR_PARAMETR_CHECKSUM								= 11,		// Ошибка контрольной суммы параметров
	ERROR_CONTROL_AND_COMMUN_CONTROLLER 	= 12,		// Ошибка связи между управляющим и связным контроллером
	ERROR_SELF_TEST_CONTROLLER						= 13,		// Ошибка самотестирования управляющего контроллера
	ERROR_POWER_ELECTRONICS 							= 14,		// Ошибка работы силовой электроники
	ERROR_HEAD														= 15,		// Высокая температура
	ERROR_INPUT_POWER_MONITORING					= 16,		// Ошибка контроля входного питания
	ERROR_SENSOR_TORQUE										= 17,		// Ошибка датчика момента
	ERROR_STEERING_ANGLE_SENS							= 18,		// Ошибка датчика угла рулевого колеса
	ERROR_STEERING_CALIBRATION						= 19,		// Ошибка калибровки рулевого колеса
	ERROR_MOTOR_ROTOR_POSITION						= 20,		// Ошибка датчика положения ротора электродвигателя
	ERROR_EXCEEDING_CURRENT_IN_MOTOR			= 21,		// Превышение допустимого тока в обмотках электродвигателя
	ERROR_BROKEN_MOTOR_WINDING						= 22,		// Обрыв обмотки(-ок) электродвигателя
	ERROR_SHORT_CIRCUIT_WINDING						= 23,		// Короткое замыкание обмотки(-ок) электродвигателя
	ERROR_GENERAL_FAIL										= 24,		// Отказ ЭГУР по общей причине
	ERROR_NO_VEHICLE_SPEED_DATA						= 25,		// Нет данных о скорости автомобиля
	ERROR_NO_ENGINE_SPEED_DATA						= 26,		// Нет данных о частоте вращения двигателя
};


//---------------- Headers from uartcan_tx.c--------------------//
uint8_t* GetUartcanTxBuff(void);
void UartcanSend(uint8_t* buff, uint8_t len, type_to_can type);
void SendAngleEgurm(uint16_t angle);
void SendUartcanError(uint16_t error);
void SendZeroWheel(int16_t status);
void SendLeftWheel(int16_t status);
void SendRightWheel(int16_t status);

//---------------- Headers from uartcan_rx.c--------------------//
int16_t GetAngleEgurm(void);
void SetUartcanType(type_to_can type, uint16_t len, uint8_t *data);
void SetUartcanParametrs(uint8_t *data);
void SendEngineSpeedToCan(void);
void SendCarSpeedToCan(void);



////////*********/////////////**********/////////
void SetZeroWheel(int16_t speed);
void SetLeftWheel(int16_t speed);
void SetRightWheel(int16_t speed);
void SetAngleWheel(int16_t speed);
void SetEngineSpeed(int16_t speed);
void SetCarSpeed(int16_t speed);


#endif
