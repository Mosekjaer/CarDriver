/*
 * UART.c
 *
 * Created: 06-05-2025 13:52:04
 *  Author: Julius
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"

// Target CPU frequency
#define XTAL 16000000

/*************************************************************************
UART 0 initialization:
    Asynchronous mode.
    Baud rate = 9600.
    Data bits = 8.
    RX and TX enabled.
    No interrupts enabled.
    Number of Stop Bits = 1.
    No Parity.
    Baud rate = 9600.
    Data bits = 8.
*************************************************************************/
void UART_Init()
{
	unsigned char ubrr = 103; //(F_CPU / (16 * BAUD)) - 1 |(16000000/(16*9600))-1 = 103.166 ? 103
	
	UBRR1 = ubrr;
	
	UCSR1B = 0b00011000; // disable interrupts og enable tx og rx
	
	UCSR1C = 0b00000110; // s�tter frameformat til 8 data bits, 1 stop bit og ingen paritet
	
	
	
}

/*************************************************************************
  Returns 0 (FALSE), if the UART has NOT received a new character.
  Returns value <> 0 (TRUE), if the UART HAS received a new character.
*************************************************************************/
unsigned char CharReady()
{
	if(UCSR1A & (0b10000000))
	{
		return 1;
	} else {
		return 0;
	}
	
   // <---- Skriv din kode her
}

/*************************************************************************
Awaits new character received.
Then this character is returned.
*************************************************************************/
char ReadChar()
{
	while(!(UCSR1A &(0b10000000)));
	return UDR0;
   // <---- Skriv din kode her
}

/*************************************************************************
Awaits transmitter register ready.
Then send the character.
Parameter :
	Tegn : Character for sending. 
*************************************************************************/
void SendCommand(unsigned char *command)
{

	for (int i = 0; i < 8; i++)
	{
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1 = command[i];
	}
}
	

/*************************************************************************
Sends 0 terminated string.
Parameter:
   Streng: Pointer to the string. 
*************************************************************************/
void SendString( char* Streng)
{
	while(*Streng != '\0')
	{
		SendCommand(*Streng);
		
		Streng++;
	}
   // <---- Skriv din kode her
}

/*************************************************************************
Converts the integer "Tal" to an ASCII string - and then sends this string
using the USART.
Makes use of the C standard library <stdlib.h>.
Parameter:
    Tal: The integer to be converted and sent. 
*************************************************************************/
void SendInteger(int Tal)
{
	char buffer[7];
	
	
	itoa(Tal, buffer, 10);
	
	SendString(buffer);
	
	
   
}

