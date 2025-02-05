#include "stand_control.h"

uint8_t buff_usb_set_param[LEN_REP_SET_PAR];
uint8_t buff_usb_req_param[LEN_REP_REQ_PAR];
uint8_t buff_usb_write_firmware[LEN_REP_WRT_FIRM];

uint8_t buff_usb_tx_ret_param[LEN_REP_RET_PAR];
uint8_t buff_usb_read_firmware[LEN_REP_RD_FIRM];

void (*pSetStandParameters[])(int16_t val) = {
	[PARAM_EMPTY]									= SetEmptyParam,						// s/g
	[PARAM_VOLT_EGURM]						= SetVoltageEgurm,					// s/g
	[PARAM_INGITION]							= SetIngition,		 					// s
	[PARAM_ENGINE_SPEED]					= SetEngineSpeed,						// s
	[PARAM_CAR_SPEED]							= SetCarSpeed,		 					// s
	[PARAM_TORQUE_IN]							= SetServoTorque,						// s/g
	[PARAM_TORQUE_OUT]						= SetMomentOut,		 					// s/g
	[PARAM_TEMPERAT]							= SetEmptyParam,						// g
	[PARAM_CURR_EGURM]						= SetEmptyParam,						// g
	[PARAM_STEERING_SPEED]				= SetServoSpeed,						// s/g
	[PARAM_SHAFT_ANGLE_IN]				= SetServoPosition,					// s/g
	[PARAM_SHAFT_ANGLE_OUT]				= SetEmptyParam,						// g
	[PARAM_SHAFT_ANGLE_EGURM]			= SetEmptyParam,						// g
	[PARAM_VOLT_SENS_MOM_1]				= SetEmptyParam,						// g
	[PARAM_VOLT_SENS_MOM_2]				= SetEmptyParam,						// g
	[PARAM_VOLT_SENS_MOM_3]				= SetEmptyParam,						// g
	[PARAM_VOLT_SENS_MOM_4]				= SetEmptyParam,						// g
	[PARAM_CURR_EGURM_mA]					= SetEmptyParam,						// g
	[PARAM_CURR_INGITION]					= SetEmptyParam,						// g
	[PARAM_STAND_ERROR]						= SetEmptyParam,						// g
	[PARM_STEERING_WHEEL_ZERO]		= SetZeroWheel,							// s/g
	[PARM_STEERING_WHEEL_LEFT]		= SetLeftWheel,							// s/g
	[PARM_STEERING_WHEEL_RIGHT]		= SetRightWheel,						// s/g
};

//*********************************************************//
int16_t (*pRetStandParameters[])(void) = {
	[PARAM_EMPTY]									= RetEmptyParam,					// s/g
	[PARAM_VOLT_EGURM]						= GetVoltage,							// s/g
	[PARAM_INGITION]							= RetEmptyParam,					// s
	[PARAM_ENGINE_SPEED]					= RetEmptyParam,					// s
	[PARAM_CAR_SPEED]							= RetEmptyParam,					// s
	[PARAM_TORQUE_IN]							= GetMomentIn,						// s/g
	[PARAM_TORQUE_OUT]						= GetMomentOut,						// s/g
	[PARAM_TEMPERAT]							= GetTemperature,					// g
	[PARAM_CURR_EGURM]						= GetCurrent,							// g
	[PARAM_STEERING_SPEED]				= GetSteeringSpeed,				// s/g
	[PARAM_SHAFT_ANGLE_IN]				= GetAngleEncoderIn,			// s/g
	[PARAM_SHAFT_ANGLE_OUT]				= GetAngleEncoderOut,			// g
	[PARAM_SHAFT_ANGLE_EGURM]			= GetAngleEgurm,					// g
	[PARAM_VOLT_SENS_MOM_1]				= GetVoltageDM_1,					// g
	[PARAM_VOLT_SENS_MOM_2]				= GetVoltageDM_2,					// g
	[PARAM_VOLT_SENS_MOM_3]				= GetVoltageDM_3,					// g
	[PARAM_VOLT_SENS_MOM_4]				= GetVoltageDM_4,					// g
	[PARAM_CURR_EGURM_mA]					= GetCurrentMilliAmpere,	// g
	[PARAM_CURR_INGITION]					= GetCurrentInjition,			// g
	[PARAM_STAND_ERROR]						= GetErrorStand,					// g
	[PARM_STEERING_WHEEL_ZERO]		= GetZeroAngle,						// s/g
	[PARM_STEERING_WHEEL_LEFT]		= GetLeftAngle,						// s/g
	[PARM_STEERING_WHEEL_RIGHT]		= GetRightAngle,					// s/g
};

//*********************************************//
uint8_t* ReadBuffUsbSetParam(void)
{
	return buff_usb_set_param;
}

//*********************************************//
uint8_t* ReadBuffUsbReqParam(void)
{
	return buff_usb_req_param;
}

//*********************************************//
uint8_t* ReadBuffUsbWriteFirmware(void)
{
	return buff_usb_write_firmware;
}

//*********************************************//
void SetStandParameters(void)
{
	for(uint8_t i = 1; (i + 3) < LEN_REP_SET_PAR; i += 3){
		int16_t value = (buff_usb_set_param[i + 1] << 8) + buff_usb_set_param[i + 2];
		if(buff_usb_set_param[i] < PARAM_NUMBER_END)
			pSetStandParameters[buff_usb_set_param[i]](value);
	}
}

//*********************************************//
void SetEmptyParam(int16_t val)
{
	__NOP();
}

//*********************************************//
int16_t RetEmptyParam(void)
{
	return 0;
}

//*********************************************//
void RetStandParameters(void)
{
	int16_t value = 0;
	
	for(size_t i = 1; i < LEN_REP_REQ_PAR-1; ++i){
		value = pRetStandParameters[buff_usb_req_param[i]]();
		if(buff_usb_req_param[i] != PARAM_EMPTY){
			AddTestUSB(buff_usb_req_param[i], value);
		}
		if(buff_usb_req_param[i] == PARAM_EMPTY && StatusErrorStand()){
			//buff_usb_req_param[i] = PARAM_STAND_ERROR;
		}
		buff_usb_tx_ret_param[(i - 1) * 3 + 1] = buff_usb_req_param[i];
		buff_usb_tx_ret_param[(i - 1) * 3 + 2] = (uint8_t)(value >> 8);
		buff_usb_tx_ret_param[(i - 1) * 3 + 3] = (uint8_t)(value & 0xFF);
	}
	SendRetStandParameters(buff_usb_tx_ret_param);
}
