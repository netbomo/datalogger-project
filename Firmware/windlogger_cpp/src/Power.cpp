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

char* Power::print(char *string){
	char temp_char[4];
	 strcpy(string,"Power ");
	 strcat(string,itoa(m_id,temp_char,10));
	return string;
}

// This method print the sensor configuration for the sensor. It's a good idea to overload this function to do it more explicit for each sensor.
void Power::print_config(char arg_id1[], char arg_id2[],char arg_id3[], char arg_id4[],char arg_id5[],char arg_id6[]){

	char temp_conv[10];

	FSM::uart0.print("Sensor "); FSM::uart0.print(print(temp_conv));FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id1); FSM::uart0.print(" Voltage Factor: ");	FSM::uart0.print(dtostrf(v_factor,0,3,temp_conv));
	FSM::uart0.print(arg_id2); FSM::uart0.print(" Voltage  Offset: ");	FSM::uart0.print(dtostrf(v_offset,0,3,temp_conv));
	FSM::uart0.print(arg_id3); FSM::uart0.print(" Voltage  Delay: ");	FSM::uart0.print(dtostrf(v_phase,0,3,temp_conv)); FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id4); FSM::uart0.print(" Current Factor: ");	FSM::uart0.print(dtostrf(i_factor,0,3,temp_conv));
	FSM::uart0.print(arg_id5); FSM::uart0.print(" Current  Offset: ");	FSM::uart0.print(dtostrf(i_offset,0,3,temp_conv)); //FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id6); FSM::uart0.print(" Supply Voltage: ");	FSM::uart0.print(dtostrf(showSupply,0,3,temp_conv)); FSM::uart0.print("\r\n");

}


