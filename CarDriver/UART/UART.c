/*
 * UART.c
 *
 * Created: 4/23/2025 9:55:34 AM
 * Author: Frederik & Stine
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "UART.h"

// Target CPU frequency
#define XTAL 16000000

// USART0, og derfor:
// TX = PE1 
// RX = PE0 
// Initialiserer UART 
void UART_Init(unsigned long BaudRate, unsigned char DataBit, unsigned char Rx_Int) {
	if (BaudRate < 300 || BaudRate > 115200)
		return;
	
	if (DataBit < 5 || DataBit > 8)
		return;
	
	UCSR0B = Rx_Int == 0 ? 0b00011000 : 0b10011000;// Sluk/Tænd Interrupts (3) og T?nd RX og TX (2)
	switch (DataBit)
	{
		case 5:
		UCSR0C = 0b00000000; // Asynkron (2), Parity (2), Stop bit (1), Data bits (2)
		break;
		case 6:
		UCSR0C = 0b00000010;
		break;
		case 7:
		UCSR0C = 0b00000100;
		break;
		case 8:
		UCSR0C = 0b00000110;
		break;
	}

	UBRR0 = (XTAL / (16 * BaudRate)) - 1; // Set baud rate
}

// Sender text via UART 
void UART_SendCommand(char* command) {
	while (*command != 0)
	{
		while ((UCSR0A & 0b00100000) == 0){}
		UDR0 = *command;
		command++;
	}
}

// Sender data som bytes via UART
void UART_SendBytes(uint8_t* bytesArr, uint8_t arrLength){
	for (uint8_t i = 0; i < arrLength; i++) {
		while ((UCSR0A & 0b00100000) == 0){}
		UDR0 = bytesArr[i];	
	}
}