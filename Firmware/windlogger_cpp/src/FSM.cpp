/**
 *******************************************************************************
 *******************************************************************************
 *
 *	License :
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
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
 *    @Author gilou
 *    @date   20 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../lib/FSM.hpp"

	/// flag set when a new char is receive by the micro-controller
	bool FSM::flag_UART0_rx_char = 0;
	/// set when the timerX overflow (stop frequencies measurement)
	/// todo select timer
	bool FSM::flag_freq_data_ready = 0;
	/// set when time come for a new measure
	bool FSM::flag_new_measure = 0;
	/// set when each averages are calculated and ready for the OUTPUT state
	bool FSM::flag_data_averages_ready = 0;

	unsigned long int FSM::timestamp = 0;


FSM::FSM():nextState(idle){

	TIMSK2 |= _BV(TOIE2);				// enable overflow interrupt
	TCNT2 = 0;
	TCCR2B = _BV(CS22) | _BV(CS20); // prescaler for overload interrupt each 1 second : CS2[2:0]=101;
	ASSR |=  _BV(AS2);					// Set the bit AS2 in the ASSR register to clock the timer 2 from the external crystal

	sei();						// enable interrupt
}


void FSM::execute(){
	nextState.execute();
}

/**
 * \brief This is an Interrupt Sub Routine, it's execute when timer2 overflows
 */
ISR(TIMER2_OVF_vect){
	FSM::timestamp++;
}
