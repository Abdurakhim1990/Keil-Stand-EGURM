#ifndef VOLTAFE_REVERS_H
#define VOLTAFE_REVERS_H

#include "main.h"

#define VOLTAGE_REVERS_RCU_GPIO		RCU_GPIOB
#define VOLTAGE_REVERS_GPIO				GPIOB
#define VOLTAGE_REVERS_GPIO_PIN		GPIO_PIN_14

void VoltageReversInit(void);
void VoltageReversOn(void);
void VoltageReversOff(void);

#endif
