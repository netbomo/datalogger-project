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
 *    @file   Config.cpp
 *    @author gilou
 *    @date   20 avr. 2017
 *    @brief  This file define the Config class.
 *
 *    The class Config inherits from the class State.
 *    This class manage input between the FSM, the uarts (serial interface) and eeprom.
 *    With this class, the windlogger can display eeprom saved data and update there from the uart0.
 *    It can control time update with the DateTime class to have correct convertion from a human
 *    date and time display and the unix time stamp use by the logger to timestamp data.
 */

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <string.h>
#include <stdlib.h>
#include <avr/eeprom.h>


#include "../lib/main.h"
#include "../lib/Usart.h"
#include "../lib/FSM.h"
#include "../lib/st_Config.h"

Config::Config():request_i(0){
	strcpy(m_name,"config\0");
}

// Destructor
Config::~Config(){}

/******************************************************************************
 * inherits from State
 */
// The execute method is the config "main function"
void Config::execute (){
	//print();
	if(request[0]=='$'&&request[1]=='$'){
		display();		// display on the usart0 the config menu
		/// @todo correct display bug
	}
	else if (request[0]=='$'&& request[3]=='=') {
		uint8_t item = atoc(request + 1);

		uint8_t arg_uc = atoi(request + 4);

		double arg_f = atof(request + 4);

		switch (item) {
		case 0:	// choose measurement periode parameter
			switch (arg_uc) {
			case 0:	// no measurement, use at the first wake up
				FSM::logger.measure_sample_conf = 0;FSM::logger.measure_max = 0;FSM::logger.measure_periode = 0;
				update_logger();	FSM::logger.measure_counter =0;
				break;
			case 1:	// Config 1 : 2 measures in 10 secondes
				FSM::logger.measure_sample_conf = 1;FSM::logger.measure_max = 2;FSM::logger.measure_periode = 5;
				update_logger();	FSM::logger.measure_counter =0;
				break;
			case 2:	// Config 2 : 4 measures in 1 minute
				FSM::logger.measure_sample_conf = 2;FSM::logger.measure_max = 4;FSM::logger.measure_periode = 15;
				update_logger();	FSM::logger.measure_counter =0;
				break;
			case 3:	// Config 3 : 10 measures in 10 minutes
				FSM::logger.measure_sample_conf = 3;FSM::logger.measure_max = 10;FSM::logger.measure_periode = 60;
				update_logger();	FSM::logger.measure_counter =0;
				break;
			default:
				FSM::uart0.print("Not a correct value");
				break;
			}
			break;
			case 01:	// Set the Node id number
				FSM::logger.node_id = arg_uc;
				update_logger();
				break;
			case 02:
				FSM::timestamp = strtoul(request + 4,NULL,10);
				break;
			case 10:	// set anemometer 1 factor, update eeprom method in the setter
				FSM::anemo1.set_enable(arg_uc);
				break;
			case 11:	// set anemometer 1 factor, update eeprom method in the setter
				FSM::anemo1.set_factor(arg_f);
				break;
			case 12:	// set anemometer 1 offset, update eeprom method in the setter
				FSM::anemo1.set_offset(arg_f);
				break;
			case 20:	// set anemometer 1 factor, update eeprom method in the setter
				FSM::anemo2.set_enable(arg_uc);
				break;
			case 21:	// set anemometer 2 factor, update eeprom method in the setter
				FSM::anemo2.set_factor(arg_f);
				break;
			case 22:	// set anemometer 2 offset, update eeprom method in the setter
				FSM::anemo2.set_offset(arg_f);
				break;
			case 30:	// set anemometer 1 factor, update eeprom method in the setter
				FSM::windvane.set_enable(arg_uc);
				break;
			case 31:	// set windvane factor, update eeprom method in the setter
				FSM::windvane.set_factor(arg_f);
				break;
			case 32:	// set windvane offset, update eeprom method in the setter
				FSM::windvane.set_offset(arg_f);
				break;
			case 51:	// set windvane factor, update eeprom method in the setter
				FSM::powerAC.set_v_factor(arg_f);
				break;
			case 52:	// set windvane offset, update eeprom method in the setter
				FSM::powerAC.set_v_offset(arg_f);
			break;
		case 53:	// set windvane factor, update eeprom method in the setter
			FSM::powerAC.set_v_phase(arg_f);
			break;
		case 54:	// set windvane offset, update eeprom method in the setter
			FSM::powerAC.set_i_factor(arg_f);
			break;
		case 55:	// set windvane factor, update eeprom method in the setter
			FSM::powerAC.set_i_offset(arg_f);
			break;
		case 91:
			if(arg_uc>=0 &&arg_uc<60)	FSM::rtc.update_reg(FSM::rtc.SECS,arg_uc);
			else FSM::uart0.print("must be: 0=<secs<60");
			break;
		case 92:
			if(arg_uc>=0 &&arg_uc<60)	FSM::rtc.update_reg(FSM::rtc.MINS,arg_uc);
			else FSM::uart0.print("must be: 0=<mins<60");
			break;
		case 93:
			if(arg_uc>=0 &&arg_uc<24)	FSM::rtc.update_reg(FSM::rtc.HOURS,arg_uc);
			else FSM::uart0.print("must be: 0=<hour<24");
			break;
		case 94:
			//arg_uc = atoi(_config_request+4);
			if(arg_uc>=0 &&arg_uc<7)	FSM::rtc.update_reg(FSM::rtc.WEEKDAY,arg_uc);
			else FSM::uart0.print("must be: 0=<weekday<7");
			break;
		case 95:
			if(arg_uc>0 &&arg_uc<32)	FSM::rtc.update_reg(FSM::rtc.DAY,arg_uc);
			else FSM::uart0.print("must be: 0<day<32");
			break;
		case 96:
			if(arg_uc>0 &&arg_uc<13)	FSM::rtc.update_reg(FSM::rtc.MONTH,arg_uc);
			else FSM::uart0.print("must be: 0<month<13");
			break;
		case 97:
			if(arg_uc>=16 &&arg_uc<99)	FSM::rtc.update_reg(FSM::rtc.YEAR,arg_uc);
			else FSM::uart0.print("must be: 16=<year<99");
			break;
		default:
			break;
		}
		display();	// display config after modification
	}
	else FSM::uart0.print("Bad request");
	request_i=0;
}

