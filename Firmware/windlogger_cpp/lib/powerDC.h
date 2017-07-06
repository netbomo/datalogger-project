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
 *    @file   powerDC.h
 *    @author gilou
 *    @date   5 juil. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef POWERDC_H_
#define POWERDC_H_

#include "../lib/Power.h"

class powerDC : public Power {
public:
	/******************************************************************************
	 * Class constructor and destructor
	 */
	powerDC(unsigned char v_pin, unsigned char i_pin, unsigned char id);

	virtual ~powerDC();

	/******************************************************************************
	 * Sensor methods
	 */

	void read_values(unsigned char measure_number, unsigned int timeout);

	void calc_averages();

	/******************************************************************************
	 * print methods
	 */

	char* print(char *string);

	/**
	 * \brief The print_config function display voltage and current factor and offset
	 * @param arg_id1
	 * @param arg_id2
	 * @param arg_id3
	 * @param arg_id4
	 * @param arg_id5
	 * @param arg_id6
	 */
	void print_config(char arg_id1[], char arg_id2[],char arg_id3[], char arg_id4[]);

	char* print_average(unsigned char prec, char *string);

	void print_data_array();


};

#endif /* POWERDC_H_ */
