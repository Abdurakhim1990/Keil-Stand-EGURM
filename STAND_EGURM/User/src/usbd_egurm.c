#include "usbd_egurm.h"

usb_dev usbd_custom_hid;

extern hid_fop_handler fop_handler;

char Send_Buffer[50] = " What is this life if, full of care.  ";

uint8_t length = 38;


//**-- Инициализация USB --**/
//*******************************************************//
void UsbdEgurmInit(void)
{
	/* system clocks configuration */
	rcu_config();

	/* GPIO configuration */
	usb_gpio_config();
	
	custom_hid_itfop_register(&usbd_custom_hid, &fop_handler);

	/* USB device configuration */
	usbd_init(&usbd_custom_hid, &custom_hid_desc, &custom_hid_class);

	/* NVIC configuration */
	nvic_config();

	/* enabled USB pull-up */
	usbd_connect(&usbd_custom_hid);
	
	/* wait for standard USB enumeration is finished */
	while (USBD_CONFIGURED != usbd_custom_hid.cur_status) {
	}
}


//*******************************************************//
void usbSendResponse(void)
{
	static uint16_t temp = 0;
	Send_Buffer[0] = 21;
	Send_Buffer[1] = (uint8_t)temp;
	Send_Buffer[2] = (uint8_t)(temp >> 8);
	//Send_Buffer[3] = (uint8_t)(0xffff - temp);
	//Send_Buffer[4] = (uint8_t)((0xffff - temp) >> 8);
	
	if (USBD_CONFIGURED == usbd_custom_hid.cur_status) {
			
		custom_hid_report_send (&usbd_custom_hid, (uint8_t*)Send_Buffer, LEN_REP_RET_PAR);
			temp++;
	}
}

typedef struct {
	uint16_t send_count;
	uint16_t en_count;
}UsbCounter;
UsbCounter usb_count = {.send_count = 0, .en_count = 0};
//*********************************************//
void SendRetStandParameters(uint8_t* send_buff)
{
	send_buff[0] = REP_RET_PAR;
	++usb_count.send_count;
	if (USBD_CONFIGURED == usbd_custom_hid.cur_status) {
		custom_hid_report_send (&usbd_custom_hid, (uint8_t*)send_buff, LEN_REP_RET_PAR);
		++usb_count.en_count;
	}
}


