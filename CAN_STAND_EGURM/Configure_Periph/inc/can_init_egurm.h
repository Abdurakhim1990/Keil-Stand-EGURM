#ifndef CAN_INIT_EGURM_H
#define CAN_INIT_EGURM_H

#include "can_egurm.h"
#include "main.h"

#define CAN_IDE_32					4

enum {
	GPIO_CAN = GPIOB,
	GPIO_CAN_RX = GPIO_PIN_12,
	GPIO_CAN_TX = GPIO_PIN_13
};

enum BAUD_RATE{
	BAUD_RATE_250 = 250,
	BAUD_RATE_500 = 500
};

void CanConfig(uint16_t baud_rate);
void ConfigBaudRateCan(can_parameter_struct *can_parameter, uint16_t baud_rate);
void FilterInit(void);
void CanEgurmInit(void);
void CanGpioConfig(void);

#endif
