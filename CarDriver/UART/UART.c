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
void UART_Init(unsigned long baudRate, unsigned char dataBit, unsigned char enableRxInterrupt) {
	if (baudRate < 300 || baudRate > 115200)
		return;
	
	if (dataBit < 5 || dataBit > 8)
		return;
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Aktiver RX og TX
	if (enableRxInterrupt) {
		UCSR0B |= (1 << RXCIE0); // Aktiver RX Interrupt
	}
	
	// Sæt data bits (i UCSZ0 og UCSZ1 i UCSR0C)
	switch (dataBit) {
		case 5:
		UCSR0C = (0 << UCSZ01) | (0 << UCSZ00);
		break;
		case 6:
		UCSR0C = (0 << UCSZ01) | (1 << UCSZ00);
		break;
		case 7:
		UCSR0C = (1 << UCSZ01) | (0 << UCSZ00);
		break;
		case 8:
		UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
		break;
	}

	UBRR0 = (XTAL / (16 * baudRate)) - 1; // Set baud rate
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