/*
 * DA3.c
 *
 * Created: 4/5/2018 10:44:55 AM
 * Author : sawar1
 */ 

#define F_CPU 16000000UL		//set clock rate at 16 MHz
#define BAUD 9600				//set baud rate to 9600
#define MYUBRR F_CPU/16/BAUD-1	//set the UBRR number


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

void read_adc(void); // Function Declarations
void adc_init(void);
void USART_init( unsigned int ubrr );
void USART_tx_string(char *data);
volatile unsigned int adc_temp;
char outs[20];

int main(void) {
	
	adc_init();
	USART_init(MYUBRR);
	USART_tx_string("Connected!\r\n");
	_delay_ms(125); 

	sei();
	while(1)
	{
	}
}

void adc_init(void)
{
	/**  ADC **/
	ADMUX = (0<<REFS1)| 

	(1<<REFS0)| // AVcc - external cap at AREF
	(0<<ADLAR)| // ADC Left Adjust Result
	(0<<MUX2)| // ANalog Channel 
	(1<<MUX1)| // ADC2 (PC2 PIN25)
	(0<<MUX0);

	ADCSRA = (1<<ADEN)| // ADC ENable

	(0<<ADSC)| //  Start Conversion
	(0<<ADATE)| // Auto Trigger Enable
	(0<<ADIF)| // Interrupt Flag
	(0<<ADIE)| //  Interrupt Enable
	(1<<ADPS2)| //  Prescaler
	(0<<ADPS1)|
	(1<<ADPS0);

	//  Interrupt Mask Register

	TIMSK1 |= (1<<TOIE1); // enable overflow interrupt
	TCCR1B |= (1<<CS12)|(1<<CS10); // native clock
	TCNT1 = 49911;		//set Timer counter

}

/* READ ADC PINS */
void read_adc(void) {
	unsigned char i =4;
	adc_temp = 0;
	while (i--) {
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp / 8; // Average out samples
	adc_temp = adc_temp /2;
}

/* INIT USART (RS-232) */
void USART_init( unsigned int ubrr ) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0); // Enable receiver
	UCSR0C = (3 << UCSZ00); //asynchronous 8 N 1
}

void USART_tx_string( char *data ) {
	while ((*data != '\0')) {
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

ISR(TIMER1_OVF_vect)
{
	read_adc();		//ADC read function
	snprintf(outs,sizeof(outs),"%3d\r\n", adc_temp);// display it
	USART_tx_string(outs);	//Outputs a string to the screen
	TCNT1 = 49911;			//reset 
}