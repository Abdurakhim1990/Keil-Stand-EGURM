#include "can_egurm.h"

can_trasnmit_message_struct g_transmit_message;
can_receive_message_struct g_receive_message;
FlagStatus can1_receive_flag;

can_trasnmit_message_struct* pGetTransmitMessage(void)
{
	return &g_transmit_message;
}

can_receive_message_struct* pGetReceiveMessage(void)
{
	return &g_receive_message;
}


//*******************************************************//
/*!
    \brief      receive CAN
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CanReceive(void)
{
	uint8_t i = 0;
    /* CAN0 receive data correctly, the received data is printed */
    if(SET == can1_receive_flag) {
        can1_receive_flag = RESET;
        //printf("\r\n can0 receive data:");
        for(i = 0U; i < g_receive_message.rx_dlen; i++){
            //printf(" %02x", g_receive_message.rx_data[i]);
        }
    }
}

//*******************************************************//
/*!
    \brief      receive CAN
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CanTransmit(void)
{
		//uint8_t i = 0U;
    g_transmit_message.tx_efid = CAN_ID_TX_1;
		g_transmit_message.tx_data[0] = 0xA0U;
		g_transmit_message.tx_data[1] = 0xA1U;
		g_transmit_message.tx_data[2] = 0xA2U;
		g_transmit_message.tx_data[3] = 0xA3U;
		g_transmit_message.tx_data[4] = 0xA4U;
		g_transmit_message.tx_data[5] = 0xA5U;
		g_transmit_message.tx_data[6] = 0xA6U;
		g_transmit_message.tx_data[7] = 0xA7U;
		//printf("\r\n can0 transmit data:");
		//for(i = 0; i < g_transmit_message.tx_dlen; i++){
				//printf(" %02x", g_transmit_message.tx_data[i]);
		//}
		/* transmit message */
		can_message_transmit(CAN1, &g_transmit_message);
}
