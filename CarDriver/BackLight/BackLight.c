/*
 * BackLight.c
 *
 * Created: 4/23/2025 9:55:04 AM
 * Author: Frederik & Stine
 */ 
#include <avr/io.h>
#include "BackLight.h"
#define F_CPU 16000000UL
#include <avr/delay.h>

void BackLight_Test() {
	BackLight_Init();
	for (int i = 0; i < 5; i++){
		BackLight_Off();
		_delay_ms(5000);
		BackLight_OnMedium();
		_delay_ms(5000);
		BackLight_OnBreak();
		_delay_ms(5000);
	}
}

// Initialisere lyset bagpå 
void BackLight_Init(){
	if (DDRB != 0xFF)
		DDRB = 0xFF;
	
	if (PORTB != 0)
		PORTB = 0;
	
    // Nulstiller TCCR1A og TCCR1B først
    TCCR1A = 0;
    TCCR1B = 0;
	
    // Phase Correct PWM, mode 10 (Tælleren går op til TOP og ned igen, glatte PWM-overgange) Kigger på slide 29 og kan se at det er nummer 3 man skal gå ud fra
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << CS11); // Prescaler = 8
	
	ICR1 = 999; // Sætter TOP til 999
}

// Tænder lyset på mellemstyrke 
void BackLight_OnMedium() {
	//Bruger formlen for duty cycle på slide 4 i PWM
	OCR1A = 500; // 25% duty cycle
}

// Tænder lyset på høj styrke 
void BackLight_OnBreak(){
	//Bruger formlen for duty cycle på slide 4 i PWM
	OCR1A = 999; // 100% duty cycle
}

// Slukker lyset bagpå 
void BackLight_Off() {
	OCR1A = 0;
}