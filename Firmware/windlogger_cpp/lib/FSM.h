#ifndef FSM__HPP
#define FSM__HPP

#include "../lib/Usart.h"
#include "../lib/Config.h"
#include "../lib/Measure.h"
#include "../lib/Output.h"
#include "../lib/Sleep.h"
#include "../lib/State.h"

struct Eeprom{
		unsigned int structure_version;		/// this permit to improve the structure by auto reset eeprom data when the structure evolve to prevent data bad reading
		unsigned char measure_sample_conf;	/// measurement sampling (0: no measure, 1 : 10 secs, 2: 1 min, 3: 10 min...)
		unsigned char measure_max;			/// measure_max is the number of measure by measure_sample_conf (ex by minute or by 10 minutes...)
		unsigned char measure_periode;		///	measure_periode is the interval between two measures in seconds
		unsigned char measure_counter;		/// this counter is use organize measurement
		unsigned char sensors_enable;		/// this register permit to enable or disable sensor
		unsigned char output_enable;		/// this register permit to enable or disable output
	};

class FSM {
public :
	// Operations
	/**
	 * \brief Class constructor
	 */
	FSM();

	/**
	 * \brief This method read the last char receive by the UART0 and check what is need to do
	 * @param character
	 */
	void character_processing ();

	/**
	 * \brief This method define the next state by deduction
	 */
	void newState_definition ();

	/**
	 * \brief this method control if a new measure is needed
	 * \return void
	 */
	void measurement_timing_control ();

	/**
	 * \brief This method execute the next state
	 */
	void execute();

	/// static members declaration
	static bool flag_new_measure;			/*!< set when time come for a new measure>*/
	static bool flag_data_averages_ready;	/*!< set when each averages are calculated and ready for the OUTPUT state>*/
	static bool flag_config_request;		/*!< set when a string from usart0 finish by "\r\n">*/

	static unsigned long int timestamp;		/*!< timestamp is an uint32 to stock the current unix time stamp>*/

	static Usart uart0;					/*!< This is the uart0 declaration>*/

	static Eeprom eeprom;				/*!< This is the structure of data stored in the eeprom>*/

private :
	/// State list declaration
	Idle idle;				///	this state is used just for the first time in the while loop
	Config config;			///	this state is used to download or upload configuration
	Measure measure;		///	this state is used for measurement process
	Output output;			///	this state is used to save or send data
	Sleep sleep;			///	When nothing is need to do, the micro-controller go to sleep

	State *nextState;

	unsigned char second_counter;	/// The second_counter is used to compare to the measure_stamp interval
};

#endif
