/*
 * task1C.c
 *
 * Created: 3/15/2018 12:50:39 PM
 * Author : sawar1
 */ 

#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
	DDRB = 32;		//PORTB 5 to output
	TCCR1B = 13;	//set prescaler to 1024
	TCNT1 = 0;		//counter to 0
	OCR1A = 0x00F3;	//set value to count to
	
	while(1)
	{
		if((TIFR1 & 0b00000001) == 0b000000001)	//overflow
		{
			PORTB = 0xFF;		//PORTB to 1
			_delay_ms(250);		//stay on about 250ms
			TCNT1 = 0;			//reset counter to 0
		}
		else
		PORTB = 0x00;		//PORTB to 0
	}
}