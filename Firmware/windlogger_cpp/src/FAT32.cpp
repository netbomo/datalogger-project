//************************************************************
// **** ROUTINES FOR FAT32 IMPLEMATATION OF SD CARD ****
//************************************************************
//Controller		: ATmega32 (Clock: 8 Mhz-internal)
//Compiler			: AVR-GCC (winAVR with AVRStudio-4)
//Project Version	: DL_1.0
//Author			: CC Dharmani, Chennai (India)
//			  		  www.dharmanitech.com
//Date				: 10 May 2011
//************************************************************


// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "../lib/FSM.h"
#include "../lib/fct_SDCard.h"
#include "../lib/FAT32.h"

	FAT32::FAT32(){
		firstDataSector = 0;
		rootCluster = 0;
		totalClusters = 0;
		bytesPerSector = 0;
		sectorPerCluster = 0;
		reservedSectorCount = 0;
		unusedSectors = 0;
		appendFileSector = 0;
		appendFileLocation = 0;
		fileSize = 0;
		appendStartCluster = 0;
		freeClusterCountUpdated = 0;

	}

	FAT32::~FAT32(){

	}

//***************************************************************************
//Function: to read data from boot sector of SD card, to determine important
//parameters like bytesPerSector, sectorsPerCluster etc.
//Arguments: none
//return: none
//***************************************************************************
unsigned char FAT32::getBootSectorData (void)
{
struct BS_Structure *bpb; //mapping the buffer onto the structure
struct MBRinfo_Structure *mbr;
struct partitionInfo_Structure *partition;
unsigned long dataSectors;

unusedSectors = 0;

fct.SD_readSingleBlock(0);
bpb = (struct BS_Structure *)fct.buffer;

if(bpb->SjumpBoot[0]!=0xE9 && bpb->SjumpBoot[0]!=0xEB)   //check if it is boot sector
{
  mbr = (struct MBRinfo_Structure *) fct.buffer;       //if it is not boot sector, it must be MBR
  
  if(mbr->signature != 0xaa55) return 1;       //if it is not even MBR then it's not FAT32
  	
  partition = (struct partitionInfo_Structure *)(mbr->partitionData);//first partition
  unusedSectors = partition->firstSector; //the unused sectors, hidden to the FAT
  
  fct.SD_readSingleBlock(partition->firstSector);//read the bpb sector
  bpb = (struct BS_Structure *)fct.buffer;
  if(bpb->SjumpBoot[0]!=0xE9 && bpb->SjumpBoot[0]!=0xEB) return 1;
}

bytesPerSector = bpb->SbytesPerSector;
//transmitHex(INT, bytesPerSector); transmitByte(' ');
sectorPerCluster = bpb->SsectorPerCluster;
//transmitHex(INT, sectorPerCluster); transmitByte(' ');
reservedSectorCount = bpb->SreservedSectorCount;
rootCluster = bpb->SrootCluster;// + (sector / sectorPerCluster) +1;
firstDataSector = bpb->ShiddenSectors + reservedSectorCount + (bpb->SnumberofFATs * bpb->SFATsize_F32);

dataSectors = bpb->StotalSectors_F32
              - bpb->SreservedSectorCount
              - ( bpb->SnumberofFATs * bpb->SFATsize_F32);
totalClusters = dataSectors / sectorPerCluster;
//transmitHex(LONG, totalClusters); transmitByte(' ');

if((getSetFreeCluster (TOTAL_FREE, GET, 0)) > totalClusters)  //check if FSinfo free clusters count is valid
     freeClusterCountUpdated = 0;
else
	 freeClusterCountUpdated = 1;
return 0;
}

//***************************************************************************
//Function: to calculate first sector address of any given cluster
//Arguments: cluster number for which first sector is to be found
//return: first sector address
//***************************************************************************
unsigned long FAT32::getFirstSector(unsigned long clusterNumber)
{
  return (((clusterNumber - 2) * sectorPerCluster) + firstDataSector);
}

