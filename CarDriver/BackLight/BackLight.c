/*
 * BackLight.c
 *
 * Created: 4/23/2025 9:55:04 AM
 * Author: Frederik & Stine
 */ 
#include <avr/io.h>
#include "BackLight.h"
#define F_CPU 16000000UL
#include <util/delay.h>

void BackLight_Test() {
	BackLight_Init();
	for (int i = 0; i < 5; i++){
		BackLight_Off();
		_delay_ms(2500);
		BackLight_OnMedium();
		_delay_ms(2500);
		BackLight_OnBreak();
		_delay_ms(2500);
	}
}

// Initialisere lyset bagp� 
void BackLight_Init(){
	if (DDRB != 0xFF)
		DDRB = 0xFF;
	
	if (PORTB != 0)
		PORTB = 0;
	
    // Nulstiller TCCR1A og TCCR1B f�rst
    TCCR1A = 0;
    TCCR1B = 0;
	
    // Phase Correct PWM, mode 10 (T�lleren g�r op til TOP og ned igen, glatte PWM-overgange) Kigger p� slide 29 og kan se at det er nummer 3 man skal g� ud fra
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << CS11); // Prescaler = 8
	
	ICR1 = 999; // S�tter TOP til 999
}

// T�nder lyset p� mellemstyrke 
void BackLight_OnMedium() {
	OCR1A = 250; // 25% duty cycle (ca. 10 mA)
}

// T�nder lyset p� h�j styrke 
void BackLight_OnBreak(){
	OCR1A = 999; // 100% duty cycle (ca. 50 mA)
}

// Slukker lyset bagp� 
void BackLight_Off() {
	OCR1A = 0;
}