#define F_CPU 8000000UL
/**
*/

#define F_CPU 8000000UL		//Set clk speed to 8 Mhz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "drive_motor.h"
#include "uart.h"


int main(void){
	// initialize WIFI 
	initUART();
	init_motor();
	

}
