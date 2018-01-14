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
 *    @author LONGUET Gilles
 *    @date   27 Dec. 2017
 *    @brief  Calculates the AC or DC power with one voltage sensors and 1 to 3 currents sensors
 *
 *    This file is based on the emonlib from OpenEnergyMonitor. Some changes
 *    have been made to adapt the calculation of the AC power to the hardware
 *    setting of the Windlogger.
 *
 *    On this hardware you can have 2 voltage sources and 4 currents sources for power measurements
 *    So we chose to split in 2 configurations
 *    V1 and C1,C2 and C3 possible
 *    and a second V2, C2, C3 and C4 possible
 *
 *    The Cx attribution will be decided in the config state
 *
 */


#ifndef POWER_H_
#define POWER_H_

#include <avr/io.h>
#include "../lib/main.h"


/**
 * \brief The Power class is the base container for measuring and calculating power.
 *
 * The Power class gives base layer for any type of power calculation, either AC or DC.</br>
 * On this hardware you can have 2 voltage sources and 4 currents sources for power measurements
 * So we chose to split in 2 configurations
 * V1, C1,C2 and C3 possible
 * and a second V2, C2, C3 and C4 possible
 *
 * The power measurement process is devided in three parts for better performance
 * mono phased AC power
 * bi or tri phased AC power
 * DC power
 *
 * The Cx attribution is decided in the config state
 * DC or AC is decided in the config state
 * mono or multi process by Current sensor attribution
 */


class Power {

public:
	/******************************************************************************
	 * Constructor and destructor
	 */
	/**
	 * Class constructor
	 * id id number permit to clarify power1(Vac) and power2(Vdc)
	 */
	Power(unsigned char id, bool is_AC);

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
	/// @todo see if needs to move offset to auto offset

	inline void set_v_factor(double factor) {m_v_factor=factor; update_param();}
	inline void set_v_offset(double offset) {m_v_offset=offset; update_param();}
	inline void set_v_phase(double phase) {m_v_phase=phase; update_param();}

	/******************************************************************************
	 * current configuration setters
	 */
	inline void set_ct_enable(unsigned char ct, bool enable){if(enable) m_ct_enable |= _BV(ct); else m_ct_enable &= ~_BV(ct);update_param();} // enable or disable the ctx
	inline unsigned char get_ct_enable() const{return m_ct_enable;}

	inline void set_ct1_factor(double factor) {m_ct1_factor=factor; update_param();}
	inline void set_ct2_factor(double factor) {m_ct2_factor=factor; update_param();}
	inline void set_ct3_factor(double factor) {m_ct3_factor=factor; update_param();}
	inline void set_ct4_factor(double factor) {m_ct4_factor=factor; update_param();}


	inline void set_ct1_offset(double offset) {m_ct1_offset=offset; update_param();}
	inline void set_ct2_offset(double offset) {m_ct2_offset=offset; update_param();}
	inline void set_ct3_offset(double offset) {m_ct3_offset=offset; update_param();}
	inline void set_ct4_offset(double offset) {m_ct4_offset=offset; update_param();}


	/******************************************************************************
	 * Sensor methods
	 */
	void read_values(unsigned char measure_number, unsigned char crossings, unsigned int timeout);

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
	void print_config(char arg_id1[], char arg_id2[],char arg_id3[], char arg_id4[],char arg_id5[], char arg_id6[]);

	char* print_average(unsigned char prec, char *string);

	void print_data_array();

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

	//long readVcc();



private:
	/******************************************************************************
	 * private constant
	 */
	/******************************************************************************
	 * */
	const int ADC_COUNTS = 1024;
	const unsigned char VOLTAGE_AC_PIN = 0;
	const unsigned char VOLTAGE_DC_PIN = 7;
	const unsigned char CT1_PIN = 1;
	const unsigned char CT2_PIN = 2;
	const unsigned char CT3_PIN = 3;
	const unsigned char CT4_PIN = 4;

	/******************************************************************************
	 * private members
	 */
	double m_vcc;
	/******************************************************************************
	 * class management
	 */
	unsigned char m_id;					/**< this the sensor's id in the Eeprom::sensor_counter */
	unsigned char m_eeprom_addr;		/**< This is the eeprom address calc from the m_id and the EEPROM_OFFSET */
	bool m_isAC;							/**< This boolean member permit to chose if the power is AC:1 or DC:0 */
	unsigned char m_ct_enable;			/**< this member organise cts repartition between power measurements */
	/******************************************************************************
	 * mono phased power data
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
	double m_v_factor;	/** this is the voltage calibration factor. it initial value is 1, but if the sensor data need to be multiply, change the factor value. */
	double m_v_offset;	/** this is the voltage offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */
	double m_v_phase;		/** this is the phase delay of the voltage offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */

	/******************************************************************************
	 * current1 data
	 */
	double ct1_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the where current data is stored and used to calculate the average. */
	double ct1_average;	/**< there is the result from the m_data average */
	double m_ct1_factor;	/** this is the current calibartion factor. it initial value is 1, but if the sensor data need to be multiply, change the factor value. */
	double m_ct1_offset;	/** this is the current offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */

	/******************************************************************************
	 * current2 data
	 */
	double ct2_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the where current data is stored and used to calculate the average. */
	double ct2_average;	/**< there is the result from the m_data average */
	double m_ct2_factor;	/** this is the current calibartion factor. it initial value is 1, but if the sensor data need to be multiply, change the factor value. */
	double m_ct2_offset;	/** this is the current offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */

	/******************************************************************************
	 * current3 data
	 */
	double ct3_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the where current data is stored and used to calculate the average. */
	double ct3_average;	/**< there is the result from the m_data average */
	double m_ct3_factor;	/** this is the current calibartion factor. it initial value is 1, but if the sensor data need to be multiply, change the factor value. */
	double m_ct3_offset;	/** this is the current offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */

	/******************************************************************************
	 * current4 data
	 */
	double ct4_data[MAX_DATA_SAMPLE]={0.0};	/**< this is the where current data is stored and used to calculate the average. */
	double ct4_average;	/**< there is the result from the m_data average */
	double m_ct4_factor;	/** this is the current calibartion factor. it initial value is 1, but if the sensor data need to be multiply, change the factor value. */
	double m_ct4_offset;	/** this is the current offset. It initial value is zero. Change it if you want add an offset to the sensor's data. */


	/******************************************************************************
	 * Calculation variables
	 */

	double Vmeas;	/** Variable that holds measurement */
	double ct1meas;	/** Variable that holds current measurement */
	double ct2meas;	/** Variable that holds current measurement */
	double ct3meas;	/** Variable that holds current measurement */
	double ct4meas;	/** Variable that holds current measurement */
	double Vshifted;	/** Variable that holds the voltage value after phase compensation*/
	long double sumP, sumV, sumI1, sumI2, sumI3,sumI4;
	double referenceV;
	double v_filtered, i1_filtered, i2_filtered, i3_filtered;


	/******************************************************************************
	 * Other variables
	 */

	int startV; 						//Instantaneous voltage at start of sample window.

    bool lastVCross, checkVCross;       /**< Used to measure number of times threshold is crossed. */



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




