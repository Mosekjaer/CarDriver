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
	if (DDRE != 0xFF)
		DDRE = 0xFF;
		
	if (PORTE != 0)
		PORTE = 0;
		
	//S�tter TCCR1B og TCCR1A til 10 bit
	//Har valgt timer 3, og den k�rer p� ben OC3B (PE4).
	TCCR3B = 0b00000000; 
	TCCR3A |= (1 << WGM10);
	TCCR3A |= (1 << WGM11);
		
	//S�tter prescaler til no prescaling
	TCCR3B |= (1 << CS10);
		
	//Kigger p� slide 29 og kan se at det er nummer 3 man skal g� ud fra
	TCCR3A |= (1 << COM1B1);
}

// S�tter hastigheden p� motoren (0-100%)
void Motor_SetSpeed(int speed) {
	int motor_speed = 1023 * (speed / 100);
	OCR1A = motor_speed; // duty cycle based on the motor speed provided.
}

// S�tter motoren til at k�rer fremad 
void Motor_Forward() {
	PORTB = PINB & ~(1 << PB4);
}

// S�tter motoren til at k�rer bagl�ns 
void Motor_Reverse(){
	PORTB = PINB | (1 << PB4);
}