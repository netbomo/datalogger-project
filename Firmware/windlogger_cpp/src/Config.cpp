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

#include "../lib/DateTime.hpp"
#include "../lib/State.hpp"
#include "../lib/Config.hpp"

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
        	if(strcmp(request,"$$")){
        		print_config();
        	}
        	else if (request[0]=='$'&& request[3]=='=') {
				update(request);
			}
        	///else usart_print(USART0,"Bad request");
        	/// \todo improve USART lib
        }

        /**
         * \brief display all config on USART0
         * \return void
         */
        void Config::print_config (){

        }

        /**
         * \brief update confif register
         * \param request (???)
         * \return void
         */
        void Config::update(char request[]){

        }

        /**
         * \brief add char to request
         * \param c (???)
         * \return void
         */
        void Config::addToRequest (char c){

        }
