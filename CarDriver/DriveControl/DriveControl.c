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

#define DEBOUNCE_TICKS 31250 // 2 sekunder ved prescaler 1024

unsigned int checkpoint_counter = 0;

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
	EIMSK |= (1 << INT0); // Aktiver INT0 benet på arduino
	EIMSK |= (1 << INT1); // Aktiver INT1 benet på arduino
}

void handle_interrupt() {
	// Deaktiver både INT0 og INT1 midlertidigt
	EIMSK &= ~(1 << INT0);
	EIMSK &= ~(1 << INT1);

	// Frontlight on er for testing
	FrontLight_On();
	checkpoint_counter++;
	    
	// Skriver counter ud til Tera Term (Terminal program) for debugging.
	char arr[7];
	itoa(checkpoint_counter, arr, 10);
	UART_SendCommand(arr);

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
	
	// Bruger frontlight off til test sluk af led.
	FrontLight_Off();

	EIMSK |= (1 << INT0);           // Genaktiver INT0
	EIMSK |= (1 << INT1);
}

// Initialiserer logiken for kørslen 
void DriveControl_Init(){
	BackLight_Init();
	FrontLight_Init();
	UART_Init(9600, 8, 0); // Ingen interrupt for nu
	Sound_Init();
	setup_interrupt();
	sei();
	
	
	//// Test operationer
	//BackLight_OnMedium(); //Ekstra
	//_delay_ms(5000); //Ekstra for at se om det virkede
	//BackLight_OnBreak();
	//_delay_ms(5000);
	//BackLight_Off(); //Ekstra
}

// Starter kørsel af banen 
void DriveControl_Run(){
	
}