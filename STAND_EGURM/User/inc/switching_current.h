#ifndef SWITCHING_CURRENT_H
#define SWITCHING_CURRENT_H

#include "main.h"

#define SWITCHING_CURRENT_RCU_GPIO		RCU_GPIOC
#define SWITCHING_CURRENT_GPIO				GPIOC
#define SWITCHING_CURRENT_GPIO_PIN		GPIO_PIN_7


void SwitchingCurrentInit(void);
void SelectMilliAmpere(void);
void SelectAmpere(void);

#endif
