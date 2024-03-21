#include "test.h"



void Test(void)
{
	TestSendUartCan();
}


char test_write_ident[] = "Assalam";
char test_read_ident[] = "My Parametr";
char test_write_firmware[] = "Firmware EGURM";
uint8_t mode_uart_can = 0;
void TestSendUartCan(void)
{
	char * test_send_canuart;
	
	if(mode_uart_can == 1){
		test_send_canuart = test_write_ident;
		volatile uint8_t len = sizeof(test_write_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 2;
	} else if(mode_uart_can == 2){
		test_send_canuart = test_read_ident;
		volatile uint8_t len = sizeof(test_read_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 3;
	} else if(mode_uart_can == 3){
		test_send_canuart = test_write_firmware;
		volatile uint8_t len = sizeof(test_write_firmware) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 0;
	} else if(mode_uart_can == 4){
		test_send_canuart = test_write_firmware;
		volatile uint8_t len = 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 0;
	}
}
