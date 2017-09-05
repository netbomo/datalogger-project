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
 *    @file   powerAC.cpp
 *    @author gilou
 *    @date   5 juil. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../lib/FSM.h"
#include "../lib/powerAC.h"

	/******************************************************************************
	 * Class constructor and destructor
	 */
	powerAC::powerAC(unsigned char v_pin, unsigned char i_pin, unsigned char id):Power(v_pin, i_pin, id){
		s_average = 0;	/**< there is the result from the apparent power average */
		pf_average = 0;	/**< there is the result from the power factor average */

		v_offset = 512;
		i_offset=512;


		for(int i=0;i<50;++i){
			Vmeas=adc_value(m_v_pin); // channel ?? for the voltage compensated with the offset
			Imeas=adc_value(m_i_pin); // channel ?? for the current compensated with the offset

			// display measures
			//FSM::uart0.print(dtostrf(Vmeas,0,3,temp_char));FSM::uart0.print("	");FSM::uart0.print(dtostrf(Imeas,0,3,temp_char));FSM::uart0.print("\r\n");


			//-----------------------------------------------------------------------------
			// B) Apply digital low pass filters to extract the 2.5 V or 1.65 V dc offset,
			//     then subtract this - signal is now centred on 0 counts.
			//-----------------------------------------------------------------------------
			v_offset = v_offset + ((Vmeas-v_offset)/1024);
			i_offset = i_offset + ((Imeas-i_offset)/1024);
		}

	}

	powerAC::~powerAC(){

	}

	/******************************************************************************
	 * Sensor methods
	 */

	void powerAC::read_values(unsigned char measure_number, unsigned char crossings, unsigned int timeout){
		 unsigned char crossCount = 0;                             //Used to measure number of times threshold is crossed.
			  unsigned int numberOfSamples = 0;                        //This is now incremented

			  unsigned int saved_num_samples[4]={0};				// used for display debug value
			  long int saved_timeout = 0;				// used for display debug value
			   char temp_char[12];				// used for display debug value


			  sumV = 0;
			  sumI = 0;
			  sumP = 0;

			  /// @todo increase offset calibration with an external 2.5V reference
			  double SupplyVoltage = 4900; // VCC/2 is present on ADC5
			  //long SupplyVoltage = readVcc();
			  showSupply = SupplyVoltage ;

		  //-------------------------------------------------------------------------------------------------------------------------
		   // 1) Waits for the waveform to be close to 'zero' (mid-scale adc) part in sin curve.
		   //-------------------------------------------------------------------------------------------------------------------------
		   bool st=false;                                  //an indicator to exit the while loop

		   unsigned long start = TCNT3;    //millis()-start makes sure it doesnt get stuck in the loop if there is an error.

		   while(st==false)                                   //the while loop...
		   {
			 startV = adc_value(m_v_pin);                    //using the voltage waveform
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


			   Vmeas=adc_value(m_v_pin); // channel ?? for the voltage compensated with the offset
			   Imeas=adc_value(m_i_pin); // channel ?? for the current compensated with the offset

			   // display measures
			    //FSM::uart0.print(dtostrf(Vmeas,0,3,temp_char));FSM::uart0.print("	");FSM::uart0.print(dtostrf(Imeas,0,3,temp_char));FSM::uart0.print("\r\n");


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

		   double V_RATIO = v_factor *((SupplyVoltage/1000.0) / (ADC_COUNTS));

		   double I_RATIO = i_factor *((SupplyVoltage/1000.0) / (ADC_COUNTS));

		   v_data[measure_number]= sqrt(sumV/numberOfSamples) *V_RATIO; 			// stores the rms voltage data
		   i_data[measure_number]= sqrt(sumI/numberOfSamples) * I_RATIO; 			// stores the rms current data
		   p_data[measure_number]= (sumP/numberOfSamples)*V_RATIO* I_RATIO; 	// stores the power data
		   s_data[measure_number]= v_data[measure_number]*i_data[measure_number];
		   pf_data[measure_number] = p_data[measure_number]/s_data[measure_number];

//		   // measuring data : numberof samples, timeout...
//
//		   FSM::uart0.print("number of samples : \r\n");
//		   FSM::uart0.print(ultoa(saved_num_samples[0], temp_char,10 )); FSM::uart0.print("	");
//		   FSM::uart0.print(ultoa(saved_num_samples[1], temp_char,10 )); FSM::uart0.print("	");
//		   FSM::uart0.print(ultoa(saved_num_samples[2], temp_char,10 )); FSM::uart0.print("	");
//		   FSM::uart0.print(ultoa(saved_num_samples[3], temp_char,10 )); FSM::uart0.print("\r\n");
//		   FSM::uart0.print("timeout : ");
//		   FSM::uart0.print(ultoa(saved_timeout, temp_char,10 )); FSM::uart0.print("\r\n");
	}

	void powerAC::calc_averages(){
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
		s_average+=s_data[counter]/FSM::logger.measure_max;
		pf_average+=pf_data[counter]/FSM::logger.measure_max;

	}
	 //FSM::uart0.print("-> ");FSM::uart0.print(dtostrf(p_average,0,3,temp_char)); FSM::uart0.print("\r\n");

}//end of calc_all function

	/******************************************************************************
	 * print methods
	 */

	char* powerAC::print(char *string){
		char temp_char[4];
		 strcpy(string,"Power AC ");
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
	void powerAC::print_config(char arg_id1[], char arg_id2[],char arg_id3[], char arg_id4[],char arg_id5[], char arg_id6[]){
		char temp_conv[10];

			FSM::uart0.print("Sensor "); FSM::uart0.print(print(temp_conv));FSM::uart0.print("\r\n");
			FSM::uart0.print(arg_id1); FSM::uart0.print(" Voltage Factor: ");	FSM::uart0.print(dtostrf(v_factor,0,3,temp_conv));
			FSM::uart0.print(arg_id2); FSM::uart0.print(" Voltage  Offset: ");	FSM::uart0.print(dtostrf(v_offset,0,3,temp_conv));
			FSM::uart0.print(arg_id5); FSM::uart0.print(" Voltage  Delay: ");	FSM::uart0.print(dtostrf(v_phase,0,3,temp_conv)); FSM::uart0.print("\r\n");
			FSM::uart0.print(arg_id3); FSM::uart0.print(" Current Factor: ");	FSM::uart0.print(dtostrf(i_factor,0,3,temp_conv));
			FSM::uart0.print(arg_id4); FSM::uart0.print(" Current  Offset: ");	FSM::uart0.print(dtostrf(i_offset,0,3,temp_conv)); //FSM::uart0.print("\r\n");
			FSM::uart0.print(arg_id6); FSM::uart0.print(" Supply Voltage: ");	FSM::uart0.print(dtostrf(showSupply,0,3,temp_conv)); FSM::uart0.print("\r\n");

	}

	char* powerAC::print_average(unsigned char prec, char *string){
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
		dtostrf(s_average,0,prec,temp_conv);
		strcat(string,temp_conv);
		strcat(string," ");
		dtostrf(pf_average,0,prec,temp_conv);
		strcat(string,temp_conv);
		strcat(string," ");
		return string;
	}

	void powerAC::print_data_array(){
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
