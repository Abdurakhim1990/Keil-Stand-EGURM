#ifndef TEST_H
#define TEST_H

#include "main.h"

typedef enum 
{
	START_TIM = 1,
	STOP_TIM = 0
}status_timer;


void MotorTest(void);
void TestSendUartCan(void);


void ConfigTimerCounter(uint8_t num, status_timer st);
void CounterTimerUsb(void);
void ServoMotor(void);
void GetClockFreq(void);
void MainTestServo(void);

#endif
