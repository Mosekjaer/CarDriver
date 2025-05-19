/*
 * DriveControl.c
 *
 * Created: 4/23/2025 9:54:34 AM
 * Author: Frederik & Stine
 */ 

#define F_CPU 16000000UL
#include <avr/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "DriveControl.h"
#include "../FrontLight/FrontLight.h"
#include "../BackLight/BackLight.h"
#include "../UART/UART.h"
#include "../Sound/Sound.h"
#include "../Motor/Motor.h"
#include "../Switch/switch.h"

#define RUN_TESTS 0
#define DEBOUNCE_TICKS 31250 // 2 sekunder ved prescaler 1024

// Brug af volatile for at undgå eventuelle compiler optimeringer da vi bruger den i både ISR og main loop.
volatile unsigned int checkpoint_counter = 0;
volatile unsigned int max_checkpoint_handled = 0;
volatile char start_triggered = 0;

void setup_timer3_for_debounce() {
	TCCR3A = 0;
	TCCR3B = 0;

	TCCR3B |= (1 << WGM32); // CTC mode (WGM32 = 1)
	TCCR3B |= (1 << CS32) | (1 << CS30); // Prescaler = 1024

	OCR3A = DEBOUNCE_TICKS;
	TCNT3 = 0;

	TIMSK3 |= (1 << OCIE3A); // Aktiver Compare Match A interrupt
}


void setup_interrupt() {
	EICRA |= (1 << ISC01); // ISC01 = 1, ISC00 = 0 ? Falling edge og IKKE rising edge da det trigger ved at gå low.
	EICRA &= ~(1 << ISC00);
	
	// Ryd interrupt flager for INT0 og INT1
	EIFR |= (1 << INTF0);
	EIFR |= (1 << INTF1);
	
	EIMSK |= (1 << INT0); // Aktiver INT0 benet på arduino
	EIMSK |= (1 << INT1); // Aktiver INT1 benet på arduino
}

void handle_interrupt() {
	// Deaktiver både INT0 og INT1 midlertidigt
	EIMSK &= ~(1 << INT0);
	EIMSK &= ~(1 << INT1);

	checkpoint_counter++;

	setup_timer3_for_debounce();
}

ISR(INT0_vect) {
	handle_interrupt();
}

ISR(INT1_vect) {
	handle_interrupt();
}

ISR(TIMER3_COMPA_vect) {
	TCCR3B = 0;                     // Stop Timer3
	TIMSK3 &= ~(1 << OCIE3A);       // Slå compare match fra
	
	// Ryd interrupt flaget for INT0 og INT1 før genaktivering for at fjerne ventende interupts der måtte være for at forhindre ekstra trigger
	EIFR |= (1 << INTF0);
	EIFR |= (1 << INTF1);
		
	EIMSK |= (1 << INT0);           // Genaktiver INT0
	EIMSK |= (1 << INT1);			// Genaktiver INT1
}

// Initialiserer logiken for kørslen 
void DriveControl_Init(){
#if !RUN_TESTS
	// Initialisering af drivers
	initSwitchPort();
	BackLight_Init();
	FrontLight_Init();
	Motor_Init();
	UART_Init(9600, 8, 0);
	Sound_Init();
	setup_interrupt();
	sei();
	
	// Sætter start værdier
	BackLight_OnMedium();
	FrontLight_On();
	Motor_SetSpeed(50);
#else
	BackLight_Test();
	FrontLight_Test();
#endif
}

void handle_checkpoint(unsigned int cp) {
	switch(cp) {
		case 2:
		Motor_SetSpeed(100);
		break;
		case 3:
		Motor_SetSpeed(50);
		break;
		case 6:
		Motor_SetSpeed(0);
		BackLight_OnBreak();
		Motor_Reverse();
		Motor_SetSpeed(50);
		BackLight_OnMedium();
		break;
		case 8:
		Motor_SetSpeed(0);
		Motor_Forward();
		Motor_SetSpeed(50);
		break;
		case 11:
		Motor_SetSpeed(0);
		FrontLight_Off();
		BackLight_Off();
		break;
		default:
		break;
	}
}


// Starter kørsel af banen 
void DriveControl_Run(){
	if (!start_triggered && switchOn(7)) {
		start_triggered = 1;
	}
	if (!start_triggered) return;
	
	if (checkpoint_counter > max_checkpoint_handled) {
		handle_checkpoint(checkpoint_counter);
		max_checkpoint_handled = checkpoint_counter;
	}
}