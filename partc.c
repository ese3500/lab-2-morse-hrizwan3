/*
 * partc.c
 *
 * Created: 2/10/2023 1:50:12 PM
 *  Author: hrizwan3
 */ 



#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

char String[25];
volatile int time1 = 0;
volatile int time2 = 0;
volatile int overflowCount = 0;
volatile int risingOrFalling = 0;
volatile int nextSpot = 0;
volatile int shouldPrint = 0;
volatile int output = 0;

int hardCodeSpace = 0;


char symbols[5];
int currSymbolsPos = 0;


void Init() {

	cli();
	UART_init(BAUD_PRESCALER);
		
	DDRB &= ~(1 << DDB0); // set input pin (button)
	DDRB |= (1 << DDB5); // set output pin (internal led)
	DDRB |= (1 << PORTB5); // set port5 to high
		

	TCCR1B |= (1 << ICES1); // need to set pin change interrupt enable, sets to look for falling edge
	TIFR1 |= (1 << ICF1); // clear interrupt flag
	
	TIMSK1 |= (1 << ICIE1); // set pin change mask reg: needed for input capt interrupt
	TIMSK1 |= (1 << TOIE1); // set overflow interrupt
	
	// set clock
	TCCR1B |= (1 << CS10);
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS12);
	
	TCCR1A &= ~(1 << WGM10);
	TCCR1A &= ~(1 << WGM11);
	TCCR1B &= ~(1 << WGM12);
	TCCR1B &= ~(1 << WGM13);
		
		
	sei();
}

ISR(TIMER1_CAPT_vect) {

	if (risingOrFalling == 0) {
		risingOrFalling = 1;
		if (overflowCount >= 99) {
			output = 3;
		}
		shouldPrint = 1;
		overflowCount = 0;
		PORTB ^= (1 << PORTB5);
		TIFR1 |= (1 << ICF1);
		time1 = ICR1;
		TCCR1B ^= (1 << ICES1);
		
	} else if (risingOrFalling == 1) {
		PORTB ^= (1 << PORTB5);
		TIFR1 |= (1 << ICF1);
		TCCR1B ^= (1 << ICES1); // clear interrupt
		risingOrFalling = 0;
		shouldPrint = 1;
		time2 = ICR1;
		if (overflowCount >= 7 && overflowCount < 49) {
			output = 1;
		} else if (overflowCount >= 49 && overflowCount < 99) {
			output = 2;
		}
		overflowCount = 0;
	}
}

ISR(TIMER1_OVF_vect) {
		overflowCount++;
//	TIFR1 |= (1 << ICF1); // clear interrupt flag
}

// determine whether you have a dot, space, or a dash
// dot: 30ms to 200ms (480,000 to 3,200,000 ticks)
// dash: 200ms to 400ms (3,200,000 to 6,400,000 ticks)
// space: > 400ms (6,400,000 ticks or more)

char checkSymbol() {
	if (symbols[0] == '.' && symbols[1] == '-' && symbols[2] == ' ') {
		return 'A';
	} else if (symbols[0] == '-' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == '.' && symbols[4] == ' ') {
		return 'B';
	} else if (symbols[0] == '-' && symbols[1] == '.' && symbols[2] == '-' && symbols[3] == '.' && symbols[4] == ' ') {
		return 'C';
	} else if (symbols[0] == '-' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == ' ') {
		return 'D';
	} else if (symbols[0] == '.' && symbols[1] == ' ') {
		return 'E';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == '-' && symbols[3] == '.' && symbols[4] == ' ') {
		return 'F';
	} else if (symbols[0] == '-' && symbols[1] == '-' && symbols[2] == '.' && symbols[3] == ' ') {
		return 'G';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == '.' && symbols[4] == ' ') {
		return 'H';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == ' ') {
		return 'I';
	} else if (symbols[0] == '.' && symbols[1] == '-' && symbols[2] == '-' && symbols[3] == '-' && symbols[4] == ' ') {
		return 'J';
	} else if (symbols[0] == '-' && symbols[1] == '.' && symbols[2] == '-' && symbols[3] == ' ') {
		return 'K';
	} else if (symbols[0] == '.' && symbols[1] == '-' && symbols[2] == '.' && symbols[3] == '.' && symbols[4] == ' ') {
		return 'L';
	} else if (symbols[0] == '-' && symbols[1] == '-' && symbols[2] == ' ') {
		return 'M';
	} else if (symbols[0] == '-' && symbols[1] == '.' && symbols[2] == ' ') {
		return 'N';
	} else if (symbols[0] == '-' && symbols[1] == '-' && symbols[2] == '-' && symbols[3] == ' ') {
		return 'O';
	} else if (symbols[0] == '.' && symbols[1] == '-' && symbols[2] == '-' && symbols[3] == '.' && symbols[4] == ' ') {
		return 'P';
	} else if (symbols[0] == '-' && symbols[1] == '-' && symbols[2] == '.' && symbols[3] == '-' && symbols[4] == ' ') {
		return 'Q';
	} else if (symbols[0] == '.' && symbols[1] == '-' && symbols[2] == '.' && symbols[3] == ' ') {
		return 'R';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == ' ') {
		return 'S';
	} else if (symbols[0] == '-' && symbols[1] == ' ') {
		return 'T';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == '-' && symbols[3] == ' ') {
		return 'U';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == '-' && symbols[4] == ' ') {
		return 'V';
	} else if (symbols[0] == '.' && symbols[1] == '-' && symbols[2] == '-' && symbols[3] == ' ') {
		return 'W';
	} else if (symbols[0] == '-' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == '-' && symbols[4] == ' ') {
		return 'X';
	} else if (symbols[0] == '-' && symbols[1] == '.' && symbols[2] == '-' && symbols[3] == '-' && symbols[4] == ' ') {
		return 'Y';
	} else if (symbols[0] == '-' && symbols[1] == '-' && symbols[2] == '.' && symbols[3] == '.' && symbols[4] == ' ') {
		return 'Z';
	} else if (symbols[0] == '.' && symbols[1] == '-' && symbols[2] == '-' && symbols[3] == '-' && symbols[4] == '-') {
		return '1';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == '-' && symbols[3] == '-' && symbols[4] == '-') {
		return '2';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == '-' && symbols[4] == '-') {
		return '3';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == '.' && symbols[4] == '-') {
		return '4';
	} else if (symbols[0] == '.' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == '.' && symbols[4] == '.') {
		return '5';
	} else if (symbols[0] == '-' && symbols[1] == '.' && symbols[2] == '.' && symbols[3] == '.' && symbols[4] == '.') {
		return '6';
	} else if (symbols[0] == '-' && symbols[1] == '-' && symbols[2] == '.' && symbols[3] == '.' && symbols[4] == '.') {
		return '7';
	} else if (symbols[0] == '-' && symbols[1] == '-' && symbols[2] == '-' && symbols[3] == '.' && symbols[4] == '.') {
		return '8';
	} else if (symbols[0] == '-' && symbols[1] == '-' && symbols[2] == '-' && symbols[3] == '-' && symbols[4] == '.') {
		return '9';
	} else if (symbols[0] == '-' && symbols[1] == ' ' && symbols[2] == ' ' && symbols[3] == ' ' && symbols[4] == ' ') {
		return '0';
	}
}


