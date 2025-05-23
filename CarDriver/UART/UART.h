/****************************************
* "uart.h":                             *
* Header file for Mega2560 UART driver. *
* Using UART 0.                         *
* Henning Hargaard                      *
*****************************************/
void UART_Init();
unsigned char CharReady();
char ReadChar();
void SendCommand(unsigned char *command);
void SendString(char* Streng);
void SendInteger(int Tal);
/****************************************/
