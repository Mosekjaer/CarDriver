/*
 * uart.h
 *
 * Created: 4/22/2025 10:05:36 AM
 *  Author: frede
 */ 


#ifndef UART_H_
#define UART_H_

void InitUART(unsigned long BaudRate, unsigned char DataBit, unsigned char Rx_Int);
unsigned char CharReadyy();
char ReadCharr();
void SendCharr(char Tegn);
void SendStringg(char* Streng);
void SendIntegerr(int Tal);

#endif /* UART_H_ */