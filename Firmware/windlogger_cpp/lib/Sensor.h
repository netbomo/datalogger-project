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
 *    @file   Sensor.h
 *    @author gilou
 *    @date   21 mai 2017
 *    @brief  This class is the base class of sensor.
 *
 *    Detailed description of file.
 */
#ifndef SENSOR_H_
#define SENSOR_H_

#include "../lib/main.h"


/**
 * \brief The sensor class is the base container for sensor, need to create a inherited class from sensor
 *
 * The sensor class give a sensor skeleton, this allow you to create an inherited sensor class.</br>
 * Take a look on sen_** class for more details.
 */
class Sensor {

public:
	/******************************************************************************
	 * Constructor and destructor
	 */
	/**
	 * Class constructor
	 */
	Sensor(unsigned char id);

	/**
	 * class destructor
	 */
	virtual ~Sensor();

	/******************************************************************************
	 * setters
	 */
	inline void set_factor(double factor) {m_factor=factor; update_param();}

	inline void set_offset(double offset) {m_offset=offset; update_param();}

	/******************************************************************************
	 * Sensor's methods
	 */
	/**
	 * This method need to be overload to personalize config display for the sensor
	 * @return return string pointer
	 */
	virtual char* print(char *string);
	/**
	 * This method read the sensors value, needs to be implemented for each sensor
	 */
	void read_value(unsigned char measure_number);
	/**
	 * \brief This method print the sensor configuration for the sensor. It's a good idea to overload this function to do it more explicit for each sensor.
	 */
	void print_config(char arg_id1[], char arg_id2[]);

	/**
	* \brief This method calculate the average from the data array.
	*/
	void calc_average();

	/**
	 * \brief The print_average method print the average's value in the string.
	 */
	char* print_average(unsigned char prec, char *string);

	void print_data_array()const;

	/******************************************************************************
	 * sens_param management
	 */
	/**
	 * \brief Load saved parameters for sensors from the eeprom
	 * \return void
	 */
	void load_param();

	/**
	 * \brief Update saved parameters for sensors in the eeprom
	 * \return void
	 */
	void update_param();

	/**
	 * \brief Initialize the eeprom memory
	 * \return void
	 */
	void initialize_param();

protected:
	/******************************************************************************
	 * private members
	 */
	unsigned char m_id;	/**< this the sensor's id in the Eeprom::sensor_counter */

	unsigned char m_eeprom_addr;		/**< This is the eeprom address calc from the m_id and the EEPROM_OFFSET */

	double m_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the data array where witch data are put and from where the average is done. */

	double m_average;	/**< there is the result from the m_data average */

	double m_factor;	/** this is the sensor's factor. it initial value is 1, but if the sensor data need to be multiply, change the factor value. */

	double m_offset;	/** this is the sensor's offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */

	/******************************************************************************
	 * sens_param management
	 */
	/**
	 *\brief This method read return int value from the ADC channel
	 * @param channel (0 to 7) from ADC pin mapping
	 * @return int value of the ADC conversion
	 */
	int adc_value(unsigned char channel);
};

#endif /* SENSOR_H_ */
