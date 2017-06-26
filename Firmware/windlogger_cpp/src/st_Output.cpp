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

#include "../lib/main.h"
#include "../lib/FSM.h"
#include "../lib/Usart.h"
#include "../lib/st_Output.h"


Output::Output(){
	strcpy(m_name,"output\0");
	ptr_string = string;
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
}

void Output::print(){
	FSM::uart0.print(m_name);
}

bool Output::isEqual(char *name)const {
	return State::isEqual(name);
}


void Output::usart0_print (){
	ptr_string =&string[0];
	FSM::windvane.print_average(ptr_string);  /// pas fonctionnel

	//strcpy(string,"\r\n");
	FSM::uart0.print(string);
	FSM::uart0.print("\r\n");
	ptr_string =&string[0];
}

void Output::usart1_print (){

}

void Output::SDCard_print (){

}
