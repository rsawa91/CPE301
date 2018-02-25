/*
 * AssemblerAssign1.asm
 *
 *  Created: 2/24/2018 1:04:02 PM
 *   Author: sawar1
 */ 


 



	.org 0
	ldi R21, 0			;initialize R21
	ldi XL, 0x22		;X to 0x0222
	ldi XH, 0x02
	ldi YL, 0x00		;Y to 0x0400
	ldi YH, 0x04
	ldi ZL, 0x00		;Z to 0x0600
	ldi ZH, 0x06

FILL:
	ldi R20, 0
	add R20, XL	
	add R20, XH			;add H and L 
	st X, R20			;stores into X 
	inc XL				;increment
	cpi XL, 255			;verify if L of X is greater than 255
	brsh RESETX			;reset to 0
CONTINUE:
	inc R21				;increment 
	jmp DIVIDE			;divide by 5
DIVIDERET:
	cpi R21, 255		;verify if counter is greater than 255
	brne FILL			;continue counting
	ldi R21, 0			;reset 
FILLMORE:
	ldi R20, 0
	add R20, XL			;add L and H
	add R20, XH
	st X, R20			;storing in X 
	inc XL				;incrementing 
	inc R21				;continue incrementing
	jmp DIVIDE2			;divide by 5
DIVIDERET2:
	cpi R21, 42			;verify if it reached 42
	brne FILLMORE
	jmp END				;end program

RESETX:
	ldi XL, 0			;reset to 0
	inc XH				;increment 
	jmp CONTINUE

DIVIDE:
	ldi R22, 5			;load 5 
	ldi R23, 0			;load quotient into R23
	mov R24, R20		;copy address
L1:
	inc R23				;increment 
	sub R20, R22		;subtract 5
	BRCC L1				
	dec R23				;decrement 
	add R20, R22		;add 5 
	cpi R20, 0			;equals zero
	brne NOTDIV			;branch
	st Y, R24			;store into Y address
	inc YL				;increment 
	ldi R25, 0	
	add R16, R24		;add value and R16
	adc R17, R15		;add carry
	jmp DIVIDERET

NOTDIV:
	st Z, R24			;store Z address
	inc ZL				
	ldi R25, 0
	add R18, R24		;add value and R18
	adc R19, R15		;add carry
	jmp DIVIDERET

	DIVIDE2:
	ldi R22, 5			;load 5 into R22
	ldi R23, 0
	mov R24, R20		;copy into R24
L2:
	inc R23				;increment
	sub R20, R22		;subtract 5
	BRCC L2				;branch is greater than 0
	dec R23				;decrement 
	add R20, R22		;add 5 
	cpi R20, 0			;equals 0
	brne NOTDIV2		 
	st Y, R24			;store into Y 
	inc YL				;increment Y 
	ldi R25, 0
	add R16, R24		;add to R16
	adc R17, R25		;add carry
	jmp DIVIDERET2

NOTDIV2:
	st Z, R24			;store Z address
	inc ZL				;increment 
	ldi R25, 0
	add R18, R24		;add to R18
	add R19, R25		;add carry
	jmp DIVIDERET2

END:
	jmp END				;keep jumping forever