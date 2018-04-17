/*
 * DA4DC.c
 *
 * Created: 4/16/2018 4:53:02 PM
 *  Author: sawar1
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int check = 1;

ISR(INT0_vect)
{
	if(check == 1)
	check = 0;
	else
	check = 1;
}

int main(void)
{
	DDRD = 0xFF;
	DDRC = 0xFF;
	PORTC = 0xFF;
	TCCR0B = 5;
	TCCR0A = 0x83;
	
	EICRA = 0x03;
	EIMSK = (1 << INT0);
	EIFR = (1 << INTF0);
	sei();
	
	while (1)
	{
		if(check == 1)
		{
			PORTC = 1;
			OCR0A = 50;
		}
		else
		{
			PORTC = 0;
		}
	}
}