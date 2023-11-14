/*
 * ARBK_P2_ZSM.c
 *
 * Created: 12.11.2023 13:21:55
 * Author : jonat
 */ 

#include <avr/io.h>
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




int main(void)
{
	DDRD = ~(1 << PORTD2) & ~(1 << PORTD3);
	PORTD = (1 << PORTD2) | (1 << PORTD3);
	DDRB |= (1 << PORTB0) | (1 << PORTB1);
	
	uint8_t current_state = OO;
	uint8_t prev_button_state = 0;
	
    /* Replace with your application code */
    while (1) 
    {
		uint8_t current_button_state = PIND & ((1 << PORTD2) | (1 << PORTD3)); // FEHLER
		if (current_button_state != prev_button_state)
		{
			if (current_button_state != ((1 << PORTD2) | (1 << PORTD3)))
			{
				current_state = next_state(current_state, current_button_state);
			}
			
			
			prev_button_state = current_button_state;
		}
		
		led_controller(current_state);
    }
}

