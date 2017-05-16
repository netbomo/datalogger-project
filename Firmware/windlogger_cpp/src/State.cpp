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
 *    @file   State.cpp
 *    @author gilou
 *    @date   30 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "../lib/Usart.h"
#include "../lib/main.h"
#include "../lib/State.h"

State::State(){

}

State::~State(){

}

Idle::Idle(){

}

Idle::~Idle(){

}

void Idle::execute(){
}

void Idle::print(Usart &usart){
	usart.print("idle state");
}
