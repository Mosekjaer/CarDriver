/*
 * uart.c
 *
 * Created: 4/22/2025 10:06:18 AM
 *  Author: frede
 */ 
#include <avr/io.h>
#include <stdlib.h>
#include "uart_int.h"

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
*************************************************************************/
void InitUART(unsigned long BaudRate, unsigned char DataBit, unsigned char Rx_Int)
{  
	if (BaudRate < 300 || BaudRate > 115200)
		return;
		
	if (DataBit < 5 || DataBit > 8)
		return;
	
	UCSR0B = Rx_Int == 0 ? 0b00011000 : 0b10011000;// Sluk/Tænd Interrupts (3) og T�nd RX og TX (2)
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

/*************************************************************************
  Returns 0 (FALSE), if the UART has NOT received a new character.
  Returns value <> 0 (TRUE), if the UART HAS received a new character.
*************************************************************************/
unsigned char CharReadyy()
{
	if ((UCSR0A & 0b10000000) != 0)
		return 1;
		
	return 0;
}

/*************************************************************************
Awaits new character received.
Then this character is returned.
*************************************************************************/
char ReadCharr()
{
	while ((UCSR0A & 0b10000000) == 0){}
	   
	return UDR0;
}

/*************************************************************************
Awaits transmitter register ready.
Then send the character.
Parameter :
	Tegn : Character for sending. 
*************************************************************************/
void SendCharr(char Tegn)
{
	while ((UCSR0A & 0b00100000) == 0){}
	   
	UDR0 = Tegn;
}

/*************************************************************************
Sends 0 terminated string.
Parameter:
   Streng: Pointer to the string. 
*************************************************************************/
void SendStringg(char* Streng)
{
   while (*Streng != 0)
   {
	   SendCharr(*Streng);
	   Streng++;
   }
}

/*************************************************************************
Converts the integer "Tal" to an ASCII string - and then sends this string
using the USART.
Makes use of the C standard library <stdlib.h>.
Parameter:
    Tal: The integer to be converted and sent. 
*************************************************************************/
void SendIntegerr(int Tal)
{
   char arr[7];
   itoa(Tal, arr, 10);
   SendString(arr);
}

/************************************************************************/