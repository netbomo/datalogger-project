/**
 *******************************************************************************
 *******************************************************************************
 *
  *	License :
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 
 *******************************************************************************
 *******************************************************************************
 *
 *
 *    @file   main.c
 *    @author betzy
 *    @date   20 août 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../lib/main.h"
#include "../lib/usart.h"


/**
 * \brief the timer2 is use with an external 32.768 Hz crystal to provide a RTC
 *
 * At each timer2 overflow, the uint32 timestamp is incremented
 */
ISR(TIMER2_OVF_vect){
	timestamp++;
}


int main(){
	/************************************** variables **************************************/
	timestamp = 0;					// create and initialize second counter
	unsigned long int timestamp_old = 0;				// create and initialize the timestamp's old value
	/********************************** end variables **************************************/


	/************************************** I/O initializations **************************************/
	DDRA = 0xFF;
	DDRB = 0xFF;
	PORTA = 0x00;
	PORTB = 0xF0;
	/********************************** end I/O initializations **************************************/


	/************************************** timer 2 initialization **************************************/
	TIMSK2 |= _BV(TOIE2);				// enable overflow interrupt
	TCNT2 = 0;							// reset timer value
	TCCR2B = _BV(CS22) | _BV(CS20); 	// prescaler for overload interrupt each 1 second : CS2[2:0]=101;
	ASSR |=  _BV(AS2);					// Set the bit AS2 in the ASSR register to clock the timer 2 from the external crystal
	/********************************** end timer 2 initialization **************************************/

	/************************************** uart0 initialization **************************************/
	usart0_baudrate =BR_57600;			// set the bauderate
	USART0_init();						// initialize the usart0
	/********************************** end uart0 initialization **************************************/

	sei();								// Enable global interrupt

	USART0_print("Initialization Windlogger test 0.1.0");			// If the uart0 connection is OK the text can be read

	timestamp_old = timestamp;


	/************************************** start loop **************************************/
	while(1){

		/************************************** check if timestamp change **************************************/
		if(timestamp_old!= timestamp){
			if(PORTB == 0xF0) PORTB = 0x00;		// switch off the led
			else PORTB = 0xF0;					// or switch on the led
			timestamp_old= timestamp;			// save the timestamp value
		}
		/********************************** end check if timestamp change **************************************/

		_delay_ms(60);							// wait a little

		/************************************** test UART0 by echo **************************************/
		if(flag_USART0_read==1){				// if a new char come by the uart
			USART0_set(data_udr0);				// resend it
			flag_USART0_read = 0;				// clear the flag
		}
		/********************************** end test UART0 by echo **************************************/

	}
	/********************************** stop loop (never) **************************************/

	return 0;
}
