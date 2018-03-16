/*
 * task2C.c
 *
 * Created: 3/15/2018 12:54:18 PM
 * Author : sawar1
 */ 

#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
	DDRD = 0x00;	//DDRD to read input
	DDRB = 0xFF;	//DDRB to output
	
	while(1)
	{
		if((PIND & 0b00000001) == 0b00000001)	//button was pressed
		{
			PORTB |= 0b00000010;	//PORTB 1 to output
			_delay_ms(250);			//delay 250ms
		}
		else
		PORTB &= 0b11111101;	//toggle PORTB output
	}
}
