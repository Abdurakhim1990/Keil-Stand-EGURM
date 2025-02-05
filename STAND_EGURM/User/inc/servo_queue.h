#pragma once

#include "servo_control.h"

void StartServoQueue(void);
void NextModeServoQueue(void);
void ServoFinishCommand(void);
void TestModeEnable(void);
void ServoQueueTestModeEnable(void);
void ServoQueueTestModeDisable(void);
