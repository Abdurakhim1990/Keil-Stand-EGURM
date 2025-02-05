#ifndef SERVO_CONTROL
#define SERVO_CONTROL

#include "main.h"
#include "servo_mr_j2s_a_init.h"
#include "servo_motor_define.h"
#include "servo_com_port.h"
#include "servo_queue.h"

//--------------------------------------------------------SERVO FUNCTIONS---------------------------------------------------
typedef enum {
	SERVO_NONE							= 0,
	SERVO_SET_TORQUE				= 1,
	SERVO_POS_ON						= 2,
	SERVO_JOG_ON						= 3,
	SERVO_OFF								= 4,
	SERVO_POS_CHK_RD_ON			= 5,
	SERVO_JOG_CHK_RD_ON			= 6,
} servo_mode_param;


//--------------------------------------------------------Тестовый режим (данных)--------------------------------------------------
typedef enum {
	TEST_MODE_BREAK							= 0x0000,
	TEST_MODE_JOG								= 0x0001,
	TEST_MODE_POSITIONING				= 0x0002,
} servo_test_mode;


//--------------------------------------------------------Режим JOG-----------------------------------------------------------------------
typedef enum{
	MOTOR_FORWARD_ROTATION			= 0x30000807,
	MOTOR_REVERSE_ROTATION			= 0x30001007,
	MOTOR_STOP_ROTATION					= 0x30000007,
} motor_direct_rotation;
	
#define COMMAND_SIZE													10
#define WRITE_COMMAND_RESPONSE_SIZE						6

#define BUFF_SIZE_RX				16
#define BUFF_SIZE_TX				30

typedef struct {
	int16_t								speed;
	int16_t								acceleration;
	int16_t								length_position;
	motor_direct_rotation		direct_rotat;
	
	int16_t									torque;
	servo_mode_param				mode;
} Servo_motor_parametr;



void WriteToServoQueue(servo_mode_param mode_param);

//*****************************************************//

Servo_motor_parametr* GetServoParam(void);
void ServoFuncEmpty(void);
void ServoTorque(void);
void SetServoTorque(int16_t torque);
void SetServoPosition(int16_t grad);
void ServoPosition(void);
void SetServoSpeed(int16_t speed);
void ServoRotation(void);
void SetServoTestOff(void);
void ServoTestOff(void);

#endif //	!SERVO_CONTROL