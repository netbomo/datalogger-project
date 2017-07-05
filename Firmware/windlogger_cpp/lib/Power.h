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
 *    @file   ACpower.h
 *    @author Luiz Fernando Lavado Villa
 *    @date   26 Juin 2017
 *    @brief  Calculates the AC power of a certain system
 *
 *    This file is based on the emonlib from OpenEnergyMonitor. Some changes
 *    have been made to adapt the calculation of the AC power to the hardware
 *    setting of the Windlogger.
 *
 *
 *
 *
 *  Created on: Jun 26, 2017
 *      Author: luiz
 */




#ifndef POWER_H_
#define POWER_H_

#include "../lib/main.h"

// define theoretical vref calibration constant for use in readvcc()
// 1100mV*1024 ADC steps http://openenergymonitor.org/emon/node/1186
// override in your code with value for your specific AVR chip
// determined by procedure described under "Calibrating the internal reference voltage" at
// http://openenergymonitor.org/emon/buildingblocks/calibration
#ifndef READVCC_CALIBRATION_CONST
#define READVCC_CALIBRATION_CONST 1126400L
#endif

/**
 * \brief The Power class is the base container for measuring and calculating power.
 *
 * The Power class gives base layer for any type of power calculation, either AC or DC.</br>
 * Take a look on pow_** class for more details.
 */


class Power {

public:
	/******************************************************************************
	 * Constructor and destructor
	 */
	/**
	 * Class constructor
	 */
	Power(unsigned char v_pin, unsigned char  i_pin, unsigned char id);

	/**
	 * class destructor
	 */
	virtual ~Power();

	/******************************************************************************
	 * setters
	 */

	/******************************************************************************
	 * voltage configuration setters
	 */

	inline void set_v_factor(double factor) {v_factor=factor; update_param();}

	inline void set_v_offset(double offset) {v_offset=offset; update_param();}

	inline void set_v_phase(double phase) {v_phase=phase; update_param();}

	/******************************************************************************
	 * current configuration setters
	 */

	inline void set_i_factor(double factor) {i_factor=factor; update_param();}

	inline void set_i_offset(double offset) {i_offset=offset; update_param();}

	/******************************************************************************
	 * Power class methods
	 */
	/**
	 * This method displays the config for the power class
	 * @return return string pointer
	 */
	virtual char* print(char *string);
	/**
	 * This method read the sensors value for the power class and calculates the rms values. Needs a reference V to calculate power.
	 */
	void read_rms_value(unsigned char measure_number, unsigned int crossings, unsigned int timeout);


	void read_dc_value(unsigned char measure_number, unsigned int timeout);


	/**
	 * \brief This method print the configuration for the power measurement. It's a good idea to overload this function to do it more explicit for each sensor.
	 */
	void print_config(char arg_id1[], char arg_id2[],char arg_id3[], char arg_id4[],char arg_id5[],char arg_id6[]);

	/**
	* \brief This method calculate the mean RMS voltage, mean RMS current, active power, apparent power and power factor from the data array.
	*/
	void calc_all();

	/**
	 * \brief The print_average method print the average's value in the string.
	 */
	char* print_average(unsigned char prec, char *string);

	char* print_dc_average(unsigned char prec, char *string);

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

	/**
	 * \brief Reads the VCC used during the ADC conversion
	 * \return void
	 */

	long readVcc();



protected:
	/******************************************************************************
	 * private constante
	 */
	/******************************************************************************
	 * */
	const int ADC_COUNTS = 1024;

	/******************************************************************************
	 * private members
	 */
	/******************************************************************************
	 * Pin
	 */
	unsigned char m_v_pin;
	unsigned char m_i_pin;

	/******************************************************************************
	 * power data
	 */
	double p_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the data array where where real power data is stored and used to calculate the average. */
	double s_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the data array where where apparent power data is stored and used to calculate the average. */
	double pf_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the data array where where power factor data is stored and used to calculate the average. */

	double p_average;	/**< there is the result from the real power average */
	double s_average;	/**< there is the result from the apparent power average */
	double pf_average;	/**< there is the result from the power factor average */

	/******************************************************************************
	 * voltage data
	 */
	double v_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the data array where voltage data is stored and used to calculate the average. */
	double v_average;	/**< there is the result from the m_data average */
	double v_factor;	/** this is the voltage calibration factor. it initial value is 1, but if the sensor data need to be multiply, change the factor value. */
	double v_offset;	/** this is the voltage offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */
	double v_phase;		/** this is the phase delay of the voltage offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */

	/******************************************************************************
	 * current data
	 */
	double i_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the where current data is stored and used to calculate the average. */
	double i_average;	/**< there is the result from the m_data average */
	double i_factor;	/** this is the current calibartion factor. it initial value is 1, but if the sensor data need to be multiply, change the factor value. */
	double i_offset;	/** this is the current offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */


	/******************************************************************************
	 * Calculation variables
	 */

	double Vmeas;	/** Variable that holds measurement */
	double Imeas;	/** Variable that holds current measurement */
	double Vshifted;	/** Variable that holds the voltage value after phase compensation*/
	long double sumP, sumV, sumI;
	double referenceV;
	double v_filtered, i_filtered;
	long showSupply;


	/******************************************************************************
	 * Other variables
	 */

	int startV; 						//Instantaneous voltage at start of sample window.

    bool lastVCross, checkVCross;       /**< Used to measure number of times threshold is crossed. */

	unsigned char m_id;					/**< this the sensor's id in the Eeprom::sensor_counter */

	unsigned char m_eeprom_addr;		/**< This is the eeprom address calc from the m_id and the EEPROM_OFFSET */


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

#endif /* POWER_H_ */




