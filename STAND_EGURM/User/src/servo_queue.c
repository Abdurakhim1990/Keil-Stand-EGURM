#include "servo_queue.h"

struct {
	#define QUEUE_BUFF_SIZE		5
	servo_mode_param mode_buff[QUEUE_BUFF_SIZE];
	uint8_t finish_command;
	uint8_t start_lock;
	uint8_t curr_index;
	uint8_t test_mode;
} ServoQueue;

//*******************************************************************//
void (*servo_funcs[]) (void) = {
	[SERVO_NONE]					= ServoFuncEmpty,
	[SERVO_SET_TORQUE]		=	ServoTorque,
	[SERVO_POS_ON]				=	ServoPosition,
	[SERVO_JOG_ON]				=	ServoRotation,
	[SERVO_OFF]						=	ServoTestOff,
	[SERVO_POS_CHK_RD_ON]	=	ServoCheckRdOn,
	[SERVO_JOG_CHK_RD_ON]	=	ServoCheckRdOn
};


void StartServoQueue(void)
{
	servo_funcs[ServoQueue.mode_buff[0]]();
	if(ServoQueue.mode_buff[0] != SERVO_NONE){
		ServoQueue.start_lock = 1;
	}
}

void WriteToServoQueue(servo_mode_param mode_param)
{
	uint8_t search = 0;
	for(uint8_t i = 1; i < ServoQueue.curr_index; ++i){
		search = (mode_param == ServoQueue.mode_buff[i]);
		if(search){
			break;
		}
	}
	if(!search/* || (mode_param == ServoQueue.mode_buff[0])*/){
		servo_mode_param mode_index = SERVO_NONE;
		if(ServoQueue.curr_index > 0)
			mode_index = ServoQueue.mode_buff[ServoQueue.curr_index - 1];
		if(mode_index == SERVO_POS_CHK_RD_ON || mode_index == SERVO_JOG_CHK_RD_ON){
			if(mode_param == SERVO_OFF){
				ServoQueue.mode_buff[ServoQueue.curr_index - 1] = mode_param;
			} else {
				ServoQueue.mode_buff[ServoQueue.curr_index - 1] = mode_param;
				ServoQueue.mode_buff[ServoQueue.curr_index++] = mode_index;
			}
		} else {
			ServoQueue.mode_buff[ServoQueue.curr_index++] = mode_param;
		}
		if(!ServoQueue.start_lock){
			StartServoQueue();
		}
	}
}

void NextModeServoQueue(void)
{
	volatile servo_mode_param curr_mode = ServoQueue.mode_buff[0];
	Servo_motor_parametr* Servo_param = GetServoParam();
	
	if(ServoQueue.finish_command){
		ServoQueue.finish_command = 0;
		ServoQueue.start_lock = 0;
		if(ServoQueue.curr_index){
			if((curr_mode != SERVO_JOG_CHK_RD_ON && curr_mode != SERVO_POS_CHK_RD_ON) || !ServoQueue.test_mode){
				for(int8_t i = 0; i < ServoQueue.curr_index; ++i){
					ServoQueue.mode_buff[i] = ServoQueue.mode_buff[i+1];
				}
				ServoQueue.mode_buff[--ServoQueue.curr_index] = SERVO_NONE;
			}
		}
	}
	if(ServoQueue.mode_buff[0] == SERVO_JOG_CHK_RD_ON || ServoQueue.mode_buff[0] == SERVO_POS_CHK_RD_ON){
		Servo_param->mode = SERVO_JOG_CHK_RD_ON;
		ServoTimerEnable();
	} else if(ServoQueue.mode_buff[0] != SERVO_NONE){
		StartServoQueue();
	}
}

void ServoFinishCommand(void)
{
	ServoQueue.finish_command = 1;
}

void TestModeEnable(void)
{
	Servo_motor_parametr* Servo_param = GetServoParam();
	
	WriteToServoQueue(SERVO_JOG_CHK_RD_ON);
	Servo_param->mode = SERVO_JOG_CHK_RD_ON;
	ServoQueueTestModeEnable();
}

void ServoQueueTestModeEnable(void)
{
	ServoQueue.test_mode = 1;
}

void ServoQueueTestModeDisable(void)
{
	ServoQueue.test_mode = 0;
}
