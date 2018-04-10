/*
 * Midterm1.c
 *
 * Created: 4/10/2018 1:02:37 PM
 *  Author: sawar1
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

void usart_send( unsigned char ascii)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = ascii;
}
unsigned char usart_receive(void)
{
	while (!(UCSR0A & (1<<RXCIE0)));
	return UDR0;
}
void send_AT( unsigned char message[])
{
	unsigned char i=0;
	while(message[i] != '\0')
	{
		usart_send(message[i]); //sends data to esp
		i++;
	}
}

int main(void) {
	
	unsigned char AT[] = "AT\r\n";
	unsigned char CIPMUX[] = "AT+CIPMUX=1\r\n";
	unsigned char CIPSTART[] = "AT+CIPSTART=0,\"TCP\",\"api.thingspeak.com\",80\r\n";
	unsigned char CIPSEND[] = "AT+CIPSEND=0,110\r\n";
	unsigned char GET_DATA[] = "GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=SBYXUXDC8TLMA50V\r\n";
	unsigned char SEND_DATA[] = "GET https://api.thingspeak.com/update?api_key=RLIBH668P4MZTRB6=50\r\n";
	
	USART_init(MYUBRR); // Initialize the USART 
	USART_tx_string("Connected!\r\n"); 
	_delay_ms(125); //wait 
	
	_delay_ms(200);
	send_AT(AT);
	_delay_ms(2000);
	send_AT(CIPMUX);
	_delay_ms(2000);
	send_AT(CIPSTART);
	_delay_ms(2000);
	send_AT(GET_DATA);
	_delay_ms(2000);
	send_AT(SEND_DATA);
	
	adc_init(); // Initialize the ADC 

	sei();

	while(1)
	{
	}
}

void adc_init(void)
{
	/** Setup & enable ADC **/
	ADMUX = (0<<REFS1)| // Reference Selection Bits

	(1<<REFS0)| // AVcc - external cap at AREF
	(0<<ADLAR)| // ADC Left Adjust Result
	(0<<MUX2)| // ANalog Channel Selection Bits
	(1<<MUX1)| // ADC2 (PC2 PIN25)
	(0<<MUX0);

	ADCSRA = (1<<ADEN)| // ADC enable

	(0<<ADSC)| //Start Conversion
	(0<<ADATE)| //Auto Trigger Enable
	(0<<ADIF)| //Interrupt Flag
	(0<<ADIE)| //Interrupt Enable
	(1<<ADPS2)| //Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);

	// Timer/Counter1 Interrupt Mask Register

	TIMSK1 |= (1<<TOIE1); // enable overflow interrupt
	TCCR1B |= (1<<CS12)|(1<<CS10); // native clock
	TCNT1 = 49911;		//set Timer counter

}

/* READ PINS */
void read_adc(void) {
	unsigned char i =4;
	adc_temp = 0;
	while (i--) {
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp / 8; // Average
	adc_temp = adc_temp /2;
}

/* INIT USART */
void USART_init( unsigned int ubrr ) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0); // Enable receiver, transmitter & RX interrupt
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
	read_adc();		//call read function
	snprintf(outs,sizeof(outs),"%3d\r\n", adc_temp);// print
	USART_tx_string(outs);	//Outputs screen
	TCNT1 = 49911;			//reset 
}