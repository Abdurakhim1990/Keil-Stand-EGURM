#include "mufta_egurm_init.h"

/*!
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MUFTA_PWMEgurmInit(void)
{
	rcu_periph_clock_enable(MUFTA_PWM_RCU_GPIO);
	rcu_periph_clock_enable(RCU_AF);

	/*Configure GPIOx (MUFTA_PWM_TIMER CHx) as alternate function*/
	gpio_init(MUFTA_PWM_GPIO, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, MUFTA_PWM_GPIO_PIN);

	/* -----------------------------------------------------------------------
	MUFTA_PWM_TIMER configuration: generate PWM signal
	MUFTA_PWM_TIMER channel1 duty cycle = (8000/ 16000)* 100  = 50%
	----------------------------------------------------------------------- */
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(MUFTA_PWM_RCU_TIMER);

	timer_deinit(MUFTA_PWM_TIMER);

	/* MUFTA_PWM_TIMER configuration */
	timer_initpara.prescaler         = 1679;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = MUFTA_PWM_PERIOD;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(MUFTA_PWM_TIMER,&timer_initpara);

	/* CHx configuration in PWM mode */
	timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

	timer_channel_output_config(MUFTA_PWM_TIMER, MUFTA_PWM_TIMER_CH, &timer_ocintpara);

	/* CH1 configuration in PWM mode0,duty cycle 50% */
	timer_channel_output_pulse_value_config(MUFTA_PWM_TIMER, MUFTA_PWM_TIMER_CH, 500);
	timer_channel_output_mode_config(MUFTA_PWM_TIMER, MUFTA_PWM_TIMER_CH, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(MUFTA_PWM_TIMER, MUFTA_PWM_TIMER_CH, TIMER_OC_SHADOW_DISABLE);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(MUFTA_PWM_TIMER);
	/* auto-reload preload enable */
	timer_enable(MUFTA_PWM_TIMER);
	
	InitMuftaTimer();
}

//*********************************************************************//
/*!
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void InitMuftaTimer(void)
{
	/* ----------------------------------------------------------------------------
	SERVO_TIMER Configuration: 
	SERVO_TIMERCLK = SystemCoreClock/10 = 16.8MHz, the period is 1s(168000000/1680 = 100 MH).
	1/100 MH = 5 ms
	---------------------------------------------------------------------------- */
	timer_oc_parameter_struct timer_ocinit_struct;
	timer_parameter_struct timer_init_struct;

	/* enable the peripherals clock */
	rcu_periph_clock_enable(MUFTA_RCU_TIMER);

	/* deinit a TIMER */
	timer_deinit(MUFTA_TIMER);
	/* initialize TIMER init parameter struct */
	timer_struct_para_init(&timer_init_struct);
	/* SERVO_TIMER configuration */
	timer_init_struct.prescaler					= 1679;
	timer_init_struct.alignedmode				= TIMER_COUNTER_EDGE;
	timer_init_struct.counterdirection	= TIMER_COUNTER_UP;
	timer_init_struct.period						= 200;
	timer_init_struct.clockdivision			= TIMER_CKDIV_DIV1;
	timer_init(MUFTA_TIMER, &timer_init_struct);

	/* enable the TIMER interrupt */
	//timer_interrupt_enable(MUFTA_TIMER, TIMER_INT_UP);
	/* enable a TIMER */
	timer_enable(MUFTA_TIMER);
	
	nvic_irq_enable(TIMER7_BRK_TIMER11_IRQn, 0, 0);
}
