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
 *    @file   esp826612.cpp
 *    @author betzy
 *    @date   6 sept. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#include "../lib/esp826612.h"
#include <avr/io.h>
#include <stdlib.h>

#include "../lib/FSM.h"

esp8266_12::esp8266_12():str_count(0){
	// Define portD[4-7] as output
	DDRD |= 0xF0;
	PORTD = 0x00;

}

esp8266_12::~esp8266_12() {

}

void  esp8266_12::print(char *string){
	FSM::uart1.print(string);
}

/****************************************************************************************
 * This part is used only in the test mode or in the test firmware
 */
void esp8266_12::test_init(){
	str_count = 0;
	char recording = 0, temp =0;

	FSM::uart0.print("wifi initialization (~10s) :\r\n");

		/** Waiting for the char 'E' first char of "EmonESP xxxxxx string" **********/
		while(!recording){
			if(FSM::uart1.flag_rx1 && FSM::uart1.data_udr1 == 'E')		// Test if the new char is a 'E'
			{
				ringBufS_put(FSM::pRBuf, FSM::uart1.data_udr1);	// if right save it in the first line, first column.
				recording = 1;												// we can go starting record char
				FSM::uart1.flag_rx1 = 0;									// reset the flag
			}
			_delay_us(1);													// If no an 'E', delay needs do not leave the loop empty (compilation optimization)
		}

		/** Record the 5 string from the wifi module *****************************/
		while(str_count < NB_OF_STRING && recording){						// record the 5 strings											// recording to find '\r' char, end of line
			if(FSM::uart1.flag_rx1 && FSM::uart1.data_udr1 != '\n')									// if new char comes
			{
				temp = FSM::uart1.data_udr1;
				ringBufS_put(FSM::pRBuf,temp);	// Put the char in the buffer pointed by pRBuf
				if(temp == '\r') str_count++;			// if the char is '\r', one line recorded
				FSM::uart1.flag_rx1 = 0;									// reset the flag
			}
			if(ringBufS_full(FSM::pRBuf))recording=0;
		}// end of while


		ringBufS_put(FSM::pRBuf,'\0');

		/** Print strings on uart0 ***********************************************/
		while(!ringBufS_empty(FSM::pRBuf)){
			FSM::uart0.set(ringBufS_get(FSM::pRBuf));						// print string
		}

		//FSM::usart0.print("\r\nnumber of string : "); FSM::usart0.set(str_count+'0');
		str_count = 0;
}



void esp8266_12::test_wifi_tx(char *string, const unsigned char nb_string){
	// local variables
	str_count = 0;
	char recording = 0, temp =0;

	// sent a test
	print(string);

	/** Waiting for the char 'G' first char of "EmonESP xxx string" **********/
	while(!recording){
		if(FSM::uart1.flag_rx1 && FSM::uart1.data_udr1 == 'G')		// Test if the new char is a 'E'
		{
			temp = FSM::uart1.data_udr1;
			ringBufS_put(FSM::pRBuf,temp);	// Put the char in the buffer pointed by pRBuf
			recording = 1;												// we can go starting record char
			FSM::uart1.flag_rx1 = 0;									// reset the flag
		}
		_delay_us(1);													// If no an 'E', delay needs do not leave the loop empty (compilation optimization)
	}

	/** Record the 5 string from the wifi module *****************************/
	while(str_count < nb_string && recording){						// record the 5 strings											// recording to find '\r' char, end of line
		if(FSM::uart1.flag_rx1 && FSM::uart1.data_udr1 != '\n')									// if new char comes
		{
			temp = FSM::uart1.data_udr1;
			ringBufS_put(FSM::pRBuf,temp);	// Put the char in the buffer pointed by pRBuf
			if(temp == '\r') str_count++;			// if the char is '\r', one line recorded
			FSM::uart1.flag_rx1 = 0;									// reset the flag
		}
		if(ringBufS_full(FSM::pRBuf))recording=0;
	}// end of while


	/** Print strings on uart0 ***********************************************/
	while(!ringBufS_empty(FSM::pRBuf)){
		FSM::uart0.set(ringBufS_get(FSM::pRBuf));						// print string
	}

	//FSM::usart0.print("\r\nnumber of string : "); FSM::usart0.set(str_count+'0');
	str_count = 0;

}
