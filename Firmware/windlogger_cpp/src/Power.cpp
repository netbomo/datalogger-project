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
Power::Power(unsigned char id,bool isAC):m_id(id),m_isAC(isAC) {
	load_param();
	m_eeprom_addr = EEPROM_OFFSET + 100 + m_id*100;		/**< This calc permit to store sensor data each 16 bytes after the EEPROM_OFFSET */

	Vmeas =0;
	set_v_offset(512);

	/// @todo increase offset calibration with an external 2.5V reference
	m_vcc = 4900; // VCC/2 is present on ADC5

	checkVCross = false;

	s_average = 0;	/**< there is the result from the apparent power average */
	pf_average = 0;	/**< there is the result from the power factor average */

	///todo update to have different sensor offset init if isAC or not
	m_v_offset = 512;
	m_ct1_offset=512;


	for(int i=0;i<50;++i){
		Vmeas=adc_value(VOLTAGE_AC_PIN); // channel ?? for the voltage compensated with the offset
		ct1meas=adc_value(CT1_PIN); // channel ?? for the current compensated with the offset

		// display measures
		//FSM::uart0.print(dtostrf(Vmeas,0,3,temp_char));FSM::uart0.print("	");FSM::uart0.print(dtostrf(Imeas,0,3,temp_char));FSM::uart0.print("\r\n");


		//-----------------------------------------------------------------------------
		// B) Apply digital low pass filters to extract the 2.5 V or 1.65 V dc offset,
		//     then subtract this - signal is now centred on 0 counts.
		//-----------------------------------------------------------------------------
		m_v_offset = m_v_offset + ((Vmeas-m_v_offset)/1024);
		m_ct1_offset = m_ct1_offset + ((ct1meas-m_ct1_offset)/1024);
	}
}
Power::~Power() {
}

/******************************************************************************
 * Power class methods
 */
/******************************************************************************
 * Sensor methods
 */
