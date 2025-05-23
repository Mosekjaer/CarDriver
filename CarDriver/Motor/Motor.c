/*
 * Motor.c
 *
 * Created: 4/23/2025 9:54:41 AM
 * Author: Frederik & Stine
 */ 

#define F_CPU 16000000UL
#include <avr/delay.h>
#include <avr/io.h>
#include "Motor.h"

void Motor_Test() {
	Motor_Init();
	_delay_ms(500);
	for (int i = 10; i < 50; i++) {
		Motor_SetSpeed(i);
		_delay_ms(500);
	}
	Motor_Reverse();
	for (int i = 50; i > 0; i--) {
		Motor_SetSpeed(i);
		_delay_ms(500);
	}
}

// Initialiserer styring af motoren 
void Motor_Init() {
	PORTB &= ~(1 << PB4); // Motor starter med fremad
	DDRB |= (1 << PB4); // PB4 sættes til output
	
	DDRH |= (1 << PH4); // PH4 gøres til output for PWM
	
    // Nulstiller TCCR3A og TCCR3B først
    TCCR4A = 0;
    TCCR4B = 0;
	
	//Sætter TCCR3B og TCCR3A til 10 bit
	//Har valgt timer 3, og den kører på ben OC3B (PE4).
	TCCR4A |= (1 << WGM40);
	TCCR4A |= (1 << WGM41);
		
	//Sætter prescaler til no prescaling
	TCCR4B |= (1 << CS40);
		
	//Kigger på slide 29 og kan se at det er nummer 3 man skal gå ud fra (Ikke inverterings mode for OC3B)
	TCCR4A |= (1 << COM4B1);
}

// Sætter hastigheden på motoren (0-100%)
void Motor_SetSpeed(int speed) {
	int motor_speed = (1023 * speed) / 100;
	OCR4B = motor_speed; // duty cycle based on the motor speed provided.
}

// Sætter motoren til at kører fremad 
void Motor_Forward() {
	PORTB = PINB & ~(1 << PB4);
}

// Sætter motoren til at kører baglæns 
void Motor_Reverse(){
	PORTB = PINB | (1 << PB4);
}