/*!
    \file    gd32e50x_it.c
    \brief   interrupt service routines

    \version 2020-03-10, V1.0.0, firmware for GD32E50x
    \version 2020-08-26, V1.1.0, firmware for GD32E50x
    \version 2021-03-23, V1.2.0, firmware for GD32E50x
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32e50x_it.h"
#include "usbd_lld_int.h"
#include "systick.h"

//uint8_t Send_Buffer[4] = {0x00, 0x01, 0x00, 0x01};

extern uint8_t custom_hid_report_send (usb_dev *udev, uint8_t *report, uint16_t len);
extern usb_dev usbd_custom_hid;

extern can_receive_message_struct g_receive_message;
extern FlagStatus can1_receive_flag;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
	
	struct 
  {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
  }*stack_ptr; //Указатель на текущее значение стека(SP)

 
  __asm(
      "TST lr, #4 \n" //Тестируем 3ий бит указателя стека(побитовое И)
      "ITE EQ \n"   //Значение указателя стека имеет бит 3?
      "MRSEQ %[ptr], MSP  \n"  //Да, сохраняем основной указатель стека
      "MRSNE %[ptr], PSP  \n"  //Нет, сохраняем указатель стека процесса
      : [ptr] "=r" (stack_ptr)
      );
	
    while (1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
  delay_decrement();
	Period1ms();
}


/*!
    \brief      this function handles USBD interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void  USBD_LP_CAN0_RX0_IRQHandler(void)
{
    usbd_isr();
}

/*!
    \brief      this function handles USBD interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void  USBD_LP_CAN0_TX0_IRQHandler(void)
{
    usbd_isr();
}

/*!
    \brief      this function handles CAN1 RX0 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN1_RX0_IRQHandler(void)
{
    /* check the receive message */
    can_message_receive(CAN1, CAN_FIFO0, &g_receive_message);
    can1_receive_flag = SET;
}

/*!
    \brief      this function handles CAN1 RX0 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN1_RX1_IRQHandler(void)
{
    /* check the receive message */
    can_message_receive(CAN1, CAN_FIFO1, &g_receive_message);
    can1_receive_flag = SET;
}

/*!
    \brief      this function handles TIMER5 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER5_DAC_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_UP)){
			/* clear channel 0 interrupt bit */
			timer_interrupt_flag_clear(TIMER5, TIMER_INT_UP);
			
			Period10us();
	}
}

/*!
    \brief      this function handles TIMER5 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER6_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_UP)){
			/* clear channel 0 interrupt bit */
			timer_interrupt_flag_clear(TIMER6, TIMER_INT_UP);
			
			Period100us();
	}
}

//***************---AKIP-1148A RS232_Tx---***********//
void AKIP_DMA_Channel_IRQHandler_Tx(void)
{
    if(RESET != dma_interrupt_flag_get(AKIP_DMA, AKIP_DMA_CH_TX, DMA_INT_FLAG_FTF)){
				dma_interrupt_flag_clear(AKIP_DMA, AKIP_DMA_CH_TX, DMA_INT_FLAG_G);
    }
}

//***************---AKIP-1148A RS232_Rx---***********//
void AKIP_DMA_Channel_IRQHandler_Rx(void)
{
    if(RESET != dma_interrupt_flag_get(AKIP_DMA, AKIP_DMA_CH_RX, DMA_INT_FLAG_FTF)){
				dma_flag_clear(AKIP_DMA, AKIP_DMA_CH_RX, DMA_FLAG_G);
				dma_channel_disable(AKIP_DMA, AKIP_DMA_CH_RX);
				
				dma_transfer_number_config(AKIP_DMA, AKIP_DMA_CH_RX, AKIP_DMA_RX_SIZE);
				dma_channel_enable(AKIP_DMA, AKIP_DMA_CH_RX);
				
				dma_interrupt_flag_clear(AKIP_DMA, AKIP_DMA_CH_RX, DMA_INT_FLAG_G);
    }
}


/*!
    \brief      this function handles EXTI0_IRQ Handler.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI0_IRQHandler (void)
{
    
}

/*!
    \brief      this function handles EXTI10_15_IRQ Handler.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI10_15_IRQHandler (void)
{
    
}

