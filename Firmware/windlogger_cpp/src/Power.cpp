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
Power::Power(unsigned char id,unsigned char v_pin,unsigned char i_pin):m_id(id) {
	load_param();
	m_eeprom_addr = EEPROM_OFFSET + 100 + m_id*30;		/**< This calc permit to store sensor data each 16 bytes after the EEPROM_OFFSET */
}
Power::~Power() {
	// TODO Auto-generated destructor stub
}

/******************************************************************************
 * Power class methods
 */

char* Power::print(char *string){
	char temp_char[4];
	 strcpy(string,"Power ");
	 strcat(string,itoa(m_id,temp_char,10));
	return string;
}

// This method print the sensor configuration for the sensor. It's a good idea to overload this function to do it more explicit for each sensor.
void Power::print_config(char arg_id1[], char arg_id2[],char arg_id3[], char arg_id4[],char arg_id5[]){

	char temp_conv[10];

	FSM::uart0.print("Sensor"); FSM::uart0.print(print(temp_conv));FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id1); FSM::uart0.print(" Voltage Factor: ");	FSM::uart0.print(dtostrf(v_factor,0,3,temp_conv));
	FSM::uart0.print(arg_id2); FSM::uart0.print(" Voltage  Offset: ");	FSM::uart0.print(dtostrf(v_offset,0,3,temp_conv));
	FSM::uart0.print(arg_id3); FSM::uart0.print(" Voltage  Delay: ");	FSM::uart0.print(dtostrf(v_phase,0,3,temp_conv)); FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id4); FSM::uart0.print(" Current Factor: ");	FSM::uart0.print(dtostrf(i_factor,0,3,temp_conv));
	FSM::uart0.print(arg_id5); FSM::uart0.print(" Current  Offset: ");	FSM::uart0.print(dtostrf(i_offset,0,3,temp_conv)); FSM::uart0.print("\r\n");

}


void Power::read_rms_value(unsigned char measure_number, unsigned int crossings, unsigned int timeout){

	  unsigned int crossCount = 0;                             //Used to measure number of times threshold is crossed.
	  unsigned int numberOfSamples = 0;                        //This is now incremented
	  sumV = 0;
	  sumI = 0;
	  sumP = 0;

  //-------------------------------------------------------------------------------------------------------------------------
   // 1) Waits for the waveform to be close to 'zero' (mid-scale adc) part in sin curve.
   //-------------------------------------------------------------------------------------------------------------------------
   bool st=false;                                  //an indicator to exit the while loop

   unsigned long start = TCNT3;    //millis()-start makes sure it doesnt get stuck in the loop if there is an error.

   while(st==false)                                   //the while loop...
   {
	 startV = adc_value(m_v_pin);                    //using the voltage waveform
	 if ((startV < (ADC_COUNTS*0.55)) && (startV > (ADC_COUNTS*0.45))) st=true;  //check its within range
	 if ((TCNT3-start)>timeout)  st = true;
   }

   //-------------------------------------------------------------------------------------------------------------------------
   // 2) Main measurement loop
   //-------------------------------------------------------------------------------------------------------------------------
   Vmeas =0;
   start = TCNT3;			// read timer3 value (started for anemometer measure)

   while ((crossCount < crossings) && ((TCNT3-start)<timeout))
   {

	   numberOfSamples++; //Count number of times looped.
	   referenceV = Vmeas; //Used for delay/phase compensation

	   Vmeas=adc_value(m_v_pin)+v_offset; // channel ?? for the voltage compensated with the offset
	   Imeas=adc_value(m_i_pin)+i_offset; // channel ?? for the current compensated with the offset


//	   Vrms = 1;			//voltage squared
	   //	   Irms = 1;			//current squared
	   Vrms = Vmeas*Vmeas;			//voltage squared
	   Irms = Imeas*Imeas;			//current squared

	   Vshifted = referenceV + v_phase * (Vmeas - referenceV);  //calculates the voltage shift after the measurement

	   sumV += Vrms; 			// stores the rms voltage data
	   sumI += Irms; 			// stores the rms current data
	   sumP += Vshifted*Imeas; 	// stores the power data

	//-----------------------------------------------------------------------------
	// G) Find the number of times the voltage has crossed the initial voltage
	//    - every 2 crosses we will have sampled 1 wavelength
	//    - so this method allows us to sample an integer number of half wavelengths which increases accuracy
	//-----------------------------------------------------------------------------
	lastVCross = checkVCross;
	if (Vmeas > startV) checkVCross = true;
					 else checkVCross = false;
	if (numberOfSamples==1) lastVCross = checkVCross;

	if (lastVCross != checkVCross) crossCount++;

   }

   v_data[measure_number]= sqrt(sumV/numberOfSamples) *v_factor; 			// stores the rms voltage data
   i_data[measure_number]= sqrt(sumI/numberOfSamples) * i_factor; 			// stores the rms current data
   p_data[measure_number]= (sumP/numberOfSamples)*v_factor* i_factor; 	// stores the power data
   s_data[measure_number]= v_data[measure_number]*i_data[measure_number];
   pf_data[measure_number] = p_data[measure_number]/s_data[measure_number];

	//referenceV local ou global?

}// end of the read_rms_value



