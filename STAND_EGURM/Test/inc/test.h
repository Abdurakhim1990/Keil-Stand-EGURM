#ifndef TEST_H
#define TEST_H

#include "main.h"
#include "test_servo.h"

typedef enum 
{
	START_TIM = 1,
	STOP_TIM = 0
}status_timer;


void Test(void);
void TestSendUartCan(void);


void ConfigTimerCounter(uint8_t num, status_timer st);
void CounterTimerUsb(void);
void ServoMotor(void);
void GetClockFreq(void);
void MainTest(void);

void AddTestInEncoder(uint16_t val);
void AddTestUSB(uint16_t par, uint16_t val);
void ClearTestUSB(void);
void ClearTestEncoder(void);
void TestAkip(void);
#endif
