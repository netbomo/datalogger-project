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
 *    @file   RTC.cpp
 *    @author gilou
 *    @date   1 juil. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <avr/io.h>
#include <stdlib.h>

#include "../lib/FSM.h"
#include "../lib/RTC.h"

RTC::RTC() {
	// constant timestamp au 01/01/2016 à 00:00:00 :1451599200
		epoch_010116.secs=0;
		epoch_010116.mins=0;
		epoch_010116.hours=0;
		epoch_010116.day=1;
		epoch_010116.month=1;
		epoch_010116.year=15;
		//epoch_010116.timestamp=1420070400;
}

RTC::~RTC() {}

//void debug_RTC_print_Time(Time *time)
//{
//	debug_print("","",1);
//	debug_print("_______________","_______________",1);
//	debug_print("|weekday","",0);
//	debug_print("=",time->weekday,1);
//	debug_printu("|seconds","=",time->secs,1);
//	debug_printu("|minutes","=",time->mins,1);
//	debug_printu("|hours	","=",time->hours,1);
//	debug_printu("|day	","=",time->day,1);
//	debug_printu("|month	","=",time->month,1);
//	debug_printu("|year	","=",time->year,1);
//	debug_print("_______________","_______________",1);
//	debug_print("","",1);
//}
//
//void debug_RTC_timestamp_calc(Time *start,Time *stop,unsigned char leapY, double result_timesamp)
//{
//	//send a report on timestamp calculation
//	debug_print("","",1);
//	debug_print("_______________","_______________",0);
//	debug_print("_______________","_______________",0);
//	debug_print("_______________","_______________",1);
//	debug_printu("|start year","=",start->year,0);
//	debug_printu("	stop year","=",stop->year,0);
//	debug_printu("	add to days","=",stop->year-start->year,1);
//
//	debug_printu("|start month","=",start->month,0);
//	debug_printu("	stop month","=",stop->month,0);
//	debug_printu("	add to days","=",stop->month-start->month,1);
//
//	debug_printu("|start day","=",start->day,0);
//	debug_printu("	stop day","=",stop->day,0);
//	debug_printu("	add to days","=",stop->day-start->day,1);
//
//	debug_print("_______________","_______________",0);
//	debug_print("_______________","_______________",0);
//	debug_print("_______________","_______________",1);
//
//	debug_printl("|timestamp result",":",result_timesamp,1);
//	//debug_printl("|epoch timestamp",":",epoch_010116.timestamp,1);
//	///debug_printl("|writed timestamp",":",stop->timestamp,1);
//	debug_print("_______________","_______________",0);
//	debug_print("_______________","_______________",0);
//	debug_print("_______________","_______________",1);
//	debug_printu("leap  Years","=",leapY,1);
//	debug_print("_______________","_______________",0);
//	debug_print("_______________","_______________",0);
//	debug_print("_______________","_______________",1);
//	debug_print("","",1);
//}

/*
 * A year will be a leap year if it is divisible by 4 but not by 100.
 * If a year is divisible by 4 and by 100, it is not a leap year unless it is also divisible by 400.
 */
/**
 * function to return if the ear is leap
 * @param year the year to test
 * @return 0 for false, 1 for true
 */
unsigned char RTC::is_leap_year(unsigned int year)
{
	if(year%4==0)
	{
		if(year%100==0){
			if(year%400==0)return 1;
			else return 0;
		}
		else return 1;
	}
	else return 0;
}

