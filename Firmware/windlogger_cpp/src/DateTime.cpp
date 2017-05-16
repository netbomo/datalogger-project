/**
 *******************************************************************************
 *******************************************************************************
 *
 *	License :
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
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
 *    @file   DateTime.cpp
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief  @todo
 *
 *   @todo
 */

#include "../lib/DateTime.h"

// Epoch year constante
// The epoch time is the timestamp strating time. The value is the 01/01/1970 at 00:00:00
const unsigned int EPOCH_YEAR = 1970;
// Epoch month constante
const unsigned char EPOCH_MONTH = 1;
// Epoch date constante
const unsigned char EPOCH_DATE = 1;

// Class constructor
DateTime::DateTime(unsigned char s,
		unsigned char mi,
		unsigned char h,
		unsigned char d,
		unsigned char mo,
		unsigned int y):m_second(s),m_minute(mi),m_hour(h),m_date(d),m_month(mo),m_year(y){}

// convert from timestamp (unsigned long int) to date time format
void toDateTime (unsigned long int timestamp){

}

// convert the date time in unix time
unsigned long int getDateTime (){

	return 0;
}

// test if the year is leap or not
bool isLeap (unsigned int year){

	return 0;
}
