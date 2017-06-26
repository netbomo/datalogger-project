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
 *    @file   Measure.cpp
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief The class Measure inherits from the class State.
 *
 *    This class organize measurement process and call sub functions to get data from inputs and average there.
 */

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/main.h"
#include "../lib/FSM.h"
#include "../lib/st_Measure.h"

/******************************************************************************
 * static hardware flags definition
 */
bool Measure::flag_data_frequencies_ready = 0;


/******************************************************************************
 * Constructor and destructor
 */
Measure::Measure(){
	strcpy(m_name,"measure\0");
} // Constructor

Measure::~Measure(){} // Destructor


/******************************************************************************
 * inherits from State
 */
void Measure::execute (){

	char temp_conv[10];

	PORTB |= _BV(LED_PIN);						// Turn on the green led (blink each time the measure.execute() come)
print();
	if(FSM::flag_new_measure){
		if(FSM::logger.measure_counter==0){		// if it's a new data flow
			clear_data_array();					// clear the data array
		}
		freq_init_measurement();				// init frequencies measurement
		windvane_value();						// read the value from the windvane, convert it in 0-359° value
		power_read_value();						// do power measurement todo à préciser!
		temperature_read_value();				// read temperature from the DS18B20
		FSM::logger.measure_counter++;			// increase the measureCounter

		FSM::flag_new_measure = 0;				// reset the flag
	}

	if(Measure::flag_data_frequencies_ready){
		freq_read_value();								//	read data from anemometer and RPM
		Measure::flag_data_frequencies_ready = 0;		//	reset the flag
	}
	FSM::uart0.print(itoa(FSM::logger.measure_counter,temp_conv,10));FSM::uart0.print("\r\n");
	if(FSM::logger.measure_counter==FSM::logger.measure_max){
		calc_average();									// if measurement sequence is finish, average data
		FSM::flag_data_averages_ready=1;
	}

	PORTB &= ~_BV(LED_PIN);						// turn off the green led
}

void Measure::print(){
	FSM::uart0.print(m_name);		// Print sub routine
}

bool Measure::isEqual(char *name)const {
	return State::isEqual(name);
}




//this method clear data value to save news
void Measure::clear_data_array (){

}

//this method initialize frequency measurement for anemometer and RPM
void Measure::freq_init_measurement (){

}

//read the windvane value en map it between 0 to 359 degres
void Measure::windvane_value (){
	FSM::windvane.read_value(FSM::logger.measure_counter);
	FSM::windvane.print_data_array();
}

// read power value - not define realy (see emonLib)
void Measure::power_read_value (){

}

// read the temperature value from one DS18b20 sensor
void Measure::temperature_read_value (){

}

// read value from anemometer and RPM
void Measure::freq_read_value (){

}

// calcul average from the data array
void Measure::calc_average (){
	FSM::windvane.calc_average();
	FSM::logger.measure_counter=0;	// initialize the next measure

}
