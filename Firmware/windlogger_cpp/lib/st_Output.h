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
 *    @file   Output.h
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief  The class Output inherits from the class State.
 *
 *    @todo Detailed description of file.
 */

#ifndef OUTPUT__HPP
#define OUTPUT__HPP

#include "../lib/State.h"

class Output : public State {

	// Attributes
public :
	// Operations
	Output ();

	virtual ~Output();

	virtual void execute ();

	virtual void print();

	bool isEqual(char *name)const;

	void usart0_print ();

	void usart1_print ();

	void SDCard_print ();

	// Constants
	static const unsigned char USART0 = 1;		/// enable constant for USART0

	static const unsigned char USART1 = 2;		/// enable constant for USART1

	static const unsigned char SD_CARD = 4;	/// enable constant for SD_CARD

	static const unsigned char WIFI = 8;		/// enable constant for WIFI

	static const unsigned char GPRS = 16;		/// enable constant for GPRS


};

#endif
