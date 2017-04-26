/*
 * main.cpp
 *
 *  Created on: 20 avr. 2017
 *      Author: gilou
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "../lib/FSM.hpp"
#include "../lib/Usart.h"

/**
 * \brief when a new char come on the USART0, it's need to echo it and do stuff
 * \return void
 */
void character_processing (){

}
/**
 * \brief this method if a new measure is needed
 * \return void
 */
void measurement_timing_control (){

}
/**
 * \brief This method define the next state by deduction
 */
void newState_definition (){

}


int main(){

	DDRB = 0xFF;
	PORTB = 0xF0;

	FSM fsm; /// <!FSM instance>

	Usart usart0(0,Usart::BR_57600);

	usart0.print("Initialization");

	/// flag set when a new char is receive by the microcontroller
	fsm.flag_UART0_rx_char = 0;
	/// set when the timerX overflow (stop frequencies measurement)
	/// todo select timer
	fsm.flag_freq_data_ready = 0;
	/// set when time come for a new measure
	fsm.flag_new_measure = 0;
	/// set when each averages are calculated and ready for the OUTPUT state
	fsm.flag_data_averages_ready = 0;

	fsm.timestamp = 0;


    unsigned long int timestamp_old = 0;
    char conv_string[10];

	while(1){

		if(timestamp_old!=FSM::timestamp){

			if(PORTB == 0xF0) PORTB = 0x00;
			else PORTB = 0xF0;
			timestamp_old=FSM::timestamp;
		}

		_delay_ms(60);

		/**
		 * If a new char come from the USART0
		 * process this char
		 */
//		if(fsm.flag_UART0_rx_char) {
//			character_processing();
//		}
//
//		measurement_timing_control ();	/// check if time come to do a new measure or not
//
//		newState_definition();	/// Permit to choose the next state
//
//		fsm.execute();		/// execute the next state


	}

	return 0;
}
