/*
  * fct_SDCard.c
 *
 *  Created on: 21 juin 2016
 *      Author: formateur
 */


// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <avr/io.h>
#include <util/delay.h>

#include "../lib/FSM.h"
#include "../lib/fct_SDCard.h"
#include "../lib/SPI.h"



//******************************************************************
//Function	: to initialize the SD/SDHC card in SPI mode
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************
unsigned char fct_SDCard::SD_init(void)
{
	unsigned char response, SD_version;
	unsigned int retry=0 ;
	int i=0;

	//Spi init
	FSM::spi.init();																/**envoyer l'initialition du périphérique SPI**/

	for(i=0;i<10;i++){
		SD_CS_ASSERT;														/**set chip select*/
		do
		{
		   response = SD_sendCommand(GO_IDLE_STATE, 0); 					/**envoyer un reset du logiciel */
		   retry++;
		   if(retry>0x20){
			   //
			   FSM::uart0.print("\rpas de carte\n");
			   //
			   return 1;  													/**time out, la carte non detecté*/
		   }


		} while(response != 0x01);											/**response = 0x01 : la carte est en idle stat et pas d'érreur*/

		SD_CS_DEASSERT;
		FSM::spi.transmit (0xff);												/**transmission du 1er octet */
		FSM::spi.transmit (0xff);												/**transmission du 2eme octet */

		retry = 0;															/**mettre le compteur d'essais a zero*/

		SD_version = 2; //default set to SD compliance with ver2.x;
						//this may change after checking the next command
		do
		{
		response = SD_sendCommand(SEND_IF_COND,0x000001AA); 				/**vérifier power supply status,pour SDHC card*/
		retry++;
		if(retry>0xfe)
		   {
			  FSM::uart0.print("\r\n");;
			  SD_version = 1;
			  cardType = 1;
			  break;
		   } //time out

		}while(response != 0x01);

		retry = 0;

		do
		{
		response = SD_sendCommand(APP_CMD,0); //CMD55, must be sent before sending any ACMD command
		response = SD_sendCommand(SD_SEND_OP_COND,0x40000000); //ACMD41

		retry++;
		if(retry>0xfe)
		   {
			  FSM::uart0.print("\r\n");;
			  FSM::uart0.print("\rinitialisation a échoué\n");
			  return 2;  //time out, card initialization failed
		   }

		}while(response != 0x00);


		retry = 0;
		SDHC_flag = 0;

		if (SD_version == 2)
		{
		   do
		   {
			 response = SD_sendCommand(READ_OCR,0);
			 retry++;
			 if(retry>0xfe)
			 {
			   FSM::uart0.print("\r\n");;
			   cardType = 0;
			   break;
			 } //time out

		   }while(response != 0x00);

		   if(SDHC_flag == 1) cardType = 2;
		   else cardType = 3;
		}

		//SD_sendCommand(CRC_ON_OFF, OFF); //disable CRC; deafault - CRC disabled in SPI mode
		//SD_sendCommand(SET_BLOCK_LEN, 512); //set block size to 512; default size is 512

		//
		switch (cardType)													/** un switch pour le type de la carte (pour vérifier la communication avec la carte)*/
		{
		case 1 : FSM::uart0.print("\rver1.x\n");break;
		case 2 : FSM::uart0.print("\rSDHC\n");break;
		case 3 : FSM::uart0.print("\rver2.x\n");break;
		default :FSM::uart0.print("\runknown Sd card\n");
		}
		//

	return 0;break;															/**successful return*/
	}
	_delay_ms(1);															/** laisser du temps pour finir l'initialisation*/
	return 0;
}


//******************************************************************
//Function	: to send a command to SD card
//Arguments	: unsigned char (8-bit command value)
// 			  & unsigned long (32-bit command argument)
//return	: unsigned char; response byte
//******************************************************************

unsigned char fct_SDCard::SD_sendCommand(unsigned char cmd, unsigned long arg)
{
	unsigned char response, retry=0, status;

	//SD card accepts byte address while SDHC accepts block address in multiples of 512
	//so, if it's SD card we need to convert block address into corresponding byte address by
	//multipying it with 512. which is equivalent to shifting it left 9 times
	//following 'if' loop does that

		if(SDHC_flag == 0)
			if(cmd == READ_SINGLE_BLOCK     ||
			   cmd == READ_MULTIPLE_BLOCKS  ||
			   cmd == WRITE_SINGLE_BLOCK    ||
			   cmd == WRITE_MULTIPLE_BLOCKS ||
			   cmd == ERASE_BLOCK_START_ADDR||
			   cmd == ERASE_BLOCK_END_ADDR )
			   {
				 	 arg = arg << 9;
			   }

		SD_CS_ASSERT;

		FSM::spi.transmit(cmd | 0x40); //send command, first two bits always '01'
		FSM::spi.transmit(arg>>24);
		FSM::spi.transmit(arg>>16);
		FSM::spi.transmit(arg>>8);
		FSM::spi.transmit(arg);

		if(cmd == SEND_IF_COND)	 //it is compulsory to send correct CRC for CMD8 (CRC=0x87) & CMD0 (CRC=0x95)
			FSM::spi.transmit(0x87);    //for remaining commands, CRC is ignored in SPI mode
		else
			FSM::spi.transmit(0x95);

		while((response = FSM::spi.receive()) == 0xff) //wait response
			if(retry++ > 0xfe) break; //time out error

		if(response == 0x00 && cmd == 58)  //checking response of CMD58
		{
			  status = FSM::spi.receive() & 0x40;     //first byte of the OCR register (bit 31:24)
			  if(status == 0x40) SDHC_flag = 1;  //we need it to verify SDHC card
			  else SDHC_flag = 0;

			  FSM::spi.receive(); //remaining 3 bytes of the OCR register are ignored here
			  FSM::spi.receive(); //one can use these bytes to check power supply limits of SD
			  FSM::spi.receive();
		}

		FSM::spi.receive(); //extra 8 CLK
		SD_CS_DEASSERT;


	return response; //return state
}

