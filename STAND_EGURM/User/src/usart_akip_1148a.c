#include "usart_akip_1148a.h"

char akip_rx_buffer[30];
char akip_tx_buffer[30];

uint8_t akip_code_error = 0;
int16_t set_voltage = 0;
int16_t set_current = 0;
uint8_t set_delta_volt = 0;
uint8_t number_command = 0;
uint8_t set_mode_akip = 0;


//**-- Передача пакета сообщения к АКИП1148 через COM - порт --**//
//**************************************************//
void AkipSend(void)
{
	uint16_t len = GetAkipCommandLength(akip_tx_buffer);
	
	dma_channel_disable(AKIP_DMA, AKIP_DMA_CH_TX);
	
	dma_memory_address_config(AKIP_DMA, AKIP_DMA_CH_TX, (uint32_t)akip_tx_buffer);
	dma_transfer_number_config(AKIP_DMA, AKIP_DMA_CH_TX, len);
	
	/* enable DMA channel1 */
	dma_channel_enable(AKIP_DMA, AKIP_DMA_CH_TX);
	/* USART DMA enable for transmission and reception */
	usart_dma_transmit_config(AKIP_USART, USART_TRANSMIT_DMA_ENABLE);
	/* wait DMA Channel transfer complete */
	//while(RESET == dma_flag_get(AKIP_DMA, AKIP_DMA_CH_TX, DMA_FLAG_FTF));
}

//**-- Взять адрес буффера akip_tx_buffer --**//
//******************************************************//
uint8_t* GetAkipTxBuff(void)
{
	return (uint8_t*)akip_tx_buffer;
}

//----------------------------------------------------------------------
//******************************************************//
void Period1MsForAkip(void)
{
	static uint16_t peiod_ms = 500;
	if(peiod_ms > 0){
		--peiod_ms;
	} else{
		peiod_ms = 500;
		SetParametr();
	}
}

//**-- Определяет длина строки --**//
//******************************************************//
uint8_t GetAkipCommandLength(char* str)
{
	uint8_t len = 0;
	while(str[len] != '\0')
	{
		++len;
	}
	return len;
}

//**-- Пакеты для установки напряжения и тока --**//
//******************************************************//
void SetParametr(void)
{
	int16_t get_voltage = 0;
	
	switch(number_command){
		case 0:
						sprintf(akip_tx_buffer, "SYSTem:ERRor?\n");
						number_command = 1;
						set_delta_volt = 0;
						AkipSend();
		break;
		case 1:
						if(akip_code_error == 0){
							number_command = 2;
						} else{
							sprintf(akip_tx_buffer, "SYSTem:ERRor?\n");
							AkipSend();
							break;
						}
		case 2:
						if(set_mode_akip & (1 << MODE_CURRENT_FLAG)){
							SetCurrentAkip();
						} else if(set_mode_akip == (1 << MODE_VOLTAGE_FLAG)){
							SetVoltageAkip();
						}
		break;
		case 3:
						if(set_mode_akip & (1 << MODE_CURRENT_FLAG)){
							number_command = 0;
						} else if(set_mode_akip == (1 << MODE_VOLTAGE_FLAG)){
							number_command = 2;
						}
		break;
	}
}

//**-- Установить напряжение питания --**//
//**-- voltage = (0 - 500), Uизд. = voltage/10 (0 - 50.0)В
//******************************************************//
void SetVoltageEgurm(int16_t voltage)
{
	SetCurrentEgurm(MAX_CURRENT);
	set_mode_akip |= (1 << MODE_VOLTAGE_FLAG);
	if(voltage < 0){
		VoltageReversOn();
		voltage = -voltage;
	} else{
		VoltageReversOff();
	}
	set_voltage = (voltage > MAX_VOLTAGE ? MAX_VOLTAGE : voltage);
	number_command = 0;
	SetParametr();
}

//**-- Формирования пакета для установки напряжения
//******************************************************//
void SetVoltageAkip(void)
{
	if(set_voltage == 0){
		set_mode_akip &= ~(1 << MODE_VOLTAGE_FLAG);
		number_command = 0;
		sprintf(akip_tx_buffer, "SYST:REM\nOUTP 0\nVOLT 0\n");
		AkipSend();
	} else{
		uint8_t int_volt = (set_voltage + set_delta_volt)/10;
		uint8_t mantis_volt = (set_voltage + set_delta_volt)%10;
		number_command = 3;
		sprintf(akip_tx_buffer, "SYST:REM\nOUTP 1\nVOLT %d.%d\n", int_volt, mantis_volt);
		AkipSend();
	}
}

//**-- Установить ток потребления --**//
//**-- current = (0 - 600), Iизд. = current/10 (0 - 60.0)В
//******************************************************//
void SetCurrentEgurm(uint16_t current)
{
	if(set_current != current){
		set_current = (current > MAX_CURRENT ? MAX_CURRENT : current);
		set_mode_akip |= (1 << MODE_CURRENT_FLAG);
		number_command = 0;
		SetParametr();
	}
}

//**-- Формирования пакета для установки тока
//******************************************************//
void SetCurrentAkip(void)
{
	uint8_t int_curr = set_current/10;
	uint8_t mantis_curr = set_current%10;
	
	sprintf(akip_tx_buffer, "SYST:REM\nCURR %d.%d\n", int_curr, mantis_curr);
	AkipSend();
	set_mode_akip &= ~(1 << MODE_CURRENT_FLAG);
}

//*******************************************************//
void AKIP_USART_IRQHandler_Rx(void)
{
	volatile uint8_t code_error = 0;
	volatile char rx_data;
	
	if(RESET != usart_interrupt_flag_get(AKIP_USART, USART_INT_FLAG_IDLE)){
		/* clear IDLE flag */
		rx_data = usart_data_receive(AKIP_USART);
		
		dma_channel_disable(AKIP_DMA, AKIP_DMA_CH_RX);
		//dma_transfer_number_config(AKIP_DMA, AKIP_DMA_CH_RX, AKIP_DMA_RX_SIZE);
		dma_memory_address_config(AKIP_DMA, AKIP_DMA_CH_RX, (uint32_t)akip_rx_buffer);
		dma_channel_enable(AKIP_DMA, AKIP_DMA_CH_RX);
		
		for(uint8_t i = 0; akip_rx_buffer[i] != ','; ++i){
			code_error *= 10;
			code_error += akip_rx_buffer[i] - '0';
		}
		akip_code_error = code_error;
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