//***************************************************************************
//Function: get cluster entry value from FAT to find out the next cluster in the chain
//or set new cluster entry in FAT
//Arguments: 1. current cluster number, 2. get_set (=GET, if next cluster is to be found or = SET,
//if next cluster is to be set 3. next cluster number, if argument#2 = SET, else 0
//return: next cluster number, if if argument#2 = GET, else 0
//****************************************************************************
unsigned long FAT32::getSetNextCluster (unsigned long clusterNumber,
                                 unsigned char get_set,
                                 unsigned long clusterEntry)
{
unsigned int FATEntryOffset;
unsigned long *FATEntryValue;
unsigned long FATEntrySector;
unsigned char retry = 0;

//get sector number of the cluster entry in the FAT
FATEntrySector = unusedSectors + reservedSectorCount + ((clusterNumber * 4) / bytesPerSector) ;

//get the offset address in that sector number
FATEntryOffset = (unsigned int) ((clusterNumber * 4) % bytesPerSector);

//read the sector into a buffer
while(retry <10)
{ if(!fct.SD_readSingleBlock(FATEntrySector)) break; retry++;}

//get the cluster address from the buffer
FATEntryValue = (unsigned long *) &(fct.buffer)[FATEntryOffset];

if(get_set == GET)
  return ((*FATEntryValue) & 0x0fffffff);


*FATEntryValue = clusterEntry;   //for setting new value in cluster entry in FAT

fct.SD_writeSingleBlock(FATEntrySector);

return (0);
}

//********************************************************************************************
//Function: to get or set next free cluster or total free clusters in FSinfo sector of SD card
//Arguments: 1.flag:TOTAL_FREE or NEXT_FREE, 
//			 2.flag: GET or SET 
//			 3.new FS entry, when argument2 is SET; or 0, when argument2 is GET
//return: next free cluster, if arg1 is NEXT_FREE & arg2 is GET
//        total number of free clusters, if arg1 is TOTAL_FREE & arg2 is GET
//		  0xffffffff, if any error or if arg2 is SET
//********************************************************************************************
unsigned long FAT32::getSetFreeCluster(unsigned char totOrNext, unsigned char get_set, unsigned long FSEntry)
{
struct FSInfo_Structure *FS = (struct FSInfo_Structure *) &(fct.buffer);
unsigned char error = 0;

fct.SD_readSingleBlock(unusedSectors + 1);

if((FS->leadSignature != 0x41615252) || (FS->structureSignature != 0x61417272) || (FS->trailSignature !=0xaa550000))
  return 0xffffffff;

 if(get_set == GET)
 {
   if(totOrNext == TOTAL_FREE)
      return(FS->freeClusterCount);
   else // when totOrNext = NEXT_FREE
      return(FS->nextFreeCluster);
 }
 else
 {
   if(totOrNext == TOTAL_FREE)
      FS->freeClusterCount = FSEntry;
   else // when totOrNext = NEXT_FREE
	  FS->nextFreeCluster = FSEntry;
 
   error = fct.SD_writeSingleBlock(unusedSectors + 1);	//update FSinfo
 }
 return 0xffffffff;
}

