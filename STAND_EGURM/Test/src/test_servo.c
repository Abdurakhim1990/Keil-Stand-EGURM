#include "test_servo.h"


void MotorTest(void)
{
	MainTestServo();
}



void MainTestServo(void)
{
	static uint8_t temp = 0;
	ServoMotor();
}





int32_t pos_angle = 360;			// угол поворота
int16_t speed_motor = 50;		// частота вращения
uint32_t torque = 10;					// крутящий момент
uint8_t main_servo_mode = 0;	


void ServoMotor(void)
{
	switch (main_servo_mode) {
		/*--------------------------------- SET TORQUE -----------------------------------*/
		case 1:
			SetServoTorque(torque); 									// (1) set torque
			break;
		/*------------------------------------ JOG ---------------------------------------*/
		case 2:
			SetServoSpeed(speed_motor);								// (2) jog mode - on and rotation
			break;
		case 3:
			SetServoSpeed(0);													// (3) jog mode - stop rotation
			break;
		/*-------------------------------- POSITIONING -----------------------------------*/
		case 4:
			SetServoPosition(pos_angle);									// (4) pos mode - on and direct rotation
			break;
		case 5:
			SetServoPosition(0);													// (5) pos mode - set path
			break;
		case 6:
			break;
		case 7:
			speed_motor = 15;
			torque = 5;
			SetServoSpeed(speed_motor);
			SetServoTorque(torque);
			break;
		case 8:
			speed_motor = 100;
			SetServoSpeed(speed_motor);
			break;
		case 9:
			speed_motor = -100;
			SetServoSpeed(speed_motor);
			break;
		case 10:
			speed_motor = 200;
			SetServoSpeed(speed_motor);
			break;
		case 11:
			speed_motor = -200;
			SetServoSpeed(speed_motor);
			break;
		case 12:
			speed_motor = 100;
			torque = 1;
			SetServoSpeed(speed_motor);
			SetServoTorque(torque);
			break;
		case 13:
			speed_motor = -100;
			torque = 3;
			SetServoSpeed(speed_motor);
			SetServoTorque(torque);
			break;
		case 14:
			speed_motor = 200;
			torque = 4;
			SetServoSpeed(speed_motor);
			SetServoTorque(torque);
			break;
		case 15:
			speed_motor = -200;
			torque = 2;
			SetServoSpeed(speed_motor);
			SetServoTorque(torque);
			break;
		default:
			break;
	}
	main_servo_mode = 0;
}
