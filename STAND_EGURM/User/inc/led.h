#ifndef LED_H
#define LED_H

#include "main.h"

#define LED_RCU_GPIO			RCU_GPIOA
#define LED_GPIO					GPIOA
#define LED_1_PIN					GPIO_PIN_7
#define LED_2_PIN					GPIO_PIN_8
#define LED_3_PIN					GPIO_PIN_9
#define LED_4_PIN					GPIO_PIN_10

void LedInit(void);
void Led1_2Display(uint8_t temp);
void Led1_4Display(uint8_t temp);
void ToggleLed_3(void);
void ToggleLed_4(void);

#endif
