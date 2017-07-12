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
 *    @file   Output.cpp
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief  The class Output inherits from the class State.
 *
 *    @todo Detailed description of file.
 */

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/main.h"
#include "../lib/FSM.h"
#include "../lib/Usart.h"
#include "../lib/SDcard.h"
#include "../lib/st_Output.h"


Output::Output(){
	last_month = FSM::rtc.get_month();		// initialize month reference in order to set data in the correct filename on the SD card

	FSM::SD.build_filename(FSM::rtc.get_month(),FSM::rtc.get_year());	// build the filename

	strcpy(m_name,"output\0");
	FSM::logger.output_enable= USART0 | SD_CARD;			/// @todo enable must be choose by config
}

Output::~Output(){

}

void Output::execute (){

	if(FSM::logger.output_enable||USART0){
		usart0_print();
	}
	if(FSM::logger.output_enable||USART1){
		usart1_print();
	}
	if(FSM::logger.output_enable||SD_CARD){
		FSM::uart0.print("SD present? : ");
		FSM::uart0.set((PINC&&(1<<3))+'0');FSM::uart0.print("\r\n");

		if(PINC&&(1<<3)){
			// test the month, if is different, build a new filename in order to store data in the sd card
			if(last_month!=FSM::rtc.get_month()){
				FSM::SD.build_filename(FSM::rtc.get_month(),FSM::rtc.get_year());
				FSM::SD.print_filename();
			}
			FSM::uart0.print("go to sd write ->");
			SDCard_print();
		}
		else FSM::uart0.print("Pas de carte SD\r\n");

	}

	if(FSM::logger.output_enable||WIFI){
		usart1_print();
	}
	if(FSM::logger.output_enable||GPRS){
		usart1_print();
	}

	//FSM::uart0.print("exit output\r\n");
}

void Output::print(){
	FSM::uart0.print(m_name);
}

bool Output::isEqual(char *name)const {
	return State::isEqual(name);
}


void Output::usart0_print (){
	char tmp_char[20];	// char array used for number to string conversion

	strcpy(string,itoa(15,tmp_char,10));				// Start by copy node number
	strcat(string," ");									// todo change " " separation by a variable
	//strcat(string,itoa(FSM::rtc.get_second(),tmp_char,10));
	strcat(string,ultoa(FSM::timestamp,tmp_char,10));	// Add timestamp
	strcat(string," ");

	FSM::anemo1.print_average(1,string);					// add data average
	FSM::anemo2.print_average(1,string);					// add data average
	FSM::windvane.print_average(0,string);
	FSM::pDC.print_average(3,string);

	strcat(string,"\r\n");								// Close string

	FSM::uart0.print(string);							// Send the string on uart0
}

void Output::usart1_print (){

}

void Output::SDCard_print (){

	FSM::SD.init();
	FSM::uart0.print(" init ->");
	FSM::SD.write("It's working\r\n");
	FSM::uart0.print(" write ->");
	FSM::SD.write("fine on 2 lines!");
	FSM::uart0.print(" write too!");
}
