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
 *    @file   Sensor.cpp
 *    @author gilou
 *    @date   21 mai 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#include <avr/eeprom.h>
#include "../lib/FSM.h"
#include "../lib/Sensor.h"

/******************************************************************************
 * Class constructor and destructor
 */
Sensor::Sensor(unsigned char id):m_id(id),m_average(0) {
	m_eeprom_addr = EEPROM_OFFSET + m_id*8;		/**< This calc permit to store sensor data each 16 bytes after the EEPROM_OFFSET */
	load_param();
}
Sensor::~Sensor() {

}

/******************************************************************************
 * Sensor's methods
 */

char* Sensor::print(char *string){

	itoa(m_id,string,10);

	return string;
}

// This method print the sensor configuration for the sensor. It's a good idea to overload this function to do it more explicit for each sensor.
void Sensor::print_config(char arg_id0[],char arg_id1[], char arg_id2[]){
	char temp_conv[10];

	FSM::uart0.print("Sensor "); FSM::uart0.print(print(temp_conv)); FSM::uart0.print(" "); FSM::uart0.print(arg_id0); FSM::uart0.print(" enable : ");FSM::uart0.print(itoa(m_enable,temp_conv,10)); FSM::uart0.print("\r\n");
	FSM::uart0.print("	"); FSM::uart0.print(arg_id1); FSM::uart0.print(" Factor: "); FSM::uart0.print(dtostrf(m_factor,0,3,temp_conv)); FSM::uart0.print("	");
	FSM::uart0.print(arg_id2); FSM::uart0.print(" Offset: "); FSM::uart0.print(dtostrf(m_offset,0,3,temp_conv)); FSM::uart0.print("\r\n");

}

// This method calculate the average from the data array.
void Sensor::calc_average(){
	if(is_enable()){
		m_average=0;
		for(int i =0; i< FSM::logger.measure_max;++i)
		{
			m_average+=m_data[i];
		}
		m_average/=FSM::logger.measure_max;
	}

}

// The print_average method print the average's value in the string.
char* Sensor::print_average(unsigned char prec, char *string){
	if(is_enable()){
		char temp_conv[10];
		dtostrf(m_average,0,prec,temp_conv);
		strcat(string,temp_conv);
		strcat(string," ");

	}
	return string;
}

void Sensor::print_data_array()const{
	char temp[20];
	FSM::uart0.print("\r\n");
	for(int i=0;i<10;i++)
	{
		FSM::uart0.set(i+'0');FSM::uart0.set('	');FSM::uart0.print(dtostrf(m_data[i],0,3,temp));FSM::uart0.print("\r\n");
	}
}

/******************************************************************************
 * sens_param management
 */
// Load saved parameters for sensors from the eeprom
void Sensor::load_param(){
	m_factor = eeprom_read_float((float*)m_eeprom_addr);
	m_offset = eeprom_read_float((float*)m_eeprom_addr+5);
	m_enable = eeprom_read_byte((unsigned char*)m_eeprom_addr+6);
}

// Update saved parameters for sensors in the eeprom
void Sensor::update_param(){
	eeprom_update_float((float*)m_eeprom_addr,m_factor);
	eeprom_update_float((float*)m_eeprom_addr+5,m_offset);
	eeprom_update_byte((unsigned char*)m_eeprom_addr+6,m_enable);
}

// Initialize the eeprom memory and the sens_param array
void Sensor::initialize_param(){

}

/******************************************************************************
 * Hardware acess
 */
/**
 *	This method permit to do conversion from adc channel
 * @param channel
 * @return
 */
int Sensor::adc_value(unsigned char channel)
{
	ADMUX = (1<<REFS0)|channel;													/// Use the AVcc for voltage reference and add the channel number
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0) | (1<<ADPS0) | (1<<ADSC);		/// ADEN : conversion enable, ADPS[2:0] : divide the ADC frequency clock, ADSC : start one conversion

	while (ADCSRA & (1<<ADSC)); 												/// wait for conversion to complete

	ADCSRA = 0x00;																/// Stop the ADC (reduce power)

	return ADCW;
}