void Power::read_values(unsigned char measure_number, unsigned char crossings, unsigned int timeout){
	unsigned char crossCount = 0;                             //Used to measure number of times threshold is crossed.
	unsigned int numberOfSamples = 0;                        //This is now incremented

	unsigned int saved_num_samples[4]={0};				// used for display debug value
	long int saved_timeout = 0;				// used for display debug value
	char temp_char[12];				// used for display debug value


	sumV = 0;
	sumI1 = 0;
	sumI2 = 0;
	sumI3 = 0;
	sumI4 = 0;
	sumP = 0;

	if (m_isAC){		// test if were are doing AC power measurement
		if(m_ct_enable>1){		// test if mono or multi ct
			// we're doing multi phases power measurement
			// we will "just" average voltage and each current sensor

		}
		else{	// we're doing monophased power measurement
			// we will calculate real and apparent power and power factor
			//-------------------------------------------------------------------------------------------------------------------------
			// 1) Waits for the waveform to be close to 'zero' (mid-scale adc) part in sin curve.
			//-------------------------------------------------------------------------------------------------------------------------
			bool st=false;                                  //an indicator to exit the while loop

			unsigned long start = TCNT3;    //millis()-start makes sure it doesnt get stuck in the loop if there is an error.

			while(st==false)                                   //the while loop...
			{
				startV = adc_value(VOLTAGE_AC_PIN);                    //using the voltage waveform
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

					numberOfSamples++; //Count number of times looped.
					referenceV = v_filtered; //Used for delay/phase compensation


					Vmeas=adc_value(VOLTAGE_AC_PIN); // channel ?? for the voltage compensated with the offset
					ct1meas=adc_value(CT1_PIN); // channel ?? for the current compensated with the offset

					// display measures
					//FSM::uart0.print(dtostrf(Vmeas,0,3,temp_char));FSM::uart0.print("	");FSM::uart0.print(dtostrf(Imeas,0,3,temp_char));FSM::uart0.print("\r\n");


					//-----------------------------------------------------------------------------
					// B) Apply digital low pass filters to extract the 2.5 V or 1.65 V dc offset,
					//     then subtract this - signal is now centred on 0 counts.
					//-----------------------------------------------------------------------------
					m_v_offset = m_v_offset + ((Vmeas-m_v_offset)/1024);
					v_filtered = Vmeas - m_v_offset;
					m_ct1_offset = m_ct1_offset + ((ct1meas-m_ct1_offset)/1024);
					i1_filtered = ct1meas - m_ct1_offset;

					sumV += v_filtered*v_filtered;			//voltage squared
					sumI1 += i1_filtered*i1_filtered;			//current squared

					Vshifted = referenceV + m_v_phase * (v_filtered - referenceV);  //calculates the voltage shift after the measurement

					sumP += Vshifted*i1_filtered; 	// stores the power data
					//sumP += v_filtered*i_filtered; 	// debug

					//-----------------------------------------------------------------------------
					// G) Find the number of times the voltage has crossed the initial voltage
					//    - every 2 crosses we will have sampled 1 wavelength
					//    - so this method allows us to sample an integer number of half wavelengths which increases accuracy
					//-----------------------------------------------------------------------------



					lastVCross = checkVCross;
					if (Vmeas > startV) checkVCross = true;
					else checkVCross = false;
					if (numberOfSamples==1) lastVCross = checkVCross;

					if (lastVCross != checkVCross) {
						saved_num_samples[crossCount] = numberOfSamples;
						crossCount++;
					}

					exit_by_crosscount = crossCount < crossings;

					saved_timeout = (TCNT3-start);

					exit_by_timeout = saved_timeout<timeout;

				}

				// check exit

				//		   if(!exit_by_crosscount) FSM::uart0.print("exit_by_crosscount\r\n");
				//		   if(!exit_by_timeout) FSM::uart0.print("exit_by_timeout\r\n");

				//-------------------------------------------------------------------------------------------------------------------------
				// 3) Post loop calculations
				//-------------------------------------------------------------------------------------------------------------------------
				//Calculation of the root of the mean of the voltage and current squared (rms)
				//Calibration coefficients applied.

				double V_RATIO = m_v_factor *((m_vcc/1000.0) / (ADC_COUNTS));

				double I_RATIO = m_ct1_factor *((m_vcc/1000.0) / (ADC_COUNTS));

				v_data[measure_number]= sqrt(sumV/numberOfSamples) *V_RATIO; 			// stores the rms voltage data
				ct1_data[measure_number]= sqrt(sumI1/numberOfSamples) * I_RATIO; 			// stores the rms current data
				p_data[measure_number]= (sumP/numberOfSamples)*V_RATIO* I_RATIO; 	// stores the power data
				s_data[measure_number]= v_data[measure_number]*ct1_data[measure_number];
				pf_data[measure_number] = p_data[measure_number]/s_data[measure_number];

		}

	}
	else{			// we're doing DC measurement
		// we will "just" average voltage and each current sensor

		unsigned long start = TCNT3;    //millis()-start makes sure it doesn't get stuck in the loop if there is an error.

		//char temp_char[10];
		//FSM::uart0.print("m_v_pin : ");FSM::uart0.print(itoa(m_v_pin,temp_char,10));FSM::uart0.print("	m_i_pin : 0");FSM::uart0.print(itoa(m_i_pin,temp_char,10));FSM::uart0.print(" m_id : ");FSM::uart0.print(itoa(m_id,temp_char,10));FSM::uart0.print("\r\n");

		//-------------------------------------------------------------------------------------------------------------------------
		// 2) Main measurement loop
		//-------------------------------------------------------------------------------------------------------------------------

		bool exit_by_timeout = true;

		while ((exit_by_timeout))
		{
			//char temp_char[12];

			numberOfSamples++; //Count number of times looped.


			Vmeas=adc_value(VOLTAGE_DC_PIN); // channel ?? for the voltage compensated with the offset
			ct1meas=adc_value(CT1_PIN); // channel ?? for the current compensated with the offset


			// display measures
			//FSM::uart0.print(dtostrf(Vmeas,0,3,temp_char));FSM::uart0.print("	");FSM::uart0.print(dtostrf(Imeas,0,3,temp_char));FSM::uart0.print("\r\n");


			//-----------------------------------------------------------------------------
			// B) Apply digital low pass filters to extract the 2.5 V dc offset,
			//     then subtract this - signal is now centred on 0 counts.
			//-----------------------------------------------------------------------------

			i1_filtered = ct1meas - m_ct1_offset;

			sumV += Vmeas;			//voltage squared
			sumI1 += i1_filtered;			//current squared
			sumP += Vmeas*i1_filtered; 	// stores the power data

			exit_by_timeout = (TCNT3-start)<timeout;

		}

		// check exit
		//if(!exit_by_timeout) FSM::uart0.print("exit_by_timeout\r\n");

		//-------------------------------------------------------------------------------------------------------------------------
		// 3) Post loop calculations
		//-------------------------------------------------------------------------------------------------------------------------
		//Calculation of the root of the mean of the voltage and current squared (rms)
		//Calibration coefficients applied.

		double V_RATIO = m_v_factor *((m_vcc/1000.0) / (ADC_COUNTS));

		double I_RATIO = m_ct1_factor *((m_vcc/1000.0) / (ADC_COUNTS));

		v_data[measure_number]= sumV/numberOfSamples *V_RATIO; 			// stores the rms voltage data
		ct1_data[measure_number]= sumI1/numberOfSamples * I_RATIO; 			// stores the rms current data
		p_data[measure_number]= sumP/numberOfSamples*V_RATIO* I_RATIO; 	// stores the power data


	}


}