int main() {
	Init();	
	while(1) {
//		sprintf(String, "Hello world! \n");
//		UART_putstring(String);
//		_delay_ms(1000);
		
//		sprintf(String, "The year is %u\n", 2020);
//		UART_putstring(String);
//		_delay_ms(1000);
//
//	sprintf(String, "Hi %u\n", output);

	//sprintf(String, "ISROVF: %u\n", checkIfOverFlowISRWorks);
	//UART_putstring(String);
	
//	sprintf(String, "ISR IP CAPT: %u\n", checkIfInputCaptISRWorks);
//	UART_putstring(String);
	
//	sprintf(String, "time1, time2: %u, %u \n", time1, time2);
//	UART_putstring(String);


	if (shouldPrint == 1) {
		shouldPrint = 0;
		if (output == 1) {

//			sprintf(String, "Dot\n");
//			UART_putstring(String);
			PORTB ^= (1 << PORTB4);
			_delay_ms(500);
			PORTB ^= (1 << PORTB4);
			
			symbols[nextSpot] = '.';
			if (nextSpot == 4) {
				char c = checkSymbol();
				sprintf(String, "%c\n", c);
				UART_putstring(String);
			
			int i = 0;
			while (i < 5) {
				symbols[i] = 'g';
				i++;
			}
				
				nextSpot = 0;
			} else {
				nextSpot++;
			}
			
			hardCodeSpace = 0;			
		} else if (output == 2) {
//			sprintf(String, "Dash \n");
//			UART_putstring(String);
			PORTB ^= (1 << PORTB3);
			_delay_ms(500);
			PORTB ^= (1 << PORTB3);
			symbols[nextSpot] = '-';
			
			if (nextSpot == 4) {
				char c = checkSymbol();
				sprintf(String, "%c\n", c);
				UART_putstring(String);
				
				int i = 0;
				while (i < 5) {
					symbols[i] = 'g';
					i++;
				}
				
				nextSpot = 0;
				} else {
				nextSpot++;
			}
			
			nextSpot++;
			hardCodeSpace = 0;
		} 
		else if (output == 3) {
//			sprintf(String, "Space\n");
//			UART_putstring(String);
			PORTB ^= (1 << PORTB2);
			_delay_ms(500);
			PORTB ^= (1 << PORTB2);
			symbols[nextSpot] = ' ';
			
			char c = checkSymbol();
			sprintf(String, "%c\n", c);
			UART_putstring(String);
			
			int i = 0;
			while (i < 5) {
				symbols[i] = 'g';
				i++;
			}
			nextSpot = 0;
			hardCodeSpace = 0;
		}
	}
	else if (overflowCount > 99 && hardCodeSpace == 0) {
//			sprintf(String, "Space\n");
//			UART_putstring(String);
			PORTB ^= (1 << PORTB2);
			_delay_ms(500);
			PORTB ^= (1 << PORTB2);
			symbols[nextSpot] = ' ';
			
			char c = checkSymbol();
			sprintf(String, "%c\n", c);
			UART_putstring(String);
			
			int i = 0;
			while (i < 5) {
				symbols[i] = 'g';
				i++;
			}
			nextSpot = 0;
			hardCodeSpace = 1;
	}	
}                                                    
}