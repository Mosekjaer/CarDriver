/*
 * Sound.c
 *
 * Created: 4/23/2025 9:55:42 AM
 * Author: Frederik & Stine
 */ 

#include <stdint.h> 
#include "Sound.h"
#include "../UART/UART.h"

// Få high og low bytes af checksum
void Sound_Checksum(uint8_t cmd, uint8_t feedback, uint8_t para1, uint8_t para2, uint8_t* high, uint8_t* low) {
	uint16_t sum = (uint16_t)cmd + (uint16_t)feedback + (uint16_t)para1 + (uint16_t)para2;
	uint16_t checksum = 0xFFFF - sum + 1;

	*high = (uint8_t)((checksum >> 8) & 0xFF);
	*low  = (uint8_t)(checksum & 0xFF);
}

// [0x7E] [CMD] [Feedback] [Para1] [Para2] [ChecksumHigh] [ChecksumLow] [0xEF]
void Sound_SendCommand(uint8_t cmd, uint8_t feedback, uint8_t para1, uint8_t para2) {
	uint8_t checksumHigh, checksumLow;
	Sound_Checksum(cmd, feedback, para1, para2, &checksumHigh, &checksumLow);

	uint8_t frame[8];
	frame[0] = 0x7E;
	frame[1] = cmd;
	frame[2] = feedback;
	frame[3] = para1;
	frame[4] = para2;
	frame[5] = checksumHigh;
	frame[6] = checksumLow;
	frame[7] = 0xEF;

	UART_SendBytes(frame, sizeof(frame));
}

// Initialiserer selve lyd hardwaren 
void Sound_Init() {
	// Lydstyrke til max (30)
	Sound_SendCommand(0x06, 0x00, 0x00, 0x1E);
}

// Afspiller “Free Bird” ved start 
void Sound_PlayStartup() {
	Sound_SendCommand(0x03, 0x00, 0x00, 0x01);
}

// Afspiller “Yes” ved refleksbrik 
void Sound_PlayCheckpoint() {
	Sound_SendCommand(0x03, 0x00, 0x00, 0x02);
}

// Afspiller konfetti-lyd ved mål 
void Sound_PlayFinish() {
	Sound_SendCommand(0x03, 0x00, 0x00, 0x03);
}