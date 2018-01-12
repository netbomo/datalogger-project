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
 *    @file   Anemometer.h
 *    @author gilou
 *    @date   24 mai 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef ANEMOMETER_H_
#define ANEMOMETER_H_

#include "../lib/Sensor.h"

class Anemometer: public Sensor {
public:
	Anemometer(unsigned char id);
	virtual ~Anemometer();

	/**
	 * This method need to be overload to personalize sensor config display's
	 * @return return string pointer
	 */
	char* print(char *string);

	/**
	 * This method read the sensors value, needs to be implemented for each sensor
	 */
	void read_value(unsigned char measure_number);

	static void start();

	const double TIMER3_OVF_PERIODE = (double) 65536/15625.0; // 15625 constant to convert from 1/fclock*1024(prescaler) in second
};

#endif /* ANEMOMETER_H_ */
