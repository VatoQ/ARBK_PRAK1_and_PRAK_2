/*
 * ARBK_P2_INTERRUPRS_ZSM.c
 *
 * Created: 12.11.2023 14:58:30
 * Author : jonat
 */
 #include <avr/io.h>
 #include <avr/interrupt.h>
 
 #define F_CPU 16000000
 #include <util/delay.h>

 // DEFINE BUTTONS
 #define SWI1 (1 << PORTD2)
 #define SWI2 (1 << PORTD3)

 // DEFINE STATES
 #define OO 0
 #define OB 1
 #define OL 2
 #define BO 3
 #define LO 4
 
 
/************************************************************************/
/* STATE MACHINE SECTION                                                */
/************************************************************************/
uint8_t next_state(uint8_t current_state, uint8_t button)
{
	/*
	State machine
	
	STATE	|	SWI1	|	SWI2
	----------------------------
	OO		|	BO		|	OB
	OB		|	BO		|	OL
	OL		|	BO		|	OB
	BO		|	LO		|	OB
	LO		|	BO		|	OB
	*/
	if (button == SWI1)
	{		
		if (current_state == BO)
		{
			return LO;
		}
			
		return BO;
	}
	
	else
	{		
		if (current_state == OB)
		{
			return OL;
		}
		return OB;
	}
	
	return 0;
}

void led_controller(uint8_t current_state)
{
	switch (current_state)
	{
		case OO: 
			PORTB &= ~(1 << PORTB0) & ~(1 << PORTB1); // FEHLER
		break;
		
		case OB:
			PORTB &= ~(1 << PORTB0); 
			PORTB ^= (1 << PORTB1);
		break;
		
		case OL:
			PORTB &= ~(1 << PORTB0);
			PORTB |= (1 << PORTB1);
		break;
		
		case BO:
			PORTB &= ~(1 << PORTB1);
			PORTB ^= (1 << PORTB0);
		break;
		
		case LO:
			PORTB &= ~(1 << PORTB1);
			PORTB |= (1 << PORTB0);
		break;
	}
	
	_delay_ms(200);
}

volatile uint8_t current_state = OO;

ISR(INT0_vect)
{
	
	current_state = next_state(current_state, SWI1);
}

ISR(INT1_vect)
{
	current_state = next_state(current_state, SWI2);
}


 

int main(void)
{
	DDRD = ~(1 << PORTD2) & ~(1 << PORTD3);
	PORTD = (1 << PORTD2) | (1 << PORTD3);
	
	DDRB |= (1 << PORTB0) | (1 << PORTB1);
	
	
	EICRA |= (1 << ISC11);
	EIMSK |= (1 << INT0) | (1 << INT1);
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		led_controller(current_state);
    }
}

