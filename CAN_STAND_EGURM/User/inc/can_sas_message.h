#ifndef CAN_SAS_MESSAGE_H
#define CAN_SAS_MESSAGE_H

#include "main.h"

typedef struct{
	// Byte -- 1-2
	int16_t steering_wheel_angle;											// start - 1.1, length = 16, -31.374/31.374.
																											// Угол поворота руля (рад.) (влево "-"/ вправо "+").
	// Byte -- 3
	int8_t steering_wheel_angle_range_counter;					// start - 3.1, length = 6, -32/29.
																											// Счетчик полных оборотов руля (влево "-"/ вправо "+").
	uint8_t steering_wheel_angle_range_counter_type;		// start - 3.7, length = 2, 0/3.
																											// Тип счетчика: 0-относит., 1-обсолют.
	// Byte -- 4
	// Byte -- 5-6
	int16_t steering_wheel_angle_range;								// start - 5.1, length = 16, -31.374/31.374. 
																											// Угол поворота руля (рад.) врамках текущего оборота
	// Byte -- 7
	uint8_t steering_angle_sensor_active_mode;					// start - 7.1, length = 2, 0/3.
	uint8_t steering_angle_sensor_calibrated;						// start - 7.3, length = 2, 0/3.
	// Byte -- 8
	uint8_t message_counter;														// start - 8.1, length = 4, 0/15.
	uint8_t message_checksum;														// start - 8.5, length = 4, 0/15.
} SasCsMessageStruc;



void CanSasCsMessage(uint8_t* recieve_data);

#endif
