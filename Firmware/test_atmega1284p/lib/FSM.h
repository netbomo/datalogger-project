/*
 * main.hpp
 *
 *  Created on: 9 mai 2017
 *      Author: gilou
 */

#ifndef LIB_FSM_H_
#define LIB_FSM_H_

#include "../lib/Usart.h"


class FSM{

public:
	FSM();

	static Usart usart0;
	static unsigned long int timestamp;
};



#endif /* LIB_FSM_H_ */
