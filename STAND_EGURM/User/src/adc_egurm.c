#include "adc_egurm.h"
#include "t_sense_table.h"

struct valAdcForParam
{
	uint16_t moment_in;
	uint16_t moment_out;
	uint16_t voltage;
	uint16_t current;
	uint16_t temperature;
	uint16_t cur_inj;
	uint16_t cur_mA;
	uint16_t dm[4];
} valAdcParam;

uint16_t adc_buffer[NUMBER_ADC_PARAMETR];

uint16_t default_adc_moment_in = DEFAULT_ADC_MOMENT;
uint16_t default_adc_moment_out = DEFAULT_ADC_MOMENT;

//**-- Взять адрес буффера ADC --**//
//**************************************************************//
uint16_t* pGetAdcValue(void)
{
	return (uint16_t*) adc_buffer;
}

//**-- Установка значения ADC при моментах М = 0 --**//
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

//**-- Получить входной момент --**//
//**************************************************************//
int16_t GetMomentIn(void)
{
	int16_t delta_moment = (default_adc_moment_in > valAdcParam.moment_in ? 
		(default_adc_moment_in - valAdcParam.moment_in) : (valAdcParam.moment_in - default_adc_moment_in));
	return((delta_moment)*TORQUE_N_M_IN)/VALUE_ADC_10V_MOMENT;
}

//**-- Получить выходной момент --**//
//**************************************************************//
int16_t GetMomentOut(void)
{
	int16_t delta_moment = (default_adc_moment_out > valAdcParam.moment_out ? 
		(default_adc_moment_out - valAdcParam.moment_out) : (valAdcParam.moment_out - default_adc_moment_out));
	return((delta_moment)*TORQUE_N_M_OUT)/VALUE_ADC_10V_MOMENT;
}

//**-- Усреднение значений входного и выходного моментов --**//
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

//**-- Получить напряжение питания ЭГУРМ --**//
//**************************************************************//
int16_t GetVoltage(void)
{
	return (valAdcParam.voltage * VOLT_50_VOLTAGE)/VALUE_ADC_50V_VOLTAGE;
}

//**-- Получить ток потребления ЭГУРМ --**//
//**************************************************************//
int16_t GetCurrent(void)
{
	return((valAdcParam.current - ADC_CURRENT_0A)*CURRENT_1_AMPER)/DELTA_ADC_1A_CURRENT;
}

//**-- Усреднение значений напряжения, тока и температуры --**//
//**************************************************************//
void AverageValueAdcVoltAmper(void)
{
	static uint32_t aver_adc_buff[NUMBER_ADC_PARAMETR] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static uint8_t count = 0;
	
	if(count < 100){
		for(uint8_t i = 2; i < NUMBER_ADC_PARAMETR; ++i){
			aver_adc_buff[i] += adc_buffer[i];
		}
		++count;
	} else{
		valAdcParam.voltage = aver_adc_buff[VOLTAGE]/count;
		valAdcParam.current = aver_adc_buff[CURRENT]/count;
		valAdcParam.temperature = aver_adc_buff[TEMPERATURE]/count;
		valAdcParam.cur_inj = aver_adc_buff[CUR_INJ]/count;
		valAdcParam.cur_mA = aver_adc_buff[CUR_mA]/count;
		for(size_t i = 0; i < 4; ++i){
			valAdcParam.dm[i] = aver_adc_buff[DM_1 + i]/count;
		}
		
		for(uint8_t i = 2; i < NUMBER_ADC_PARAMETR; ++i){
			aver_adc_buff[i] = 0;
		}
		count = 0;
	}
}

//**-- Получить температуру окружающей среды --**//
//**************************************************************//
int16_t GetTemperature(void)
{
	int32_t res_sens = (valAdcParam.temperature * RES_R_1);
	res_sens /= (4095 - valAdcParam.temperature);
	int32_t x_1 = 0, x_2 = 0, y_1 = 0, y_2 = 0;
	int8_t temper = 0;
	
	if(res_sens >= sense_table[0][1]){
		if(res_sens >= 113500){
			temper = -99;
		} else{
			x_1 = sense_table[0][1];
			x_2 = sense_table[1][1];
			y_1 = sense_table[0][0];
			y_2 = sense_table[1][0];
			int32_t temp = ((y_2 - y_1)*(res_sens - x_1))/(x_2 - x_1);
			temp += y_1;
			temper = (int16_t)temp;
		}
	} else if(res_sens <= 97){
		temper = 99;
	} else{
		for(uint8_t i = 0; i < 32; i++){
			if(res_sens <= sense_table[i][1] && res_sens > sense_table[i + 1][1]){
				x_1 = sense_table[i][1];
				x_2 = sense_table[i + 1][1];
				y_1 = sense_table[i][0];
				y_2 = sense_table[i + 1][0];
				break;
			}
		}
		int32_t temp = ((y_2 - y_1)*(res_sens - x_1))/(x_2 - x_1);
		temp += y_1;
		temper = (int16_t)temp;
	}
	return temper;
}

//**-- Получить напряжение датчика момента ДПР 1 --**//
//**************************************************************//
int16_t GetVoltageDM_1(void)
{
	int32_t volt = (valAdcParam.dm[0] * DM_5_VOLTAGE) / ADC_DM_5_V;
	return (int16_t)volt;
}

//**-- Получить напряжение датчика момента ДПР 2 --**//
//**************************************************************//
int16_t GetVoltageDM_2(void)
{
	int32_t volt = (valAdcParam.dm[1] * DM_5_VOLTAGE) / ADC_DM_5_V;
	return (int16_t)volt;
}

//**-- Получить напряжение датчика момента ДПР 3 --**//
//**************************************************************//
int16_t GetVoltageDM_3(void)
{
	int32_t volt = (valAdcParam.dm[2] * DM_5_VOLTAGE) / ADC_DM_5_V;
	return (int16_t)volt;
}

//**-- Получить напряжение датчика момента ДПР 4 --**//
//**************************************************************//
int16_t GetVoltageDM_4(void)
{
	int32_t volt = (valAdcParam.dm[3] * DM_5_VOLTAGE) / ADC_DM_5_V;
	return (int16_t)volt;
}

//**-- Получить ток потребления по цепи зажигания ЭГУРМ --**//
//**************************************************************//
int16_t GetCurrentInjition(void)
{
	uint32_t current = (valAdcParam.cur_inj * INJECTION_20_mA)/ADC_20_mA_INJ;
	return current;
}

//**-- Получить ток потребления ЭГУРМ в миллиамперах --**//
//**************************************************************//
int16_t GetCurrentMilliAmpere(void)
{
	uint32_t current = 0xFFFF;
	if(GetCurrent() < 2){
		SelectMilliAmpere();
		current = (valAdcParam.cur_mA * CURRENT_20_mA)/ADC_20_mA;
	} else {
		SelectAmpere();
	}
	
	return current;
}
