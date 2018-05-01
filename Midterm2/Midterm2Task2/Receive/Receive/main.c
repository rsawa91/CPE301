/*
 * Receive.c
 *
 * Created: 4/29/2018 9:06:15 PM
 * Author : sawar1
 */ 

#define F_CPU 16000000UL		//set clock rate
#define BAUD 9600				//set BAUD rate
#define MYUBRR F_CPU/16/BAUD-1	//get MYUBRR value

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include "nrf24l01.h"
nRF24L01 *setup_rf(void);
void process_message(char *message);

void USART_init(unsigned int ubrr);
void USART_tx_string(char *data);

inline void prepare_led_pin(void);
inline void set_led_high(void);

inline void set_led_low(void);
volatile bool rf_interrupt = false;
int main(void) {
	uint8_t address[5] = { 0x22, 0x55, 0x33, 0x88, 0x46 };	//set address to look for
	prepare_led_pin();
	sei();				//allow interrupts
	
	USART_init(MYUBRR);		//initialize USART communication
	
	nRF24L01 *rf = setup_rf();			//set rf pointer
	nRF24L01_listen(rf, 0, address);	//check for communication from given address
	uint8_t addr[5];
	nRF24L01_read_register(rf, 0x00, addr, 1);
	while (true) {
		if (rf_interrupt) {				//check rf_interrupt variable
			rf_interrupt = false;
			while (nRF24L01_data_received(rf)) {		//if data is received continue into loop
				nRF24L01Message msg;
				USART_tx_string((char *)msg.data);		//print temperature data to terminal
				USART_tx_string("\r\n");				//print an endline to terminal
				nRF24L01_read_received_data(rf, &msg);	//get message from rf
				process_message((char *)msg.data);		//convert data
			}
			nRF24L01_listen(rf, 0, address);
		}
	}
	return 0;
}
nRF24L01 *setup_rf(void) {				//initalize the rf communication
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

void process_message(char *message) {
	if (strcmp(message, "ON") == 0)
	set_led_high();
	else if (strcmp(message, "OFF") == 0)
	set_led_low();
}
inline void prepare_led_pin(void) {
	DDRB |= _BV(PB0);
	PORTB &= ~_BV(PB0);
}
inline void set_led_high(void) {
	PORTB |= _BV(PB0);
}
inline void set_led_low(void) {
	PORTB &= ~_BV(PB0);
}
// nRF24L01 interrupt
ISR(INT0_vect) {
	rf_interrupt = true;
}