//***************************************************************************
//Function: to get DIR/FILE list or a single file address (cluster number) or to delete a specified file
//Arguments: #1 - flag: GET_LIST, GET_FILE or DELETE #2 - pointer to file name (0 if arg#1 is GET_LIST)
//return: first cluster of the file, if flag = GET_FILE
//        print file/dir list of the root directory, if flag = GET_LIST
//		  Delete the file mentioned in arg#2, if flag = DELETE
//****************************************************************************
struct dir_Structure* FAT32::findFiles (unsigned char flag,char *fileName)
{
unsigned long cluster, sector, firstSector, firstCluster, nextCluster;
struct dir_Structure *dir;
unsigned int i;
unsigned char j;
char temp_char[15];

cluster = rootCluster; //root cluster

while(1)
{
   firstSector = getFirstSector (cluster);

   for(sector = 0; sector < sectorPerCluster; sector++)
   {
	   fct.SD_readSingleBlock (firstSector + sector);
	

     for(i=0; i<bytesPerSector; i+=32)
     {
	    dir = (struct dir_Structure *) &(fct.buffer)[i];

        if(dir->name[0] == EMPTY) //indicates end of the file list of the directory
		{
		  if(flag == DELETE)
		   //   FSM::uart0.print("File does not exist!")); not implemented
		  return 0;   
		}
		if((dir->name[0] != DELETED) && (dir->attrib != ATTR_LONG_NAME))
        {
          if((flag == GET_FILE) || (flag == DELETE))
          {
            for(j=0; j<11; j++)
            if(dir->name[j] != fileName[j]) break;
            if(j == 11)
			{
			  if(flag == GET_FILE)
              {
			    appendFileSector = firstSector + sector;
				appendFileLocation = i;
				appendStartCluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;
				fileSize = dir->SfileSize;
			    return (dir);
			  }	
			  else    //when flag = DELETE
			  {
				 FSM::uart0.print("\r\nDeleting..\r\n\r\n");
				 firstCluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;
                
				 //mark file as 'deleted' in FAT table
				 dir->name[0] = DELETED;    
				 fct.SD_writeSingleBlock (firstSector+sector);
				 			 
				 freeMemoryUpdate (ADD, dir->SfileSize);

				 //update next free cluster entry in FSinfo sector
				 cluster = getSetFreeCluster (NEXT_FREE, GET, 0); 
				 if(firstCluster < cluster)
				     getSetFreeCluster (NEXT_FREE, SET, firstCluster);

				 //mark all the clusters allocated to the file as 'free'
			     while(1)  
			     {
			        nextCluster = getSetNextCluster (firstCluster, GET, 0);
					getSetNextCluster (firstCluster, SET, 0);
					if(nextCluster > 0x0ffffff6) 
					   {
						FSM::uart0.print("File deleted!");
						return 0;
					   }
					firstCluster = nextCluster;
			  	 } 
			  }
            }
          }
          else  //when flag = GET_LIST
		  {
        	  FSM::uart0.print("\r\n");
			 for(j=0; j<11; j++)
		     {
			   if(j == 8) FSM::uart0.set(' ');
			   FSM::uart0.set(dir->name[j]);
			 }
			 FSM::uart0.print("   ");
		     if((dir->attrib != 0x10) && (dir->attrib != 0x08))
			 {
		    	 FSM::uart0.print("FILE" );
		    	 FSM::uart0.print("   ");
			     displayMemory (LOW, dir->SfileSize);
			 }
			 else
				 if(dir->attrib == 0x10) FSM::uart0.print("DIR");
				 else FSM::uart0.print("ROOT");
		  }
       }
     }
   }

   cluster = (getSetNextCluster (cluster, GET, 0));

   if(cluster > 0x0ffffff6)
   	 return 0;
   if(cluster == 0)
   {
	   FSM::uart0.print("Error in getting cluster");
	   return 0;
   }
 }
return 0;
}

//***************************************************************************
//Function: if flag=READ then to read file from SD card and send contents to UART 
//if flag=VERIFY then functions will verify whether a specified file is already existing
//Arguments: flag (READ or VERIFY) and pointer to the file name
//return: 0, if normal operation or flag is READ
//	      1, if file is already existing and flag = VERIFY; or if flag=READ and file does not exist
//		  2, if file name is incompatible
//***************************************************************************
unsigned char FAT32::readFile (unsigned char flag, char *fileName)
{
struct dir_Structure *dir;
unsigned long cluster, byteCounter = 0, fileSize, firstSector;
unsigned int k;
unsigned char j, error;

error = convertFileName (fileName); //convert fileName into FAT format
if(error) return 2;

dir = findFiles (GET_FILE, fileName); //get the file location
if(dir == 0) 
{
  if(flag == READ) return (1);
  else return (0);
}

if(flag == VERIFY) return (1);	//specified file name is already existing

cluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;

fileSize = dir->SfileSize;

FSM::uart0.print("\r\n\r\n");

while(1)
{
  firstSector = getFirstSector (cluster);

  for(j=0; j<sectorPerCluster; j++)
  {
	  fct.SD_readSingleBlock(firstSector + j);
    
	for(k=0; k<512; k++)
    {
      FSM::uart0.set(fct.buffer[k]);
      if ((byteCounter++) >= fileSize ) return 0;
    }
  }
  cluster = getSetNextCluster (cluster, GET, 0);
  if(cluster == 0) {
	  FSM::uart0.print("Error in getting cluster");
	  return 0;

  }
}
return 0;
}

