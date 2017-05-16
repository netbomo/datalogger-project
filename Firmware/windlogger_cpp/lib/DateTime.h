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
 *    @file   DateTime.h
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief  This file declare the DateTime class.
 *
 *    @todo brief
 */

#ifndef DATETIME__HPP
#define DATETIME__HPP

/**
 * \brief The DateTime class permit to have human readable format for the timestamp.
 *        
 */
class DateTime{
	// Operations
public :

	/**
	 * \brief Class constructor
	 * @param s	second value
	 * @param mi minute value
	 * @param h hour value
	 * @param d day number value
	 * @param mo month value
	 * @param y year value as AA format
	 */
	DateTime(unsigned char s=0, unsigned char mi=0, unsigned char h=0, unsigned char d=0, unsigned char mo=0, unsigned int y=0);

	/**
	 * \brief convert from timestamp (unsigned long int) to date time format
	 * \param timestamp unsigned long int value of the date
	 */
	void toDateTime (unsigned long int timestamp);

	/**
	 *	\brief convert the date time in unix time
	 * @return return an unsigned long int value
	 */
	unsigned long int getDateTime ();

private :
	/**
	 * \brief test if the year is leap or not
	 * \param year unsigend char year value in AA format (2 digits)
	 * \return bool
	 */
	bool isLeap (unsigned int year);

	// Attributes
private :
	unsigned char m_second;	/**< this is the second register in unsigned char format*/
	unsigned char m_minute;	/**< this is the minute register in unsigned char format*/
	unsigned char m_hour;	/**< this is the hour register in unsigned char format*/
	unsigned char m_date;	/**< this is the date register in unsigned char format*/
	unsigned char m_month;	/**< this is the month register in unsigned char format*/
	unsigned int m_year;	/**< this is the year register in unsigned char format*/

};

#endif
