#include "timers_10_and_100_us_init.h"


/*!
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void InitTimerTo10us(void)
{
    /* ----------------------------------------------------------------------------
    TIMER5 Configuration: 
    TIMER5CLK = SystemCoreClock/1 = 168MHz, the period is 1s(168000000/1680 = 10us).
    ---------------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_TIMER5);

    /* deinit a TIMER */
    timer_deinit(TIMER5);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER5 configuration */
    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 1679;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER5, &timer_initpara);

    /* enable the TIMER interrupt */
    timer_interrupt_enable(TIMER5, TIMER_INT_UP);
    /* enable a TIMER */
    timer_enable(TIMER5);
		
		nvic_irq_enable(TIMER5_DAC_IRQn, 0, 0);
}

/*!
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void InitTimerTo100us(void)
{
    /* ----------------------------------------------------------------------------
    TIMER6 Configuration: 
    TIMER6CLK = SystemCoreClock/10 = 16.8MHz, the period is 1s(16800000/1680 = 100us).
    ---------------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_TIMER6);

    /* deinit a TIMER */
    timer_deinit(TIMER6);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER5 configuration */
    timer_initpara.prescaler         = 9;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 1679;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER6, &timer_initpara);

    /* enable the TIMER interrupt */
    timer_interrupt_enable(TIMER6, TIMER_INT_UP);
    /* enable a TIMER */
    timer_enable(TIMER6);
		
		nvic_irq_enable(TIMER6_IRQn, 0, 0);
}
