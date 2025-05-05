/************************************
* "Switch.c"                        *
* Implementation for SWITCH driver. *
* Henning Hargaard                  *
************************************/
#include <avr/io.h>

#define MAX_SWITCH_NUM 7

// Klargør switch-porten
void initSwitchPort()
{
	DDRA = 0x00;
}

// Læser alle switches samtidigt
unsigned char switchStatus()
{
	return ~PINA;
}

// Returnerer TRUE, hvis switchen med nummeret
// "switch_nr" er aktiveret - ellers returneres FALSE
unsigned char switchOn(unsigned char switch_nr)
{
	if (switch_nr > MAX_SWITCH_NUM)
		return 0;
	
	unsigned char mask = 1 << switch_nr;
	if ((PINA & mask) == 0)
		return 1;
		
	return 0;
}