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

#include "../lib/main.h"
#include "../lib/FSM.h"
#include "../lib/Usart.h"
#include "../lib/Output.h"

// Operations
Output::Output (){

}

Output::~Output(){

}

void Output::execute (){
	//print();
	if(FSM::eeprom.output_enable||USART0){
		usart0_print();
	}
	if(FSM::eeprom.output_enable||USART1){
		usart1_print();
	}
	if(FSM::eeprom.output_enable||SD_CARD){
		SDCard_print();
	}
	if(FSM::eeprom.output_enable||WIFI){
		usart1_print();
	}
	if(FSM::eeprom.output_enable||GPRS){
		usart1_print();
	}
}

void Output::print(Usart &usart){
	usart.print("output state");
}

void Output::usart0_print (){

}

void Output::usart1_print (){

}

void Output::SDCard_print (){

}
