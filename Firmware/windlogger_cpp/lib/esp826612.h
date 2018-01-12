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
 *    @file   esp826612.h
 *    @author betzy
 *    @date   6 sept. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef ESP826612_H_
#define ESP826612_H_

#include <avr/io.h>
#include <util/delay.h>

#include "../lib/ringbufs.h"

class esp8266_12 {
public:
	/**
	 * Constructor
	 */
	esp8266_12();

	/**
	 * Destructor
	 */
	virtual ~esp8266_12();

	/** Constants definition */
	const char NB_OF_STRING = 5;		/*!< Wifi string number need to record 	*/
	const char NB_OF_CHAR = 30;			/*!< String char max number		 		*/

	// Module pins control

	// Reset the module

	inline void reset(){PORTD &= ~_BV(6);
						_delay_us(1);
						PORTD |= _BV(6);};

	// Module enable pin

	inline void enable(){PORTD |= _BV(4) | _BV(6);};

	inline void disable(){PORTD &= ~_BV(4);};

	// LDO enable pin

	inline void LDO_enable(){PORTD |= _BV(5);};

	inline void LDO_disable(){PORTD &= ~_BV(5);};

	// print

	void print(char *string);

	/****************************************************************************************
	 * This part is used only in the test mode or in the test firmware
	 */
	void test_init();
	void test_wifi_tx(char *string, const unsigned char nb_string);

private:
	unsigned char str_count;

};

#endif /* ESP826612_H_ */
