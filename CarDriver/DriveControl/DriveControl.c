/*
 * DriveControl.c
 *
 * Created: 4/23/2025 9:54:34 AM
 * Author: Frederik & Stine
 */ 

#define F_CPU 16000000UL
#include <avr/delay.h>
#include "DriveControl.h"
#include "../FrontLight/FrontLight.h"
#include "../BackLight/BackLight.h"
#include "../UART/UART.h"
#include "../Sound/Sound.h"

// Initialiserer logiken for kørslen 
void DriveControl_Init(){
	BackLight_Init();
	FrontLight_Init();
	UART_Init(9600, 8, 0); // Ingen interrupt for nu
	Sound_Init();
	
	// Test operationer
	BackLight_OnMedium(); //Ekstra
	_delay_ms(5000); //Ekstra for at se om det virkede
	BackLight_OnBreak();
	_delay_ms(5000);
	BackLight_Off(); //Ekstra
}

// Starter kørsel af banen 
void DriveControl_Run(){
	
}