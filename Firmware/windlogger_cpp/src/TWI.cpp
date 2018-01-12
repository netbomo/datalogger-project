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
 *    @file   TWI.cpp
 *    @author gilou
 *    @date   1 juil. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../lib/TWI.h"

TWI::TWI() {
	TWSR = 0x00; 					//prescaler à 1
	TWBR = (F_CPU / 100000UL - 16) / 2;					//bite rate register à 0x20 = 32, 0x0C for 400kHz
	TWCR = (1<<TWEN);				//enable TWI
}

TWI::~TWI() {
	// TODO Auto-generated destructor stub
}

// Fonction de démarrage du protocole TWI
// Retour : void
// Paramètre 1 : void
void TWI::start () 	// Fonction de démarrage du protocole TWI

{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); 	//mettre à 1 twint, twstart, twenable sur le registre de controle (TWCR)
	while (!(TWCR & (1<<TWINT))); 				//attendre que TWINT soit denouveau à l'état haut

}

// Fonction de redémarrage du protocole TWI
// Retour : void
// Paramètre 1 : void
void TWI::restart () // Fonction de redémarrage du protocole TWI
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);		//mettre à 1 twint, twstart, twenable sur le registre de controle (TWCR)
	while (!(TWCR & (1<<TWINT)));				//attendre que TWINT soit denouveau à l'état haut

}

// Fonction d'arrêt du protocole TWI
// Retour : void
// Paramètre 1 : void
void TWI::stop ()	// Fonction d'arrêt du protocole TWI
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);		//mettre à 1 twint, twstop, twenable sur le registre de controle (TWCR)
}

// Fonction d'ecriture d'un octet dans le registre de donnée (TWDR)
// Retour : void
// Paramètre 1 : unsigned char = adresse du périph, du registre ou valeur de la donnée à écrire sur TWDR
void TWI::set (uint8_t data)	// Fonction d'ecriture d'un octet dans le registre de donnée (TWDR)
{
	TWDR = data;							//le registre de donnée (TWDR) reçoit la valeur de "data"
	TWCR = (1<<TWINT)|(1<<TWEN);			//mettre à 1 twint et twen dans le registre de controle (TWCR)
	while (!(TWCR & (1<<TWINT)));			//attendre que TWINT soit denouveau à l'état haut
}

// Fonction de lecture du registre de donnée et ACKNOWLEDGEMENT
// Retour : unsigned char = la fonction renvoi la valeur du registre de donnée (TWDR)
// Paramètre 1 : void
uint8_t TWI::get_ack(void)	// Fonction de lecture du registre de donnée et d'envoie d'ACKNOWLEDGEMENT
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);	//mettre à 1 twint, twen, et twea dans le registre de controle (TWCR)
    while (!(TWCR & (1<<TWINT)));			//attendre que TWINT soit denouveau à l'état haut
    return TWDR;							//quand twint denouveau à l'état haut, renvoie la valeur du registre de donnée (TWDR)
}

// Fonction de lecture du registre de donnée et NOT ACKNOWLEDGEMENT
// Retour : unsigned char = la fonction renvoi la valeur du registre de donnée (TWDR)
// Paramètre 1 : void
uint8_t TWI::get_nack(void)	// Fonction de lecture du registre de donnée et pas d'envoie d'ACKNOWLEDGEMENT

{
	TWCR = (1<<TWINT)|(1<<TWEN);			//mettre à 1 twint, et twen dans le registre de controle (TWCR)
	while (!(TWCR & (1<<TWINT)));			//attendre que twint soit denouveau à l'état haut
	return TWDR;							//quand twint denouveau à l'état haut, renvoie la valeur du registre de donnée (TWDR)
}

// Fonction de lecture du registre de status (TWSR)
// Retour : unsigned char = la fonction renvoi la valeur du registre de status (TWSR) en masquant les deux bits de prescaler
// Paramètre 1 : void
uint8_t TWI::get_status(void)	// Fonction de lecture du registre de status (TWSR)

{
    return TWSR & 0xF8; 		//renvoie la valeur du registre de status (TWSR) en masquant les bits de prescaler
}


uint8_t TWI::set_char(uint8_t addr,uint8_t reg, uint8_t data)	// Fonction d'écriture à la volée d'une donnée, sur un périph, inclus dans un registre, sur un nombre de registres définis

{
	start();
	if (get_status()!= 0x08)		//Si condition Start non reconnue renvoie erreur : 0x08
		return 0x08;

	set(addr);
	if (get_status() != 0x18)		//Si condition set non reconnue renvoie erreur : 0x18
		return 0x18;

	set(reg);
	if (get_status() != 0x28)	//Si condition set non reconnue renvoie erreur : 0x18
		return 0x19;

	set(data);
	if (get_status() != 0x28)	//Si condition set non reconnue renvoie erreur : 0x28
		return 0x28;

	stop();

	return 0x00;						//renvoie le code 0x00 si la fonction print c'est executée sans soucis
}

