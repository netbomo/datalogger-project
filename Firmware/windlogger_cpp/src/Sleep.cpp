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
 *    @file   Sleep.cpp
 *    @Author gilou
 *    @date   20 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <avr/io.h>
#include <util/delay.h>

#include "../lib/main.h"
#include "../lib/Usart.h"
#include "../lib/Sleep.h"

    // Operations

        Sleep::Sleep(){

        }

        Sleep::~Sleep(){

        }

        void Sleep::execute(){
        	_delay_ms(80);

//        	SMCR |= _BV(SE);			// enanble sleep mode
//        	SMCR |= _BV(SM2) | _BV(SM1) | _BV(SM0);		// Sleep mode = extended standby
        }

        void Sleep::print(Usart &usart){
        	usart.print("sleep state");
        }
