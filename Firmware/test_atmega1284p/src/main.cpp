/*
 * main.cpp
 *
 *  Created on: 6 mars 2017
 *      Author: gilou
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "../lib/FSM.h"

#define PORT_LED PORTB		// define de PORT where the LED is connected

int main(){
	/**************************************************************************
	 * 		Constants declaration
	 **************************************************************************/
	const char LED_SCLK = 7;			// the led is connected on the PB7 SCLK pin

	/**************************************************************************
	 * 		Ports initialization
	 **************************************************************************/

	DDRA = 0x00;		// PortA as analog inputs

	DDRB = 0xF7;		// PortB[0-2] are input, other is set to outputs, SPI will be initialise when it will be used.

	DDRD= 0xFF;			// Config the port D as outputs. D0 to D3 are Usart0 and 1, they will be rewrited.
	PORTD = 0x00;		// Don"t start the Wifi module at the wake up. (D4-7)

	/**************************************************************************
	 * 		Initialize the final state machine (not realy use it but simpler
	 * 		to access peripherals
	 **************************************************************************/
	FSM fsm;							// Initialize FSM (use for access to peripherals)

	sei();								// Active globals interrupts

	fsm.usart0.print("Initialization Windlogger test 0.0.4\r\n\r\n");		// Send string on UART0 to try it

	fsm.timestamp = 0;		// Initialize the timestamp value ?? why not whith rtc?

	/**************************************************************************
	 * 		Create variables and initialize them.
	 **************************************************************************/

	unsigned long int timestamp_old, waiting_10s;		// time references
	timestamp_old = fsm.timestamp;						// set value
	waiting_10s = fsm.timestamp + 10;					// set value

	/**************************************************************************
	 * 		Test the wifi module
	 **************************************************************************/

	fsm.usart0.print("WiFi initialization :\r\n");	// title on uart0
	fsm.wifi.enable();		// start the module wifi

	// test the module initialization
	fsm.wifi.test_init();

	/**************************************************************************
	 * 		looping test
	 **************************************************************************/
	fsm.usart0.print("\r\nlooping test :\r\n");
	// switch on LED_SCLK
	PORT_LED |= _BV(LED_SCLK);

	fsm.usart0.print("\r\n - The green led near the usb module, is blinking?\r\n");

	while(1){
		/**************************************************************************
		 * 		Blinking test
		 **************************************************************************/
		if(timestamp_old!=fsm.timestamp){

			if(PORT_LED && _BV(LED_SCLK)) PORT_LED &= ~_BV(LED_SCLK);
			else PORT_LED |= _BV(LED_SCLK);
			timestamp_old=fsm.timestamp;
		}

		_delay_ms(1);

		if(fsm.timestamp == waiting_10s){
			fsm.wifi.test_wifi_tx("wifi:1");

			waiting_10s = fsm.timestamp + 10;
		}

	}


	return 0;
}





