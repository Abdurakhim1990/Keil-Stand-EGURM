#include "can_init_egurm.h"

//*******************************************************//
/*!
    \brief      initialize CAN function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CanConfig(uint16_t baud_rate)
{
    can_parameter_struct can_parameter;

    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
    /* initialize CAN register */
    can_deinit(CAN1);

    /* initialize CAN parameters */
    can_parameter.time_triggered = DISABLE;
    can_parameter.auto_bus_off_recovery = DISABLE;
    can_parameter.auto_wake_up = DISABLE;
    can_parameter.auto_retrans = DISABLE;
    can_parameter.rec_fifo_overwrite = DISABLE;
    can_parameter.trans_fifo_order = DISABLE;
    can_parameter.working_mode = CAN_NORMAL_MODE;
    
		ConfigBaudRateCan(&can_parameter, baud_rate);
	
    /* initialize CAN */
    can_init(CAN1, &can_parameter);

    FilterInit();

    /* configure CAN1 NVIC */
    nvic_irq_enable(CAN1_RX0_IRQn, 0U, 0U);

    /* enable can receive FIFO0 not empty interrupt */
    can_interrupt_enable(CAN1, CAN_INTEN_RFNEIE0);
}

//*******************************************************//
/*!
    \brief      configure CAN1 baud rate
		\param[in]  can_parameter: pointer to a can_parameter_struct structure
		\param[in]	baud_rate:
		\arg					BAUD_RATE_250
    \arg					BAUD_RATE_500
    \param[out] baud_rate:
		\retval     none
*/
void ConfigBaudRateCan(can_parameter_struct *can_parameter, uint16_t baud_rate)
{
		if(baud_rate == BAUD_RATE_250){
				/* configure CAN1 baud rate 250kBps, sample point at 87.5% */
				can_parameter->resync_jump_width = CAN_BT_SJW_1TQ;
				can_parameter->time_segment_1 = CAN_BT_BS1_13TQ;
				can_parameter->time_segment_2 = CAN_BT_BS2_2TQ;
				can_parameter->prescaler = 21U;
		} else if(baud_rate == BAUD_RATE_500){
				/* configure CAN1 baud rate 500kBps, sample point at 87.5% */
				can_parameter->resync_jump_width = CAN_BT_SJW_1TQ;
				can_parameter->time_segment_1 = CAN_BT_BS1_6TQ;
				can_parameter->time_segment_2 = CAN_BT_BS2_1TQ;
				can_parameter->prescaler = 21U;
		}
}

//*******************************************************//
/*!
    \brief      initialize filter CAN
    \param[in]  none
    \param[out] none
    \retval     none
*/
void FilterInit(void)
{
		can_filter_parameter_struct can_filter;
	
		/* initialize filter */
    /* configure filter mode */
    can_filter.filter_mode = CAN_FILTERMODE_LIST;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
		can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable = ENABLE;
    
    /* configure filter ID */
    //*******************************************************************************//
		can_filter.filter_list_high = (uint16_t)(CAN_ID_SAS_CS >> 13);
    can_filter.filter_list_low = (uint16_t)(CAN_ID_SAS_CS << 3)|CAN_IDE_32;
    /* configure filter mask */
    can_filter.filter_mask_high = (uint16_t)(CAN_ID_DM1SINGLE >> 13);
    can_filter.filter_mask_low = (uint16_t)(CAN_ID_DM1SINGLE << 3)|CAN_IDE_32;
    /* select receiver fifo */
    can_filter.filter_number = 15U;
    can_filter_init(CAN1, &can_filter);
		
		//*******************************************************************************//
		can_filter.filter_list_high = (uint16_t)(CAN_ID_DM1BAM >> 13);
    can_filter.filter_list_low = (uint16_t)(CAN_ID_DM1BAM << 3)|CAN_IDE_32;
    /* configure filter mask */
    can_filter.filter_mask_high = (uint16_t)(CAN_ID_DM1PACK >> 13);
    can_filter.filter_mask_low = (uint16_t)(CAN_ID_DM1PACK << 3)|CAN_IDE_32;
    /* select receiver fifo */
    can_filter.filter_number = 16U;
    can_filter_init(CAN1, &can_filter);
		
		//*******************************************************************************//
		can_filter.filter_list_high = (uint16_t)(CAN_ID_DM2SINGLE >> 13);
    can_filter.filter_list_low = (uint16_t)(CAN_ID_DM2SINGLE << 3)|CAN_IDE_32;
    /* configure filter mask */
    can_filter.filter_mask_high = (uint16_t)(CAN_ID_DM2BAM >> 13);
    can_filter.filter_mask_low = (uint16_t)(CAN_ID_DM2BAM << 3)|CAN_IDE_32;
    /* select receiver fifo */
    can_filter.filter_number = 17U;
    can_filter_init(CAN1, &can_filter);
		
		//*******************************************************************************//
		can_filter.filter_list_high = (uint16_t)(CAN_ID_DM2PACK >> 13);
    can_filter.filter_list_low = (uint16_t)(CAN_ID_DM2PACK << 3)|CAN_IDE_32;
    /* configure filter mask */
    can_filter.filter_mask_high = (uint16_t)(CAN_ID_UDS_RESPONSE >> 13);
    can_filter.filter_mask_low = (uint16_t)(CAN_ID_UDS_RESPONSE << 3)|CAN_IDE_32;
    /* select receiver fifo */
    can_filter.filter_number = 18U;
    can_filter_init(CAN1, &can_filter);
}

//*******************************************************//
/*!
    \brief      configure CAN EGURM
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CanEgurmInit(void)
{
		/* configure GPIO */
		CanGpioConfig();
		/* initialize CAN and filter */
		CanConfig(BAUD_RATE_500);
		
		/* initialize transmit message */
		can_trasnmit_message_struct* TransmitMessage = pGetTransmitMessage();
	
		can_struct_para_init(CAN_TX_MESSAGE_STRUCT, TransmitMessage);
		TransmitMessage->tx_sfid = 0x00U;
		TransmitMessage->tx_efid = 0x00U;
		TransmitMessage->tx_ft = CAN_FT_DATA;
		TransmitMessage->tx_ff = CAN_FF_EXTENDED;
		TransmitMessage->tx_dlen = 8U;
		/* initialize receive message */
		can_struct_para_init(CAN_RX_MESSAGE_STRUCT, pGetReceiveMessage());
}

//*******************************************************//
/*!
    \brief      configure GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CanGpioConfig(void)
{
    /* enable CAN clock */
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_CAN1);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);

    /* configure CAN1 GPIO */
    gpio_init(GPIO_CAN, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_CAN_RX);
    gpio_init(GPIO_CAN, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_CAN_TX);

    //gpio_pin_remap_config(GPIO_CAN1_REMAP, ENABLE);
}
