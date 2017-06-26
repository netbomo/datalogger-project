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
 *    @file   Windvane.cpp
 *    @author gilou
 *    @date   23 mai 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <string.h>

#include "../lib/Windvane.h"

Windvane::Windvane(unsigned char id):Sensor(id) {
}

Windvane::~Windvane() {
}

char* Windvane::print(char *string){
	return strcpy(string,"windvane");
}

void Windvane::read_value(unsigned char measure_number){
	m_data[measure_number]=(adc_value(6)*m_factor)+m_offset; // channel 6 for the windvane
}
