#include "adc_init_egurm.h"

void AdcEgurnInit(void)
{
	/* system clocks configuration */
    AdcRcuConfig();
   /* GPIO configuration */
    AdcGpioConfig();
    /* DMA configuration */
    AdcDmaConfig();
    /* ADC configuration */
    AdcConfig();
		
	delay_1ms(1);
	SetDefaultSenseMoment();
}


//*************************************************//
/*!
	\brief      configure the different system clocks
	\param[in]  none
	\param[out] none
	\retval     none
*/
void AdcRcuConfig(void)
{
	/* enable GPIO clock */
	rcu_periph_clock_enable(ADC_EGURM_RCU_GPIO);
	rcu_periph_clock_enable(ADC_EGURM_DM__RCU_GPIO);
	/* enable ADC clock */
	rcu_periph_clock_enable(ADC_EGURM_RCU_ADC);
	/* enable ADC_EGURM_DMA clock */
	rcu_periph_clock_enable(ADC_EGURM_RCU_DMA);
	/* config ADC clock */
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
}

//************************************************//
/*!
	\brief      configure the GPIO peripheral
	\param[in]  none
	\param[out] none
	\retval     none
*/
void AdcGpioConfig(void)
{
	/* config the GPIO as analog mode */
	gpio_init(ADC_EGURM_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_VCC_GPIO_PIN);
	gpio_init(ADC_EGURM_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_CURR_GPIO_PIN);
	gpio_init(ADC_EGURM_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_MOM_IN_GPIO_PIN);
	gpio_init(ADC_EGURM_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_MOM_OUT_GPIO_PIN);
	gpio_init(ADC_EGURM_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_TEMPER_GPIO_PIN);
	gpio_init(ADC_EGURM_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_CUR_ING_GPIO_PIN);
	
	gpio_init(ADC_EGURM_DM__GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_DM_1_GPIO_PIN);
	gpio_init(ADC_EGURM_DM__GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_DM_2_GPIO_PIN);
	gpio_init(ADC_EGURM_DM__GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_DM_3_GPIO_PIN);
	gpio_init(ADC_EGURM_DM__GPIO, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_EGURM_DM_4_GPIO_PIN);
}

//******************************************************//
/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void AdcDmaConfig(void)
{
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
    /* ADC DMA_channel configuration */
    dma_deinit(ADC_EGURM_DMA, ADC_EGURM_DMA_CH);

    /* initialize DMA single data mode */
		dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC_EGURM_ADC));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(pGetAdcValue());
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = 10;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
    dma_init(ADC_EGURM_DMA, ADC_EGURM_DMA_CH, &dma_data_parameter);

    dma_circulation_enable(ADC_EGURM_DMA, ADC_EGURM_DMA_CH);
    /* enable DMA channel */
    dma_channel_enable(ADC_EGURM_DMA, ADC_EGURM_DMA_CH);
}

//*****************************************************//
/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void AdcConfig(void)
{
	/* ADC mode config */
	adc_mode_config(ADC_MODE_FREE); 
	/* ADC continuous function enable */
	adc_special_function_config(ADC_EGURM_ADC, ADC_CONTINUOUS_MODE, ENABLE);
	/* ADC scan function enable */
	adc_special_function_config(ADC_EGURM_ADC, ADC_SCAN_MODE, ENABLE);
	/* ADC data alignment config */
	adc_data_alignment_config(ADC_EGURM_ADC, ADC_DATAALIGN_RIGHT);

	/* ADC channel length config */
	adc_channel_length_config(ADC_EGURM_ADC, ADC_REGULAR_CHANNEL, 10);

	/* ADC regular channel config */ 
	adc_regular_channel_config(ADC_EGURM_ADC, 0, ADC_EGURM_MOM_IN_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	adc_regular_channel_config(ADC_EGURM_ADC, 1, ADC_EGURM_MOM_OUT_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	adc_regular_channel_config(ADC_EGURM_ADC, 2, ADC_EGURM_VCC_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	adc_regular_channel_config(ADC_EGURM_ADC, 3, ADC_EGURM_CURR_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	adc_regular_channel_config(ADC_EGURM_ADC, 4, ADC_EGURM_TEMPER_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	adc_regular_channel_config(ADC_EGURM_ADC, 5, ADC_EGURM_CUR_INJ_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);

	adc_regular_channel_config(ADC_EGURM_ADC, 6, ADC_EGURM_DM_1_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	adc_regular_channel_config(ADC_EGURM_ADC, 7, ADC_EGURM_DM_2_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	adc_regular_channel_config(ADC_EGURM_ADC, 8, ADC_EGURM_DM_3_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	adc_regular_channel_config(ADC_EGURM_ADC, 9, ADC_EGURM_DM_4_ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);


	/* ADC trigger config */
	adc_external_trigger_source_config(ADC_EGURM_ADC, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	adc_external_trigger_config(ADC_EGURM_ADC, ADC_REGULAR_CHANNEL, ENABLE);

	/* ADC DMA function enable */
	adc_dma_mode_enable(ADC_EGURM_ADC);
	/* enable ADC interface */
	adc_enable(ADC_EGURM_ADC);
	delay_1ms(1);
	/* ADC calibration and reset calibration */
	adc_calibration_enable(ADC_EGURM_ADC);

	/* ADC software trigger enable */
	adc_software_trigger_enable(ADC_EGURM_ADC, ADC_REGULAR_CHANNEL);
}
