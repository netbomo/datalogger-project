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
 *    @file   main.h
 *    @author betzy
 *    @date   20 août 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef LIB_MAIN_H_
#define LIB_MAIN_H_

/************************************** globals constants **************************************/
#define BR_57600 34;	/**< UBBRn registers value to work at 57600 baud**/
#define BR_115200 16;	/**< UBBRn registers value to work at 115200 baud**/
/********************************** end globals constants **************************************/

/************************************** globals variables **************************************/
volatile unsigned int	usart0_baudrate;				// usart0 baudrate valueset in main.c
volatile unsigned char	data_udr0;						//récupération du registre de donnée UDR0 dans la variable globale data_udr0
volatile unsigned char	flag_USART0_read;				//mise à 1 du drapeau de reception pour interruption
volatile unsigned long int timestamp;					// create and initialize second counter
/********************************** end globals variables **************************************/



#endif /* LIB_MAIN_H_ */
