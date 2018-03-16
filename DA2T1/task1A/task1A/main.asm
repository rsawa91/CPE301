;
; task1A.asm
;
; Created: 3/15/2018 12:52:28 PM
; Author : sawar1
;


; Replace with your application code
.org 0
	LDI R16, 32
	SBI DDRB, 5			;output Port5
	LDI R17, 0
	OUT PORTB, R17		;PortB to 0
	LDI R20, 5
	STS TCCR1B, R20		;1024 prescaler
begin:
	RCALL delay			;call delay function
	EOR R17, R16		;xor 32 with 0
	OUT PORTB, R17		;output the xored value
	RJMP begin			;jump to begin
delay:
	LDS R29, TCNT1H		;get high value 
	LDS R28, TCNT1L		;get low value 
	CPI R28, 0xF3		;compare low value with F3
	BRSH body			;branch same or higher
	RJMP delay			;jump to delay
body:
	CPI R29, 0x00		;high value to 0
	BRSH done			;branch same or higher
	RJMP delay			;jump to delay
done:
	LDI R20, 0x00
	STS TCNT1H, R20		;reset TCNT1H
	LDI R20, 0x00
	STS TCNT1L, R20		;reset TCNT1L
	RET
