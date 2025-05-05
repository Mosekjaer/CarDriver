/*
 * BackLight.c
 *
 * Created: 4/23/2025 9:55:04 AM
 * Author: Frederik & Stine
 */ 
#include <avr/io.h>
#include "BackLight.h"

// Initialisere lyset bagpå 
void BackLight_Init(){
	if (DDRB != 0xFF)
		DDRB = 0xFF;
	
	if (PORTB != 0)
		PORTB = 0;
	
	//Sætter TCCR1B og TCCR1A til 10 bit
	//Har valgt timer 1, og den kører på ben 5.
	TCCR1B = 0b00000000; // Skal vi gøre dette?
	TCCR1A |= (1 << WGM10);
	TCCR1A |= (1 << WGM11);
	
	//Sætter prescaler til no prescaling
	TCCR1B |= (1 << CS10);
	
	//Kigger på slide 29 og kan se at det er nummer 3 man skal gå ud fra
	TCCR1A |= (1 << COM1A1);
}

// Tænder lyset på mellemstyrke 
void BackLight_OnMedium() {
	//Bruger formlen for duty cycle på slide 4 i PWM
	OCR1A = 511; // 50% duty cycle
}

// Tænder lyset på høj styrke 
void BackLight_OnBreak(){
	//Bruger formlen for duty cycle på slide 4 i PWM
	OCR1A = 1023; // 100% duty cycle
}

// Slukker lyset bagpå 
void BackLight_Off() {
	OCR1A = 0;
}