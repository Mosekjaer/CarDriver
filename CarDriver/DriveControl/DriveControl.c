/*
 * DriveControl.c
 *
 * Created: 4/23/2025 9:54:34 AM
 * Author: Frederik & Stine
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
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

// Laver ikke blokerende delay metode til baglyset når der bremses.
volatile uint8_t backlight_state = 0;
#define BACKLIGHT_NONE 0
#define BACKLIGHT_BREAK 1

#define BREAK_DURATION_MS 2000  // hvor længe vi vil have break til at være aktiv 
volatile uint32_t break_start_time = 0;

// Laver ikke blokerende delay metode til sound checkpoint når der registreres en brik.
volatile uint8_t sound_state = 0;
#define SOUND_NONE 0
#define SOUND_WAITING 1

#define SOUND_DELAY_MS 1000
volatile uint32_t sound_start_time = 0;


// Bruger timer0 til at styrer break & sound tiden
volatile uint32_t milliseconds = 0;

ISR(TIMER0_COMPA_vect) {
	milliseconds++;
}

void init_timer0() {
	TCCR0A = (1 << WGM01); // CTC mode
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
	OCR0A = 249; // 1 ms ved 16 MHz
	TIMSK0 |= (1 << OCIE0A); // Aktiver compare match interrupt
}

void Motor_SmoothDirectionChange(uint16_t target_speed, uint8_t direction, uint16_t current_speed) {
	uint8_t step = 50;
	uint8_t min_speed = 250;

	// vi laver en glidende nedbremsning
	while (current_speed > min_speed) {
		Motor_SetSpeed(current_speed);
		_delay_ms(100);
		current_speed -= step;
		if (current_speed < min_speed) current_speed = min_speed;
	}

	// vi skifter retning
	if (direction == 0) Motor_Forward();
	else Motor_Reverse();

	// vi laver en glidende opstart
	current_speed = min_speed;
	while (current_speed < target_speed) {
		Motor_SetSpeed(current_speed);
		_delay_ms(100);
		current_speed += step;
		if (current_speed > target_speed) current_speed = target_speed;
	}

	Motor_SetSpeed(target_speed);
}

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
	 
	 // Styring af tid og interrupts
	 init_timer0();
	 setup_interrupt();
	 sei();
}

void handle_checkpoint(unsigned int cp) {
	if (cp != 0) {
		Sound_PlayCheckpoint(2);
	}
		
	switch(cp) {
		case 0:
		BackLight_OnMedium();
		FrontLight_On();
		Motor_SetSpeed(306);
		break;
		case 2:
		Motor_SetSpeed(1000);
		break;
		case 3:
		Motor_SetSpeed(153);
		break;
		case 4:
		Motor_SetSpeed(400); //32
		break;
		case 6:
		BackLight_OnBreak();
		Motor_SmoothDirectionChange(600, 1, 400);  // 1 for bagleems
		break_start_time = milliseconds;
		backlight_state = BACKLIGHT_BREAK;
		break;
		case 8:
		// Vent med at køre forlængs så den kan registrere brikken igen.
		Motor_SmoothDirectionChange(700, 0, 600);  // 0 for forlængs
		break;
		case 11:
		// Vent lidt med at bremse til vi er i mål.
		_delay_ms(300);
		BackLight_OnBreak();
		Motor_SetSpeed(0);
		break_start_time = milliseconds;
		backlight_state = BACKLIGHT_BREAK;
		break;
		default:
		break;
	}
	
	if (cp < 11) {
		sound_state = SOUND_WAITING;
		sound_start_time = milliseconds;
	}
}


// Starter kørsel af banen 
void DriveControl_Run(){
	if (!start_triggered && switchOn(7)) {
		start_triggered = 1;
		sound_PlayStartup(1);
		_delay_ms(5000); // Vi skal holde og spille lidt inden vi kører
	}
	if (!start_triggered) return;
	
	if (checkpoint_counter > max_checkpoint_handled || max_checkpoint_handled == -1) {
		handle_checkpoint(checkpoint_counter);
		max_checkpoint_handled = checkpoint_counter;
	}
	
	// Sæt backlight tilbage til medium når backlight timer er ovre.
	if (backlight_state == BACKLIGHT_BREAK && (milliseconds - break_start_time >= BREAK_DURATION_MS))
	{
		if (checkpoint_counter != 11) 
		{	
			BackLight_OnMedium();
		}
		else
		{
			BackLight_Off();
			FrontLight_Off();
			Sound_PlayFinish(3);
		}
			
		backlight_state = BACKLIGHT_NONE;
	}
		
	// Fjern checkpoint sound når tiden er gået. 
	if (sound_state == SOUND_WAITING && (milliseconds - sound_start_time >= SOUND_DELAY_MS) && checkpoint_counter != 0) {
		sound_PlayStartup(1);
		sound_state = SOUND_NONE;
	}
}