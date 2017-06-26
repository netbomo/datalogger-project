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
	Anemometer();
	virtual ~Anemometer();
};

#endif /* ANEMOMETER_H_ */
