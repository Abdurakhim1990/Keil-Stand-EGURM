#ifndef STAND_CONTROL_INIT_H
#define STAND_CONTROL_INIT_H

#include "stand_control.h"

#define USB_RETURN_PARAM_RCU_TIMER						RCU_TIMER0
#define USB_RETURN_PARAM_TIMER								TIMER0

void PeriodUsbReturnInit(void);

#endif
