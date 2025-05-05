/*
 * BackLight.c
 *
 * Created: 4/23/2025 9:55:04 AM
 * Author: Frederik & Stine
 */ 
#include <avr/io.h>
#include "BackLight.h"

// Initialisere lyset bagp� 
void BackLight_Init(){
	if (DDRB != 0xFF)
		DDRB = 0xFF;
	
	if (PORTB != 0)
		PORTB = 0;
	
	//S�tter TCCR1B og TCCR1A til 10 bit
	//Har valgt timer 1, og den k�rer p� ben 5.
	TCCR1B = 0b00000000; // Skal vi g�re dette?
	TCCR1A |= (1 << WGM10);
	TCCR1A |= (1 << WGM11);
	
	//S�tter prescaler til no prescaling
	TCCR1B |= (1 << CS10);
	
	//Kigger p� slide 29 og kan se at det er nummer 3 man skal g� ud fra
	TCCR1A |= (1 << COM1A1);
}

// T�nder lyset p� mellemstyrke 
void BackLight_OnMedium() {
	//Bruger formlen for duty cycle p� slide 4 i PWM
	OCR1A = 511; // 50% duty cycle
}

// T�nder lyset p� h�j styrke 
void BackLight_OnBreak(){
	//Bruger formlen for duty cycle p� slide 4 i PWM
	OCR1A = 1023; // 100% duty cycle
}

// Slukker lyset bagp� 
void BackLight_Off() {
	OCR1A = 0;
}