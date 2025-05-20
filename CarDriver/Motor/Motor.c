/*
 * Motor.c
 *
 * Created: 4/23/2025 9:54:41 AM
 * Author: Frederik & Stine
 */ 

#include <avr/io.h>
#include "Motor.h"

// Initialiserer styring af motoren 
void Motor_Init() {
	PORTB &= ~(1 << PB4); // Motor starter med fremad
	DDRB |= (1 << PB4); // PB4 sættes til output
	
	// Sætter PE4 til 0 i PORTE og PE4 = 1 I DDRE
	DDRE |= (1 << PE4); // PE4 gøres til output for PWM
	
    // Nulstiller TCCR1A og TCCR1B først
    TCCR3A = 0;
    TCCR3B = 0;
	
	//Sætter TCCR3B og TCCR3A til 10 bit
	//Har valgt timer 3, og den kører på ben OC3B (PE4).
	TCCR3A |= (1 << WGM30);
	TCCR3A |= (1 << WGM31);
		
	//Sætter prescaler til no prescaling
	TCCR3B |= (1 << CS30);
		
	//Kigger på slide 29 og kan se at det er nummer 3 man skal gå ud fra (Ikke inverterings mode for OC3B)
	TCCR3A |= (1 << COM3B1);
}

// Sætter hastigheden på motoren (0-100%)
void Motor_SetSpeed(int speed) {
	int motor_speed = (1023 * speed) / 100;
	OCR3B = motor_speed; // duty cycle based on the motor speed provided.
}

// Sætter motoren til at kører fremad 
void Motor_Forward() {
	PORTB = PINB & ~(1 << PB4);
}

// Sætter motoren til at kører baglæns 
void Motor_Reverse(){
	PORTB = PINB | (1 << PB4);
}