#include <stm32f4xx.h>
#include "onewire.h"
#include "delay.h"

void uart6_tx(char tx);
char uart6_rx(void);
void uart6_set_rate(int rate);

void onewire_init()
{
	//Enable portA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	//Enable USART clock
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
	
	//Configure Tx:PA11 and  Rx:PA12
	GPIOA->MODER |= (0x2<<GPIO_MODER_MODER11_Pos)| (0x2<<GPIO_MODER_MODER12_Pos); //alternate function for both
	GPIOA->AFR[1]|= (0x8<<GPIO_AFRH_AFSEL11_Pos) | (0x8<<GPIO_AFRH_AFSEL12_Pos);  //USART6 alterative function
	GPIOA->PUPDR |= (0x1<<GPIO_PUPDR_PUPD11_Pos) | (0x1<<GPIO_PUPDR_PUPD12_Pos);  //set the pull-up resistor for both pins
	GPIOA->OTYPER|= GPIO_OTYPER_OT11; // make Tx open-drain
	
	//USART Configuration
	USART6->BRR |= 0x683; //set baud rate at 9600
	USART6->CR1 |= USART_CR1_RE | USART_CR1_TE; //enable transmit and receive
	
	//UART single wire half-duplex
	USART6->CR3 |= USART_CR3_HDSEL;
	
	USART6->CR1 |= USART_CR1_UE; //enable the uart
}

uint8_t onewire_transaction_init()
{
	// Set Initiliazation Baud Rate
	uart6_set_rate(9600);
	
	// Transmit a valid Reset-Pulse
	uart6_tx(0xF0);
	
	// Receive Presence-Pulse
	uint8_t pp = uart6_rx();
	
	return pp == 0xF0 ? 0 : 1;
}

uint8_t onewire_read(void)
{
	// Set Read/Write Baud Rate
	uart6_set_rate(115200);
	
	// Issue a Read Time Slot
	uart6_tx(0xFF);
	
	// Receive character from UART
	uint8_t rx = uart6_rx();
	
	// LSB is the return bit of slave
	return rx & 0x1;
}

void onewire_write(uint8_t bit)
{
	// Set Read/Write Baud Rate
	uart6_set_rate(115200);
	
	if(bit)	uart6_tx(0xFF);
	else		uart6_tx(0x00);
}

uint8_t onewire_read_byte(void)
{
	uint8_t byte = 0x00;
	uint8_t bit;
	for(int i=0; i<8; i++)
	{
		bit = onewire_read();
		byte |= (bit<<i);
	}
	
	return byte;
}
void onewire_write_byte(uint8_t byte)
{
	for(int i=0; i<8; i++)
	{
		onewire_write(byte & 1);
		byte = byte >> 1;
	}
}
void uart6_tx(char tx)
{
	// Wait until tx data register is empty
	while(!(USART6->SR & USART_SR_TXE))
		;

	// Transmit Data
	USART6->DR = tx;
	
	// Wait until data transmission is complete
	// because we are in half-duplex mode
	while(!(USART6->SR & USART_SR_TC))
		;

}

char uart6_rx()
{
	// Wait until data is available
	while(!(USART6->SR & USART_SR_RXNE))
		;
	
	return USART6->DR;
}

void uart6_set_rate(int rate)
{
	while(!(USART6->SR & USART_SR_TXE))
		;
	
	USART6->BRR &= 0; //clear previous setup
	if(rate == 9600)
		USART6->BRR |= 0x683;
	else //(baud rate 1152000) 
		USART6->BRR |= 0x8B;
}
