#include "debug.h"


uint8_t debug_rx_buffer[DEBUG_RX_SIZE];


struct {
	char tx_buffer[DEBUG_TX_SIZE];
	uint16_t ind_begin;
	uint16_t ind_end;
	uint16_t count;
} debug_uart_tx;

__IO uint16_t debug_rxcount = 0;

//***********************************************************
void UART4_IRQHandler(void)
{
#ifdef DEBUG
	if(RESET != usart_interrupt_flag_get(DEBUG_USART, USART_INT_FLAG_RBNE)){
		usart_interrupt_flag_clear(DEBUG_USART, USART_INT_FLAG_RBNE);
		if(!debug_rxcount){
			for(uint8_t i = 0; i < DEBUG_RX_SIZE; ++i){
				debug_rx_buffer[i] = 0;
			}
		}
		char data = usart_data_receive(DEBUG_USART);
		debug_rx_buffer[debug_rxcount++] = data;
		debug_rxcount %= DEBUG_RX_SIZE;
		if(data == 0x0D){
			debug_rxcount = 0;
		}
	}
	
	if(RESET != usart_interrupt_flag_get(DEBUG_USART, USART_INT_FLAG_TBE)){
		usart_interrupt_flag_clear(DEBUG_USART, USART_INT_FLAG_TBE);
		
		if(debug_uart_tx.count){
			uint16_t data = (uint16_t)debug_uart_tx.tx_buffer[debug_uart_tx.ind_begin++];
			usart_data_transmit(DEBUG_USART, data);
			debug_uart_tx.ind_begin %= DEBUG_TX_SIZE;
			--debug_uart_tx.count;
		} else {
			debug_uart_tx.ind_begin = 0;
			debug_uart_tx.ind_end = 0;
			usart_interrupt_disable(DEBUG_USART, USART_INT_TBE);
		}
	}
#endif
}

void PrintDebug(char* ch)
{
#ifdef DEBUG
	for(uint8_t ind = 0; ch[ind] != '\0'; ++ind){
		debug_uart_tx.tx_buffer[debug_uart_tx.ind_end++] = ch[ind];
		debug_uart_tx.ind_end %= DEBUG_TX_SIZE;
		++debug_uart_tx.count;
	}
	usart_interrupt_enable(DEBUG_USART, USART_INT_TBE);
#endif
}

void PrintDebugLF(char* ch)
{
#ifdef DEBUG
	for(uint8_t ind = 0; ch[ind] != '\0'; ++ind){
		debug_uart_tx.tx_buffer[debug_uart_tx.ind_end++] = ch[ind];
		debug_uart_tx.ind_end %= DEBUG_TX_SIZE;
		++debug_uart_tx.count;
	}
	char line_feed[] = {"\n"};
	for(uint8_t ind = 0; line_feed[ind] != '\0'; ++ind){
		debug_uart_tx.tx_buffer[debug_uart_tx.ind_end++] = line_feed[ind];
		debug_uart_tx.ind_end %= DEBUG_TX_SIZE;
		++debug_uart_tx.count;
	}
	usart_interrupt_enable(DEBUG_USART, USART_INT_TBE);
#endif
}


uint64_t DebugPow10(uint8_t pow)
{
#ifdef DEBUG
	uint64_t res = 1;
	for(uint8_t i = 0; i < pow; ++i){
		res *= 10;
	}
	return res;
#else
	return 0;
#endif
}

void PrintDebugNumber(int32_t num)
{
#ifdef DEBUG
	char ch[12];
	uint8_t index = 0;
	uint8_t flag_null = 0;
	
	if(num < 0){
		ch[index++] = '-';
		num = -num;
	}
	for(int8_t i = 10; i != 0;){
		num %= DebugPow10(i);
		uint8_t sym = num / DebugPow10(--i);
		if(sym || flag_null || i == 0){
			ch[index++] = sym + '0';
			flag_null = 1;
		}
	}
	ch[index] = '\0';
	PrintDebug(ch);
#endif
}

void PrintDebugNumberLF(int32_t num)
{
#ifdef DEBUG
	PrintDebugNumber(num);
	PrintDebugLF("");
#endif
}

void PrintDebugCommentAndNumber(char*ch, int32_t num)
{
#ifdef DEBUG
	PrintDebug(ch);
	PrintDebug(" -- ");
	PrintDebugNumber(num);
#endif
}

void PrintDebugCommentAndNumberLF(char* ch, int32_t num)
{
#ifdef DEBUG
	PrintDebugCommentAndNumber(ch, num);
	PrintDebugLF("");
#endif
}
