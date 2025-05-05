/************************************
* "Switch.h"                        *
* Header file for SWITCH driver.    *
* Henning Hargaard                  *
************************************/
void initSwitchPort();
unsigned char switchStatus();
unsigned char switchOn(unsigned char switch_nr);
