#ifndef DRIVE_MOTOR_H
#define DRIVE_MOTOR_H

#include <avr/io.h>

/**
* @defgroup driveMotorFunct Drive Motor
* @brief Enables the wheels to move in any direction
* @{
*/
#define PMW0A PORTD6
#define PMW0B PORTD5
#define PMW2A PORTB3
#define PMW2B PORTD3
//! Initialize motor pins
void init_motor(){			
	DDRB |= (1<<PMW2A);			
	DDRD |= (1<<PMW2B);			
	DDRD |= (1<<PMW0B)|(1<<PMW0A);
	
	//  255
	TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM00)|(1<<WGM01);		// non-inverting MODE
	TCCR0B = (1<<CS00);											
	
	TCCR2A = (1<<COM2A1)|(1<<COM2B1)|(1<<WGM20)|(1<<WGM21);		TCCR2B = (1<<CS20);											
	
}

//forwards and backwards movement
void drive_motor(int left_pct, int right_pct){
	int tempL = (int)(left_pct/100.0 * 255);
	int tempR = (int)(right_pct/100.0 * 255);
	
	if (tempL>=0 && tempR>=0){
		OCR0A = 0;											
		OCR0B = (tempL)<255?tempL:255;						// change duty cycle
		OCR2A = 0;										
		OCR2B = (tempR)<255?tempR:255;						// change duty cycle
		
	}
	else if (tempL>=0 && tempR<0){
		OCR0A = 0;											
		OCR0B = (tempL)<255?tempL:255;						// change duty cycle
		OCR2A = (-tempR)<255?(-tempR):255;					
		OCR2B = 0;											// change duty cycle
		
	}
	else if (tempL<0 && tempR>=0){
		OCR0A = (-tempL)<255?(-tempL):255;					// no movement in other direction
		OCR0B = 0;											// change duty cycle
		OCR2A = 0;											// no movement in other direction
		OCR2B = (tempR)<255?tempR:255;						// change duty cycle
		
	}
	else{
		OCR0A = (-tempL)<255?-tempL:255;					// no movement in other direction
		OCR0B = 0;											// change duty cycle
		OCR2A = (-tempR)<255?-tempR:255;					// no movement in other direction
		OCR2B = 0;											// change duty cycle
	}
}
/**@}*/
#endif