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
 *    @file   Anemometer.cpp
 *    @author gilou
 *    @date   24 mai 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/FSM.h"
#include "../lib/Anemometer.h"

Anemometer::Anemometer(unsigned char id):Sensor(id) {
	if(m_id==0){
		// Clear All timer0 register
		TCCR0A = 0;
		TCCR0B = 0;
		TIMSK0 = 0;
		TCNT0 = 0;
	}
	else if(m_id==1){
		// Clear All timer1 register
		TCCR1A = 0;
		TCCR1B = 0;
		TCCR1C = 0;
		TIMSK1 = 0;
		TCNT1 = 0;
	}

	// Clear timer3 register
	TCCR3A = 0;
	TCCR3B = 0;
	TCCR3C = 0;
	TIMSK3 = 0;
}

Anemometer::~Anemometer() {

}

char* Anemometer::print(char *string){
	char temp_char[6];
	 strcpy(string,"Anemometer ");
	 strcat(string,itoa(m_id,temp_char,10));
	return string;
}

void Anemometer::read_value(unsigned char measure_number){
	double tmp_value = 0;

	if(m_id==0)
	{
		if(TCNT0!=0)									// Control to not devide 0
		{
			tmp_value = TCNT0 /  TIMER3_OVF_PERIODE;	// Convert pulse in hertz
		}
	}
	else if(m_id==1)
	{
		if(TCNT1!=0)									// Control to not devide 0
		{
			tmp_value = TCNT1 /  TIMER3_OVF_PERIODE;	// Convert pulse in hertz
		}
	}

	m_data[measure_number]=(tmp_value * m_factor) + m_offset; 	// apply the factor and offset before save the data
}

void Anemometer::start(){
	TCNT0 = 0;	// reset the timer 0 value register
	TCNT1 = 0;	// reset the timer 1 value register
	TCNT3 = 0;	// reset the timer 3 value register

	TCCR0B = _BV(CS02) | _BV(CS01) | _BV(CS00);	// use external clock on rising edge

	TCCR1B = _BV(ICNC1) | _BV(ICES1) | _BV(CS12) | _BV(CS11) | _BV(CS10);  //ICNC1: noise canceler, ICES1: capture on rising edge, CS1[2:0] external clock on rising edge

	TCCR3B = (1 << CS32) | (1 << CS30);	// CCS3[2:0] use 1024 prescaler to start the timer3
	TIMSK3 = (1 << TOIE1);	// start to count for TNCT4 timer

}

ISR(TIMER3_OVF_vect)
{
	TCCR0B = 0;		// stop Timer0
	TCCR1B = 0;		// stop Timer1
	TCCR3B = 0;		// stop Timer3

	FSM::flag_data_frequencies_ready=1;	// Set the flag
}