// It's just a simple method to display the state name, use to debug
void Config::print(){
	FSM::uart0.print(m_name);
}


bool Config::isEqual(char *name)const {
	return State::isEqual(name);
}

/******************************************************************************
 * Some methods
 */
// display all config on USART0
void Config::display(){
	char conversion_string[20];

	FSM::uart0.print("Configuration :\r\n");
	FSM::uart0.print("eeprom :\r\n");
	FSM::uart0.print("$00=");FSM::uart0.print(itoa(FSM::logger.measure_sample_conf,conversion_string,10));FSM::uart0.print("	0: not measure,1: 10s average,2:1min average,3:10min average.\r\n");
	FSM::uart0.print("Node id $01= ");FSM::uart0.print(itoa(FSM::logger.node_id,conversion_string,10));FSM::uart0.print("	permit identify each datalogger (0 - 255).\r\n");
	FSM::uart0.print("timestamp $02= "); FSM::uart0.print(ultoa(FSM::timestamp,conversion_string,10));FSM::uart0.print("	update with the current timestamp.\r\n");
	FSM::uart0.print("\r\n");

	//FSM::anemo1.print_config("$10","$11","$12");
	//FSM::anemo2.print_config("$20","$21","$22");
	FSM::windvane.print_config("$30","$31","$32");
	FSM::powerAC.print_config("$51"," $52"," $53","$54"," $55"," $56");

	FSM::uart0.print("\r\n");
	FSM::rtc.print_config();

	FSM::uart0.print("\r\n");
}

//addToRequest : add c to the end of request
void Config::addToRequest (char c){
	request[request_i]= c;
	request_i++;	// increment index : config_request_i++
}

// This method convert a 2 character array in a char value
unsigned char Config::atoc(char string[])
{
	uint8_t temp_unit;  	//variable temporaire recevant la valeur entière du caractère "unité" du pointeur string
	uint8_t temp_diz_unit; 	//variable temporaire recevant la valeur entière du caractère "dizaine" du pointeur string

	temp_unit = (uint8_t) (*(string+1) - '0');		// le pointeur s_nombre+1 pointe l'unité du nombre à convertir
	temp_diz_unit = (uint8_t) (*string - '0');		// le pointeur s_nombre pointe la dizaine du nombre à convertir

	return 10*temp_diz_unit + temp_unit;			// renvoi le nombre converti
}

/******************************************************************************
 * Eeprom management
 */
// Load saved config data from the eeprom
void Config::load_logger(){
	eeprom_read_block(&FSM::logger,0,sizeof(Logger));
	if(FSM::logger.structure_version != DATA_STRUCTURE_VERSION) initialize_logger();

}

//Update saved config data in the eeprom
void Config::update_logger (){
	eeprom_update_block(&FSM::logger,0,sizeof(Logger));
}

// Initialize the eeprom memory
void Config::initialize_logger (){
	eeprom_update_block(0,0,sizeof(Logger));

	FSM::logger.structure_version = DATA_STRUCTURE_VERSION;

	update_logger();

	load_logger();
}

