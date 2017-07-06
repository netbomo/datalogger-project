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
 *    @file   FSM.h
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief  The FSM is the finish state machine mechanism.
 *
 *    The class FSM works with a central state (the main program) select the next state(children state) from hardware or software flags and execute it.
 *
 *		This is an illustration :
 *    @image html FSM.svg
 */

#ifndef FSM__HPP
#define FSM__HPP

#include "../lib/Usart.h"
#include "../lib/TWI.h"
#include "../lib/RTC.h"
#include "../lib/Windvane.h"
#include "../lib/Anemometer.h"
#include "../lib/powerDC.h"

#include "../lib/State.h"
#include "../lib/st_Config.h"
#include "../lib/st_Measure.h"
#include "../lib/st_Output.h"
#include "../lib/st_Sleep.h"


/**
 * \brief The Logger structure regroup members need to be saved in the micro-controler's eeprom
 */
struct Logger{
	unsigned int structure_version;		/**< This permit to improve the structure by auto reset eeprom data when the structure evolve to prevent data bad reading */
	unsigned char node_id;				/**< The node id, permit identify each datalogger (0 - 255)*/
	unsigned char measure_sample_conf;	/**< Measurement sampling (0: no measure, 1 : 10 secs, 2: 1 min, 3: 10 min...) */
	unsigned char measure_max;			/**< Measure_max is the number of measure by measure_sample_conf (ex by minute or by 10 minutes...). !Be careful! is a new configuration is create, the Sensor::MAX_DATA_SAMPLE needs to be adjust to the highest value of measure_max! */
	unsigned char measure_periode;		/**< Measure_periode is the interval between two measures in seconds */
	unsigned char measure_counter;		/**< This counter is used organize measurement */
	unsigned char sensors_enable;		/**< This register permit to enable or disable sensor */
	unsigned char output_enable;		/**< This register permit to enable or disable output */
};

/**
 * \brief The class FSM regroup hardware peripherals declaration and mechanic methods
 * @image html FSM.svg
 */
class FSM {

public :
	/**
	 * \brief Class constructor
	 */
	FSM();

	virtual ~FSM();

	/******************************************************************************
	 * State machine mechanic methods
	 */
	/**
	 * \brief This method define the next state from hardware and software flags
	 */
	void new_State_definition ();

	/**
	 * \brief This method control if a new measure is needed
	 */
	void measurement_timing_control ();

	/**
	 * \brief This method execute the next state
	 */
	void execute();

	/******************************************************************************
	 * Methods call from hardware flags
	 */
	/**
	 * \brief Call when a string from usart0 finish by "\r\n"
	 */
	void character_processing ();

	/******************************************************************************
	 * static software flags declaration
	 */
	static bool flag_new_measure;			/**< set when time come for a new measure */
	static bool flag_data_averages_ready;	/**< set when each averages are calculated and ready for the OUTPUT state */
	static bool flag_config_request;		/**< set when a string from usart0 finish by "\r\n" */
	static bool flag_data_frequencies_ready;/**< Set when the timer3 overflow, data are ready*/

	/******************************************************************************
	 * static global variable
	 */
	static unsigned long int timestamp;					/**< timestamp is an uint32 to stock the current unix time, needs to be static to be use by hardware Interrupt Sub Routine (ISR)*/

	/******************************************************************************
	 * static class, that refer to hardware peripherals
	 */
	static Usart uart0;				/**< This is the uart0 declaration */
	static TWI twi;					/**< This is the twi declaration */
	static RTC rtc;					/**< This is the rtc declaration */

	static Logger logger;			/**< This is the structure of data stored in the eeprom */

	/******************************************************************************
	 * static sensors list
	 */
	static Anemometer anemo1;		/**< this is the anemometer 1 declaration */
	static Anemometer anemo2;		/**< this is the anemometer 2 declaration */
	static Windvane windvane;		/**< this is the windvane declaration */
	static powerDC pDC;			/**< this is the powerAC declaration */


private :
	/******************************************************************************
	 * private member
	 */
	unsigned char second_counter;	/**< The second_counter is used to compare to the measure_stamp interval */

	/******************************************************************************
	 * State list declaration
	 */
	Idle idle;				/**< this state is used just for the first time in the while loop */
	Config config;			/**< this state is used to download or upload configuration */
	Measure measure;		/**< this state is used for measurement process */
	Output output;			/**< this state is used to save or send data */
	Sleep sleep;			/**< When nothing is need to do, the micro-controller go to sleep */

	/******************************************************************************
	 * State pointer
	 */
	State *nextState;		/**< This pointer is use to manipulate the next state */

};

#endif
