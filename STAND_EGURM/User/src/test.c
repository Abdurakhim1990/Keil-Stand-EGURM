#include "test.h"



void Test(void)
{
	TestSendUartCan();
}


char test_write_ident[] = "Assalam 1111";
char test_read_ident[] = "My Parametr";
char test_write_firmware[] = "Firmware EGURM 2024";
uint8_t mode_uart_can = 0;
uint16_t test_speed_car = 8888;
uint16_t test_speed_engine = 1111;

void TestSendUartCan(void)
{
	char * test_send_canuart;
	
	if(mode_uart_can == 1){
		test_send_canuart = test_write_ident;
		volatile uint8_t len = sizeof(test_write_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 0;
	} else if(mode_uart_can == 2){
		test_send_canuart = test_read_ident;
		volatile uint8_t len = sizeof(test_read_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 0;
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
	} else if(mode_uart_can == 5){
		SetEngineSpeed(test_speed_engine);
		mode_uart_can = 0;
	} else if(mode_uart_can == 6){
		SetCarSpeed(test_speed_car);
		mode_uart_can = 0;
	} else if(mode_uart_can == 7){
		SetZeroWheel(555);
		mode_uart_can = 8;
	} else if(mode_uart_can == 8){
		SetLeftWheel(1111);
		mode_uart_can = 9;
	} else if(mode_uart_can == 9){
		SetRightWheel(5656);
		mode_uart_can = 10;
	} else if(mode_uart_can == 10){
		SetAngleWheel(3546);
		mode_uart_can = 0;
	} else if(mode_uart_can == 11){
		test_send_canuart = test_write_ident;
		volatile uint8_t len = sizeof(test_write_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 12;
	} else if(mode_uart_can == 12){
		test_send_canuart = test_read_ident;
		volatile uint8_t len = sizeof(test_read_ident) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 13;
	} else if(mode_uart_can == 13){
		test_send_canuart = test_write_firmware;
		volatile uint8_t len = sizeof(test_write_firmware) - 1;
		UartcanSend((uint8_t*)test_send_canuart, len, TYPE_WRITE_ID);
		mode_uart_can = 0;
	}
}
