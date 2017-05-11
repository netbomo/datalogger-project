/*
DO  * main.cpp
 *
 *  Created on: 20 avr. 2017
 *      Author: gilou
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>
#include <stdlib.h>

#include "../lib/main.h"
#include "../lib/FSM.h"
#include "../lib/State.h"
#include "../lib/Usart.h"


int main(){

	DDRB = 0xFF;
	PORTB = 0x00;

	FSM fsm; /// <!FSM instance>

	//State &nextState = fsm.get_idle();		/// the nexte state

	//Usart uart0(0,Usart::BR_57600);

	fsm.uart0.print("Initialization");

    unsigned long int timestamp_old = 0;

    char conv_string[20];

	while(1){

		/**
		 * If a new char come from the USART0
		 * process this char
		 */
		if(fsm.uart0.flag_rx0) {
			fsm.character_processing();						/// process the char
			fsm.uart0.flag_rx0 = 0;			/// reset the flag
		}

		if(timestamp_old!=fsm.timestamp){
			fsm.measurement_timing_control ();	/// check if time come to do a new measure or not
			 timestamp_old = fsm.timestamp;
		}

		fsm.newState_definition();	/// Permit to choose the next state

		fsm.execute();		/// execute the next state

	}

	return 0;
}
