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
	DDRB = 0xFF;
	PORTB = 0;
	
}

// Tænder lyset foran 
void FrontLight_On() {
	unsigned char mask;
	
	mask = 0b00000001 << 7;
	
	PORTB = PINB | mask;
}

// Slukker lyset foran 
void FrontLight_Off() {
	unsigned char mask;
	
	mask = 0b11111110 << 7;
	
	PORTB = PINB & mask;
}