//*****************************************************************
//Function	: to erase specified no. of blocks of SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//*****************************************************************

unsigned char fct_SDCard::SD_erase (unsigned long startBlock, unsigned long totalBlocks)
{
	unsigned char response;

		response = SD_sendCommand(ERASE_BLOCK_START_ADDR, startBlock); 						/**envoyer l'adresse du starting block*/
		if(response != 0x00) 																/**vérifier le SD status: 0x00 - OK (No flags set)*/
		  return response;

		response = SD_sendCommand(ERASE_BLOCK_END_ADDR,(startBlock + totalBlocks - 1));		/**envoyer l'adresse du end block*/
		if(response != 0x00)																/**vérifier le SD status: 0x00 - OK (No flags set)*/
		  return response;

		response = SD_sendCommand(ERASE_SELECTED_BLOCKS, 0); 								/**effacer tous les blocks séléctionés*/
		if(response != 0x00)																/**vérifier le SD status: 0x00 - OK (No flags set)*/
		  return response;

	return 0; //normal return
}

//******************************************************************
//Function	: to read a single block from SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************

unsigned char fct_SDCard::SD_readSingleBlock(unsigned long startBlock)
{
	unsigned char response;
	unsigned int i, retry=0;

		 response = SD_sendCommand(READ_SINGLE_BLOCK, startBlock); 							/**read a Block command*/

		 if(response != 0x00) return response; 												/**vérifié si SD status: 0x00 - OK (pas de flag activé)*/

		SD_CS_ASSERT;

		retry = 0;
		while(FSM::spi.receive() != 0xfe) 														/**attendre le start block qu'il prend la val 0xfe*/
		  if(retry++ > 0xfffe){
			  	 	 SD_CS_DEASSERT;
			  	 	 return 1;  } 															/**sortie de la fonction time-out*/

		for(i=0; i<512; i++) 																/** lire toutes les 512 bytes*/
			buffer[i] = FSM::spi.receive();


		FSM::spi.receive(); 																		/**laisser coulé 8 clock pulses*/
		SD_CS_DEASSERT;

	return 0;
}

//******************************************************************
//Function	: to write to a single block of SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************

unsigned char fct_SDCard::SD_writeSingleBlock(unsigned long startBlock)
{
	unsigned char response;
	unsigned int i, retry=0;

		 response = SD_sendCommand(WRITE_SINGLE_BLOCK, startBlock); 						/** commande d'écriture du block*/

		 if(response != 0x00) return response; 												/**vérifié si SD status: 0x00 - OK (pas de flag activé)*/

		SD_CS_ASSERT;

		FSM::spi.transmit(0xfe);     															/**envoyer le start block 0xfe (page 197 datasheet SD)*/

		for(i=0; i<512; i++)    															/**envoyer les 512 bytes data*/
			FSM::spi.transmit(buffer[i]);

		FSM::spi.transmit(0xff);     //transmit dummy CRC (16-bit), CRC is ignored here
		FSM::spi.transmit(0xff);

		response = FSM::spi.receive();

		if( (response & 0x1f) != 0x05)														 /**response= 0xXXX0AAA1 ; si: {	AAA='010' - data accepted
		                              						 	 																AAA='101'-data rejected due to CRC error
																																AAA='110'-data rejected due to write error
																															}*/
		{
			SD_CS_DEASSERT;
			return response;
		}

		while(!FSM::spi.receive()) 																/** attendre que la carte SD finis d'écrire et passe a l'état get idle*/
			if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;}									/**sortie de la fonction time-out*/

		SD_CS_DEASSERT;
		FSM::spi.transmit(0xff);   																/**juste laisser passer 8 clock cycle delay avant la réaffirmation de CS line*/
		SD_CS_ASSERT;         																/**Réaffirmation de CS line pour vérifier si la carte est toujours occupé*/

		while(!FSM::spi.receive()) 																/** attendre que la carte SD finis d'écrire et passe a l'état get idle*/
			if(retry++ > 0xfffe){
				SD_CS_DEASSERT; return 1;
			}

		SD_CS_DEASSERT;

	return 0;
}



