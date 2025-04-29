/*
 * DriveControl.c
 *
 * Created: 4/23/2025 9:54:34 AM
 * Author: Frederik & Stine
 */ 

#define F_CPU 16000000UL
#include <avr/delay.h>
#include "DriveControl.h"
//#include "../FrontLight/FrontLight.h"
#include "../BackLight/BackLight.h"

// Initialiserer logiken for kørslen 
void DriveControl_Init(){
	BackLight_Init();
	BackLight_OnMedium(); //Ekstra
	_delay_ms(5000); //Ekstra for at se om det virkede
	BackLight_OnBreak();
	_delay_ms(5000);
	BackLight_Off(); //Ekstra
}

// Starter kørsel af banen 
void DriveControl_Run(){
	
}