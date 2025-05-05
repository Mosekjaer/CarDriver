/*
 * Motor.c
 *
 * Created: 4/23/2025 9:54:41 AM
 * Author: Frederik & Stine
 */ 

#include "Motor.h"

// Initialiserer styring af motoren 
void Motor_Init() {
	if (DDRE != 0xFF)
		DDRE = 0xFF;
		
	if (PORTE != 0)
		PORTE = 0;
		
	//Sætter TCCR1B og TCCR1A til 10 bit
	//Har valgt timer 3, og den kører på ben OC3B (PE4).
	TCCR3B = 0b00000000; // Skal vi gøre dette?
	TCCR3B |= (1 << WGM10);
	TCCR3B |= (1 << WGM11);
		
	//Sætter prescaler til no prescaling
	TCCR3B |= (1 << CS10);
		
	//Kigger på slide 29 og kan se at det er nummer 3 man skal gå ud fra
	TCCR3B |= (1 << COM1A1);
}

// Sætter hastigheden på motoren (0-100%)
void Motor_SetSpeed(int speed) {
	int motor_speed = 1023 * (speed / 100);
	OCR1A = motor_speed; // duty cycle based on the motor speed provided.
}

// Sætter motoren til at kører fremad 
void Motor_Forward() {
	PORTB = PINB | (0 << PB4);
}

// Sætter motoren til at kører baglæns 
void Motor_Reverse(){
	PORTB = PINB | (1 << PB4);
}

// Stopper motoren
void Motor_Stop() {
	OCR1A = 0; // duty cycle set to 0 to stop motor
}