//***************************************************************************
//Function: to convert normal short file name into FAT format
//Arguments: pointer to the file name
//return: 0, if successful else 1.
//***************************************************************************
unsigned char FAT32::convertFileName (char *fileName)
{
unsigned char fileNameFAT[11];
unsigned char j, k;

for(j=0; j<12; j++)
if(fileName[j] == '.') break;

if(j>8) {
	FSM::uart0.print("Invalid fileName..");
	return 1;
}

for(k=0; k<j; k++) //setting file name
  fileNameFAT[k] = fileName[k];

for(k=j; k<=7; k++) //filling file name trail with blanks
  fileNameFAT[k] = ' ';

j++;
for(k=8; k<11; k++) //setting file extention
{
  if(fileName[j] != 0)
    fileNameFAT[k] = fileName[j++];
  else //filling extension trail with blanks
    while(k<11)
      fileNameFAT[k++] = ' ';
}

for(j=0; j<11; j++) //converting small letters to caps
  if((fileNameFAT[j] >= 0x61) && (fileNameFAT[j] <= 0x7a))
    fileNameFAT[j] -= 0x20;

for(j=0; j<11; j++)
  fileName[j] = fileNameFAT[j];

return 0;
}

//************************************************************************************
//Function: to create a file in FAT32 format in the root directory if given 
//			file name does not exist; if the file already exists then append the data
//Arguments: pointer to the file name
//return: none
//************************************************************************************
unsigned char FAT32::writeFile (char *fileName, char *dataString)
{
unsigned char j,k, data, error = 0, fileCreatedFlag = 0, start = 0, appendFile = 0, sector=0;
unsigned int i, firstClusterHigh=0, firstClusterLow=0;  //value 0 is assigned just to avoid warning in compilation
struct dir_Structure *dir;
unsigned long cluster, nextCluster, prevCluster, firstSector, clusterCount, extraMemory;

j = readFile (VERIFY, fileName);

if(j == 1) 
{
  //FSM::uart0.print(" File already exists, appending data.."));
  appendFile = 1;
  cluster = appendStartCluster;
  clusterCount=0;
  while(1)
  {
    nextCluster = getSetNextCluster (cluster, GET, 0);
    if(nextCluster == EOF) break;
	cluster = nextCluster;
	clusterCount++;
  }

  sector = (fileSize - (clusterCount * sectorPerCluster * bytesPerSector)) / bytesPerSector; //last sector number of the last cluster of the file
  start = 1;
}
else if(j == 2) 
   return 1; //invalid file name

else
{
  FSM::uart0.print("\r\n");
 // FSM::uart0.print(" Creating File.."));

  cluster = getSetFreeCluster (NEXT_FREE, GET, 0);
  if(cluster > totalClusters)
     cluster = rootCluster;

  cluster = searchNextFreeCluster(cluster);
   if(cluster == 0)
   {
      FSM::uart0.print("\r\n");
      FSM::uart0.print(" No free cluster!");
	  return 1;
   }
  getSetNextCluster(cluster, SET, EOF);   //last cluster of the file, marked EOF
   
  firstClusterHigh = (unsigned int) ((cluster & 0xffff0000) >> 16 );
  firstClusterLow = (unsigned int) ( cluster & 0x0000ffff);
  fileSize = 0;
}

k=0;

while(1)
{
   if(start)
   {
      start = 0;
      fct.startBlock = getFirstSector (cluster) + sector;
      fct.SD_readSingleBlock (startBlock);
	  i = fileSize % bytesPerSector;
	  j = sector;
   }
   else
   {
	   fct.startBlock = getFirstSector (cluster);
	  i=0;
	  j=0;
   }
   
   
   do
   {

	 data = dataString[k++];
	 //transmitByte(data);
	 fct.buffer[i++] = data;
	 fileSize++;
     
     if(i >= 512)   //though 'i' will never become greater than 512, it's kept here to avoid 
	 {				//infinite loop in case it happens to be greater than 512 due to some data corruption
	   i=0;
	   error = fct.SD_writeSingleBlock (fct.startBlock);
       j++;
	   if(j == sectorPerCluster) {j = 0; break;}
	   fct. startBlock++;
     }
   } while((data != '\n') && (k < MAX_STRING_SIZE)); //stop when newline character is found
   													 //or when string size limit reached

   if((data == '\n') || (k >= MAX_STRING_SIZE))
   {
      for(;i<512;i++)  //fill the rest of the buffer with 0x00
    	  fct.buffer[i]= 0x00;
   	  error = fct.SD_writeSingleBlock (fct.startBlock);

      break;
   } 
 
   prevCluster = cluster;

   cluster = searchNextFreeCluster(prevCluster); //look for a free cluster starting from the current cluster

   if(cluster == 0)
   {
      FSM::uart0.print("\r\n");
      FSM::uart0.print(" No free cluster!");
	  return 1;
   }

   getSetNextCluster(prevCluster, SET, cluster);
   getSetNextCluster(cluster, SET, EOF);   //last cluster of the file, marked EOF
}        

getSetFreeCluster (NEXT_FREE, SET, cluster); //update FSinfo next free cluster entry

//error = getDateTime_FAT();    //get current date & time from the RTC
//if(error) { dateFAT = 0; timeFAT = 0;}

if(appendFile)  //executes this loop if file is to be appended
{
	fct.SD_readSingleBlock (appendFileSector);
  dir = (struct dir_Structure *) &(fct.buffer)[appendFileLocation];

  dir->lastAccessDate = 0;   //date of last access ignored
//  dir->writeTime = timeFAT;  //setting new time of last write, obtained from RTC
//  dir->writeDate = dateFAT;  //setting new date of last write, obtained from RTC
  extraMemory = fileSize - dir->SfileSize;
  dir->SfileSize = fileSize;
  fct.SD_writeSingleBlock (appendFileSector);
  freeMemoryUpdate (REMOVE, extraMemory); //updating free memory count in FSinfo sector;

  
  FSM::uart0.print("\r\n");
 // FSM::uart0.print(" File appended!"));

  return 0;
}

//executes following portion when new file is created

prevCluster = rootCluster; //root cluster

while(1)
{
   firstSector = getFirstSector (prevCluster);

   for(sector = 0; sector < sectorPerCluster; sector++)
   {
	   fct.SD_readSingleBlock (firstSector + sector);
	

     for(i=0; i<bytesPerSector; i+=32)
     {
	    dir = (struct dir_Structure *) &(fct.buffer)[i];

		if(fileCreatedFlag)   //to mark last directory entry with 0x00 (empty) mark
		 { 					  //indicating end of the directory file list
		   //dir->name[0] = EMPTY;
		   //SD_writeSingleBlock (firstSector + sector);
           return 0;
         }

        if((dir->name[0] == EMPTY) || (dir->name[0] == DELETED))  //looking for an empty slot to enter file info
		{
		  for(j=0; j<11; j++)
  			dir->name[j] = fileName[j];
		  dir->attrib = ATTR_ARCHIVE;	//settting file attribute as 'archive'
		  dir->NTreserved = 0;			//always set to 0
		  dir->timeTenth = 0;			//always set to 0
	//	  dir->createTime = timeFAT; 	//setting time of file creation, obtained from RTC
		//  dir->createDate = dateFAT; 	//setting date of file creation, obtained from RTC
		  dir->lastAccessDate = 0;   	//date of last access ignored
	//	  dir->writeTime = timeFAT;  	//setting new time of last write, obtained from RTC
		//  dir->writeDate = dateFAT;  	//setting new date of last write, obtained from RTC
		  dir->firstClusterHI = firstClusterHigh;
		  dir->firstClusterLO = firstClusterLow;
		  dir->SfileSize = fileSize;

		  fct.SD_writeSingleBlock (firstSector + sector);
		  fileCreatedFlag = 1;

		  FSM::uart0.print("\r\n");
		  FSM::uart0.print("\r\n");
		  //FSM::uart0.print(" File Created! "));

		  freeMemoryUpdate (REMOVE, fileSize); //updating free memory count in FSinfo sector
	     
        }
     }
   }

   cluster = getSetNextCluster (prevCluster, GET, 0);

   if(cluster > 0x0ffffff6)
   {
      if(cluster == EOF)   //this situation will come when total files in root is multiple of (32*sectorPerCluster)
	  {  
		cluster = searchNextFreeCluster(prevCluster); //find next cluster for root directory entries
		getSetNextCluster(prevCluster, SET, cluster); //link the new cluster of root to the previous cluster
		getSetNextCluster(cluster, SET, EOF);  //set the new cluster as end of the root directory
      } 

      else
      {	
	    FSM::uart0.print("End of Cluster Chain");
	    return 1;
      }
   }
   if(cluster == 0) {
	   FSM::uart0.print("Error in getting cluster");
	   return 1;
   }
   
   prevCluster = cluster;
 }
 
 return 0;
}


