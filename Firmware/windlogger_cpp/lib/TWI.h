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
 *    @file   TWI.h
 *    @author gilou
 *    @date   1 juil. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef TWI_H_
#define TWI_H_

class TWI {
public:
	TWI();
	virtual ~TWI();

	// Fonction d'écriture à la volée d'une donnée, sur un périph, inclus dans un registre, sur un nombre de registres définis
	// Retour : unsigned char = la fonction renvoi un code erreur correspondant à la sous fonction en défaut (cf:datasheet Atmega 128)
	// Paramètre 1 : unsigned char = adresse du périphérique
	// Paramètre 2 : unsigned char = adresse du registre
	// Paramètre 3 : unsigned char = nombre de registres à écrire
	// Paramètre 4 : unsigned char = valeur à écrire
	unsigned char print(unsigned char , unsigned char, unsigned char, unsigned char *);

	// Fonction de lecture d'une donnée, sur un périph, inclus dans un registre, sur un nombre de registres définis
	// Retour : unsigned char = la fonction renvoi un code erreur correspondant à la sous fonction en défaut (cf: datasheet Atmega 128)
	// Paramètre 1 : adresse du périphérique
	// Paramètre 2 : adresse du registre
	// Paramètre 3 : nombre de registres
	// Paramètre 4 : valeur de l'adresse du pointeur à lire
	unsigned char receive(unsigned char, unsigned char, unsigned char, unsigned char *);


	unsigned char set_char(unsigned char addr, unsigned char reg, unsigned char data);

	unsigned char  get_char(unsigned char addr, unsigned char reg);

private:

	void start ();

	void restart ();

	void stop ();

	void set (unsigned char data);

	unsigned char get_ack(void);

	unsigned char get_nack(void);

	unsigned char get_status(void);

};

#endif /* TWI_H_ */
