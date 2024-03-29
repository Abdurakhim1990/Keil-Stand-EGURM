#include "stand_error.h"

uint8_t stand_error = 0;

//************************************************//
uint8_t StatusErrorStand(void)
{
	return stand_error;
}

//************************************************//
int16_t GetErrorStand(void)
{
	uint8_t error = 0;
	if(stand_error & (1 << ERROR_EGURM)){
		volatile uint8_t counter = 0;
		error = GetCanError(counter++);
		if(!GetCanError(counter)){
			stand_error &= ~(1 << ERROR_EGURM);
			counter = 0;
		}
	} else if(stand_error & (1 << ERROR_SERVO_MOTOR)){
		error = ERROR_SERVO_MOTOR;
		stand_error &= ~(1 << ERROR_SERVO_MOTOR);
	} else if(stand_error & (1 << ERROR_AKIP)){
		error = ERROR_AKIP;
		stand_error &= ~(1 << ERROR_AKIP);
	}
	return error;
}

//************************************************//
void SetCanError(uint8_t error)
{
	stand_error |= (error << ERROR_EGURM);
}

//************************************************//
void SetServoMotorError(uint8_t error)
{
	stand_error |= (error << ERROR_SERVO_MOTOR);
}

//************************************************//
void SetAkipError(uint8_t error)
{
	stand_error |= (error << ERROR_AKIP);
}
