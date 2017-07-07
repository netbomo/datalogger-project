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
 *    @file   ACpower.cpp
 *    @author Luiz Fernando Lavado Villa
 *    @date   26 Juin 2017
 *    @brief  Calculates the AC power of a certain system
 *
 *    This file is based on the emonlib from OpenEnergyMonitor. Some changes
 *    have been made to adapt the calculation of the AC power to the hardware
 *    setting of the Windlogger.
 */

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "../lib/FSM.h"
#include "../lib/Power.h"

/******************************************************************************
 * Class constructor and destructor
 */
Power::Power(unsigned char v_pin, unsigned char i_pin, unsigned char id):m_v_pin(v_pin),m_i_pin(i_pin),m_id(id) {
	load_param();
	m_eeprom_addr = EEPROM_OFFSET + 100 + m_id*30;		/**< This calc permit to store sensor data each 16 bytes after the EEPROM_OFFSET */

	Vmeas =0;
	v_offset = 512;

	checkVCross = false;
}
Power::~Power() {
	// TODO Auto-generated destructor stub
}

/******************************************************************************
 * Power class methods
 */


/******************************************************************************
 * sens_param management
 */
// Load saved parameters for sensors from the eeprom
void Power::load_param(){
	v_factor = eeprom_read_float((float*)m_eeprom_addr);
	v_offset = eeprom_read_float((float*)m_eeprom_addr+5);
	v_phase = eeprom_read_float((float*)m_eeprom_addr+10);
	i_factor = eeprom_read_float((float*)m_eeprom_addr+15);
	i_offset = eeprom_read_float((float*)m_eeprom_addr+20);
}

// Update saved parameters for sensors in the eeprom
void Power::update_param(){
	eeprom_update_float((float*)m_eeprom_addr,v_factor);
	eeprom_update_float((float*)m_eeprom_addr+5,v_offset);
	eeprom_update_float((float*)m_eeprom_addr+10,v_phase);
	eeprom_update_float((float*)m_eeprom_addr+15,i_factor);
	eeprom_update_float((float*)m_eeprom_addr+20,i_offset);
}

// Initialize the eeprom memory and the sens_param array
void Power::initialize_param(){

}

/******************************************************************************
 * Hardware acess
 */
/**
 *	This method permit to do conversion from adc channel
 * @param channel
 * @return
 */
int Power::adc_value(unsigned char channel) // Has to be rewritten
{
	ADMUX = (1<<REFS0)|channel;													/// Use the AVcc for voltage reference and add the channel number
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0) | (1<<ADPS0) | (1<<ADSC);		/// ADEN : conversion enable, ADPS[2:0] : divide the ADC frequency clock, ADSC : start one conversion

	while (ADCSRA & (1<<ADSC)); 												/// wait for conversion to complete

	ADCSRA = 0x00;																/// Stop the ADC (reduce power)

	return ADCW;
}


/**
 *	\brief This method reads the reference VCC voltage used during the ADC convertiona
 * @param none
 * @return VCC
 *
 * Draft for the moment, not used
 *
 * @ todo add 2.5V ref voltage on adc5
 */


//long Power::readVcc() {
//	long vRef1100;
//
////// calib 1100mV Vref
////#if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
////	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
////#endif
////
////#if defined(__AVR__)
////	_delay_ms(2);                                        // Wait for Vref to settle
////#endif
////	ADCSRA |= _BV(ADSC);                             // Convert
////	while (bit_is_set(ADCSRA,ADSC));
////	vRef1100 = ADCL;
////	vRef1100 |= ADCH<<8;
////	//vRef1100 = READVCC_CALIBRATION_CONST / vRef1100;  //1100mV*1024 ADC steps http://openenergymonitor.org/emon/node/1186
////
////	char temp_char[12];
////	FSM::uart0.print(dtostrf(vRef1100,0,3,temp_char));FSM::uart0.print("\r\n");
////
////	ADCSRA |= _BV(ADSC);                             // Convert
////	while (bit_is_set(ADCSRA,ADSC));
////	vRef1100 = ADCL;
////	vRef1100 |= ADCH<<8;
////	vRef1100 = READVCC_CALIBRATION_CONST / vRef1100;  //1100mV*1024 ADC steps http://openenergymonitor.org/emon/node/1186
////
////	FSM::uart0.print(dtostrf(vRef1100,0,3,temp_char));FSM::uart0.print("\r\n");
////
////	// use 1100mV vref to correct AVCC
////	ADMUX = (1<<REFS1)| (1<<REFS0)| 5;													/// Use internal 1.1V voltage reference and read Vcc/2 on the channel 5
////	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0) | (1<<ADSC);		/// ADEN : conversion enable, ADPS[2:0] : divide the ADC frequency clock, ADSC : start one conversion
////
////	while (ADCSRA & (1<<ADSC)); 												/// wait for conversion to complete
////
////	ADCSRA = 0x00;																/// Stop the ADC (reduce power)
////
//	long temp = 0;
////	long temp = ADCL;
////	temp |= ADCH<<8;
////
////	FSM::uart0.print(dtostrf(temp,0,3,temp_char));FSM::uart0.print("\r\n");
////
////	double Vref_filtered = (temp * vRef1100) / 1100;
////
////	Vref_filtered = Vref_filtered *5 / 1024;
////	FSM::uart0.print(dtostrf(temp,0,3,temp_char));FSM::uart0.print("\r\n");
//
//	return temp;
//
//
//}
