#ifndef TEST_H
#define TEST_H

#include "main.h"

typedef enum 
{
	START_TIM = 1,
	STOP_TIM = 0
}status_timer;


void Test(void);
void TestSendUartCan(void);


void ConfigTimerCounter(uint8_t num, status_timer st);
void CounterTimerUsb(void);

#endif