void Power::read_rms_value(unsigned char measure_number, unsigned int crossings, unsigned int timeout){

	  unsigned int crossCount = 0;                             //Used to measure number of times threshold is crossed.
	  unsigned int numberOfSamples = 0;                        //This is now incremented
	  sumV = 0;
	  sumI = 0;
	  sumP = 0;

	  double SupplyVoltage = 5000; // VCC/2 is present on ADC5
	  //long SupplyVoltage = readVcc();
	  showSupply = SupplyVoltage ;

  //-------------------------------------------------------------------------------------------------------------------------
   // 1) Waits for the waveform to be close to 'zero' (mid-scale adc) part in sin curve.
   //-------------------------------------------------------------------------------------------------------------------------
   bool st=false;                                  //an indicator to exit the while loop

   unsigned long start = TCNT3;    //millis()-start makes sure it doesnt get stuck in the loop if there is an error.

   while(st==false)                                   //the while loop...
   {
	 startV = adc_value(7);                    //using the voltage waveform
	 if ((startV < (ADC_COUNTS*0.55)) && (startV > (ADC_COUNTS*0.45))) {
		 st=true;  //check its within range
		 //FSM::uart0.print("exit while st startV\r\n");
	 }
	 if ((TCNT3-start)>timeout)  {
		 st = true;
		 //FSM::uart0.print("exit while st timeout\r\n");
	 }
   }

   //-------------------------------------------------------------------------------------------------------------------------
   // 2) Main measurement loop
   //-------------------------------------------------------------------------------------------------------------------------

   start = TCNT3;			// read timer3 value (started for anemometer measure)

   bool exit_by_crosscount = true;

   bool exit_by_timeout = true;

   while ((exit_by_crosscount) && (exit_by_timeout))
   {
	   char temp_char[12];

	   numberOfSamples++; //Count number of times looped.
	   referenceV = Vmeas; //Used for delay/phase compensation

	   ///@ todo use m_v_pin and m_i_pin def and correct the bug
	   Vmeas=adc_value(7); // channel ?? for the voltage compensated with the offset
	   Imeas=adc_value(5); // channel ?? for the current compensated with the offset

	   // display measures
	    FSM::uart0.print(dtostrf(Vmeas,0,3,temp_char));FSM::uart0.print("	");FSM::uart0.print(dtostrf(Imeas,0,3,temp_char));FSM::uart0.print("\r\n");


	   //-----------------------------------------------------------------------------
	   // B) Apply digital low pass filters to extract the 2.5 V or 1.65 V dc offset,
	   //     then subtract this - signal is now centred on 0 counts.
	   //-----------------------------------------------------------------------------
	   v_offset = v_offset + ((Vmeas-v_offset)/1024);
	   v_filtered = Vmeas - v_offset;
	   i_offset = i_offset + ((Imeas-i_offset)/1024);
	   i_filtered = Imeas - i_offset;

	   sumV += v_filtered*v_filtered;			//voltage squared
	   sumI += i_filtered*i_filtered;			//current squared

	   Vshifted = referenceV + v_phase * (v_filtered - referenceV);  //calculates the voltage shift after the measurement

	   sumP += Vshifted*i_filtered; 	// stores the power data

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

	   exit_by_crosscount = crossCount < crossings;

	   exit_by_timeout = (TCNT3-start)<timeout;

   }

   // check exit

   //if(!exit_by_crosscount) FSM::uart0.print("exit_by_crosscount\r\n");
   //if(!exit_by_timeout) FSM::uart0.print("exit_by_timeout\r\n");

   //-------------------------------------------------------------------------------------------------------------------------
   // 3) Post loop calculations
   //-------------------------------------------------------------------------------------------------------------------------
   //Calculation of the root of the mean of the voltage and current squared (rms)
   //Calibration coefficients applied.

   double V_RATIO = v_factor *((SupplyVoltage/1000.0) / (ADC_COUNTS));

   double I_RATIO = i_factor *((SupplyVoltage/1000.0) / (ADC_COUNTS));

   v_data[measure_number]= sqrt(sumV/numberOfSamples) *V_RATIO; 			// stores the rms voltage data
   i_data[measure_number]= sqrt(sumI/numberOfSamples) * I_RATIO; 			// stores the rms current data
   p_data[measure_number]= (sumP/numberOfSamples)*V_RATIO* I_RATIO; 	// stores the power data
   s_data[measure_number]= v_data[measure_number]*i_data[measure_number];
   pf_data[measure_number] = p_data[measure_number]/s_data[measure_number];

   //referenceV local ou global?

}// end of the read_rms_value