void Power::calc_averages(){
	//initialize variables
	v_average=0;
	ct1_average=0;
	ct2_average=0;
	ct3_average=0;
	ct4_average=0;
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
		ct1_average+=ct1_data[counter]/FSM::logger.measure_max;
		ct1_average+=ct2_data[counter]/FSM::logger.measure_max;// todo calc only if enable
		ct1_average+=ct3_data[counter]/FSM::logger.measure_max;
		ct1_average+=ct4_data[counter]/FSM::logger.measure_max;
		p_average+=p_data[counter]/FSM::logger.measure_max;
		s_average+=s_data[counter]/FSM::logger.measure_max;
		pf_average+=pf_data[counter]/FSM::logger.measure_max;

	}
	 //FSM::uart0.print("-> ");FSM::uart0.print(dtostrf(p_average,0,3,temp_char)); FSM::uart0.print("\r\n");

}//end of calc_all function

/******************************************************************************
 * print methods
 */

char* Power::print(char *string){
	char temp_char[4];
	strcpy(string,"Power ");
	strcat(string,itoa(m_id,temp_char,10));
	return string;
}

/**
 * \brief The print_config function display voltage and current factor and offset
 * @param arg_id1
 * @param arg_id2
 * @param arg_id3
 * @param arg_id4
 * @param arg_id5
 * @param arg_id6
 */
