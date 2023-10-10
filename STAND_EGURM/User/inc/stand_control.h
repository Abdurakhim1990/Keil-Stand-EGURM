#ifndef STAND_CONTROL_H
#define STAND_CONTROL_H

#include "main.h"
#include "stand_control_init.h"

enum _out_reports
{
	REP_SET_PAR						= 17,
	REP_REQ_PAR						= 18,
	REP_WRT_FIRM					= 19
};

enum _in_reports
{
	REP_RET_PAR						= 21,
	REP_RD_FIRM						= 22
};

enum _length_out_reports
{
	LEN_REP_SET_PAR				= 40,
	LEN_REP_REQ_PAR				= 20,
	LEN_REP_WRT_FIRM			= 40
};

enum _length_in_reports
{
	LEN_REP_RET_PAR				= 60,
	LEN_REP_RD_FIRM				= 40
};

enum _usb_param
{
	PARAM_EMPTY									= 0,
	PARAM_VOLT_EGURM						= 1,
	PARAM_INGITION							= 2,
	PARAM_ENGINE_SPEED					= 3,
	PARAM_CAR_SPEED							= 4,
	PARAM_TORQUE_IN							= 5,
	PARAM_TORQUE_OUT						= 6,
	PARAM_TEMPERAT							= 7,
	PARAM_CURR_EGURM						= 8,
	PARAM_STEERING_SPEED				= 9,
	PARAM_SHAFT_ANGLE_IN				= 10,
	PARAM_SHAFT_ANGLE_OUT				= 11,
	PARAM_SHAFT_ANGLE_EGURM			= 12,
	PARAM_VOLT_SENS_MOM_1				= 13,
	PARAM_VOLT_SENS_MOM_2				= 14,
	PARAM_VOLT_SENS_MOM_3				= 15,
	PARAM_VOLT_SENS_MOM_4				= 16,
	PARAM_CURR_EGURM_mA					= 17,
	PARAM_CURR_INGITION					= 18,
	PARAM_STAND_ERROR						= 19,
	PARAM_TEST_TIME							= 38
};


uint8_t* ReadBuffUsbSetParam(void);
uint8_t* ReadBuffUsbReqParam(void);
uint8_t* ReadBuffUsbWriteFirmware(void);
void SetStandParameters(void);
void SetEmptyParam(int16_t val);
int16_t RetEmptyParam(void);
void RetStandParameters(void);

#endif