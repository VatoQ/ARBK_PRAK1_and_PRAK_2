/*
 * PRAK2_INTERRUPT.c
 *
 * Created: 01.11.2023 17:55:30
 * Author : jonat
 */ 

/************************************************************************/
/* INCLUDES                                                             */
/************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

/************************************************************************/
/* PRECOMPILER SECTION                                                  */
/************************************************************************/
/*
Defining the states for the state-machine.
Semantic meaning of the states:
	=>	O == off
	=>	B == blinking
	=>	L == constantly on
	
letters are concatenated in the specified combinations.
*/
#define OO 0
#define BO 1
#define LO 2
#define OB 3
#define OL 4

/*
Defining which bits correlate to which button
*/
#define SWI1 (1 << PORTD2) // 0b 0000 0100
#define SWI2 (1 << PORTD3) // 0b 0000 1000


/************************************************************************/
/* FUNCTIONS SECTION                                                    */
/************************************************************************/

/*
Function to calculate the next state 
according to the specifications
*/
uint8_t next_state(uint8_t current_state, uint8_t input)
{
	/*
	STATE MACHINE TABLE:
	
	STATE	|	SWI1	|	SWI2	|
	---------------------------------
	OO		|	BO		|	OB		|
	BO		|	LO		|	OB		|
	LO		|	BO		|	OB		|
	OB		|	BO		|	OL		|
	OL		|	BO		|	OB		|
	
	*/
	switch (current_state)
	{
		case OO:
		{
			if (input == SWI1)
			{
				return BO;
			}
			
			else
			{
				return OB;
			}
		}
		
		case BO:
		{
			if (input == SWI1)
			{
				return LO;
			}
			
			else 
			{
				return OB;
			}
		}
		
		case LO:
		{
			if (input == SWI1)
			{
				return BO;
			}
			
			else 
			{
				return OB;
			}
		}
		
		case OB:
		{
			if (input == SWI2)
			{
				return OL;
			}
			
			else 
			{
				return BO;
			}
		}
		
		case OL:
		{
			if (input == SWI2)
			{
				return OB;
			}
			
			else
			{
				return BO;
			}
		}
	}
	
	return 0;
}


/*
LEDs get turned on or off, according to the current state
*/
void led_controller(uint8_t current_state)
{
	switch (current_state)
	{
		// Idle State, both LEDs are off
		case OO:
		{
			PORTB &= ~(1 << PORTB0) & ~(1 << PORTB1);
			break;
		}
		
		/*
		In the following cases, the descriptive state name
		gets implemented: For B the corresponding bit gets flipped,
		for L it gets activated and for O it gets turned off.
		*/
		case BO:
		{
			PORTB &= ~(1 << PORTB0);
			PORTB ^= (1 << PORTB1);
			break;
		}
		
		case LO:
		{
			PORTB &= ~(1 << PORTB0);
			PORTB |= (1 << PORTB1);
			break;
			
		}
		
		case OB:
		{
			PORTB &= ~(1 << PORTB1);
			PORTB ^= (1 << PORTB0);
			break;
		}
		
		case OL:
		{
			PORTB &= ~(1 << PORTB1);
			PORTB |= (1 << PORTB0);
			break;
		}
	}
	_delay_ms(200);
}



/************************************************************************/
/* INTERRUPT SECTION                                                    */
/************************************************************************/
volatile uint8_t current_state = OO;

ISR(INT0_vect)
{
	current_state = next_state(current_state, SWI1);
}

ISR(INT1_vect)
{
	current_state = next_state(current_state, SWI2);
}


/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/

int main(void)
{
	DDRD = ~(1 << PORTD2) & ~(1 << PORTD3);	// SET INPUT PINS
	DDRB = (1 << PORTB0) | (1 << PORTB1);	// SET OUTPUT LEDs
	
	PORTD |= (1 << PORTD2) | (1 << PORTD3);	// ENABLE PULL-UP RESISTORS
	
	
	EICRA |= (1 << ISC11);
	EIMSK |= (1 << INT0) | (1 << INT1);
	
	
	sei();
	
    while (1) 
    {
		led_controller(current_state);
    }
}

