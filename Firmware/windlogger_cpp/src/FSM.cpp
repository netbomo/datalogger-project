/**
 *******************************************************************************
 *******************************************************************************
 *
 *	License :
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *     any later version.
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
 *    @file   FSM.cpp
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "../lib/main.h"
#include "../lib/FSM.h"
#include "../lib/State.h"
#include "../lib/Usart.h"

/******************************************************************************
 * static software flags definition need to be out of constructor or function.
 */
bool FSM::flag_new_measure = 0;			// set when time come for a new measure
bool FSM::flag_data_averages_ready = 0;	// set when each averages are calculated and ready for the OUTPUT state
bool FSM::flag_config_request = 0;		// set when a string from usart0 finish by "\r\n"

/******************************************************************************
 * static global variable definition need to be out of constructor or function.
 */
unsigned long int FSM::timestamp = 0;		// timestamp is an uint32 to stock the current unix time stamp

/******************************************************************************
 * static class definition need to be out of constructor or function. That refer to hardware peripherals
 */
Usart FSM::uart0(0,Usart::BR_57600);	// This is the uart0 definition

Eeprom FSM::eeprom;						// This is the structure of data stored in the eeprom

//Class constructor
FSM::FSM():nextState(&idle),second_counter(0){

	config.load_eeprom();

	TIMSK2 |= _BV(TOIE2);				// enable overflow interrupt
	TCNT2 = 0;
	TCCR2B = _BV(CS22) | _BV(CS20); 	// prescaler for overload interrupt each 1 second : CS2[2:0]=101;
	ASSR |=  _BV(AS2);					// Set the bit AS2 in the ASSR register to clock the timer 2 from the external crystal

	sei();								// enable interrupt

	config.load_eeprom();
}

/******************************************************************************
 * State machine mechanic methods
 */
// This method define the next state from hardware and software flags
void FSM::new_State_definition(){
	if(flag_config_request) {
		flag_config_request = 0;							// Reset flag
		nextState = &config;								// priority 1 : config request process
	}
	else if(flag_new_measure||measure.flag_data_frequencies_ready) nextState = &measure;		// priority 2 : do measurement stuff

	else if(flag_data_averages_ready) {
		flag_data_averages_ready = 0;					// Reset flag
		nextState = &output;							// priority 3 : output process
	}
	else nextState = &sleep;								// priority 4 : go to sleep


	//uart0.print("------->"); nextState->print(uart0); uart0.print("\r\n"); //r_nextState.print(); usart0.print("\r\n");
}

// This method control if a new measure is needed
void FSM::measurement_timing_control (){
	second_counter++;						// Use a second counter to compare with the measure_stamp

	if(second_counter==eeprom.measure_periode)
	{
		flag_new_measure = 1;				// if it's true, we can do a new measure
		second_counter=0;					// reset the counter
	}
}

// This method execute the next state
void FSM::execute(){
	nextState->print(uart0);uart0.print("\r\n");
	nextState->execute();
}

/******************************************************************************
 * Methods call from hardware flags
 */
// Call when a string from usart0 finish by "\r\n"
void FSM::character_processing (){
	config.addToRequest(uart0.data_udr0);	// save the char in the config_request string

	uart0.set(uart0.data_udr0);				// do a echo to print on the serial terminal

	if (uart0.data_udr0 == 0x08)			// if char is backspace
	{
		config.backspace();
	}
	else if(uart0.data_udr0=='\r')			// if char is end line
	{
		uart0.print("\r\n");				// print carriage return and new line on the serial termial

		config.endString();

		flag_config_request=1;				// set flag to go in conf mode to do config update
	}

	uart0.flag_rx0 = 0;						// reset the flag
}

/**
 * \brief the timer2 is use with an external 32.768 Hz crystal to provide a RTC
 *
 * At each timer2 overflow, the static uint32 timestamp is incremented
 */
ISR(TIMER2_OVF_vect){
	FSM::timestamp++;
}

