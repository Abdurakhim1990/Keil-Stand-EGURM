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
		error = ERROR_EGURM;
		stand_error &= (1 << ERROR_EGURM);
	} else if(stand_error & (1 << ERROR_SERVO_MOTOR)){
		error = ERROR_SERVO_MOTOR;
		stand_error &= (1 << ERROR_SERVO_MOTOR);
	} else if(stand_error & (1 << ERROR_AKIP)){
		error = ERROR_AKIP;
		stand_error &= (1 << ERROR_AKIP);
	}
	return error;
}

//	if(stand_error & (1 << ERROR_PARAMETR_CHECKSUM)){
//		error = ERROR_PARAMETR_CHECKSUM;
//		stand_error &= (1 << ERROR_PARAMETR_CHECKSUM);
//	} else if(stand_error & (1 << ERROR_CONTROL_AND_COMMUN_CONTROLLER)){
//		error = ERROR_CONTROL_AND_COMMUN_CONTROLLER;
//		stand_error &= (1 << ERROR_CONTROL_AND_COMMUN_CONTROLLER);
//	} else if(stand_error & (1 << ERROR_SELF_TEST_CONTROLLER)){
//		error = ERROR_SELF_TEST_CONTROLLER;
//		stand_error &= (1 << ERROR_SELF_TEST_CONTROLLER);
//	} else if(stand_error & (1 << ERROR_POWER_ELECTRONICS)){
//		error = ERROR_POWER_ELECTRONICS;
//		stand_error &= (1 << ERROR_POWER_ELECTRONICS);
//	} else if(stand_error & (1 << ERROR_HEAD)){
//		error = ERROR_HEAD;
//		stand_error &= (1 << ERROR_HEAD);
//	} else if(stand_error & (1 << ERROR_INPUT_POWER_MONITORING)){
//		error = ERROR_INPUT_POWER_MONITORING;
//		stand_error &= (1 << ERROR_INPUT_POWER_MONITORING);
//	} else if(stand_error & (1 << ERROR_SENSOR_TORQUE)){
//		error = ERROR_SENSOR_TORQUE;
//		stand_error &= (1 << ERROR_SENSOR_TORQUE);
//	} else if(stand_error & (1 << ERROR_STEERING_ANGLE_SENS)){
//		error = ERROR_STEERING_ANGLE_SENS;
//		stand_error &= (1 << ERROR_STEERING_ANGLE_SENS);
//	} else if(stand_error & (1 << ERROR_STEERING_CALIBRATION)){
//		error = ERROR_STEERING_CALIBRATION;
//		stand_error &= (1 << ERROR_STEERING_CALIBRATION);
//	} else if(stand_error & (1 << ERROR_MOTOR_ROTOR_POSITION)){
//		error = ERROR_MOTOR_ROTOR_POSITION;
//		stand_error &= (1 << ERROR_MOTOR_ROTOR_POSITION);
//	} else if(stand_error & (1 << ERROR_EXCEEDING_CURRENT_IN_MOTOR)){
//		error = ERROR_EXCEEDING_CURRENT_IN_MOTOR;
//		stand_error &= (1 << ERROR_EXCEEDING_CURRENT_IN_MOTOR);
//	} else if(stand_error & (1 << ERROR_BROKEN_MOTOR_WINDING)){
//		error = ERROR_BROKEN_MOTOR_WINDING;
//		stand_error &= (1 << ERROR_BROKEN_MOTOR_WINDING);
//	} else if(stand_error & (1 << ERROR_SHORT_CIRCUIT_WINDING)){
//		error = ERROR_SHORT_CIRCUIT_WINDING;
//		stand_error &= (1 << ERROR_SHORT_CIRCUIT_WINDING);
//	} else if(stand_error & (1 << ERROR_EGURM)){
//		error = ERROR_EGURM;
//		stand_error &= (1 << ERROR_EGURM);
//	} else if(stand_error & (1 << ERROR_SERVO_MOTOR)){
//		error = ERROR_SERVO_MOTOR;
//		stand_error &= (1 << ERROR_SERVO_MOTOR);
//	} else if(stand_error & (1 << ERROR_AKIP)){
//		error = ERROR_AKIP;
//		stand_error &= (1 << ERROR_AKIP);
//	}
