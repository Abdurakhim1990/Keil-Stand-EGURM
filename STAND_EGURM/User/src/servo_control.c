#include "servo_control.h"

const uint16_t COEFIC_ACCELER = 20;
const int8_t NEXT = 1;

Servo_motor_parametr Servo_param = {.speed = 0, .acceleration = 100, .direct_rotat = MOTOR_STOP_ROTATION, .torque = 0};

int8_t ServoTestModeEnd(void);
int8_t ServoTestModeStart(servo_test_mode test_mode);
int8_t ServoStartRotation(void);
int8_t ServoStartPosition(void);

//--------------------------------------------------------SERVO FUNCTIONS---------------------------------------------------

//********************************************************//
Servo_motor_parametr* GetServoParam(void)
{
	return &Servo_param;
}

//*********************** FUNCTION EMPTY *********************************//
void ServoFuncEmpty(void){}

//******************=== Ограничение крутящего момента в процентах ===*******************//
//					0 - (0 - 100)percents
void SetServoTorque(int16_t torque)
{
	Servo_param.torque = torque / 10;
	WriteToServoQueue(SERVO_SET_TORQUE);
}

uint8_t num_param = 0x1C; //or 0x1A
uint8_t torq_point = 0;
void ServoTorque(void)
{
	ServoSendWriteCommand8WithPoint(WRITE_PARAMS, 0x1C, Servo_param.torque, 0, 0x30);
	//ServoSendWriteCommand8WithPoint(WRITE_PARAMS, num_param, Servo_param.torque, 0, torq_point);
	ServoFinishCommand();
}

//***********************=== Режим позиционирования и начало вращения ===***********************//
//				grad - угол поворота
void SetServoPosition(int16_t grad)
{
	Servo_param.speed = 50;
	Servo_param.acceleration = 500;
	if (grad != 0) {
		Servo_param.length_position = (131072 * grad)/360;
		WriteToServoQueue(SERVO_POS_ON);
	} else {
	}
}
//**************************--- Позиционирование мотора ---*********************//
void ServoPosition(void)
{
	static int8_t counter = 0;
	int8_t step = 0;
	
	if(Servo_param.mode == SERVO_POS_CHK_RD_ON){
		counter = 2;
	} else if(counter > 2){
		counter = 0;
	}
	switch(counter){
		case 0:
			Servo_param.mode = SERVO_JOG_ON;
			counter += ServoTestModeEnd();
		break;
		case 1:
			counter += ServoTestModeStart(TEST_MODE_POSITIONING);
		break;
		case 2:
			counter += ServoStartPosition();
			if(counter > 2){
				ServoFinishCommand();
				TestModeEnable();
				counter = 0;
			}
		break;
	}
}

//**********************************=== Вращение двигателя ===*********************************//
//			  speed - обороты в минуту 
void SetServoSpeed(int16_t speed)
{
	int16_t delta = (Servo_param.direct_rotat == MOTOR_FORWARD_ROTATION ? speed - Servo_param.speed : speed + Servo_param.speed);
	if(delta != 0){
		Servo_param.acceleration = (delta > 0 ? delta : -delta) * COEFIC_ACCELER;
	}
	
	if(speed == 0){
		Servo_param.speed = speed;
		Servo_param.direct_rotat = MOTOR_FORWARD_ROTATION;
		WriteToServoQueue(SERVO_JOG_ON);
	} else {
		if(speed > 0){
			Servo_param.speed = speed;
			Servo_param.direct_rotat = MOTOR_REVERSE_ROTATION;
		} else {
			Servo_param.speed = -speed;
			Servo_param.direct_rotat = MOTOR_FORWARD_ROTATION;
		}
		WriteToServoQueue(SERVO_JOG_ON);
	}
}

//*******************--- Вращение мотора ---**************************//
void ServoRotation(void)
{
	static int8_t count = 0;
	
	if(Servo_param.mode == SERVO_JOG_CHK_RD_ON){
		count = 2;
	} else if(count > 2){
		count = 0;
	}
	switch(count){
		case 0:
			Servo_param.mode = SERVO_JOG_ON;
			count += ServoTestModeEnd();
		break;
		case 1:
			count += ServoTestModeStart(TEST_MODE_JOG);
		break;
		case 2:
			count += ServoStartRotation();
			if(count > 2){
				ServoFinishCommand();
				TestModeEnable();
				count = 0;
			}
		break;
	}
}

