/*
 * RTCatmega1284p.cpp
 *
 *  Created on: 22 mars 2017
 *      Author: gilou
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../lib/FSM.h"

/// @todo change timer2 for external RTC test
unsigned long int FSM::timestamp = 0;

Usart FSM::usart0(0,Usart::BR_115200);
Usart FSM::usart1(1,Usart::BR_115200);


FSM::FSM(){
		TIMSK2 |= _BV(TOIE2);				// enable overflow interrupt
		TCNT2 = 0;
		TCCR2B = _BV(CS22) | _BV(CS20); // prescaler for overload interrupt each 1 second : CS2[2:0]=101;
		ASSR |=  _BV(AS2);					// Set the bit AS2 in the ASSR register to clock the timer 2 from the external crystal

		wifi.LDO_enable();
}

/**
 * \brief the timer2 is use with an external 32.768 Hz crystal to provide a RTC
 *
 * At each timer2 overflow, the uint32 timestamp is incremented
 */
ISR(TIMER2_OVF_vect){
	FSM::timestamp++;
}