// This method calculate the average from the data array.
void Power::calc_all(){

	//initialize variables
	v_average=0;
	i_average=0;
	p_average=0;

	//sums the data
	for(int counter =0; counter< FSM::logger.measure_max;++counter)
	{
		v_average+=v_data[counter]/FSM::logger.measure_max;
		i_average+=i_data[counter]/FSM::logger.measure_max;
		p_average+=p_data[counter]/FSM::logger.measure_max;

	}

	//calculates the average
//	v_average/=FSM::logger.measure_max;
//	i_average/=FSM::logger.measure_max;
//	p_average/=FSM::logger.measure_max;

}//end of calc_all function

// The print_average method print the average's value in the string.
char* Power::print_average(unsigned char prec, char *string){
	char temp_conv[20];
	dtostrf(v_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(i_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(p_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	return string;
}

void Power::print_data_array()const{
	char temp[20];
	FSM::uart0.print("\r\n");
	for(int i=0;i<10;i++)
	{
		FSM::uart0.set(i+'0');FSM::uart0.set('	');FSM::uart0.print(dtostrf(v_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.set('	');FSM::uart0.print(dtostrf(i_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.set('	');FSM::uart0.print(dtostrf(p_data[i],0,3,temp));FSM::uart0.print("\r\n");
	}
}

/******************************************************************************
 * sens_param management
 */
// Load saved parameters for sensors from the eeprom
void Power::load_param(){
	m_v_pin = eeprom_read_float((float*)m_eeprom_addr);
	m_i_pin = eeprom_read_float((float*)m_eeprom_addr+2);
	v_factor = eeprom_read_float((float*)m_eeprom_addr+4);
	v_offset = eeprom_read_float((float*)m_eeprom_addr+9);
	v_phase = eeprom_read_float((float*)m_eeprom_addr+14);
	i_factor = eeprom_read_float((float*)m_eeprom_addr+19);
	i_offset = eeprom_read_float((float*)m_eeprom_addr+24);
}

// Update saved parameters for sensors in the eeprom
void Power::update_param(){
	eeprom_update_float((float*)m_eeprom_addr,m_v_pin);
	eeprom_update_float((float*)m_eeprom_addr+2,m_i_pin);
	eeprom_update_float((float*)m_eeprom_addr+4,v_factor);
	eeprom_update_float((float*)m_eeprom_addr+9,v_offset);
	eeprom_update_float((float*)m_eeprom_addr+14,v_phase);
	eeprom_update_float((float*)m_eeprom_addr+19,i_factor);
	eeprom_update_float((float*)m_eeprom_addr+24,i_offset);
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
 *	This method reads the reference VCC voltage used during the ADC convertiona
 * @param none
 * @return VCC
 */


long Power::readVcc() {
	long result;


#if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

#if defined(__AVR__)
	_delay_ms(2);                                        // Wait for Vref to settle
#endif
	ADCSRA |= _BV(ADSC);                             // Convert
	while (bit_is_set(ADCSRA,ADSC));
	result = ADCL;
	result |= ADCH<<8;
	result = READVCC_CALIBRATION_CONST / result;  //1100mV*1024 ADC steps http://openenergymonitor.org/emon/node/1186
	return result;
}