//*************************--- Выход из режима тестирования ---*************************//
void SetServoTestOff(void)
{
	if(Servo_param.mode == SERVO_JOG_CHK_RD_ON){
		Servo_param.mode = SERVO_OFF;
	} else if(Servo_param.mode == SERVO_POS_CHK_RD_ON){
		Servo_param.mode = SERVO_POS_ON;
	}
	ServoQueueTestModeDisable();
	WriteToServoQueue(SERVO_OFF);
}

void ServoTestOff(void)
{
	int8_t count = 0;
	count += ServoTestModeEnd();
	if(count){
		ServoFinishCommand();
	}
}

//***************--- Команды для позиционирования мотора ---**************//
int8_t ServoStartPosition(void)
{
	static int8_t counter = 0;
	int8_t step = 0;
	
	switch(counter){
		case 0:
			if(Servo_param.mode == SERVO_POS_CHK_RD_ON){
				Servo_param.mode = SERVO_POS_ON;
			}
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_FREQUENCY, Servo_param.speed, 0);	// Запись скорости вращения (0xA0)
			counter = 1;
			break;
		case 1:
			ServoSendWriteCommand8(TEST_MODE, POS_MODE_ACCELERATION_TIME, Servo_param.acceleration, 0);	// Запись времени разгона/торможения (0xA0)
			counter = 2;
			break;
		case 2:
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, POS_MODE_SON_LSP_LSN_ON_DATA, 0);	// Стоп вращения мотора (0x92)
			counter = 3;
			break;
		case 3:
			ServoSendWriteCommand8(TEST_MODE, POS_MODE_SET_PATH_LENGTH, Servo_param.length_position, 0);		// Запуск позиционирования (0xA0)
			counter = 0;
			step = NEXT;
			break;
	}
	return step;
}

//***************--- Завершить тестовый режим ---*******************//
int8_t ServoTestModeEnd(void)
{
	static int8_t counter = 0;
	int8_t step = 0;
	
	switch (counter) {
		case 0:
			ServoTimerDisable();
			counter = 4;
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, Servo_param.direct_rotat, 0);	// СТОП мотор (0x92)
			break;
		case 1:
			counter = 3;
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_STOP, TEST_MODE_BREAK_DATA, 0);	//???????// Стирание скорости вращения (0xA0)
			break;
		case 2:
			counter = 3;
			ServoSendWriteCommand4(TEST_MODE, JOG_MODE_STOP, TEST_MODE_BREAK_DATA, 0);	// Стирание время разгона/торможения (0xA0)
			break;
		case 3:
			counter = 4;
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, TEST_MODE_BREAK, 0);	// Завершение работы тестового режима (0x8B)
			break;
		case 4:
			counter = 0;
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_UNLOCK, TEST_MODE_BREAK_DATA, 0);	// Деблокировка входов (0x90)
			step = NEXT;
			break;
	}
	return step;
}

//***************--- Запуск тестового режима ---***************//
int8_t ServoTestModeStart(servo_test_mode test_mode)
{
	static int8_t counter = 0;
	int8_t step = 0;
	
	switch(counter){
		case 0:
			ServoSendWriteCommand4(EXTERN_OUTPUT_SIGNAL_BLOCK, OUTPUT_SIGNAL_BLOCK, TEST_MODE_BREAK_DATA, 0);	// Блокировка внешних входов (0x90)
			counter = 1;
			break;
		case 1:
			ServoSendWriteCommand4(WRITE_TEST_OPERATING_MODE, SET_TEST_MODE, test_mode, 0);	// Запуск тестового режима (0x8B)
			ServoQueueTestModeEnable();
			counter = 0;
			step = NEXT;
			break;
	}
	return step;
}

//***************--- Команды для вращение мотора ---**************//
int8_t ServoStartRotation(void)
{
	static int8_t counter = 0;
	int8_t step = 0;
	
	switch(counter){
		case 0:
			if(Servo_param.mode == SERVO_JOG_CHK_RD_ON){
				Servo_param.mode = SERVO_JOG_ON;
			}
			ServoSendWriteCommand4(TEST_MODE, POS_MODE_FREQUENCY, Servo_param.speed, 0);	// Запись скорости вращения (0xA0)
			counter = 1;
			break;
		case 1:
			ServoSendWriteCommand8(TEST_MODE, POS_MODE_ACCELERATION_TIME, Servo_param.acceleration, 0);	// Запись времени разгона/торможения (0xA0)
			counter = 2;
			break;
		case 2:
			ServoSendWriteCommand8(TEST_MODE_INPUT_SIGNAL, POS_MODE_SON_LSP_LSN_ON, Servo_param.direct_rotat, 0);	// Запуск вращения мотора (0x92)
			counter = 0;
			step = NEXT;
			break;
	}
	return step;
}
