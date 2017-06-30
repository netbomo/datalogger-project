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

#include <string.h>
#include <stdlib.h>

#include "../lib/main.h"
#include "../lib/FSM.h"
#include "../lib/Usart.h"
#include "../lib/st_Output.h"


Output::Output(){
	strcpy(m_name,"output\0");
	FSM::logger.output_enable=USART0;
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
		SDCard_print();
	}
	if(FSM::logger.output_enable||WIFI){
		usart1_print();
	}
	if(FSM::logger.output_enable||GPRS){
		usart1_print();
	}

	FSM::uart0.print("exit output\r\n");
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
	strcat(string,ultoa(FSM::timestamp,tmp_char,10));	// Add timestamp
	strcat(string," ");

	FSM::anemo1.print_average(1,string);					// add data average
	FSM::anemo2.print_average(1,string);					// add data average
	FSM::windvane.print_average(0,string);
	FSM::powerAC.print_average(3,string);

	strcat(string,"\r\n");								// Close string

	FSM::uart0.print(string);							// Send the string on uart0
}

void Output::usart1_print (){

}

void Output::SDCard_print (){

}
