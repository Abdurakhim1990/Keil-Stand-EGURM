#include "stand_control_init.h"

void PeriodUsbReturnInit(void)
{
    /* ----------------------------------------------------------------------------
    TIMER1 Configuration:
    TIMER1 count with external clock, the prescaler is 0, the period is 1000.
    ---------------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;
    //timer_oc_parameter_struct timer_ocinitpara;
	
    rcu_periph_clock_enable(USB_RETURN_PARAM_RCU_TIMER);

    timer_deinit(USB_RETURN_PARAM_TIMER);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 16799;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 5000;//20;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(USB_RETURN_PARAM_TIMER, &timer_initpara);

    
    /* enable the TIMER interrupt */
    //timer_interrupt_enable(USB_RETURN_PARAM_TIMER, TIMER_INT_UP);
		
    timer_enable(USB_RETURN_PARAM_TIMER);
		nvic_irq_enable(TIMER0_UP_TIMER9_IRQn, 0, 0);
}

