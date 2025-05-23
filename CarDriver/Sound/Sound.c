/*
 * sound.c
 *
 * Created: 06-05-2025 13:51:50
 *  Author: juliu
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include "Sound.h"
#include "../UART/UART.h"


void Sound_Init()
{
	//unsigned char Reset[8]={0x7E, 0x0c, 0x00, 0x00, 0x00, 0xFF,0xF4, 0xEF};
	//SendCommand(Reset);
	
	_delay_ms(1000);
	
	// set volume to max
	//unsigned char volumeUp[8]={0x7E, 0x06, 0x00, 0x00, 0x1E, 0xFF, 0xDC, 0xEF };
	unsigned char volumeUp[8]={0x7E, 0x06, 0x00, 0x00, 0x0F, 0xFF, 0xDC, 0xEF };
	SendCommand(volumeUp);
	
	_delay_ms(1000);
	
	// sets media source to SD card
	unsigned char media[8]= {0x7E, 0x09, 0x00, 0x00, 0x02, 0xFF, 0xF5, 0xEF};
	SendCommand(media);
	
	_delay_ms(1000);

}




void sound_PlayStartup(unsigned char track)
{
	
	unsigned char trackA[8]={0x7E, 0x03, 0x00, 0x00, 0x01, 0xFF, 0xFC, 0xEF};
	if(track==1)
	{
		SendCommand(trackA);
		return;
	}
	
	
}


void Sound_PlayCheckpoint(unsigned char track)
{
	unsigned char TrackB[8] = {0x7E, 0x03, 0x00, 0x00, 0x02, 0xFF, 0xFB, 0xEF};
	if (track==2)
	{
		SendCommand(TrackB);
		return;
	}
	
}


void Sound_PlayFinish(unsigned char track)
{
	unsigned char TrackC[8]={0x7E, 0x03, 0x00, 0x00, 0x03, 0xFF, 0xFA, 0xEF};
	if(track == 3)
	{
		SendCommand(TrackC);
		return;
	}
}


