;
; task4A.asm
;
; Created: 3/15/2018 1:02:27 PM
; Author : sawar1
;


; Replace with your application code
.ORG 0x00
	JMP MAIN
.ORG 0x20		//timer overflow interrupt
	JMP T0_OV_ISR

MAIN:
	LDI R20, HIGH(RAMEND)
	OUT SPH, R20			;stack pointer high address
	LDI R20, LOW(RAMEND)
	OUT SPL, R20			;stack pointer low address
	LDI R17, 0
	SBI DDRB, 5				;PORTB 5 to output
	LDI R20, 13
	STS TCCR0B, R20			;prescaler to 1024
	LDI R20, 71
	STS OCR0A, R20			;max value
	LDI R20, (1 << TOIE0)
	OUT TIFR0, R20			;clear interrupt 
	SEI						;set interrupt 
begin:
	RJMP begin				

T0_OV_ISR:
	LDI R20, (1 << TOIE0)	;flag bit
	OUT TIFR0, R20			;clear flag 
	LDI R16, 32
	EOR R17, R16			;xor 32 with 0
	OUT PORTB, R17			;output PORTB
	LDI R18, 0xF3			;loop the value
LOOP:
	SUBI R18, 1
	CPI R18, 0				;R18 equals 0
	BRNE LOOP				;loop till R18 is 0
	LDI R20, 0x00
	STS TCNT0, R20			;counter value to 0
	RETI