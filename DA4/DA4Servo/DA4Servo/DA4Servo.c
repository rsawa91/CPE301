/*
 * DA4Servo.c
 *
 * Created: 4/16/2018 4:56:37 PM
 *  Author: sawar1
 */ 


#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	int i;           //controlling loops 
	DDRD = 0xFF;     //PD0 to PD7
	TCCR0B = 3;      //64 prescaler
	TCCR0A = 0x83;   //clear OCR0A
	
	while (1)
	{
		for(i=0; i<10; i++)
		{
			OCR0A = 15;
			_delay_ms(1000);
			OCR0A = 30;
			_delay_ms(1000);
		}
	}
}