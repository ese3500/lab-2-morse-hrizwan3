/*
 * GccApplication3.c
 *
 * Created: 2/3/2023 12:47:25 PM
 * Author : hrizwan3
 */ 

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL


void initialize() {
	DDRB |= (1 << DDB1); // set pin as output
	DDRB |= (1 << DDB2);
	DDRB |= (1 << DDB3);
	DDRB |= (1 << DDB4);
	
	DDRD &= ~(1 << DDD7); // set push button as input
	
	int numPresses = 0;
	while(1) {
		
		if (numPresses == 0 && PIND & (1 << PORTD7)) {
			PORTB |= (1 << PORTB1);
			numPresses++;
			_delay_ms(5000);
		} 
		if (numPresses == 1 && ~(PIND & (1 << PORTD7))) {
			PORTB &= ~(1 << PORTB1);
			_delay_ms(5000);
		} 
		if (numPresses == 1 && PIND & (1 << PORTD7)) {
			PORTB |= (1 << PORTB2);
			numPresses++;
			_delay_ms(5000);
		} 
		if (numPresses == 2 && ~(PIND & (1 << PORTD7))) {
			PORTB &= ~(1 << PORTB2);
			_delay_ms(5000);
		} 
		if (numPresses == 2 && PIND & (1 << PORTD7)) {
			PORTB |= (1 << PORTB3);
			numPresses++;
			_delay_ms(5000);
		}  
		if (numPresses == 3 && ~(PIND & (1 << PORTD7))) {
			PORTB &= ~(1 << PORTB3);
			_delay_ms(5000);
		}  
		if (numPresses == 3 && PIND & (1 << PORTD7)) {
			PORTB |= (1 << PORTB4);
			numPresses = 0;
			_delay_ms(5000);
		} 
		if (numPresses == 0 && ~(PIND & (1 << PORTD7)))  {
			PORTB &= ~(1 << PORTB4);
			_delay_ms(5000);	
		}
		
	}

}


//int main(void)
//{

	
//	initialize();
//	while (1);
//}

