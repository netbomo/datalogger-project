/**
 * @file   usart.c
 * @author gilou
 * @date   15 juil. 2016
 * @brief  uart lib for the atmega 328p.
 *
 * Look the chapter called:
 * 24. USART - Universal Synchronous Asynchronous Receiver Transceiver
 * in the ATmega328/P datasheet (p225)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "../lib/main.h"

void USART0_init()
{
	UBRR0 = usart0_baudrate; // set baud rate

	UCSR0A = _BV(U2X0); // for the baud rate setting

	UCSR0B = _BV(RXCIE0) | _BV(RXEN0) |_BV(TXEN0); //enable receiver and transmitter
	UCSR0C = _BV(UPM01) | _BV(UPM00) | _BV(UCSZ01) | _BV(UCSZ00);	// Set frame format : 8data 2stop bit
}

void USART0_set(char character)
{
   /* Wait for empty transmit buffer */
   while ( !( UCSR0A & (1<<UDRE0)) )
	  ;
   /* Put data into buffer, sends the data */
   UDR0 = character;
}


void USART0_print(char *string)
{
	uint8_t i = 0;

		while(*(string+i)!='\0')
		{
			USART0_set(*(string+i));			//envoi de caractère à la volée contenu dans les adresses de data (adresses incrémentée par i)
			i++;								//tant que la data ne contient pas '\0'
		}
}

ISR(USART0_RX_vect)							//sous routine d'interruption lors d'une reception
{
	data_udr0 = UDR0;						//récupération du registre de donnée UDR0 dans la variable globale data_udr0
	flag_USART0_read = 1;					//mise à 1 du drapeau de reception pour interruption
}

unsigned char get_Arg(char string[])
{
	uint8_t temp_unit;  	//variable temporaire recevant la valeur entière du caractère "unité" du pointeur string
	uint8_t temp_diz_unit; 	//variable temporaire recevant la valeur entière du caractère "dizaine" du pointeur string

	temp_unit = (uint8_t) (*(string+1) - '0');		// le pointeur s_nombre+1 pointe l'unité du nombre à convertir
	temp_diz_unit = (uint8_t) (*string - '0');		// le pointeur s_nombre pointe la dizaine du nombre à convertir

	return 10*temp_diz_unit + temp_unit;			// renvoi le nombre converti
}

void USART0_print_conf(){

	char conv_string[10];

	USART0_print("Configuration :\r\n");
	USART0_print("Logger :\r\n");
	USART0_print("$00=");USART0_print(itoa(logger.meas_conf,conv_string,10));USART0_print("	0: not measure,1: 10s average,2:1min average,3:10min average.\r\n");
	USART0_print("Time :\r\n");
	USART0_print("$11=");USART0_print(itoa(ptrTime->secs,conv_string,10));USART0_print("	seconds\r\n");
	USART0_print("$12=");USART0_print(itoa(ptrTime->mins,conv_string,10));USART0_print("	minutes\r\n");
	USART0_print("$13=");USART0_print(itoa(ptrTime->hours,conv_string,10));USART0_print("	hours\r\n");
	USART0_print("$14=");USART0_print(ptrTime->weekday);USART0_print("	0:su,1:mo,...,6:sa\r\n");
	USART0_print("$15=");USART0_print(itoa(ptrTime->day,conv_string,10));USART0_print("	month day\r\n");
	USART0_print("$16=");USART0_print(itoa(ptrTime->month,conv_string,10));USART0_print("	year month\r\n");
	USART0_print("$17=");USART0_print(itoa(ptrTime->year,conv_string,10));USART0_print("	year (16 - 99)\r\n");
	USART0_print("Sensors :\r\n");
	USART0_print("$20=");USART0_print(dtostrf(logger.anemo1_factor,0,3,conv_string));USART0_print("	anemo1 factor\r\n");
	USART0_print("$21=");USART0_print(dtostrf(logger.anemo1_offset,0,3,conv_string));USART0_print("	anemo1 offset\r\n");
	USART0_print("$22=");USART0_print(itoa(logger.degree_offset,conv_string,10));USART0_print("	degree offset must between 0 and 359\r\n");
	USART0_print("$23=");USART0_print(itoa(logger.inPinVane,conv_string,10));USART0_print("	vane pin\r\n");
	USART0_print("AC wattmeter\r\n");
	USART0_print("$30=");USART0_print(itoa(logger.inPinV,conv_string,10));USART0_print("	Ac voltage pin\r\n");
	USART0_print("$31=");USART0_print(itoa(logger.inPinI,conv_string,10));USART0_print("	Ac current pin\r\n");
	USART0_print("$32=");USART0_print(dtostrf(logger.VCAL,0,3,conv_string));USART0_print("	AC voltage coefficient,  calib step1\r\n");
	USART0_print("$33=");USART0_print(dtostrf(logger.ICAL,0,3,conv_string));USART0_print("	AC current coefficient,  calib step2\r\n");
	USART0_print("$34=");USART0_print(dtostrf(logger.PHASECAL,0,3,conv_string));USART0_print("	phase calibration between 0 and 2, near 1, calib step3\r\n");
}

