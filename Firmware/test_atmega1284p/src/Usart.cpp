/**
 *******************************************************************************
 *******************************************************************************
 *
 *	License :
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
 *    @file   Usart.cpp
 *    @Author gilou
 *    @date   26 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../lib/Usart.h"

char Usart::data_udr0 = 0;
unsigned char Usart::flag_rx0 = 0;

Usart::Usart():m_usart(255) {}

Usart::Usart(unsigned char usart, unsigned int baudrate) {
	// TODO Auto-generated constructor stub

	switch (usart) {
		case 0:
			m_usart = usart;
			UBRR0 = baudrate; // set baud rate

			UCSR0A = _BV(U2X0); // for the baud rate setting

			UCSR0B = _BV(RXCIE0) | _BV(RXEN0) |_BV(TXEN0); //enable receiver and transmitter
			UCSR0C = _BV(UPM01) | _BV(UPM00) | _BV(UCSZ01) | _BV(UCSZ00);	// Set frame format : 8data 2stop bit

			break;
		case 1:

			break;
		default:
			break;
	}

}

Usart::~Usart() {
	// TODO Auto-generated destructor stub
}

ISR(USART0_RX_vect)							//sous routine d'interruption lors d'une reception
{
	Usart::data_udr0 = UDR0;						//récupération du registre de donnée UDR0 dans la variable globale data_udr0
	Usart::flag_rx0 = 1;					//mise à 1 du drapeau de reception pour interruption
}

void Usart::set(char character){
	switch (m_usart) {
	case 0:
		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) )
			;
		/* Put data into buffer, sends the data */
		UDR0 = character;
		break;
	case 1:

		break;
	default:
		break;
	}
}

void Usart::print(char* string){
	uint8_t i = 0;

		while(*(string+i)!='\0')
		{
			set(*(string+i));			//envoi de caractère à la volée contenu dans les adresses de data (adresses incrémentée par i)
			i++;								//tant que la data ne contient pas '\0'
		}
}


