/**
 *******************************************************************************
 *******************************************************************************
 *
 *	License :
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *     any later version.
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
 *    @author gilou
 *    @date   30 avr. 2017
 *    @brief  The main.h file regroup global constant declaration.
 */
#ifndef LIB_MAIN_HPP_
#define LIB_MAIN_HPP_

/******************************************************************************
 * Pins declaration
 */
static const unsigned char LED_PIN = 7;		/// LED_PIN is on the PB7 pin

static const unsigned char PIN_WINDVANE = 6;	/**< windvane pin constant */
static const unsigned char PIN_VOLT1 = 0;
static const unsigned char PIN_VOLT2 = 7;
static const unsigned char PIN_CT1 = 1;
static const unsigned char PIN_CT2 = 2;
static const unsigned char PIN_CT3 = 3;
static const unsigned char PIN_CT4 = 4;


/******************************************************************************
 * Structure constants
 */
static const unsigned int DATA_STRUCTURE_VERSION = 122717;		///use date of the data structure's revision
//static const unsigned char NUMBER_OF_SENSORS = 3;	/**< this need to be increase if you have more than 3 sensors! */
static const unsigned char MAX_DATA_SAMPLE = 10;	/**< this needs to be the same as the highest value of FSM::eeprom.measure_max. From now, the highest possible value is 10 (10mins average).  */
static const unsigned char EEPROM_OFFSET = 30;		/**< this is the eeprom offset use to save sensor parameters */

#endif /* LIB_MAIN_HPP_ */
