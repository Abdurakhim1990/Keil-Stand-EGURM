#include "adc_egurm.h"

struct valAdcForParam
{
	uint16_t moment_in;
	uint16_t moment_out;
	uint16_t voltage;
	uint16_t current;
} valAdcParam;

uint16_t adc_buffer[4];

uint16_t default_adc_moment_in = DEFAULT_ADC_MOMENT;
uint16_t default_adc_moment_out = DEFAULT_ADC_MOMENT;


//**************************************************************//
uint16_t* pGetAdcValue(void)
{
	return (uint16_t*) adc_buffer;
}

//**************************************************************//
void SetDefaultSenseMoment(void)
{
	if((valAdcParam.moment_in > DEFAULT_ADC_MOMENT - DELTA_ADC_MOMENT)
		& (valAdcParam.moment_in < DEFAULT_ADC_MOMENT + DELTA_ADC_MOMENT)){
			default_adc_moment_in = valAdcParam.moment_in;
	}
	if((valAdcParam.moment_out > DEFAULT_ADC_MOMENT - DELTA_ADC_MOMENT)
		& (valAdcParam.moment_out < DEFAULT_ADC_MOMENT + DELTA_ADC_MOMENT)){
			default_adc_moment_out = valAdcParam.moment_out;
	}
}

//**************************************************************//
int16_t GetMomentIn(void)
{
	return((default_adc_moment_in - valAdcParam.moment_in)*TORQUE_N_M_IN)/VALUE_ADC_10V_MOMENT;
}

//**************************************************************//
int16_t GetMomentOut(void)
{
	return((default_adc_moment_out - valAdcParam.moment_out)*TORQUE_N_M_OUT)/VALUE_ADC_10V_MOMENT;
}

//**************************************************************//
void AverageValueAdcMoment(void)
{
	static uint32_t aver_adc_buff[2] = {0, 0};
	static uint8_t count = 0;
	
	if(count < 10){
		for(uint8_t i = 0; i < 2; ++i){
			aver_adc_buff[i] += adc_buffer[i];
		}
		++count;
	} else{
		valAdcParam.moment_in = aver_adc_buff[MOMENT_IN]/count;
		valAdcParam.moment_out = aver_adc_buff[MOMENT_OUT]/count;
		for(uint8_t i = 0; i < 2; ++i){
			aver_adc_buff[i] = 0;
		}
		count = 0;
	}
}

//**************************************************************//
int16_t GetVoltage(void)
{
	return (valAdcParam.voltage * VOLT_50_VOLTAGE)/VALUE_ADC_50V_VOLTAGE;
}

//**************************************************************//
int16_t GetCurrent(void)
{
	return((valAdcParam.current - ADC_CURRENT_0A)*CURRENT_1000_AMPER)/DELTA_ADC_1000A_CURRENT;
}

//**************************************************************//
void AverageValueAdcVoltAmper(void)
{
	static uint32_t aver_adc_buff[4] = {0, 0, 0, 0};
	static uint8_t count = 0;
	
	if(count < 100){
		for(uint8_t i = 2; i < 4; ++i){
			aver_adc_buff[i] += adc_buffer[i];
		}
		++count;
	} else{
		valAdcParam.voltage = aver_adc_buff[VOLTAGE]/count;
		valAdcParam.current = aver_adc_buff[CURRENT]/count;
		for(uint8_t i = 2; i < 4; ++i){
			aver_adc_buff[i] = 0;
		}
		count = 0;
	}
}