void Power::read_dc_value(unsigned char measure_number, unsigned int timeout){

	  //unsigned int crossCount = 0;                             //Used to measure number of times threshold is crossed.
	  unsigned int numberOfSamples = 0;                        //This is now incremented
	  sumV = 0;
	  sumI = 0;
	  sumP = 0;

	  double SupplyVoltage = 5000; // VCC/2 is present on ADC5
	  //long SupplyVoltage = readVcc();
	  showSupply = SupplyVoltage ;

  //-------------------------------------------------------------------------------------------------------------------------
   // 1) Waits for the waveform to be close to 'zero' (mid-scale adc) part in sin curve.
   //-------------------------------------------------------------------------------------------------------------------------
   bool st=false;                                  //an indicator to exit the while loop

   unsigned long start = TCNT3;    //millis()-start makes sure it doesnt get stuck in the loop if there is an error.

//   while(st==false)                                   //the while loop...
//   {
//	 startV = adc_value(7);                    //using the voltage waveform
//	 if ((startV < (ADC_COUNTS*0.55)) && (startV > (ADC_COUNTS*0.45))) {
//		 st=true;  //check its within range
//		 //FSM::uart0.print("exit while st startV\r\n");
//	 }
//	 if ((TCNT3-start)>timeout)  {
//		 st = true;
//		 //FSM::uart0.print("exit while st timeout\r\n");
//	 }
//   }

   //-------------------------------------------------------------------------------------------------------------------------
   // 2) Main measurement loop
   //-------------------------------------------------------------------------------------------------------------------------

   //start = TCNT3;			// read timer3 value (started for anemometer measure)

   bool exit_by_crosscount = true;

   bool exit_by_timeout = true;

   while ((exit_by_timeout))
   {
	   char temp_char[12];

	   numberOfSamples++; //Count number of times looped.
	   //referenceV = Vmeas; //Used for delay/phase compensation

	   ///@ todo use m_v_pin and m_i_pin def and correct the bug
	   Vmeas=adc_value(7); // channel ?? for the voltage compensated with the offset
	   Imeas=adc_value(4); // channel ?? for the current compensated with the offset

	   // display measures
	    //FSM::uart0.print(dtostrf(Vmeas,0,3,temp_char));FSM::uart0.print("	");FSM::uart0.print(dtostrf(Imeas,0,3,temp_char));FSM::uart0.print("\r\n");


	   //-----------------------------------------------------------------------------
	   // B) Apply digital low pass filters to extract the 2.5 V or 1.65 V dc offset,
	   //     then subtract this - signal is now centred on 0 counts.
	   //-----------------------------------------------------------------------------
	  // v_offset = v_offset + ((Vmeas-v_offset)/1024);
//	   v_filtered = Vmeas - v_offset;
	   i_offset = 512;
	   i_filtered = Imeas - i_offset;

	   sumV += Vmeas;			//voltage squared
	   sumI += i_filtered;			//current squared

	   //Vshifted = referenceV + v_phase * (v_filtered - referenceV);  //calculates the voltage shift after the measurement

	   sumP += Vmeas*i_filtered; 	// stores the power data

	   //-----------------------------------------------------------------------------
	   // G) Find the number of times the voltage has crossed the initial voltage
	   //    - every 2 crosses we will have sampled 1 wavelength
	   //    - so this method allows us to sample an integer number of half wavelengths which increases accuracy
	   //-----------------------------------------------------------------------------
//	   lastVCross = checkVCross;
//	   if (Vmeas > startV) checkVCross = true;
//	   else checkVCross = false;
//	   if (numberOfSamples==1) lastVCross = checkVCross;
//
//	   if (lastVCross != checkVCross) crossCount++;
//
//	   exit_by_crosscount = crossCount < crossings;

	   exit_by_timeout = (TCNT3-start)<timeout;

   }

   // check exit

   //if(!exit_by_crosscount) FSM::uart0.print("exit_by_crosscount\r\n");
   //if(!exit_by_timeout) FSM::uart0.print("exit_by_timeout\r\n");

   //-------------------------------------------------------------------------------------------------------------------------
   // 3) Post loop calculations
   //-------------------------------------------------------------------------------------------------------------------------
   //Calculation of the root of the mean of the voltage and current squared (rms)
   //Calibration coefficients applied.

   double V_RATIO = v_factor *((SupplyVoltage/1000.0) / (ADC_COUNTS));

   double I_RATIO = i_factor *((SupplyVoltage/1000.0) / (ADC_COUNTS));

   v_data[measure_number]= sumV/numberOfSamples *V_RATIO; 			// stores the rms voltage data
   i_data[measure_number]= sumI/numberOfSamples * I_RATIO; 			// stores the rms current data
   p_data[measure_number]= sumP/numberOfSamples*V_RATIO* I_RATIO; 	// stores the power data
//   s_data[measure_number]= v_data[measure_number]*i_data[measure_number];
//   pf_data[measure_number] = p_data[measure_number]/s_data[measure_number];

   //referenceV local ou global?

}// end of the read_rms_value



