#include "mufta_egurm_init.h"

/*!
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MuftaEgurmInit(void)
{
	rcu_periph_clock_enable(MUFTA_RCU_GPIO);
	rcu_periph_clock_enable(RCU_AF);

	/*Configure GPIOx (MUFTA_TIMER CHx) as alternate function*/
	gpio_init(MUFTA_GPIO, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, MUFTA_GPIO_PIN);

	/* -----------------------------------------------------------------------
	MUFTA_TIMER configuration: generate PWM signal
	MUFTA_TIMER channel1 duty cycle = (8000/ 16000)* 100  = 50%
	----------------------------------------------------------------------- */
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(MUFTA_RCU_TIMER);

	timer_deinit(MUFTA_TIMER);

	/* MUFTA_TIMER configuration */
	timer_initpara.prescaler         = 1679;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = MUFTA_PWM_PERIOD;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(MUFTA_TIMER,&timer_initpara);

	/* CHx configuration in PWM mode */
	timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

	timer_channel_output_config(MUFTA_TIMER, MUFTA_TIMER_CH, &timer_ocintpara);

	/* CH1 configuration in PWM mode0,duty cycle 50% */
	timer_channel_output_pulse_value_config(MUFTA_TIMER, MUFTA_TIMER_CH, 500);
	timer_channel_output_mode_config(MUFTA_TIMER, MUFTA_TIMER_CH, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(MUFTA_TIMER, MUFTA_TIMER_CH, TIMER_OC_SHADOW_DISABLE);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(MUFTA_TIMER);
	/* auto-reload preload enable */
	timer_enable(MUFTA_TIMER);
}
