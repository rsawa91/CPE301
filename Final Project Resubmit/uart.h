#ifndef UART_H
#define UART_H

#define F_CPU 8000000UL		//set clk speed to 8Mhz
#define BUFF_SIZE 25		//buffer size
#define BAUD  9600			//UART Baud Rate


#include <avr/io.h>
#include <util/delay.h>



/**
* @defgroup UARTfunct UART I/O
* @brief Enables the UART functionality in the microcontroller
* @{
*/

//! Initialize UART in the Atmega328p
volatile signed char receivedChar;
char charBuff[BUFF_SIZE];
volatile unsigned char charread;
int l, r;
char turnt = 0, done = 0;


void initUART(){
	unsigned int baudrate;
	
	// Set baud rate:  UBRR = [F_CPU/(16*BAUD)] -1
	baudrate = ((F_CPU/16)/BAUD) - 1;
	UBRR0H = (unsigned char) (baudrate >> 8);
	UBRR0L = (unsigned char) baudrate;

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);		// Enable receiver & transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);	// Set data frame: 8 data bits, 1 stop bit, no parity
}

//! Transmit/write one character to the output
void writeChar(unsigned char c) {
	UDR0 = c;			// Display character on serial (i.e., PuTTY) terminal
	_delay_ms(10);		// delay for 10 ms
}

//! Transmit/write a NULL-terminated string to the output
void writestring(char *c){
	unsigned int i = 0;
	while(c[i] != 0)
		writeChar(c[i++]);
}

void readString(){
	done = 0;
	while (done == 0){
		if(UCSR0A & (1 << RXC0)){
			
			receivedChar = UDR0;				// Read the data from RX 
			charBuff[charread] = receivedChar;	// load char into buffer
			
			if(receivedChar == '<')				// esp32 has stopped sending gibberish
				return;
			
			else if(receivedChar == 'l')		// left value input ready
				turnt = 1;
			
			else if(receivedChar == 'r')		// right value input ready
				turnt = 0;
			
			else if(turnt){						// set right
				r = (int)receivedChar;
				done = 1;
			}
			else{								// set left
				l = (int)receivedChar;
				done = 0;
			}
		}

	}
}

/**@}*/


#endif