#ifndef STAND_ERROR_H
#define STAND_ERROR_H

#include "main.h"

enum _egurm_error{
	ERROR_PARAMETR_CHECKSUM = 1,							// Ошибка контрольной суммы параметров
	ERROR_CONTROL_AND_COMMUN_CONTROLLER,			// Ошибка связи между управляющим и связным контроллером
	ERROR_SELF_TEST_CONTROLLER,								// Ошибка самотестирования управляющего контроллера
	ERROR_POWER_ELECTRONICS,									// Ошибка работы силовой электроники
	ERROR_HEAD,																// Высокая температура
	ERROR_INPUT_POWER_MONITORING,							// Ошибка контроля входного питания
	ERROR_SENSOR_TORQUE,											// Ошибка датчика момента
	ERROR_STEERING_ANGLE_SENS,								// Ошибка датчика угла рулевого колеса
	ERROR_STEERING_CALIBRATION,								// Ошибка калибровки рулевого колеса
	ERROR_MOTOR_ROTOR_POSITION,								// Ошибка датчика положения ротора электродвигателя
	ERROR_EXCEEDING_CURRENT_IN_MOTOR,					// Превышение допустимого тока в обмотках электродвигателя
	ERROR_BROKEN_MOTOR_WINDING,								// Обрыв обмотки(-ок) электродвигателя
	ERROR_SHORT_CIRCUIT_WINDING,							// Короткое замыкание обмотки(-ок) электродвигателя
	ERROR_GENERA_FAIL,												// Отказ ЭГУР по общей причине
};

enum _stand_error{
	ERROR_EGURM = 1,													// Отказ ЭГУР по общей причине
	ERROR_SERVO_MOTOR,												// Сервомотор не готов работать
	ERROR_AKIP,																// Ошибка в источнике питания АКИП
};

uint8_t StatusErrorStand(void);
int16_t GetErrorStand(void);

#endif
