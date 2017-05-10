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
 *    @file   Usart.h
 *    @Author gilou
 *    @date   26 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef USART_H_
#define USART_H_


class Usart {
public:
	/**
	 *	\brief USART constructor on asynchronous mode
	 *	\param usart 0 for usart0 or 1 for usart1
	 *	\param bauderate	Use BR_XXXX constant to define baude rate speed
	 */
	Usart(unsigned char usart, unsigned int baudrate);
	virtual ~Usart();

	/**
	 *	\brief USART char sender function
	 *	\param character The character to send on the USART0
	 */
	void set(char character);

	/**
	 *	\brief USART string sender function
	 * 	\param string The string to send on the USART0
	 */
	void print(char *string);


	// Constants
static const unsigned int BR_9600 = 207;	/**< UBBRn registers value to work at 9600 baud**/
static const unsigned int BR_57600 = 34;	/**< UBBRn registers value to work at 57600 baud**/
static const unsigned int BR_115200 = 16;	/**< UBBRn registers value to work at 115200 baud**/

private:
	unsigned char m_usart;
};

#endif /* USART_H_ */