// This method calculate the average from the data array.
void Power::calc_all(){

		//initialize variables
	v_average=0;
	i_average=0;
	p_average=0;
	s_average=0;
	pf_average=0;

	//char temp_char[10];
	//FSM::uart0.print("Detail average"); FSM::uart0.print("\r\n");

	//sums the data
	for(int counter =0; counter< FSM::logger.measure_max;++counter)
	{
		//FSM::uart0.print(dtostrf(p_data[counter],0,3,temp_char)); FSM::uart0.print(" ");

		v_average+=v_data[counter]/FSM::logger.measure_max;
		i_average+=i_data[counter]/FSM::logger.measure_max;
		p_average+=p_data[counter]/FSM::logger.measure_max;
//		s_average+=s_data[counter]/FSM::logger.measure_max;
//		pf_average+=pf_data[counter]/FSM::logger.measure_max;

	}
	 //FSM::uart0.print("-> ");FSM::uart0.print(dtostrf(p_average,0,3,temp_char)); FSM::uart0.print("\r\n");

}//end of calc_all function

// The print_average method print the average's value in the string.
char* Power::print_average(unsigned char prec, char *string){
	char temp_conv[12];
	dtostrf(v_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(i_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(p_average,0,prec,temp_conv);
//	strcat(string,temp_conv);
//	strcat(string," ");
//	dtostrf(s_average,0,prec,temp_conv);
//	strcat(string,temp_conv);
//	strcat(string," ");
//	dtostrf(pf_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	return string;
}

char* Power::print_dc_average(unsigned char prec, char *string){
	char temp_conv[12];
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
	for(int i=0;i<FSM::logger.measure_max;i++)
	{
		FSM::uart0.set(i+'0');FSM::uart0.print(" v	");FSM::uart0.print(dtostrf(v_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" i	");FSM::uart0.print(dtostrf(i_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" p	");FSM::uart0.print(dtostrf(p_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" s	");FSM::uart0.print(dtostrf(s_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" pf	");FSM::uart0.print(dtostrf(pf_data[i],0,3,temp));FSM::uart0.print("\r\n");
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
 *	\brief This method reads the reference VCC voltage used during the ADC convertiona
 * @param none
 * @return VCC
 *
 * Draft for the moment, not used
 */


long Power::readVcc() {
	long vRef1100;

// calib 1100mV Vref
#if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

#if defined(__AVR__)
	_delay_ms(2);                                        // Wait for Vref to settle
#endif
	ADCSRA |= _BV(ADSC);                             // Convert
	while (bit_is_set(ADCSRA,ADSC));
	vRef1100 = ADCL;
	vRef1100 |= ADCH<<8;
	//vRef1100 = READVCC_CALIBRATION_CONST / vRef1100;  //1100mV*1024 ADC steps http://openenergymonitor.org/emon/node/1186

	char temp_char[12];
	FSM::uart0.print(dtostrf(vRef1100,0,3,temp_char));FSM::uart0.print("\r\n");

	ADCSRA |= _BV(ADSC);                             // Convert
	while (bit_is_set(ADCSRA,ADSC));
	vRef1100 = ADCL;
	vRef1100 |= ADCH<<8;
	vRef1100 = READVCC_CALIBRATION_CONST / vRef1100;  //1100mV*1024 ADC steps http://openenergymonitor.org/emon/node/1186

	FSM::uart0.print(dtostrf(vRef1100,0,3,temp_char));FSM::uart0.print("\r\n");

	// use 1100mV vref to correct AVCC
	ADMUX = (1<<REFS1)| (1<<REFS0)| 5;													/// Use internal 1.1V voltage reference and read Vcc/2 on the channel 5
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0) | (1<<ADSC);		/// ADEN : conversion enable, ADPS[2:0] : divide the ADC frequency clock, ADSC : start one conversion

	while (ADCSRA & (1<<ADSC)); 												/// wait for conversion to complete

	ADCSRA = 0x00;																/// Stop the ADC (reduce power)

	long temp = ADCL;
	temp |= ADCH<<8;

	FSM::uart0.print(dtostrf(temp,0,3,temp_char));FSM::uart0.print("\r\n");

	double Vref_filtered = (temp * vRef1100) / 1100;

	Vref_filtered = Vref_filtered *5 / 1024;
	FSM::uart0.print(dtostrf(temp,0,3,temp_char));FSM::uart0.print("\r\n");

	return temp;


}