void Power::print_config(char arg_id1[], char arg_id2[],char arg_id3[], char arg_id4[],char arg_id5[], char arg_id6[]){
	char temp_conv[10];

	FSM::uart0.print("Sensor "); FSM::uart0.print(print(temp_conv));FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id1); FSM::uart0.print(" Voltage Factor: ");	FSM::uart0.print(dtostrf(m_v_factor,0,3,temp_conv));
	FSM::uart0.print(arg_id2); FSM::uart0.print(" Voltage  Offset: ");	FSM::uart0.print(dtostrf(m_v_offset,0,3,temp_conv));
	FSM::uart0.print(arg_id5); FSM::uart0.print(" Voltage  Delay: ");	FSM::uart0.print(dtostrf(m_v_phase,0,3,temp_conv)); FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id3); FSM::uart0.print(" Current Factor: ");	FSM::uart0.print(dtostrf(m_ct1_factor,0,3,temp_conv));//// todo add ctx display by testing m_ct_enable
	FSM::uart0.print(arg_id4); FSM::uart0.print(" Current  Offset: ");	FSM::uart0.print(dtostrf(m_ct1_offset,0,3,temp_conv)); //FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id6); FSM::uart0.print(" Supply Voltage: ");	FSM::uart0.print(dtostrf(m_vcc,0,3,temp_conv)); FSM::uart0.print("\r\n");

}

char* Power::print_average(unsigned char prec, char *string){
	char temp_conv[12];

	dtostrf(v_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(ct1_average,0,prec,temp_conv);/// todo test if enable by create ct const
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(ct2_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(ct3_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(ct4_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(p_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(s_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	dtostrf(pf_average,0,prec,temp_conv);
	strcat(string,temp_conv);
	strcat(string," ");
	return string;
}

void Power::print_data_array(){
	char temp[20];
	FSM::uart0.print("\r\n");
	for(int i=0;i<FSM::logger.measure_max;i++)
	{
		FSM::uart0.set(i+'0');FSM::uart0.print(" v	");FSM::uart0.print(dtostrf(v_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" i1	");FSM::uart0.print(dtostrf(ct1_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" i2	");FSM::uart0.print(dtostrf(ct2_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" i3	");FSM::uart0.print(dtostrf(ct3_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" i4	");FSM::uart0.print(dtostrf(ct4_data[i],0,3,temp));FSM::uart0.print("\r\n");
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
	m_v_factor = eeprom_read_float((float*)m_eeprom_addr);
	m_v_offset = eeprom_read_float((float*)m_eeprom_addr+5);
	m_v_phase = eeprom_read_float((float*)m_eeprom_addr+10);
	m_ct1_factor = eeprom_read_float((float*)m_eeprom_addr+15);
	m_ct1_offset = eeprom_read_float((float*)m_eeprom_addr+20);
	m_ct2_factor = eeprom_read_float((float*)m_eeprom_addr+25);
	m_ct2_offset = eeprom_read_float((float*)m_eeprom_addr+30);
	m_ct3_factor = eeprom_read_float((float*)m_eeprom_addr+35);
	m_ct3_offset = eeprom_read_float((float*)m_eeprom_addr+40);
	m_ct4_factor = eeprom_read_float((float*)m_eeprom_addr+45);
	m_ct4_offset = eeprom_read_float((float*)m_eeprom_addr+50);
	m_ct_enable = eeprom_read_byte((uint8_t*)m_eeprom_addr+55);

}

// Update saved parameters for sensors in the eeprom
void Power::update_param(){
	eeprom_update_float((float*)m_eeprom_addr, m_v_factor);
	eeprom_update_float((float*)m_eeprom_addr+5, m_v_offset);
	eeprom_update_float((float*)m_eeprom_addr+10, m_v_phase);
	eeprom_update_float((float*)m_eeprom_addr+15, m_ct1_factor);
	eeprom_update_float((float*)m_eeprom_addr+20, m_ct1_offset);
	eeprom_update_float((float*)m_eeprom_addr+25, m_ct2_factor);
	eeprom_update_float((float*)m_eeprom_addr+30, m_ct2_offset);
	eeprom_update_float((float*)m_eeprom_addr+35, m_ct3_factor);
	eeprom_update_float((float*)m_eeprom_addr+40, m_ct3_offset);
	eeprom_update_float((float*)m_eeprom_addr+45, m_ct4_factor);
	eeprom_update_float((float*)m_eeprom_addr+50, m_ct4_offset);
	eeprom_update_byte((uint8_t*)m_eeprom_addr+55, m_ct_enable);
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
