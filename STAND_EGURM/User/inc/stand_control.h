#ifndef STAND_CONTROL_H
#define STAND_CONTROL_H

#include "main.h"
#include "stand_control_init.h"

enum _out_reports
{
	REP_SET_PAR						= 17,
	REP_REQ_PAR						= 18,
	REP_WRT_FIRM					= 19
};

enum _in_reports
{
	REP_RET_PAR						= 21,
	REP_RD_FIRM						= 22
};

enum _length_out_reports
{
	LEN_REP_SET_PAR				= 40,
	LEN_REP_REQ_PAR				= 20,
	LEN_REP_WRT_FIRM			= 80
};

enum _length_in_reports
{
	LEN_REP_RET_PAR				= 60,
	LEN_REP_RD_FIRM				= 80
};

enum _usb_param
{
	PARAM_EMPTY									= 0,		// Пустая функция
	PARAM_VOLT_EGURM						= 1,		// Установка/измерение напряжение питания ЭГУРМ
	PARAM_INGITION							= 2,		// Включение/отключение зажигания
	PARAM_ENGINE_SPEED					= 3,		// Установка оборотов двигателя
	PARAM_CAR_SPEED							= 4,		// Установка скорости автомобиля
	PARAM_TORQUE_IN							= 5,		// Установка/измерение входного момента
	PARAM_TORQUE_OUT						= 6,		// Установка/измерение выходного момента
	PARAM_TEMPERAT							= 7,		// Чтение температуры
	PARAM_CURR_EGURM						= 8,		// Измерение тока потребления ЭГУРМ в Амперах
	PARAM_STEERING_SPEED				= 9,		// Установка оборотов руля
	PARAM_SHAFT_ANGLE_IN				= 10,		// Установка/измерение угла входного вала
	PARAM_SHAFT_ANGLE_OUT				= 11,		// Измерение угла выходного вала
	PARAM_SHAFT_ANGLE_EGURM			= 12,		// Измерение угла вала по датчику ЭГУРМ
	PARAM_VOLT_SENS_MOM_1				= 13,		// Измерение момента датчика ВАЛИО 1
	PARAM_VOLT_SENS_MOM_2				= 14,		// Измерение момента датчика ВАЛИО 2
	PARAM_VOLT_SENS_MOM_3				= 15,		// Измерение момента датчика ВАЛИО 3
	PARAM_VOLT_SENS_MOM_4				= 16,		// Измерение момента датчика ВАЛИО 4
	PARAM_CURR_EGURM_mA					= 17,		// Измерение тока потребления ЭГУРМ в милли Амперах
	PARAM_CURR_INGITION					= 18,		// Измерение тока потребления в цепи зажигания
	PARAM_STAND_ERROR						= 19,		// Чтение ошибок
	PARM_STEERING_WHEEL_ZERO		= 20,		// Установка нулевого положения руля
	PARM_STEERING_WHEEL_LEFT		= 21,		// Установка левого положения руля
	PARM_STEERING_WHEEL_RIGHT		= 22,		// Установка правого положения руля
	PARAM_TEST_TIME							= 38,		// Номер последнего байта пакета USB(1 - идет передача, 0 - стоп передача)
};


uint8_t* ReadBuffUsbSetParam(void);
uint8_t* ReadBuffUsbReqParam(void);
uint8_t* ReadBuffUsbWriteFirmware(void);
void SetStandParameters(void);
void SetEmptyParam(int16_t val);
int16_t RetEmptyParam(void);
void RetStandParameters(void);

#endif