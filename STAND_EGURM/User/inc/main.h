#ifndef	MAIN_H
#define	MAIN_H

#include "gd32e50x.h"                   // Device header
#include "systick.h"
#include "usbd_egurm.h"
#include "adc_egurm.h"
#include "timers_10_and_100_us.h"
#include "usart_akip_1148a.h"
#include <stdio.h>
#include "encoder_egurm.h"
#include "mufta_egurm.h"
#include "voltage_revers.h"
#include "ingition.h"
#include "servo_control.h"
#include "uartcan.h"
#include "stand_control.h"
#include "stand_error.h"
#include "switching_current.h"

#include "test.h"

void GeneralInitEgurm(void);

#endif
