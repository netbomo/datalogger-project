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
 *    @file   Config.cpp
 *    @Author gilou
 *    @date   20 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <string.h>
#include <stdlib.h>
#include <avr/eeprom.h>


#include "../lib/main.h"
#include "../lib/Usart.h"
#include "../lib/DateTime.h"
#include "../lib/FSM.h"
#include "../lib/Config.h"

		/**
		 * \brief Constructor
		 */
        Config::Config():request_i(0){

        }

        Config::~Config(){

        }

        /**
         * \brief Main state function
         */
        void Config::execute (){
        	//print();
        	if(request[0]=='$'&&request[1]=='$'){
        		display();		// display on the usart0 the config menu
        	}
        	else if (request[0]=='$'&& request[3]=='=') {
        		uint8_t item = atoc(request + 1);

        		uint8_t arg_uc = atoi(request + 4);

        		double arg_f = atof(request + 4);

				switch (item) {
					case 0:
						switch (arg_uc) {
								case 0:	/// no measurement, use at the first wake up
									FSM::eeprom.measure_sample_conf = 0;FSM::eeprom.measure_max = 0;FSM::eeprom.measure_periode = 0;
									update_epprom();	FSM::eeprom.measure_counter =0;
									break;
								case 1:	/// Config 1 : 2 measures in 10 secondes
									FSM::eeprom.measure_sample_conf = 1;FSM::eeprom.measure_max = 2;FSM::eeprom.measure_periode = 5;
									update_epprom();	FSM::eeprom.measure_counter =0;
									break;
								case 2:	/// Config 2 : 4 measures in 1 minute
									FSM::eeprom.measure_sample_conf = 2;FSM::eeprom.measure_max = 4;FSM::eeprom.measure_periode = 15;
									update_epprom();	FSM::eeprom.measure_counter =0;
									break;
								case 3:	/// Config 3 : 10 measures in 10 minutes
									FSM::eeprom.measure_sample_conf = 3;FSM::eeprom.measure_max = 10;FSM::eeprom.measure_periode = 60;
									update_epprom();	FSM::eeprom.measure_counter =0;
									break;
								default:
									FSM::uart0.print("Not a correct value");
									break;
								}
						break;
					default:
						break;
				}
			}
        	else FSM::uart0.print("Bad request");
        	request_i=0;
        }

        void Config::print(Usart &usart){
        	usart.print("config state");
        }

        /**
         * \brief display all config on USART0
         * \return void
         */
        void Config::display(){
        	char conversion_string[10];

        	FSM::uart0.print("Configuration :\r\n");
        	FSM::uart0.print("eeprom :\r\n");
        	FSM::uart0.print("$00=");FSM::uart0.print(itoa(FSM::eeprom.measure_sample_conf,conversion_string,10));FSM::uart0.print("	0: not measure,1: 10s average,2:1min average,3:10min average.\r\n");
        	//FSM::uart0.print("	nb of measure = ");FSM::uart0.print(itoa(FSM::eeprom.measure_max,conversion_string,10));FSM::uart0.print("	periode = ");FSM::uart0.print(itoa(FSM::eeprom.measure_periode,conversion_string,10));FSM::uart0.print("\r\n");
        }

        /**
         * \brief Load saved config data from the eeprom
         * \return void
         */
        void Config::load_eeprom (){
        	eeprom_read_block(&FSM::eeprom,0,sizeof(Eeprom));
        	if(FSM::eeprom.structure_version != DATA_STRUCTURE_VERSION) initialize_eeprom();

        }

        /**
         * \brief Update saved config data in the eeprom
         * \return void
         */
        void Config::update_epprom (){
        	eeprom_update_block(&FSM::eeprom,0,sizeof(Eeprom));
        }

        /**
         * \brief Clear the eeprom memory
         * \return void
         */
        void Config::initialize_eeprom (){
        	eeprom_update_block(0,0,sizeof(Eeprom));

        	FSM::eeprom.structure_version = DATA_STRUCTURE_VERSION;

        	update_epprom();

        	load_eeprom();
        }

        /**
         * \brief add char to request
         * \param c (???)
         * \return void
         */
        void Config::addToRequest (char c){
        	request[request_i]= c;
        	request_i++;	/**< increment index : config_request_i++ */
        }

        unsigned char Config::atoc(char string[])
        {
        	uint8_t temp_unit;  	//variable temporaire recevant la valeur entière du caractère "unité" du pointeur string
        	uint8_t temp_diz_unit; 	//variable temporaire recevant la valeur entière du caractère "dizaine" du pointeur string

        	temp_unit = (uint8_t) (*(string+1) - '0');		// le pointeur s_nombre+1 pointe l'unité du nombre à convertir
        	temp_diz_unit = (uint8_t) (*string - '0');		// le pointeur s_nombre pointe la dizaine du nombre à convertir

        	return 10*temp_diz_unit + temp_unit;			// renvoi le nombre converti
        }
