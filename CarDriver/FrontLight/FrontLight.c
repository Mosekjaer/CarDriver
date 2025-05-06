/*
 * FrontLight.c
 *
 * Created: 4/23/2025 9:55:11 AM
 * Author: Frederik & Stine
 */ 

#include <avr/io.h>
#include "FrontLight.h"

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