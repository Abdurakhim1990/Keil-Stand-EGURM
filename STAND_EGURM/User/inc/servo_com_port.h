#pragma once

#include "servo_control.h"

void ServoCheckRdOn(void);
uint8_t* GetServoTxBuff(void);
void ServoSendReadCommand(uint16_t command, uint16_t data, uint16_t response_size, uint8_t servo_number);
void ServoSendWriteCommand4(uint16_t write_command, uint16_t data_number, uint16_t data_to_write, uint8_t servo_number);
void ServoSendWriteCommand8(uint16_t write_command, uint16_t data_number, uint32_t data_to_write, uint8_t servo_number);
void ServoSendWriteCommand8WithPoint(uint16_t write_command, uint16_t data_number, uint32_t data_to_write, uint8_t servo_number, uint8_t point);
uint8_t ServoHandleError(void);
void ServoTimerEnable(void);
void ServoTimerDisable(void);

