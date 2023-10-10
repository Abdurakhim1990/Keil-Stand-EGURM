#include "stand_control.h"

uint8_t buff_usb_set_param[LEN_REP_SET_PAR];
uint8_t buff_usb_req_param[LEN_REP_REQ_PAR];
uint8_t buff_usb_write_firmware[LEN_REP_WRT_FIRM];

uint8_t buff_usb_tx_ret_param[LEN_REP_RET_PAR];
uint8_t buff_usb_read_firmware[LEN_REP_RD_FIRM];

void (*pSetStandParameters[])(int16_t val) = {
	[PARAM_EMPTY]									= SetEmptyParam,
	[PARAM_VOLT_EGURM]						= SetVoltageEgurm,
	[PARAM_INGITION]							= SetIngition,
	[PARAM_ENGINE_SPEED]					= SetEngineSpeed,
	[PARAM_CAR_SPEED]							= SetCarSpeed,
	[PARAM_TORQUE_IN]							= ServoSetTorque,
	[PARAM_TORQUE_OUT]						= SetMomentOut,
	[PARAM_STEERING_SPEED]				= SetSpeedServoRotate,
	[PARAM_SHAFT_ANGLE_IN]				= ServoPosModeOnAndRotate
};

int16_t (*pRetStandParameters[])(void) = {
	[PARAM_EMPTY]									= RetEmptyParam,
	[PARAM_VOLT_EGURM]						= GetVoltage,
	[PARAM_INGITION]							= RetEmptyParam,
	[PARAM_ENGINE_SPEED]					= RetEmptyParam,
	[PARAM_CAR_SPEED]							= RetEmptyParam,
	[PARAM_TORQUE_IN]							= GetMomentIn,
	[PARAM_TORQUE_OUT]						= GetMomentOut,
	[PARAM_TEMPERAT]							= GetTemperature,
	[PARAM_CURR_EGURM]						= GetCurrent,
	[PARAM_STEERING_SPEED]				= GetSteeringSpeed,
	[PARAM_SHAFT_ANGLE_IN]				= GetAngleEncoderIn,
	[PARAM_SHAFT_ANGLE_OUT]				= GetAngleEncoderOut,
	[PARAM_SHAFT_ANGLE_EGURM]			= GetAngleEgurm,
	[PARAM_VOLT_SENS_MOM_1]				= RetEmptyParam,
	[PARAM_VOLT_SENS_MOM_2]				= RetEmptyParam,
	[PARAM_VOLT_SENS_MOM_3]				= RetEmptyParam,
	[PARAM_VOLT_SENS_MOM_4]				= RetEmptyParam,
	[PARAM_CURR_EGURM_mA]					= RetEmptyParam,
	[PARAM_CURR_INGITION]					= RetEmptyParam,
	[PARAM_STAND_ERROR]						= RetEmptyParam
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
		pSetStandParameters[buff_usb_set_param[i]](value);
	}
	if(buff_usb_set_param[PARAM_TEST_TIME]){
		timer_interrupt_enable(USB_RETURN_PARAM_TIMER, TIMER_INT_UP);
	} else{
		timer_interrupt_disable(USB_RETURN_PARAM_TIMER, TIMER_INT_UP);
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
		
		buff_usb_tx_ret_param[(i - 1) * 3 + 1] = buff_usb_req_param[i];
		buff_usb_tx_ret_param[(i - 1) * 3 + 2] = (uint8_t)(value >> 8);
		buff_usb_tx_ret_param[(i - 1) * 3 + 3] = (uint8_t)(value & 0xFF);
	}
	SendRetStandParameters(buff_usb_tx_ret_param);
}

//*********************************************//
void TIMER0_UP_TIMER9_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(USB_RETURN_PARAM_TIMER, TIMER_INT_FLAG_UP)){
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(USB_RETURN_PARAM_TIMER, TIMER_INT_FLAG_UP);
		
		RetStandParameters();
  }
}

