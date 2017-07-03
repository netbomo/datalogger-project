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
 *    @file   FSM.h
 *    @author gilou
 *    @date   24 mai 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef FSM_H_
#define FSM_H_

#include "../lib/Windvane.h"
#include "../lib/Anemometer.h"

class FSM {
public:
	FSM();
	virtual ~FSM();

	static Windvane windvane;
	static Anemometer anemo1;

};

#endif /* FSM_H_ */