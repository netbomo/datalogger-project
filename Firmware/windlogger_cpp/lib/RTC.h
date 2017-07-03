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
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef RTC_H_
#define RTC_H_

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

class RTC {

public:
	RTC();
	virtual ~RTC();

	const unsigned char SECS = 0x02;	/**< pcf8563 seconds register adress */
	const unsigned char  MINS = 0x03;	/**< pcf8563 minutes register adress */
	const unsigned char  HOURS = 0x04;	/**< pcf8563 hours register adress */
	const unsigned char  DAY = 0x05;	/**< pcf8563 day register adress */
	const unsigned char  WEEKDAY = 0x06;	/**< pcf8563 weekday register adress */
	const unsigned char  MONTH = 0x07;	/**< pcf8563 month register adress */
	const unsigned char  YEAR = 0x08;	/**< pcf8563 year register adress */

	/**
	 * Fonction pour récupérer les informations date et heure du RTC dans le PCF8563
	 * @param _time pointeur d'une structure Time, dans lequel sera mis en forme la date et l'heure
	 * @return renvoi la valeur 0 si récupération réussie, et 1 si defaut de TWI
	 *
	 * Utiliser une structure de type Time :
	 * typedef struct Time
	 *{
	 *	unsigned int secondes;
	 *	unsigned int minutes;
	 *	unsigned int heures;
	 *	char jour[5];
	 *	unsigned int mois;
	 *	unsigned int annee;
	 *	unsigned int jsemaine;
	 *}Time;
	 *
	 */
	unsigned char get_date();

	/**
	 * Fonction d'update du RTC qui permet la mise à jour du PCF8563 à partir d'une chaine de carac composé de la date et de l'heure
	 * @param _datetime pointeur vers string = %u%e%m%y%H%M%S (format linux fonction : date +%u%e%m%y%H%M%S)
	 * @return	error...
	 */
	unsigned char update (char *_datetime);

	/**
	 * Function to update a register in the pcf8563
	 * @param reg	register adress
	 * @param data	byte data
	 */
	void update_reg(unsigned char reg, unsigned char data);

	void print();



private:

	// déclaration du tableau où l'on récupère les registres du pcf8563
	char JOURS[8][3]={"Su","Mo","Tu","We","Th","Fr","Sa","er"};

	unsigned long time_to_epoch(Time &start, Time &stop);

	unsigned char is_leap_year(unsigned int year);

	char a2toBCD (char *s_nombre);

	unsigned char utoBCD(unsigned char _data);

	Time epoch_010116;	/**< Used to do simple the timestamp calc */

	Time m_time;	/**< Used to do simple the timestamp calc */

	const unsigned long EPOCH_TIMESTAMP = 1420070400;

public :
	unsigned char get_second() const{return m_time.secs;}

	unsigned long get_timestamp();
};

#endif /* RTC_H_ */
