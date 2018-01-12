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
 *    @file   powerDC.cpp
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
#include "../lib/powerDC.h"


/******************************************************************************
 * Class constructor and destructor
 */
powerDC::powerDC(unsigned char v_pin, unsigned char i_pin, unsigned char id):Power(v_pin, i_pin, id){
}

powerDC::~powerDC() {}


	/******************************************************************************
	 * Sensor methods
	 */
void powerDC::read_values(unsigned char measure_number, unsigned int timeout){

	  unsigned int numberOfSamples = 0;                        //This is now incremented
	  sumV = 0;
	  sumI = 0;
	  sumP = 0;

	  double SupplyVoltage = 4900; // VCC/2 is present on ADC5
	  showSupply = SupplyVoltage ;

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


	   Vmeas=adc_value(m_v_pin); // channel ?? for the voltage compensated with the offset
	   Imeas=adc_value(m_i_pin); // channel ?? for the current compensated with the offset


	   // display measures
	   //FSM::uart0.print(dtostrf(Vmeas,0,3,temp_char));FSM::uart0.print("	");FSM::uart0.print(dtostrf(Imeas,0,3,temp_char));FSM::uart0.print("\r\n");


	   //-----------------------------------------------------------------------------
	   // B) Apply digital low pass filters to extract the 2.5 V dc offset,
	   //     then subtract this - signal is now centred on 0 counts.
	   //-----------------------------------------------------------------------------

	   i_filtered = Imeas - i_offset;

	   sumV += Vmeas;			//voltage squared
	   sumI += i_filtered;			//current squared
	   sumP += Vmeas*i_filtered; 	// stores the power data

	   exit_by_timeout = (TCNT3-start)<timeout;

   }

   // check exit
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


}// end of the read_rms_value

// This method calculate the average from the data array.
void powerDC::calc_averages(){

		//initialize variables
	v_average=0;
	i_average=0;
	p_average=0;

	//char temp_char[10];
	//FSM::uart0.print("Detail average"); FSM::uart0.print("\r\n");

	//sums the data
	for(int counter =0; counter< FSM::logger.measure_max;++counter)
	{
		//FSM::uart0.print(dtostrf(p_data[counter],0,3,temp_char)); FSM::uart0.print(" ");

		v_average+=v_data[counter]/FSM::logger.measure_max;
		i_average+=i_data[counter]/FSM::logger.measure_max;
		p_average+=p_data[counter]/FSM::logger.measure_max;

	}
	 //FSM::uart0.print("-> ");FSM::uart0.print(dtostrf(p_average,0,3,temp_char)); FSM::uart0.print("\r\n");

}//end of calc_all function


/******************************************************************************
 * print methods
 */

char* powerDC::print(char *string){
	char temp_char[4];
	 strcpy(string,"Power DC ");
	 strcat(string,itoa(m_id,temp_char,10));
	return string;
}

// This method print the sensor configuration for the sensor. It's a good idea to overload this function to do it more explicit for each sensor.
void powerDC::print_config(char arg_id1[], char arg_id2[],char arg_id3[], char arg_id4[]){

	char temp_conv[10];

	FSM::uart0.print("Sensor "); FSM::uart0.print(print(temp_conv));FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id1); FSM::uart0.print(" Voltage Factor: ");	FSM::uart0.print(dtostrf(v_factor,0,3,temp_conv));
	FSM::uart0.print(arg_id2); FSM::uart0.print(" Voltage  Offset: ");	FSM::uart0.print(dtostrf(v_offset,0,3,temp_conv));FSM::uart0.print("\r\n");
	FSM::uart0.print(arg_id3); FSM::uart0.print(" Current Factor: ");	FSM::uart0.print(dtostrf(i_factor,0,3,temp_conv));
	FSM::uart0.print(arg_id4); FSM::uart0.print(" Current  Offset: ");	FSM::uart0.print(dtostrf(i_offset,0,3,temp_conv));FSM::uart0.print("\r\n"); //FSM::uart0.print("\r\n");
}

char* powerDC::print_average(unsigned char prec, char *string){
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

void powerDC::print_data_array(){
	char temp[20];
	FSM::uart0.print("\r\n");
	for(int i=0;i<FSM::logger.measure_max;i++)
	{
		FSM::uart0.set(i+'0');FSM::uart0.print(" v	");FSM::uart0.print(dtostrf(v_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" i	");FSM::uart0.print(dtostrf(i_data[i],0,3,temp));FSM::uart0.print("\r\n");
		FSM::uart0.set(i+'0');FSM::uart0.print(" p	");FSM::uart0.print(dtostrf(p_data[i],0,3,temp));FSM::uart0.print("\r\n");
	}
}
