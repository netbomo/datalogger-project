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
 *    @file   Usart.h
 *    @author gilou
 *    @date   26 avr. 2017
 *    @brief  This is the uart library for the atmega1284p
 *
 *    This class permit to define, the uart0 and uart1.
 *    It's possible to send data as char or string. Interruption needs to be set for receive char.
 */
#ifndef USART_H_
#define USART_H_

/**
 *    @brief  This is the uart library for the atmega1284p
 *
 *    This class permit to define, the uart0 and uart1.
 *    It's possible to send data as char or string. Interruption needs to be set for receive char.
 */
class Usart {
private:
	unsigned char m_usart;  /**< this is the uart number memory */

public:
	/******************************************************************************
	 * static hardware flags declaration
	 */
	static bool flag_rx0;			/*!< Is set by the uart0 rx interrupt>*/
	static bool flag_rx1;			/*!< Is set by the uart1 rx interrupt>*/

	/******************************************************************************
	 * static global variables
	 */
	static char data_udr0;			/*!< This register permit at the usart0 rx interrupt to store the UDR register>*/
	static char data_udr1;			/*!< This register permit at the usart1 rx interrupt to store the UDR register>*/

	/******************************************************************************
	 * static global constants
	 */
	static const unsigned int BR_9600 = 207;	/**< UBBRn registers value to work at 9600 baud**/
	static const unsigned int BR_57600 = 34;	/**< UBBRn registers value to work at 57600 baud**/
	static const unsigned int BR_115200 = 16;	/**< UBBRn registers value to work at 115200 baud**/


	/******************************************************************************
	 * Constructor
	 */
	/**
	 *	@brief Usart constructor on asynchronous mode
	 *	@param usart 0 for usart0 or 1 for usart1
	 *	@param baudrate	Use BR_XXXX constant to define baude rate speed
	 */
	Usart(unsigned char usart, unsigned int baudrate);


	/******************************************************************************
	 * sending data methods
	 */
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

};

#endif /* USART_H_ */
