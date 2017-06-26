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
 *    @file   Windvane.h
 *    @author gilou
 *    @date   23 mai 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef WINDVANE_H_
#define WINDVANE_H_

#include "../lib/Sensor.h"

class Windvane: public Sensor {
public:
	Windvane(unsigned char id);
	virtual ~Windvane();

//	/**
//	 * This method need to be overload to personalize config display for the sensor
//	 * @return return string pointer
//	 */
//	char* print(char *string);
//
//	/**
//	 * This method read the sensors value, needs to be implemented for each sensor
//	 */
//	void read_value(unsigned char measure_number);
};

#endif /* WINDVANE_H_ */