unsigned long RTC::time_to_epoch(Time &start, Time &stop)
{
	unsigned int months_days[] = {31,59,90,120,151,181,212,243,273,304,334};
	unsigned long convtemp;

	// years work
	unsigned int i =0;
	unsigned char leapY =0;
	unsigned int days = (stop.year-start.year)*365; // convert years in days

	if((stop.year-start.year)>1)
	{
		for(i = start.year; i<stop.year;i++)
			{
				if(is_leap_year(start.year+i)) leapY++; // add a day for each leap years
			}
	}

	if(stop.month>2&&is_leap_year(stop.year))
	{
		leapY++;	// stop time month > February, check leap year
		days+=leapY;
	}

	// months work
	if(stop.month>1) days = days + months_days[(stop.month-2)];

	// days work
	days += (stop.day-start.day);

	//convert in seconds
	uint32_t now_timestamp = days*86400; //*constants seconds/day

	//add time

	convtemp= stop.hours*60;
	convtemp = (convtemp+stop.mins)*60;
	convtemp += stop.secs;
	//stop->hours*3600+stop->mins*60+stop->secs;
	now_timestamp += convtemp;
#ifdef DEBUG_RTC
	debug_RTC_timestamp_calc(start,stop,leapY,now_timestamp);
#endif
	return now_timestamp;
}

uint8_t RTC::get_date()
{
	uint8_t pcf8563[7];	//Tableau pour récupérer les 8 registres du RTC (ds1307)
	uint8_t i;
	uint8_t temp_unit, temp_diz_unit;
	uint8_t error = 0;

	for(i = 0; i<7;i++)
	{
		pcf8563[i] = 0;								//initialisation des registres à 0x00 pour pouvoir vérifier le changement
	}													//de leur valeur après réception des données
	error = FSM::twi.receive(0xA2,0x02,7,pcf8563);					//récupération des données sur les 8 registres sur Ds1307

	if(pcf8563[3] == 0x00)								//on teste le registre numero de jour qui ne peut être égal à 0 (1->31)
	{
		return error;										//renvoi d'erreur critique
	}
	else
	{	// mise en format des secondes
		temp_unit = pcf8563[0] & 0x0F;						//convertion BCD to Int (cf datasheet Ds1307)
		temp_diz_unit = (pcf8563[0] & 0x70)>>4;
		m_time.secs = temp_diz_unit*10 + temp_unit;
		//debug_printu("time","secs",time->secs,1);

		// mise en format des minutes
		temp_unit = pcf8563[1] & 0x0F;
		temp_diz_unit = (pcf8563[1] & 0x70)>>4;
		m_time.mins = temp_diz_unit*10 + temp_unit;
		//debug_printu("time","mins",time->mins,1);


		// mise en format de l'heure
		temp_unit = pcf8563[2] & 0x0F;
		temp_diz_unit = (pcf8563[2] & 0x30)>>4;
		m_time.hours = temp_diz_unit*10 + temp_unit;
		//debug_printu("time","hours",time->hours,1);

		// On choisi le jour dans le tableau JOURS
		m_time.weekday[0] = JOURS[(pcf8563[4]&0b00000111)][0];
		m_time.weekday[1] = JOURS[(pcf8563[4]&0b00000111)][1];
		m_time.weekday[2] = '\0';

		// mise en format du numéro du jour
		temp_unit = pcf8563[3] & 0x0F;
		temp_diz_unit = (pcf8563[3] & 0x30)>>4;
		m_time.day = temp_diz_unit*10 + temp_unit;

		// mise en format du numéro du mois
		temp_unit = pcf8563[5] & 0x0F;
		temp_diz_unit = (pcf8563[5] & 0x10)>>4;
		m_time.month = temp_diz_unit*10 + temp_unit;

		// mise en format du numéro du mois
		temp_unit = pcf8563[6] & 0x0F;
		temp_diz_unit = (pcf8563[6] & 0xF0)>>4;
		m_time.year = (temp_diz_unit*10 + temp_unit);

		//time->timestamp = ptrepoch_010116->timestamp;
		//time->timestamp = rtc_time_to_epoch(ptrepoch_010116, time)+ ptrepoch_010116->timestamp;

#ifdef DEBUG_RTC
		debug_RTC_print_Time(time);
#endif
		return error;
	}

	//renvoi 0 si fonction executée sans erreur
}

char RTC::a2toBCD (char *s_nombre) // le pointeur s_nombre pointe la dizaine du nombre à convertir, s_nombre+1 pointe l'unité

