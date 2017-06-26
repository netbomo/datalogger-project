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
 *    @file   Config.h
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief  This file declare the Config class.
 *
 *    The class Config inherits from the class State.
 *    This class manage input between the FSM, the uarts (serial interface) and eeprom.
 *    With this class, the windlogger can display eeprom saved data and update there from the uart0.
 *    It can control time update with the DateTime class to have correct convertion from a human
 *    date and time display and the unix time stamp use by the logger to timestamp data.
 */

#ifndef CONFIG__HPP
#define CONFIG__HPP

#include "../lib/main.h"
#include "../lib/DateTime.h"
#include "../lib/State.h"

/**
 *    \brief The class Config inherits from the class State.
 *    This class manage input between the FSM, the uarts (serial interface) and eeprom.
 *    With this class, the windlogger can display eeprom saved data and update there from the uart0.
 *    It can control time update with the DateTime class to have correct convertion from a human
 *    date and time display and the unix time stamp use by the logger to timestamp data.
 */
class Config : public State {

	DateTime currentTime;	/**< current time is the human readable side of the timestamp. Use for configuration.*/
	char request[20];		/**< is the char array use to save character from USART0*/
	unsigned char request_i;			/**< this is the request string's index*/

public :
	/**
	 * \brief Constructor
	 */
	Config ();
	/**
	 * \brief Destructor
	 */
	virtual ~Config();

/******************************************************************************
 * inherits from State
 */
	/**
	 *  \brief The execute method is the config "main function"
	 *	In this method, sentence from the uart are process either
	 *	to display the configuration menu or to update configuration register,
	 *	if it's need, the eeprom are updated.
	 */
	virtual void execute ();

	/**
	 * It's just a simple method to display the state name, use to debug
	 */
	virtual void print();

	bool isEqual(char *name)const;

/******************************************************************************
 * String tricks
 */
	/**
	 * This method decrements the request_i index when the backspace key is hit.
	 */
	inline void backspace(){ request_i -= 2;	/*move on left*/}

	/**
	 * Put end string character '\0' to request
	 */
	inline void endString(){ request[request_i-1] = '\0';}

/******************************************************************************
 * Some methods
 */
	/**
	 * \brief display all config on USART0
	 * \return void
	 */
	void display();

	/**
	 * \brief add c to the end of request
	 * \param c is a char character
	 */
	void addToRequest (char c);

	/**
	 * This method convert a 2 character array in a char value
	 * @param string pointer to the dozen character
	 * @return return the char value
	 */
	unsigned char atoc(char string[]);

/******************************************************************************
 * Logger management
 */
	/**
	 * \brief Load saved config data from the eeprom
	 * \return void
	 */
	void load_logger();

	/**
	 * \brief Update saved config data in the eeprom
	 * \return void
	 */
	void update_logger ();

	/**
	 * \brief Initialize the eeprom memory
	 * \return void
	 */
	void initialize_logger ();

};

#endif