#ifndef FAT_TESTING_ONLY

//***************************************************************************/
//Function	: to read multiple blocks from SD card & send every block to UART
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//****************************************************************************/
unsigned char fct_SDCard::SD_readMultipleBlock (unsigned long startBlock, unsigned long totalBlocks)
{
unsigned char response;
unsigned int i, retry=0;

retry = 0;

response = SD_sendCommand(READ_MULTIPLE_BLOCKS, startBlock); //write a Block command

if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

SD_CS_ASSERT;

while( totalBlocks )
{
  retry = 0;
  while(FSM::spi.receive() != 0xfe) //wait for start block token 0xfe (0x11111110)
  if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;} //return if time-out

  for(i=0; i<512; i++) //read 512 bytes
    buffer[i] = FSM::spi.receive();

  FSM::spi.receive(); //receive incoming CRC (16-bit), CRC is ignored here
  FSM::spi.receive();

  FSM::spi.receive(); //extra 8 cycles
  TX_NEWLINE;
  transmitString_F(PSTR(" --------- "));
  TX_NEWLINE;

  for(i=0; i<512; i++) //send the block to UART
  {
    if(buffer[i] == '~') break;
    transmitByte ( buffer[i] );
  }

  TX_NEWLINE;
  transmitString_F(PSTR(" --------- "));
  TX_NEWLINE;
  totalBlocks--;
}

SD_sendCommand(STOP_TRANSMISSION, 0); //command to stop transmission
SD_CS_DEASSERT;
FSM::spi.receive(); //extra 8 clock pulses

return 0;
}

//***************************************************************************/
//Function: to receive data from UART and write to multiple blocks of SD card
//Arguments: none
//return: unsigned char; will be 0 if no error,
// otherwise the response byte will be sent
//****************************************************************************/
unsigned char fct_SDCard::SD_writeMultipleBlock(unsigned long startBlock, unsigned long totalBlocks)
{
unsigned char response, data;
unsigned int i, retry=0;
unsigned long blockCounter=0, size;

response = SD_sendCommand(WRITE_MULTIPLE_BLOCKS, startBlock); //write a Block command

if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

SD_CS_ASSERT;

TX_NEWLINE;
transmitString_F(PSTR(" Enter text (End with ~): "));
TX_NEWLINE;

while( blockCounter < totalBlocks )
{e. Cette librairie est dépendante de la librairie SPI. Voici le code à m
   i=0;
   do
   {
     data = receiveByte();
     if(data == 0x08)	//'Back Space' key pressed
	 {
	   if(i != 0)
	   {
	     transmitByte(data);
	     transmitByte(' ');
	     transmitByte(data);
	     i--;
		 size--;
	   }
	   continue;
	 }
     transmitByte(data);
     buffer[i++] = data;
     if(data == 0x0d)
     {
        transmitByte(0x0a);
        buffer[i++] = 0x0a;
     }
	 if(i == 512) break;
   }while (data != '~');

   TX_NEWLINE;
   transmitString_F(PSTR(" ---- "));
   TX_NEWLINE;

   FSM::spi.transmit(0xfc); //Send start block token 0xfc (0x11111100)

   for(i=0; i<512; i++) //send 512 bytes data
     FSM::spi.transmit( buffer[i] );

   FSM::spi.transmit(0xff); //transmit dummy CRC (16-bit), CRC is ignored here
   FSM::spi.transmit(0xff);

   response = FSM::spi.receive();
   if( (response & 0x1f) != 0x05) //response= 0xXXX0AAA1 ; AAA='010' - data accepted
   {                              //AAA='101'-data rejected due to CRC error
      SD_CS_DEASSERT;             //AAA='110'-data rejected due to write error
      return response;
   }

   while(!FSM::spi.receive()) //wait for SD card to complete writing and get idle
     if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;}

   FSM::spi.receive(); //extra 8 bits
   blockCounter++;
}

FSM::spi.transmit(0xfd); //send 'stop transmission token'

retry = 0;
	   if(data == '~')
	   {
		  fileSize--;	//to remove the last entered '~' character
		  i--;

		  for(;i<512;i++)  //fill the rest of the buffer with 0x00
			buffer[i]= 0x00;

		  error = SD_writeSingleBlock (startBlock);

		  FSM::uart0.print( "data = ~\n");
		  break;
	   }
while(!FSM::spi.receive()) //wait for SD card to complete writing and get idle
   if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;}

SD_CS_DEASSERT;
FSM::spi.transmit(0xff); //just spend 8 clock cycle delay before reasserting the CS signal
SD_CS_ASSERT; //re assertion of the CS signal is required to verify if card is still busy

while(!FSM::spi.receive()) //wait for SD card to complete writing and get idle
   if(retry++ > 0xfffe){SD_CS_DEASSERT; return 1;}
SD_CS_DEASSERT;

return 0;
}
//*********************************************/

#endif

