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
 *    @file   Measure.cpp
 *    @Author gilou
 *    @date   20 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <avr/io.h>

#include "../lib/main.h"
#include "../lib/FSM.h"
#include "../lib/Measure.h"

bool Measure::flag_data_frequencies_ready = 0;

// Operations
Measure::Measure(){

}

Measure::~Measure(){

}

void Measure::execute (){
	PORTB |= _BV(LED_PIN);

	if(FSM::flag_new_measure){
		if(FSM::eeprom.measure_counter==0){		// if it's a new data flow
			clear_data_array();		// clear the data array
		}
		freq_init_measurement();	// init frequencies measurement
		windvane_value();			// read the value from the windvane, convert it in 0-359° value
		power_read_value();			// do power measurement todo à préciser!
		temperature_read_value();	// read temperature from the DS18B20
		FSM::eeprom.measure_counter++;			// increase the measureCounter

		FSM::flag_new_measure = 0;	/// reset the flag
	}
	if(Measure::flag_data_frequencies_ready){
		freq_read_value();
		Measure::flag_data_frequencies_ready = 0;		/// reset the flag
	}
	if(FSM::eeprom.measure_counter==FSM::eeprom.measure_max){
		calc_average();
	}

	PORTB &= ~_BV(LED_PIN);
}

void Measure::print(Usart &usart){
	usart.print("measure state");
}


/**
 * \brief this method clear data value to save news
 * \return void
 */
void Measure::clear_data_array (){

}

/**
 * \brief this method initialize frequence measurement for anemometer and RPM
 * \return void
 */
void Measure::freq_init_measurement (){

}

/**
 * \brief read the windvane value en map it between 0 to 359 degres
 * \return void
 */
void Measure::windvane_value (){

}
/**
 * \brief read power value - not define realy (see emonLib)
 * \return void
 */
void Measure::power_read_value (){

}

/**
 * \brief read the temperature value from one DS18b20 sensor
 * \return void
 */
void Measure::temperature_read_value (){

}

/**
 * \brief read value from anemometer and RPM
 * \return void
 */
void Measure::freq_read_value (){

}

/**
 * \brief calcul average from the data array
 * \return void
 */
void Measure::calc_average (){
	FSM::eeprom.measure_counter=0;	// initialize the next measure

}
