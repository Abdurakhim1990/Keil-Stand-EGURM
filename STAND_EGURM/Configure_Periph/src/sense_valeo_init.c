#include "sense_valeo_init.h"


void ValeoGpioConfig(uint32_t gpio, uint32_t gpio_pin)
{
	gpio_init(gpio, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, gpio_pin);
}

void ValeoTimerConfig(uint32_t timer, uint16_t prescal, uint16_t period)
{
	timer_ic_parameter_struct timer_icinitpara;
	timer_parameter_struct timer_initpara;

	timer_deinit(timer);

	/* TIMER configuration */
	timer_initpara.prescaler         = prescal - 1;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = period;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(timer, &timer_initpara);

	/* TIMER configuration */
	/* TIMER CH0 PWM input capture configuration */
	timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_RISING;
	timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
	timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
	timer_icinitpara.icfilter    = 0x0;
	timer_input_pwm_capture_config(timer, TIMER_CH_0, &timer_icinitpara);

	/* slave mode selection: TIMER */
	timer_input_trigger_source_select(timer, TIMER_SMCFG_TRGSEL_CI0FE0);
	timer_slave_mode_select(timer, TIMER_SLAVE_MODE_RESTART);

	/* select the master slave mode */
	timer_master_slave_mode_config(timer, TIMER_MASTER_SLAVE_MODE_ENABLE);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(timer);
	/* clear channel 0 interrupt bit */
	timer_interrupt_flag_clear(timer, TIMER_INT_FLAG_CH0);
	/* channel 0 interrupt enable */
	timer_interrupt_enable(timer, TIMER_INT_CH0);
	/* enable the TIMER interrupt */
	timer_interrupt_flag_clear(timer, TIMER_INT_FLAG_UP);
	timer_interrupt_enable(timer, TIMER_INT_UP);
	/* TIMER counter enable */
	timer_enable(timer);
}

void ValeoTimersNVICConfig(uint8_t timer_irq)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(timer_irq, 1, 0);
}

void ValeoTimersInit(void)
{
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(VALEO_IN_PWM_DM1_RCU_GPIO);
	rcu_periph_clock_enable(VALEO_IN_PWM_DM2_RCU_GPIO);
	rcu_periph_clock_enable(VALEO_IN_PWM_DM3_RCU_GPIO);
	rcu_periph_clock_enable(VALEO_IN_PWM_DM4_RCU_GPIO);
	rcu_periph_clock_enable(VALEO_IN_PWM_DM1_RCU_TIMER);
	rcu_periph_clock_enable(VALEO_IN_PWM_DM2_RCU_TIMER);
	rcu_periph_clock_enable(VALEO_IN_PWM_DM3_RCU_TIMER);
	rcu_periph_clock_enable(VALEO_IN_PWM_DM4_RCU_TIMER);
	
	ValeoGpioConfig(VALEO_IN_PWM_DM1_RCU_GPIO, VALEO_IN_PWM_DM1_GPIO_PIN);
	ValeoGpioConfig(VALEO_IN_PWM_DM2_RCU_GPIO, VALEO_IN_PWM_DM2_GPIO_PIN);
	ValeoGpioConfig(VALEO_IN_PWM_DM3_RCU_GPIO, VALEO_IN_PWM_DM3_GPIO_PIN);
	ValeoGpioConfig(VALEO_IN_PWM_DM4_RCU_GPIO, VALEO_IN_PWM_DM4_GPIO_PIN);
	
	ValeoTimerConfig(VALEO_IN_PWM_DM1_TIMER, VALEO_IN_PWM_DM1_PRESCAL, VALEO_IN_PWM_DM1_PERIOD);
	ValeoTimerConfig(VALEO_IN_PWM_DM2_TIMER, VALEO_IN_PWM_DM2_PRESCAL, VALEO_IN_PWM_DM2_PERIOD);
	ValeoTimerConfig(VALEO_IN_PWM_DM3_TIMER, VALEO_IN_PWM_DM3_PRESCAL, VALEO_IN_PWM_DM3_PERIOD);
	ValeoTimerConfig(VALEO_IN_PWM_DM4_TIMER, VALEO_IN_PWM_DM4_PRESCAL, VALEO_IN_PWM_DM4_PERIOD);
	
	ValeoTimersNVICConfig(VALEO_IN_PWM_DM1_TIMER_IRQn);
	ValeoTimersNVICConfig(VALEO_IN_PWM_DM2_TIMER_IRQn);
	ValeoTimersNVICConfig(VALEO_IN_PWM_DM3_TIMER_IRQn);
	ValeoTimersNVICConfig(VALEO_IN_PWM_DM4_TIMER_IRQn);
}