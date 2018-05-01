/*
 * Transmit.c
 *
 * Created: 4/30/2018 5:25:15 PM
 * Author : sawar1
 */ 

#define F_CPU 16000000UL		//set clock rate
#define BAUD 9600				//set BAUD rate
#define MYUBRR F_CPU/16/BAUD-1	//get MYUBRR number

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include "nrf24l01.h"
void setup_timer(void);
nRF24L01 *setup_rf(void);

void read_adc(void);
void adc_init(void);
void USART_init(unsigned int ubrr);
void USART_tx_string(char *data);
volatile unsigned int adc_temp;
char outs[20];

volatile bool rf_interrupt = false;
volatile bool send_message = false;
int main(void) {
	uint8_t to_address[5] = { 0x22, 0x55, 0x33, 0x88, 0x46 };
	bool on = false;
	
	adc_init();					//initialize adc converter
	USART_init(MYUBRR);			//initialize USART registers
	
	sei();						//allow interrupts
	nRF24L01 *rf = setup_rf();	//get rf pointer value
	setup_timer();				//initialize timer for 1 second intervals
	while (true) {

		read_adc();				//read adc value
		snprintf(outs, sizeof(outs), "%3d\r\n", adc_temp);	//convert adc value to ascii
		USART_tx_string(outs);	//print adc value to terminal

		if (rf_interrupt) {
			rf_interrupt = false;
			int success = nRF24L01_transmit_success(rf);	//set success variable
			if (success != 0)
			nRF24L01_flush_transmit_message(rf);			//clear transmit message
		}
		if (send_message) {			//check if send message is true
			send_message = false;
			on = !on;
			nRF24L01Message msg;
			if (on)
			{
				memcpy(msg.data, outs, 3);		//move adc value into data
			}
			else
			{
				memcpy(msg.data, "OFF", 4);		//move "OFF" into data
			}
			msg.length = strlen((char *)msg.data) + 1;	//get string length
			nRF24L01_transmit(rf, to_address, &msg);	//send string through rf
		}
	}
	return 0;
}
nRF24L01 *setup_rf(void) {		//initailize the rf communication
	nRF24L01 *rf = nRF24L01_init();
	rf->ss.port = &PORTB;
	rf->ss.pin = PB2;
	rf->ce.port = &PORTB;
	rf->ce.pin = PB1;
	rf->sck.port = &PORTB;
	rf->sck.pin = PB5;
	rf->mosi.port = &PORTB;
	rf->mosi.pin = PB3;
	rf->miso.port = &PORTB;
	rf->miso.pin = PB4;
	// interrupt on falling edge of INT0 (PD2)
	EICRA |= _BV(ISC01);
	EIMSK |= _BV(INT0);
	nRF24L01_begin(rf);
	return rf;
}
// setup timer to trigger interrupt every second when at 1MHz
void setup_timer(void) {
	TCCR1B |= _BV(WGM12);
	TIMSK1 |= _BV(OCIE1A);
	OCR1A = 15624;
	TCCR1B |= _BV(CS10) | _BV(CS11);
}


void adc_init(void)
{
	/** Setup and enable ADC **/
	ADMUX = (0<<REFS1)| // Reference Selection Bits

	(1<<REFS0)| // AVcc - external cap at AREF
	(0<<ADLAR)| // ADC Left Adjust Result
	(0<<MUX2)| // ANalog Channel Selection Bits
	(1<<MUX1)| // ADC2 (PC2 PIN25)
	(0<<MUX0);

	ADCSRA = (1<<ADEN)| // ADC ENable

	(0<<ADSC)| // ADC Start Conversion
	(0<<ADATE)| // ADC Auto Trigger Enable
	(0<<ADIF)| // ADC Interrupt Flag
	(0<<ADIE)| // ADC Interrupt Enable
	(1<<ADPS2)| // ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
}

void read_adc(void) {
	unsigned char i =4;
	adc_temp = 0;
	while (i--) {
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp / 4; // Average a few samples
	adc_temp += 20;
}
void USART_init( unsigned int ubrr ) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0); // Enable receiver, transmitter & RX interrupt
	UCSR0C = (3 << UCSZ00); //asynchronous 8 N 1
}
/* SEND A STRING TO THE RS-232 */
void USART_tx_string( char *data ) {
	while ((*data != '\0')) {
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}


// each one second interrupt
ISR(TIMER1_COMPA_vect) {
	send_message = true;
}
// nRF24L01 interrupt
ISR(INT0_vect) {
	rf_interrupt = true;
}