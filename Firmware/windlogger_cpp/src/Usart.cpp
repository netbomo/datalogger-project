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
 *    @file   Usart.cpp
 *    @author gilou
 *    @date   26 avr. 2017
 *    @brief  This is the uart library for the atmega1284p
 *
 *    This class permit to define, the uart0 and uart1.
 *    It's possible to send data as char or string. Interruption needs to be set for receive char.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../lib/FSM.h"
#include "../lib/main.h"
#include "../lib/Usart.h"

/******************************************************************************
 * static software flags definition need to be out of constructor or function.
 */
	char Usart::data_udr0 = 0;			//	This register permit at the usart0 rx interrupt to store the UDR register
	bool Usart::flag_rx0 = 0;			//	Is set by the uart0 rx interrupt

Usart::Usart(unsigned char usart, unsigned int baudrate) {
	// TODO add uart1 support

	switch (usart) {
		case 0:
			m_usart = usart;	// save uart number
			UBRR0 = baudrate; 	// set the baudrate

			UCSR0A = _BV(U2X0); // for the baud rate setting

			UCSR0B = _BV(RXCIE0) | _BV(RXEN0) |_BV(TXEN0); 					//enable receiver and transmitter
			UCSR0C = _BV(UPM01) | _BV(UPM00) | _BV(UCSZ01) | _BV(UCSZ00);	// Set frame format : 8data 2stop bit

			break;
		case 1:

			break;
		default:
			break;
	}

}


/******************************************************************************
 * \fn Interrupt Sub routine(uart0 reception vector)
 * \brief this sub routine comes when a char is received on the uart0
 */
ISR(USART0_RX_vect)
{
	Usart::data_udr0 = UDR0;		// save the char in the data_udr0 register
	Usart::flag_rx0 = 1;			// and set the flag
}

/******************************************************************************
 * sending data methods
 */
// USART char sender function
void Usart::set(char character){
	switch (m_usart) {
	case 0:
		while ( !( UCSR0A & (1<<UDRE0)) );	// Wait for empty transmit buffer
		UDR0 = character;					// Put data into buffer, sends the data
		break;
	case 1:

		break;
	default:
		break;
	}
}

// USART string sender function
void Usart::print(char* string){
	uint8_t i = 0;

		while(*(string+i)!='\0')	// the char is not equal to '\0,' the end string character
		{
			set(*(string+i));		// send the char
			i++;					// increase the index
		}
}


