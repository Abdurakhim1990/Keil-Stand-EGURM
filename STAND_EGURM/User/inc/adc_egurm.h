#ifndef ADC_EGURM_H
#define ADC_EGURM_H

#include "gd32e50x.h"                   // Device header
#include "main.h"
#include "adc_init_egurm.h"


#define RES_R_1											1000

#define DEFAULT_ADC_MOMENT					1970		// ADC at N = 0 N/m
#define DELTA_ADC_MOMENT						50			// ±0.2V = 40
#define VALUE_ADC_10V_MOMENT				1850		// for 10V ADC = 1850
#define TORQUE_N_M_IN								500			// for 10V N = 50*10
#define TORQUE_N_M_OUT							1000		// for 10V N = 100*10
			
#define VALUE_ADC_50V_VOLTAGE				3945		// for V = 10V ADC = 1850
#define VOLT_50_VOLTAGE							500			// for V = 10V V = 10*10
			
#define ADC_CURRENT_0A							2044		// ADC at I = 0 A
#define DELTA_ADC_1A_CURRENT				25			// ADC = 25 at I = 1 A
#define CURRENT_1_AMPER							10			// 1 A * 100

#define ADC_DM_5_V									3050//3102
#define DM_5_VOLTAGE								50

enum adc_buff{
	MOMENT_IN = 0,
	MOMENT_OUT,
	VOLTAGE,
	CURRENT,
	TEMPERATURE,
	CUR_INJ,
	DM_1,
	DM_2,
	DM_3,
	DM_4,
};



uint16_t* pGetAdcValue(void);
void SetDefaultSenseMoment(void);
int16_t GetMomentIn(void);
int16_t GetMomentOut(void);
void AverageValueAdcMoment(void);
int16_t GetVoltage(void);
int16_t GetCurrent(void);
void AverageValueAdcVoltAmper(void);
int16_t GetTemperature(void);
int16_t GetVoltageDM_1(void);
int16_t GetVoltageDM_2(void);
int16_t GetVoltageDM_3(void);
int16_t GetVoltageDM_4(void);
int16_t GetCurrentInjition(void);

#endif
