/*
 * FrontLight.c
 *
 * Created: 4/23/2025 9:55:11 AM
 * Author: Frederik & Stine
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "FrontLight.h"

void FrontLight_Test() {
	FrontLight_Init();
	for (int i = 0; i < 10; i++){
		FrontLight_Off();
		_delay_ms(5000);
		FrontLight_On();
		_delay_ms(5000);
	}
}

// Initialiserer lyset foran 
void FrontLight_Init() {
	if (DDRB != 0xFF)
		DDRB = 0xFF;
		
	if (PORTB != 0)
		PORTB = 0;
}

// Tænder lyset foran 
void FrontLight_On() {
	PORTB = PINB | (1 << PB6);
}

// Slukker lyset foran 
void FrontLight_Off() {
	PORTB = PINB & ~(1 << PB6);
}