#ifndef	USBD_EGURM_H
#define USBD_EGURM_H

/* Ссылка описания дескрипторов
* https://microsin.net/programming/arm-working-with-usb/usb-in-a-nutshell-part2.html
*/

#include "main.h"
#include "custom_hid_core.h"
#include "usbd_hw.h"


void UsbdEgurmInit(void);
void usbSendResponse(void);
void SendRetStandParameters(uint8_t* send_buff);

#endif