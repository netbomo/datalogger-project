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
 *    @file   main.cpp
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief  This is the main file, is the firmware's body.
 *
 *    The firmware works as :
 *    @image html diag_firmware.svg
 */

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>
#include <stdlib.h>

#include "../lib/main.h"
#include "../lib/FSM.h"
#include "../lib/State.h"
#include "../lib/Usart.h"


int main(){
/******************************************************************************
 * Hardware initialization
 */
	DDRA = 0x00;	// All adc in input mode

	DDRB = 0b10111000;				//	config the port B
	PORTB = 0x00;				//	set all portb 's bits to low

	PINC = 0x00;				//Set portC in input mode

	FSM fsm; 					//	FSM instance, that is the state machine mechanism

/******************************************************************************
 * @todo If verbose mode is select ($01=1), display some infos on the uart0
 */
	//fsm.uart0.print("Initialization\r\n");		// print on uart0 "initialisation"

/******************************************************************************
 * Initialize main scope variable
 */
    unsigned char second_old = 0;	// this variable is check if a new second come for the timing management
    second_old = fsm.rtc.get_second();

/******************************************************************************
 * If all initialization are ok, start the program
 */
  	while(1){		//infinity loop, the program will never crash!

	/******************************************************************************
	 * Hardware management
	 */
  		// check if a new char come from the USART0
		if(fsm.uart0.flag_rx0) {
			fsm.character_processing();		// if it's true, process it
			fsm.uart0.flag_rx0 = 0;			// and reset the flag
		}



		// If a new second comes
		FSM::rtc.get_date();
//		mettre à jour le timestamp
		if(second_old!=fsm.rtc.get_second()){
			fsm.measurement_timing_control ();	// check if time comes to do a new measure or not
			second_old = fsm.rtc.get_second();		// update timestamp_old
		}

	/******************************************************************************
	 * This method define the next state from hardware and software flags
	 */
		fsm.new_State_definition();

	/******************************************************************************
	 * This method execute the next state
	 */
		fsm.execute();

	} // loop in the while

	return 0;
}
