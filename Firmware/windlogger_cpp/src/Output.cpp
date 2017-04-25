/**
 *******************************************************************************
 *******************************************************************************
 *
 *	Licence :
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
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
 *    @Author gilou
 *    @date   20 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include "../lib/Output.hpp"

// Operations
Output::Output ():output_enable(0){

}

Output::~Output(){

}

void Output::execute (){
	if(output_enable||USART0){
		usart0_print();
	}
	if(output_enable||USART1){
		usart1_print();
	}
	if(output_enable||SD_CARD){
		SDCard_print();
	}
	if(output_enable||WIFI){
		usart1_print();
	}
	if(output_enable||GPRS){
		usart1_print();
	}
}

void Output::usart0_print (){

}

void Output::usart1_print (){

}

void Output::SDCard_print (){

}