uint8_t TWI::get_char(uint8_t addr, uint8_t reg)	// Fonction d'écriture d'une donnée sur un pointeur, sur un périph, inclus dans un registre, sur un nombre de registres définis

{

	uint8_t data=0;						//création d'une variable d'index locale à la fonction

	start();
	if (get_status() != 0x08)return 0x08;		//Si condition Start non reconnue renvoie erreur : 0x08

	set(addr);
	if (get_status() != 0x18)return 0x18;		//Si condition set non reconnue renvoie erreur : 0x18

	set(reg);
	if(get_status() != 0x28)return 0x28;		//Si condition set non reconnue renvoie erreur : 0x28

	restart();
	if (get_status() != 0x10)return 0x10;		//si la condition repeted start non reconnue renvoie erreur : 0x10

	set(addr+1);

	if (get_status() != 0x40)return 0x40;		//si la condition set non reconnue en mode master receiver renvoie erreur : 0x40

	// l' octet à récupérer doit être sans acknowlegdment
	data = get_nack();		//la valeur de l'adresse du point read_data recoit la valeur du registre de donnée (TWDR) lorsqu'il n'y a pas d'ACK
	if (get_status() != 0x58)	//si la condition de lecture de donnée non reconnue après un NACK, renvoie code erreur : 0x58
		return 0x58;

	stop();

	return data;						//la fonction receive renvoie la valeur 0xF0 quand la fonction s'est déroulé sans erreur
}




uint8_t TWI::print (uint8_t addr,uint8_t reg, uint8_t nb, uint8_t *write_data)	// Fonction d'écriture à la volée d'une donnée, sur un périph, inclus dans un registre, sur un nombre de registres définis

{
	uint8_t i;							//création d'une variable d'index locale à la fonction

	start();
	if (get_status()!= 0x08)		//Si condition Start non reconnue renvoie erreur : 0x08
		return 0x08;

	set(addr);
	if (get_status() != 0x18)		//Si condition set non reconnue renvoie erreur : 0x18
		return 0x18;

	set(reg);
	if (get_status() != 0x28)	//Si condition set non reconnue renvoie erreur : 0x18
		return 0x19;

	for(i=0; i < nb; i++)
	{
		set(*(write_data+i));
		if (get_status() != 0x28)	//Si condition set non reconnue renvoie erreur : 0x28
			return 0x28;
	}
	stop();
	return 0x00;						//renvoie le code 0x00 si la fonction print c'est executée sans soucis
}

uint8_t TWI::receive(uint8_t addr, uint8_t reg, uint8_t nb, uint8_t *read_data)	// Fonction d'écriture d'une donnée sur un pointeur, sur un périph, inclus dans un registre, sur un nombre de registres définis

{
	uint8_t i=0;						//création d'une variable d'index locale à la fonction

	start();
	if (get_status() != 0x08)		//Si condition Start non reconnue renvoie erreur : 0x08
		return 0x08;

	set(addr);
	if (get_status() != 0x18)		//Si condition set non reconnue renvoie erreur : 0x18
		return 0x18;

	set(reg);
	if(get_status() != 0x28)		//Si condition set non reconnue renvoie erreur : 0x28
		return 0x28;

	restart();
	if (get_status() != 0x10)		//si la condition repeted start non reconnue renvoie erreur : 0x10
		return 0x10;

	set(addr+1);

	if (get_status() != 0x40) 		//si la condition set non reconnue en mode master receiver renvoie erreur : 0x40
		return 0x40;

	for (i = 0; i < nb-1; i++)			//début boucle for d'index i initialisé à 0, qui va jusqu'au paramètre nb-1 pour finir sur le dernier par get_nack, et on l'incrémente
	{
		*(read_data+i) = get_ack();	//la valeur de l'adresse du pointeur read_data+i reçoit la valeur de registre de donnée (TWDR) à l'envoie d'un ACK
		if (get_status() != 0x50)	//si la condition de lecture de donnée non reconnue après un ACK, renvoie code erreur : 0x50
			return 0x50;
	}

	// le dernier octet à récupérer doit être sans acknowlegdment
	*(read_data+nb-1) = get_nack();		//la valeur de l'adresse du point read_data recoit la valeur du registre de donnée (TWDR) lorsqu'il n'y a pas d'ACK
		if (get_status() != 0x58)	//si la condition de lecture de donnée non reconnue après un NACK, renvoie code erreur : 0x58
		return 0x58;

	stop();

	return 0x00;						//la fonction receive renvoie la valeur 0x00 quand la fonction s'est déroulé sans erreur
}
