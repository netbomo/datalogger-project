/**
 *******************************************************************************
 *******************************************************************************
 *
 *	Licence :
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
 *    @Author gilou
 *    @date   20 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include "../lib/DateTime.h"

/// Epoch year constante
        /// The epoch time is the timestamp strating time. The value is the 01/01/1970 at 00:00:00
        const unsigned int EPOCH_YEAR = 1970;
        /// Epoch month constante
        const unsigned char EPOCH_MONTH = 1;
        /// Epoch date constante
        const unsigned char EPOCH_DATE = 1;

        /**
         * \brief Constructor
         */
        DateTime::DateTime(unsigned char s,
					unsigned char mi,
					unsigned char h,
					unsigned char d,
					unsigned char mo,
					unsigned int y):m_second(s),m_minute(mi),m_hour(h),m_date(d),m_month(mo),m_year(y){}

        /**
         * \brief convert from timstamp (unsigned long int) to date time format
         * \param timestamp (???)
         * \return void
         */
        void toDateTime (unsigned long int timestamp){

        }

        /**
         * \brief test if the year is leap or not
         * \param year (???)
         * \return bool
         */
        bool isLeap (unsigned int year){

        	return 0;
        }

        unsigned long int getDateTime (){

        	return 0;
        }
