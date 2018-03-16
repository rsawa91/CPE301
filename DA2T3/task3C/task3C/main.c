/*
 * task3C.c
 *
 * Created: 3/15/2018 12:57:22 PM
 * Author : sawar1
 */ 

#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
	DDRB = 32;		//ORTB 5 to output
	TCCR0B = 13;	//prescaler to 1024
	TCNT0 = 0;		//TCNT to 0
	OCR0A = 0x00F3;	//max value 
	
	while (1)
	{
		if((TIFR0 & 0b00000001) == 0b00000001)	//overflow
		{
			PORTB = 0xFF;		//set PORTB 
			_delay_ms(250);		//delay about 250ms
			TCNT0 = 0;			//reset counter 
		}
		else
		PORTB = 0x00;		//PORTB to 0
	}
}

