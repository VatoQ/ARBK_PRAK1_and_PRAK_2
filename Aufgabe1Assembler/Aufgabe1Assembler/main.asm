;
; Aufgabe1Assembler.asm
;
; Created: 11.10.2023 17:13:44
; Author : jonat
;


; Replace with your application code

.org 0x000	; Reset Vector
	rjmp start


start:
; Stack Pointer initialisieren
	ldi r16, LOW(RAMEND)
	out SPL, r16
	ldi r16, HIGH(RAMEND)
	out SPH, r16

	;PB und PD als Outputs
	ldi r16, 0b00000011 
	out DDRB, r16        
	ldi r16, 0b11111111
	out DDRD, r16
	

main:
	rcall right_shift
	rcall run_on_pb
	rcall left_shift
	rjmp main

right_shift:
	ldi r17, 0b00000001
right_shift_loop:
	out PORTD, r17
	rcall delay
	lsl r17
	brne right_shift_loop
	
	ldi r17, 0b00000000
	out PORTD, r17
	ret

left_shift:
	ldi r17, 0b10000000
left_shift_loop:
	out PORTD, r17
	rcall delay
	lsr r17
	cpi r17, 0b00000001	;das hier vermeidet einen doppelten delay am mainstart 
	brne left_shift_loop

	ret
	      
run_on_pb:
	ldi r17, 0b00000001
	out PORTB, r17
	rcall delay

	lsl r17
	out PORTB, r17
	rcall delay

	lsr r17
	out PORTB, r17
	rcall delay

	ldi r17, 0b00000000
	out PORTB, r17
	ret




delay:
	ldi r18, 127
l0:
	ldi r19, 127
l1:
	ldi r20, 127
l2:
	dec r20
	brne l2
	dec  r19
	brne l1
	dec r18
	brne l0
	ret