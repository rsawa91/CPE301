/*
 * DA4Stepper.c
 *
 * Created: 4/16/2018 4:54:54 PM
 *  Author: sawar1
 */ 


#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    int i;
	DDRC = 0xFF;  //DORD output
    while (1) 
    {
		for(i=0; i<5; i++)
		{
			PORTC = 0x0C;
			_delay_ms(1000);
			PORTC = 0x0A;
			_delay_ms(1000);
			PORTC = 0x0;
			_delay_ms(1000);
			PORTC = 0x01;
			_delay_ms(1000);
		}
/*		for(i=0; i<7; i++)
		{
			PORTC = 0x03;
			_delay_ms(50);
			PORTC = 0x09;
			_delay_ms(50);
			PORTC = 0x0C;
			_delay_ms(50);
			PORTC = 0x06;
			_delay_ms(50);
		}*/
    }
}