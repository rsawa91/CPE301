;
; task5A.asm
;
; Created: 3/15/2018 1:04:11 PM
; Author : sawar1
;


; Replace with your application code
.ORG 0x00
	JMP MAIN
.ORG 0x06		
	JMP EX0_ISR

MAIN:
	LDI R20, HIGH(RAMEND)
	OUT SPH, R20			;stack pointer high address
	LDI R20, LOW(RAMEND)
	OUT SPL, R20			;stack pointer low address
	SBI DDRB, 5				;PORTB 5 to output
	LDI R17, 0
	LDI R20, (1 << INT0)
	OUT EIMSK, R20			;clear interrupt flag
	SEI
HERE:
	JMP HERE				;jump HERE

EX0_ISR:					;INT0 interrupt function
	LDI R20, (1 << INTF0)	;clear interrupt flag
	LDI R16, 32	
	EOR R17, R16			;xor 32 with 0
	OUT PORTB, R17			;set PORTB output
	LDI R18, 0xF3			;set loop to F3
LOOP:
	SUBI R18, 1
	CPI R18, 0x00			;R18 equals 0
	BRNE LOOP
	LDI R20, 0x00
	STS TCNT0, R20			;counter value to 0
	RETI