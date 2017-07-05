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
 *    @file   RTC.h
 *    @author gilou
 *    @date   1 juil. 2017
 *    @brief  The RTC class regroup methods and parameters to use the Real Time Clock : pcf8563
 *
 *    Detailed description of file.
 */
#ifndef RTC_H_
#define RTC_H_


/**
 * \brief The time structure is a simple date time format
 */
struct Time
{
	unsigned char secs;		//!< contain seconds number from last min
	unsigned char mins;		//!< Contain minutes number from last hour
	unsigned char hours;	//!< Contain hours number from last day
	char weekday[3];		//!< Contain the weekday short name
	unsigned char day;		//!< Contain day number from the last month
	unsigned char month;	//!< Contain month number from the last year
	unsigned char year;		//!< Contain year's counter
};

/**
 * \brief The RTC class regroup methods and parameters to use the Real Time Clock : pcf8563
 * This class use the TWI class to drive the atmega1284p's TWI peripheral
 */
class RTC {

public:
	/******************************************************************************
	 * Constructor and destructor
	 */
	/**
	 * Class constructor
	 */
	RTC();
	/**
	 * class destructor
	 */
	virtual ~RTC();

	/******************************************************************************
	 * public constants
	 */
	const unsigned char SECS = 0x02;	/**< pcf8563 seconds register adress */
	const unsigned char  MINS = 0x03;	/**< pcf8563 minutes register adress */
	const unsigned char  HOURS = 0x04;	/**< pcf8563 hours register adress */
	const unsigned char  DAY = 0x05;	/**< pcf8563 day register adress */
	const unsigned char  WEEKDAY = 0x06;	/**< pcf8563 weekday register adress */
	const unsigned char  MONTH = 0x07;	/**< pcf8563 month register adress */
	const unsigned char  YEAR = 0x08;	/**< pcf8563 year register adress */

	/******************************************************************************
	 * getters
	 */
	unsigned char get_second() const{return m_time.secs;}

	unsigned long get_timestamp();

	/******************************************************************************
	 * RTC's methods
	 */
	/**
	 * Function to get time and date registers from the pcf8563 and store them in the local m_time structure.
	 * @return return 0 if it's ok or if have TWI error.
	 * 	 */
	unsigned char get_date();

	/**
	 * \brief Function to update a register in the pcf8563
	 * This function is use in the st_config class
	 * @param reg	register adress
	 * @param data	byte data
	 */
	void update_reg(unsigned char reg, unsigned char data);


	/**
	 * This function print on the uartO the RTC's config information
	 */
	void print_config();



private:
	/******************************************************************************
	 * private members
	 */
	char JOURS[8][3]={"Su","Mo","Tu","We","Th","Fr","Sa","er"};		/**< this array permit to convert weekday number in a small string */

	Time epoch_010116;												/**< Used for time_to_epoch calc */

	Time m_time;													/**< m_time is the structure where is store current time and date */

	const unsigned long EPOCH_TIMESTAMP = 1420156800;

	/******************************************************************************
	 * private methods
	 */
	/**
	 * The time to epoch method convert m_time registers in the timestamp format.
	 * @param start	That is actually a structure egal to 01/01/16 at 00:00:00
	 * @param stop	that is actually the m_time member
	 */
	unsigned long time_to_epoch(Time &start, Time &stop);

	/**
	 * is_leap_year function return 0 if year is not a leap year and 1 when it is.
	 * @param year year want to by tested
	 * @return return 0 if year is not a leap year and 1 when it is.
	 */
	unsigned char is_leap_year(unsigned int year);

	/**
	 * This function convert two char number in BCD format because pcf8563 register are in BCD format
	 * @param s_nombre	that is a reference to a string char
	 * @return
	 */
	char a2toBCD (char &s_nombre);

	/**
	 * This function convert an unsigned char number in unsigned char BCD format because pcf8563 register are in BCD format
	 * @param _data	that is a the number need to be converted
	 */
	unsigned char utoBCD(unsigned char _data);
};

#endif /* RTC_H_ */
