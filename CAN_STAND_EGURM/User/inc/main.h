#ifndef	MAIN_H
#define	MAIN_H

//#include <stdint.h>
#include "gd32e50x.h"                   // Device header
#include "systick.h"
#include "can_egurm.h"
#include "timers_10_and_100_us.h"
#include "uartcan.h"
#include <stdio.h>
#include "can_dm.h"
#include "can_uds_response_request_service.h"
#include "can_uds_response_request.h"
#include "test.h"


void GeneralInitEgurm(void);

#endif
