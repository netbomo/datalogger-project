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
 *    @file   SDcard.cpp
 *    @author gilou
 *    @date   7 juil. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <avr/io.h>
#include <string.h>

#include "../lib/FSM.h"
#include "../lib/fct_SDCard.h"
#include "../lib/SDcard.h"

SDcard::SDcard() {
	// TODO Auto-generated constructor stub
	flag_reinit = 0;
	cardType=0;

}

SDcard::~SDcard() {
	// TODO Auto-generated destructor stub
}

void SDcard::init(){

	//do stuff

	SD_init();
	fat32.getBootSectorData();
}


void SDcard::write(char *string){
	if	((PINC & 0x03)==0){						/**si PG2 = 0 => la carte SD est dans le lécteur*/
				if(flag_reinit == 0){								/** envoyer un init de la carte si elle est remise dans le lécteur*/
					//SDCard init
					SD_init();
					// OUVERTURE DE LA PARTITION
					fat32.getBootSectorData();				/** envoyer cette fonction pour l'identification du péripherique ( a faire toujours apres l'init de SD)*/

					flag_reinit = 1;								/** set flag_reinit*/
				}
			char j=0;

			strcpy(m_tmp_filename, m_filename) ;		/**copier le nom de fichier dans le registre temporaire*/

						j = fat32.readFile(fat32.VERIFY, m_tmp_filename);				/**vérifier si le fichier éxiste*/

						strcpy(m_tmp_filename, m_filename) ;		/**copier le nom de fichier dans le registre temporaire*/

						if(j==0){													/**si le fichier n'éxiste pas*/
							//strcpy(string, ) ; /**init datastring avec cette chaine de caractére*/
							fat32.writeFile(m_tmp_filename, "timestamp,speed1,speed2,degree,temp,acPower,dcPower,rpm\n");													/** construire le fichier avec l'entête au début*/
							}

				strcpy(m_tmp_filename, m_filename) ;		/**copier le nom de fichier dans le registre temporaire*/

				fat32.writeFile(m_tmp_filename,string);

			}
			else {															/**si PG2 = 1 => la carte SD est hors lécteur*/

					FSM::uart0.print("\rpas de carte sd, insérez une dans le lécteur\n\r");
					flag_reinit = 0;										/**rénitialiser i a la sortie de cette boucle pour lancé l'init si PG2 change*/

				}
}
