#ifndef FSM__HPP
#define FSM__HPP

#include "Measure.hpp"
#include "Output.hpp"
#include "Sleep.hpp"
#include "Config.hpp"
#include "State.hpp"

class FSM {
public :
	// Operations
	/**
	 * \brief Class constructor
	 */
	FSM();

	void execute();


	// Attributes
	/// flag set when a new char is receive by the microcontroller
	static bool flag_UART0_rx_char;
	/// set when the timerX overflow (stop frequencies measurement)
	/// todo select timer
	static bool flag_freq_data_ready;
	/// set when time come for a new measure
	static bool flag_new_measure;
	/// set when each averages are calculated and ready for the OUTPUT state
	static bool flag_data_averages_ready;

	// Attributes
	static unsigned long int timestamp;

private :
	Idle idle;
	Config config;
	Measure measure;
	Output output;
	Sleep sleep;
	State &nextState;
};

#endif
