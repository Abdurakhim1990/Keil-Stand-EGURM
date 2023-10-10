#ifndef INGITION_H
#define INGITION_H

#include "main.h"

#define INGITION_RCU_GPIO		RCU_GPIOB
#define INGITION_GPIO				GPIOB
#define INGITION_GPIO_PIN		GPIO_PIN_15



void IngitionInit(void);
void IngitionOn(void);
void IngitionOff(void);
void SetIngition(int16_t val);

#endif