//***************************************************************************
//Function: to search for the next free cluster in the root directory
//          starting from a specified cluster
//Arguments: Starting cluster
//return: the next free cluster
//****************************************************************
unsigned long FAT32::searchNextFreeCluster (unsigned long startCluster)
{
  unsigned long cluster, *value, sector;
  unsigned char i;
    
	startCluster -=  (startCluster % 128);   //to start with the first file in a FAT sector
    for(cluster =startCluster; cluster <totalClusters; cluster+=128) 
    {
      sector = unusedSectors + reservedSectorCount + ((cluster * 4) / bytesPerSector);
      fct.SD_readSingleBlock(sector);
      for(i=0; i<128; i++)
      {
       	 value = (unsigned long *) &(fct.buffer)[i*4];
         if(((*value) & 0x0fffffff) == 0)
            return(cluster+i);
      }  
    } 

 return 0;
}

//************************************************************
//Function: To convert the unsigned long value of memory into 
//          text string and send to UART
//Arguments: 1. unsigned char flag. If flag is HIGH, memory will be displayed in KBytes, else in Bytes. 
//			 2. unsigned long memory value
//return: none
//************************************************************
void FAT32::displayMemory (unsigned char flag, unsigned long memory)
{
  char memoryString[] = "              Bytes"; //19 character long string for memory display
  unsigned char i;
  for(i=12; i>0; i--) //converting freeMemory into ASCII string
  {
    if(i==5 || i==9) 
	{
	   memoryString[i-1] = ',';  
	   i--;
	}
    memoryString[i-1] = (memory % 10) | 0x30;
    memory /= 10;
	if(memory == 0) break;
  }
  if(flag == HIGH)  memoryString[13] = 'K';
  FSM::uart0.print(memoryString);
}

