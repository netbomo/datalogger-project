/*
 * main.cpp
 *
 *  Created on: 6 mars 2017
 *      Author: gilou
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../lib/FSM.h"


int main(){
	DDRA = 0xFF;
	DDRB = 0xFF;
	PORTA = 0x00;
	PORTB = 0xF0;

	FSM fsm;

	sei();

	fsm.usart0.print("Initialization Windlogger test 0.1.0");

	fsm.timestamp = 0;

	unsigned long int timestamp_old;
	timestamp_old = fsm.timestamp;

	while(1){

		if(timestamp_old!=fsm.timestamp){

			if(PORTB == 0xFF) PORTB = 0x00;
			else PORTB = 0xFF;
			timestamp_old=fsm.timestamp;
		}

		_delay_ms(60);


		// test UART0 by echo
		if(FSM::usart0.flag_rx0==1){
			fsm.usart0.set(fsm.usart0.data_udr0);
			fsm.usart0.flag_rx0 = 0;
		}

	}


	return 0;
}





