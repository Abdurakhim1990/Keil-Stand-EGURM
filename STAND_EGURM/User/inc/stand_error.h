#ifndef STAND_ERROR_H
#define STAND_ERROR_H

#include "main.h"

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

enum _stand_error{
	ERROR_EGURM					= 1,		// Отказ ЭГУР по общей причине
	ERROR_SERVO_MOTOR		= 2,		// Сервомотор не готов работать
	ERROR_AKIP					= 3,		// Ошибка в источнике питания АКИП
};

uint8_t StatusErrorStand(void);
int16_t GetErrorStand(void);
void SetCanError(uint8_t error);
void SetServoMotorError(uint8_t error);
void SetAkipError(uint8_t error);

#endif
