#include "test.h"



void Test(void)
{
	TestSendUartCan();
}


char test_write_ident[] = "Assalam";
uint8_t mode_uart_can = 0;
void TestSendUartCan(void)
{
	if(mode_uart_can == 1){
		volatile uint8_t len = sizeof(test_write_ident) - 1;
		UartcanSend((uint8_t*)test_write_ident, len, TYPE_WRITE_ID);
		//mode_uart_can = 0;
	}
		
}
