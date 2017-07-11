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
 *    @file   SDcard.h
 *    @author gilou
 *    @date   7 juil. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef SDCARD_H_
#define SDCARD_H_

#include "../lib/FAT32.h"

class SDcard {
public:
	/******************************************************************************
	 * Constructor and destructor
	 */
	/**
	 * Class constructor
	 */
	SDcard();
	/**
	 * class destructor
	 */
	virtual ~SDcard();

	/******************************************************************************
	 * setters and getters
	 */


	/******************************************************************************
	 * SDcard's methods
	 */
	void init();


	void write(char *string);


protected:

	unsigned char flag_reinit;					/** flag set apres SD_init pour ne pas la renvoyer tant que la carte est dans le lécteur et on écris toujours**/


	FAT32 fat32;	/**<declaration*/

	char m_filename[15];		/**< filename string */
	char m_tmp_filename[15];		/**< tmp filename string */


};

#endif /* SDCARD_H_ */
