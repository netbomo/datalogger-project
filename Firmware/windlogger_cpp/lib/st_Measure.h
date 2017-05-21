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
 *    @file   Measure.h
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief The class Measure inherits from the class State.
 *
 *    This class organize measurement process and call sub functions to get data from inputs and average there.
 */


#ifndef MEASURE__HPP
#define MEASURE__HPP

#include "../lib/State.h"

/**
 * \brief The class Measure inherits from the class State.
 *
 * The timing measure is organize like :
 * @image html MEASURE_timing.svg
 */
class Measure : public State {


public :
	/******************************************************************************
	 * static hardware flags declaration
	 */
	static bool flag_data_frequencies_ready;	/**< Set when the timerX overflow, data are ready @todo timer number*/

	/******************************************************************************
	 * Constructor and destructor
	 */
	/**
	 * \brief Constructor
	 */
	Measure ();

	/**
	 * \brief Destructor
	 */
	virtual ~Measure();

	/******************************************************************************
	 * inherits from State
	 */
	/**
	 * \brief The execute method is the measure "main function"
	 * The execute function work as this scheme :
	 * @image html MEASURE.svg
	 */
	virtual void execute ();

	/**
	 * It's just a simple method to display the state name, use to debug
	 */
	virtual void print();

	bool isEqual(char *name)const;


private :
	/**
	 * \brief this method clear data value to save news
	 */
	void clear_data_array ();
	/**
	 * \brief this method initialize frequency measurement for anemometer and RPM
	 */
	void freq_init_measurement ();
	/**
	 * \brief read the windvane value en map it between 0 to 359 degres
	 */
	void windvane_value ();
	/**
	 * \brief read power value - not define realy (see emonLib)
	 */
	void power_read_value ();
	/**
	 * \brief read the temperature value from one DS18b20 sensor
	 */
	void temperature_read_value ();
	/**
	 * \brief read value from anemometer and RPM
	 */
	void freq_read_value ();
	/**
	 * \brief calcul average from the data array
	 */
	void calc_average ();

};

/**
 * \brief operator == overload to compare state name
 * @param a	first state
 * @param b	second state to compare to a
 * @return 1 if a.name==b.name else 0
 */
bool operator==(State const& a, State const& b);

#endif
