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

esp8266_12::esp8266_12() {
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
	// local variables
	char uart1_array[NB_OF_STRING][NB_OF_CHAR] = {0};						// use to store uart module's initializing strings
	unsigned char uart1_al = 0,uart1_ac = 0;								// index for lines and colunms
	char recording = 0;

		FSM::uart0.print("\r\nWifi test :\r\n");

		/** Waiting for the char 'E' first char of "EmonESP xxxxxx string" **********/
		while(!recording){
			if(FSM::uart1.flag_rx1 && FSM::uart1.data_udr1 == 'E')		// Test if the new char is a 'E'
			{
				uart1_array[uart1_al][uart1_ac] = FSM::uart1.data_udr1;	// if right save it in the first line, first column.
				uart1_ac++;													// increase the column
				recording = 1;												// we can go starting record char
				FSM::uart1.flag_rx1 = 0;									// reset the flag
			}
			_delay_us(1);													// If no an 'E', delay needs do not leave the loop empty (compilation optimization)
		}

		/** Record the 5 string from the wifi module *****************************/
		for(uart1_al = 0; uart1_al < NB_OF_STRING; uart1_al++){						// record the 5 strings
			while(recording){												// recording to find '\r' char, end of line
				if(FSM::uart1.flag_rx1)									// if new char comes
				{
					uart1_array[uart1_al][uart1_ac] = FSM::uart1.data_udr1;	// Save it in the column
					if((uart1_array[uart1_al][uart1_ac] == '\r') || (uart1_ac >= (NB_OF_CHAR-2))) recording = 0;			// if the char is '\r', stop recording
					uart1_ac++;													// increase the column
					FSM::uart1.flag_rx1 = 0;									// reset the flag
				}
				_delay_us(1);												// needs do not leave the loop empty (compilation optimization)
			}// end of while


			uart1_array[uart1_al][uart1_ac] = '\0';							// add '\0' char, end of string
			uart1_ac = 0;													// reset column number
			//FSM::usart0.print(uart1_array[uart1_al]);
			uart1_al++;														// increase line number
			recording = 1;													// and restart recording
		}// end of for

		/** Print strings on uart0 ***********************************************/
		for(uart1_al = 0; uart1_al < NB_OF_STRING; uart1_al++){
			//FSM::usart0.set(uart1_al+'0');FSM::usart0.print(" - ");
			FSM::uart0.print(uart1_array[uart1_al]);						// print string
		}// end of for

}

void esp8266_12::test_wifi_tx(char *string){
	// local variables
	unsigned char nb_string = 13;
	char uart1_array[nb_string][NB_OF_CHAR] = {0};						// use to store uart module's initializing strings
	unsigned char uart1_al = 0,uart1_ac = 0;								// index for lines and colunms
	char recording = 0;

		// sent a test
		print(string);

		/** Waiting for the char 'G' first char of "EmonESP xxxxx string" **********/
		while(!recording){
			if(FSM::uart1.flag_rx1 && FSM::uart1.data_udr1 == 'G')		// Test if the new char is a 'E'
			{
				uart1_array[uart1_al][uart1_ac] = FSM::uart1.data_udr1;	// if right save it in the first line, first column.
				uart1_ac++;													// increase the column
				recording = 1;												// we can go starting record char
				FSM::uart1.flag_rx1 = 0;									// reset the flag
			}
			_delay_us(1);													// If no an 'E', delay needs do not leave the loop empty (compilation optimization)
		}


	for(uart1_al = 0; uart1_al < nb_string; uart1_al++){						// record the 5 strings
		while(recording){												// recording to find '\r' char, end of line
			if(FSM::uart1.flag_rx1)									// if new char comes
			{
				uart1_array[uart1_al][uart1_ac] = FSM::uart1.data_udr1;	// Save it in the column
				//FSM::usart0.print(itoa(NB_OF_CHAR-2,temp_conv,10));
				if( ((uart1_array[uart1_al][uart1_ac] == '\r') ||uart1_ac >= (NB_OF_CHAR-2))) recording = 0;			// if the char is '\r', stop recording
				uart1_ac++;													// increase the column
				FSM::uart1.flag_rx1 = 0;									// reset the flag
			}
			_delay_us(1);												// needs do not leave the loop empty (compilation optimization)
		}// end of while


		uart1_array[uart1_al][uart1_ac] = '\0';							// add '\0' char, end of string
		uart1_ac = 0;													// reset column number
		//FSM::usart0.print(uart1_array[uart1_al]);
		uart1_al++;														// increase line number
		recording = 1;													// and restart recording
	}// end of for

	/** Print strings on uart0 ***********************************************/
	for(uart1_al = 0; uart1_al < nb_string; uart1_al++){
		//FSM::usart0.set(uart1_al+'0');FSM::usart0.print(" - ");
		FSM::uart0.print(uart1_array[uart1_al]);						// print string
	}// end of for

}
