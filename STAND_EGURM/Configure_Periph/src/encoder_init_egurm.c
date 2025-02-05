#include "encoder_init_egurm.h"



//****************************************************//
void EncoderInGpioInit(void)
{
	rcu_periph_clock_enable(ENCODER_IN_A_B_RCU_GPIO);
	rcu_periph_clock_enable(RCU_AF);

	/*configure (TIMER CH0 and CH1 for ENCODER A and ENCODER B) as alternate function*/
	gpio_init(ENCODER_IN_A_B_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, ENCODER_IN_A_GPIO_PIN|ENCODER_IN_B_GPIO_PIN);
	/*configure (TIMER CH0 for ENCODER R) as alternate function*/
	//gpio_init(ENCODER_IN_R_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, ENCODER_IN_R_GPIO_PIN);
	
	gpio_pin_remap_config(ENCODER_IN_A_B_GPIO_TIMER_REMAP, ENABLE);
}

//****************************************************//
void EncoderIn_A_B_Init(void)
{
    /* ENCODER_IN_TIMER configuration: input capture mode -------------------
    the external signal is connected to ENCODER_IN_TIMER CH0 pin (PB4)
    the rising edge is used as active edge
    the ENCODER_IN_TIMER CH0CV is used to compute the frequency value
    ------------------------------------------------------------ */
    timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;
	
    rcu_periph_clock_enable(ENCODER_IN_A_B_RCU_TIMER);

    timer_deinit(ENCODER_IN_A_B_TIMER);

    /* ENCODER_IN_TIMER configuration */
    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 36000;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(ENCODER_IN_A_B_TIMER,&timer_initpara);

		timer_quadrature_decoder_mode_config(ENCODER_IN_A_B_TIMER, TIMER_QUAD_DECODER_MODE1,
					TIMER_IC_POLARITY_RISING, TIMER_IC_POLARITY_RISING);
    
		/* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(ENCODER_IN_A_B_TIMER, TIMER_INT_FLAG_UP);
    /* enable the ENCODER_IN_A_B_TIMER interrupt */
    timer_interrupt_enable(ENCODER_IN_A_B_TIMER, TIMER_INT_UP);
		//timer_interrupt_enable(ENCODER_IN_A_B_TIMER, TIMER_INT_CH3);
		//timer_channel_output_pulse_value_config(ENCODER_IN_A_B_TIMER, TIMER_INT_CH3, 16000);
		
		/* ENCODER_IN_TIMER counter enable */
    timer_enable(ENCODER_IN_A_B_TIMER);
		nvic_irq_enable(ENCODER_IN_A_B_TIMER_IRQn, 0, 0);
}

//****************************************************//
void EncoderOUTGpioInit(void)
{
	rcu_periph_clock_enable(ENCODER_OUT_A_B_RCU_GPIO);
	rcu_periph_clock_enable(RCU_AF);

	/*configure (TIMER CH0 and CH1 for ENCODER A and ENCODER B) as alternate function*/
	gpio_init(ENCODER_OUT_A_B_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, ENCODER_OUT_A_GPIO_PIN|ENCODER_OUT_B_GPIO_PIN);
}

//******************************************************//
void EncoderOut_A_B_Init(void)
{
    /* ENCODER_OUT_TIMER configuration: input capture mode -------------------
    the external signal is connected to ENCODER_OUT_TIMER CH0 pin (PB4)
    the rising edge is used as active edge
    the ENCODER_OUT_TIMER CH0CV is used to compute the frequency value
    ------------------------------------------------------------ */
    timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;
	
    rcu_periph_clock_enable(ENCODER_OUT_A_B_RCU_TIMER);

    timer_deinit(ENCODER_OUT_A_B_TIMER);

    /* ENCODER_OUT_TIMER configuration */
    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 36000;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(ENCODER_OUT_A_B_TIMER,&timer_initpara);

		timer_quadrature_decoder_mode_config(ENCODER_OUT_A_B_TIMER, TIMER_QUAD_DECODER_MODE1,
					TIMER_IC_POLARITY_RISING, TIMER_IC_POLARITY_FALLING);
		
		/* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(ENCODER_OUT_A_B_TIMER, TIMER_INT_FLAG_UP);
    /* enable the ENCODER_IN_A_B_TIMER interrupt */
    timer_interrupt_enable(ENCODER_OUT_A_B_TIMER, TIMER_INT_UP);
		
    /* ENCODER_OUT_TIMER counter enable */
    timer_enable(ENCODER_OUT_A_B_TIMER);
		nvic_irq_enable(ENCODER_OUT_A_B_TIMER_IRQn, 0, 0);
}

//*********************************************************//
void EncoderInInit(void)
{
	EncoderInGpioInit();
	EncoderIn_A_B_Init();
	EncoderOut_A_B_Init();
}