{
	uint8_t temp_unit;  	//variable temporaire recevant la valeur entière du caractère "unité" du pointeur string
	uint8_t temp_diz_unit; 	//variable temporaire recevant la valeur entière du caractère "dizaine" du pointeur string
	uint8_t conversion=0;	//variable recevant la conversion du nombre "string" en BCD

	temp_unit = (uint8_t) (*(s_nombre+1) - '0');		// le pointeur s_nombre+1 pointe l'unité du nombre à convertir
	temp_diz_unit = (uint8_t) (*s_nombre - '0');		// le pointeur s_nombre pointe la dizaine du nombre à convertir


	conversion = (temp_diz_unit << 4)|temp_unit;		/// décalage de 4 bits des dizaine pour coder le nombre en BCD sur 2 quartets (1 octet)
	return conversion;			// renvoi le nombre converti
}

uint8_t RTC::utoBCD(uint8_t _data)
{
	uint8_t number = _data;
	return (number/10<<4) | (number%10);
}



uint8_t RTC::update(char *s_datetime)
{
	uint8_t pcf8563[7]; 							//tableau contenant les 7 registres de la RTC (h/min/sec/j/DAY/mois/année)

	pcf8563[0] = (uint8_t)a2toBCD(s_datetime+11); 					//stock la valeur converti des index 11 et 12 correspondant aux sec et diz de sec
	pcf8563[1] = (uint8_t)a2toBCD(s_datetime+9); 					//stock la valeur converti des index 9 et 11 correspondant aux min et diz de min
	pcf8563[2] = (uint8_t)a2toBCD(s_datetime+7);  					//stock la valeur converti des index 7 et 8 correspondant aux heure et diz de heure
	pcf8563[4] = (uint8_t)(s_datetime[0] -'0'); 					//stock la valeur converti de l'index 0 correspont au jour (Dim., lun. etc...)
	pcf8563[3] = (uint8_t)a2toBCD(s_datetime+1);  					//stock la valeur converti de l'index 1 et 2 correspont au numéro du jour
	pcf8563[5] = (uint8_t)a2toBCD(s_datetime+3);  					//stock la valeur converti de l'index 3 et 4 correspont au numéro du mois
	pcf8563[6] = (uint8_t)a2toBCD(s_datetime+5);  					//stock la valeur converti de l'index 5 et 6 correspont au numéro de l'année

	return FSM::twi.print(0xA2, 0x02, 7,pcf8563);					//réinitialise le RTC avec le contenu du tableau Ds1307
}

void RTC::update_reg(uint8_t reg, uint8_t data)
{

	FSM::twi.set_char(0xA2, reg, utoBCD(data));					//réinitialise le RTC avec le contenu du tableau Ds1307
}

void RTC::print(){
	char conv_string[10];

	FSM::uart0.print("Time :\r\n");
	FSM::uart0.print("$91=");FSM::uart0.print(itoa(m_time.secs,conv_string,10));FSM::uart0.print("	seconds\r\n");
	FSM::uart0.print("$92=");FSM::uart0.print(itoa(m_time.mins,conv_string,10));FSM::uart0.print("	minutes\r\n");
	FSM::uart0.print("$93=");FSM::uart0.print(itoa(m_time.hours,conv_string,10));FSM::uart0.print("	hours\r\n");
	FSM::uart0.print("$94=");FSM::uart0.print(m_time.weekday);FSM::uart0.print("	0:su,1:mo,...,6:sa\r\n");
	FSM::uart0.print("$95=");FSM::uart0.print(itoa(m_time.day,conv_string,10));FSM::uart0.print("	month day\r\n");
	FSM::uart0.print("$96=");FSM::uart0.print(itoa(m_time.month,conv_string,10));FSM::uart0.print("	year month\r\n");
	FSM::uart0.print("$97=");FSM::uart0.print(itoa(m_time.year,conv_string,10));FSM::uart0.print("	year (16 - 99)\r\n");
}

unsigned long RTC::get_timestamp(){
	return time_to_epoch(epoch_010116, m_time)+ EPOCH_TIMESTAMP;
}
