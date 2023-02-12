/*
 * partb.c
 *
 * Created: 2/8/2023 6:15:36 PM
 *  Author: hrizwan3
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL

void Initialize() {
	cli();
	DDRB &= ~(1 << DDB0); // set input pin (button)
	DDRB |= (1 << DDB5); // set output pin (led)
	
	// set clock
	TCCR1B &= ~(1 << CS10);
	TCCR1B |= (1 << CS11);
	TCCR1B |= (1 << CS12);
	
	TCCR1B &= ~(1 << WGM10);
	TCCR1B &= ~(1 << WGM11);
	TCCR1B &= ~(1 << WGM12);
	TCCR1B &= ~(1 << WGM13);

	
	TCCR1B |= (1 << ICES1); // need to set pin change interrupt enable, sets to look for falling edge
	TIFR1 |= (1 << ICF1); // clear interrupt flag
	
	TIMSK1 |= (1 << ICIE1); // need to set pin change mask reg: needed for interrupt to work
	
	sei();
	
}
/**
ISR(TIMER1_CAPT_vect) {
	if ((PINB & (1 << PINB0))) { // check button press
		PORTB |= (1 << PORTB5);
		TCCR1B &= ~(1 << ICES1); // set to look for rising edge
	} else {
		PORTB &= ~(1 << PORTB5);
		TCCR1B |= (1 << ICES1); // set to look for falling edge
	}
}**/
/**
int main() {
	Initialize();
	while(1);	
}**/


