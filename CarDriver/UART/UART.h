/*
 * UART.h
 *
 * Created: 4/23/2025 9:55:27 AM
 * Author: Frederik & Stine
 */ 


#ifndef UART_H_
#define UART_H_

void UART_Init(unsigned long BaudRate, unsigned char DataBit, unsigned char Rx_Int);
void UART_SendCommand(char* command);
void UART_SendBytes(uint8_t* bytesArr, uint8_t arrLength);

#endif /* UART_H_ */