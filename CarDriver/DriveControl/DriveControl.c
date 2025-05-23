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

#define RUN_TESTS 1
#define DEBOUNCE_TICKS (int)(31250 / 3.5) // 2 sekunder ved prescaler 1024

// Brug af volatile for at undgå eventuelle compiler optimeringer da vi bruger den i både ISR og main loop.
volatile unsigned int checkpoint_counter = 0;
volatile int max_checkpoint_handled = -1;
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
	// INT0 falling edge: ISC01 = 1, ISC00 = 0
	//EICRA &= ~((1 << ISC01) | (1 << ISC00)); // Clear begge bits først
	//EICRA |=  (1 << ISC01);                  // Sæt kun ISC01

	// INT1 falling edge: ISC11 = 1, ISC10 = 0
	//EICRA &= ~((1 << ISC11) | (1 << ISC10)); // Clear begge bits først
	//EICRA |=  (1 << ISC11);                  // Sæt kun ISC11s
	
	EICRA &= ~((1 << ISC01) | (1 << ISC00)); // Clear INT0 bits
	EICRA |=  ((1 << ISC01) | (1 << ISC00)); // Set to rising edge

	EICRA &= ~((1 << ISC11) | (1 << ISC10)); // Clear INT1 bits
	EICRA |=  ((1 << ISC11) | (1 << ISC10)); // Set to rising edge

	
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
	 //Initialisering af drivers
	 initSwitchPort();
	 BackLight_Init();
	 FrontLight_Init();
	 Motor_Init();
	 UART_Init(9600, 8, 0);
	 Sound_Init();
	 setup_interrupt();
	 sei();
	 //Sætter start værdier
	 BackLight_OnMedium();
	 FrontLight_On();
	
	
	//FrontLight_Test();
	//BackLight_Test();
}

void handle_checkpoint(unsigned int cp) {
	Sound_PlayCheckpoint(2);
	SendStringg("Checkpoint triggered!");
	SendCharr(cp);
	SendStringg("\r\n");
	switch(cp) {
		case 0:
		Motor_SetSpeed(30);
		break;
		case 2:
		Motor_SetSpeed(40);
		break;
		case 3:
		Motor_SetSpeed(15);
		break;
		case 4:
		Motor_SetSpeed(30);
		break;
		case 6:
		_delay_ms(500);
		BackLight_OnBreak();
		Motor_SetSpeed(0);
		Motor_Reverse();
		Motor_SetSpeed(30);
		_delay_ms(500);
		BackLight_OnMedium();
		break;
		case 8:
		_delay_ms(500);
		Motor_SetSpeed(0);
		Motor_Forward();
		Motor_SetSpeed(32);
		break;
		case 11:
		_delay_ms(3000);
		BackLight_OnBreak();
		Motor_SetSpeed(0);
		FrontLight_Off();
		Sound_PlayFinish(3);
		_delay_ms(1000);
		BackLight_Off();
		break;
		default:
		break;
	}
	_delay_ms(1000);
	
	if (cp < 11)
		sound_PlayStartup(1);
}


// Starter kørsel af banen 
void DriveControl_Run(){
	if (!start_triggered && switchOn(7)) {
		start_triggered = 1;
		sound_PlayStartup(1);
	}
	if (!start_triggered) return;
	
	if (checkpoint_counter > max_checkpoint_handled || max_checkpoint_handled == -1) {
		handle_checkpoint(checkpoint_counter);
		max_checkpoint_handled = checkpoint_counter;
	}
}