//********************************************************************
//Function: to delete a specified file from the root directory
//Arguments: pointer to the file name
//return: none
//********************************************************************
void FAT32::deleteFile (char *fileName)
{
  unsigned char error;

  error = convertFileName (fileName);
  if(error) return;

  findFiles (DELETE, fileName);
}

//********************************************************************
//Function: update the free memory count in the FSinfo sector. 
//			Whenever a file is deleted or created, this function will be called
//			to ADD or REMOVE clusters occupied by the file
//Arguments: #1.flag ADD or REMOVE #2.file size in Bytes
//return: none
//********************************************************************
void FAT32::freeMemoryUpdate (unsigned char flag, unsigned long size)
{
  unsigned long freeClusters;
  //convert file size into number of clusters occupied
  if((size % 512) == 0) size = size / 512;
  else size = (size / 512) +1;
  if((size % 8) == 0) size = size / 8;
  else size = (size / 8) +1;

  if(freeClusterCountUpdated)
  {
	freeClusters = getSetFreeCluster (TOTAL_FREE, GET, 0);
	if(flag == ADD)
  	   freeClusters = freeClusters + size;
	else  //when flag = REMOVE
	   freeClusters = freeClusters - size;
	getSetFreeCluster (TOTAL_FREE, SET, freeClusters);
  }
}

//******** END ****** www.dharmanitech.com *****
