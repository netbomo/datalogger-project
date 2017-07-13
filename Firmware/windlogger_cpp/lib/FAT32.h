//********************************************************
// **** ROUTINES FOR FAT32 IMPLEMATATION OF SD CARD *****
//********************************************************
//Controller		: ATmega32 (Clock: 8 Mhz-internal)
//Compiler			: AVR-GCC (winAVR with AVRStudio-4)
//Project Version	: DL_1.0
//Author			: CC Dharmani, Chennai (India)
//			  		  www.dharmanitech.com
//Date				: 10 May 2011
//*********************************************************


#include "../lib/fct_SDCard.h"


#ifndef _FAT32_H_
#define _FAT32_H_

//Structure to access Master Boot Record for getting info about partioions
struct MBRinfo_Structure{
unsigned char	nothing[446];		//ignore, placed here to fill the gap in the structure
unsigned char	partitionData[64];	//partition records (16x4)
unsigned int	signature;		//0xaa55
};

//Structure to access info of the first partioion of the disk 
struct partitionInfo_Structure{ 				
unsigned char	status;				//0x80 - active partition
unsigned char 	headStart;			//starting head
unsigned int	cylSectStart;		//starting cylinder and sector
unsigned char	type;				//partition type 
unsigned char	headEnd;			//ending head of the partition
unsigned int	cylSectEnd;			//ending cylinder and sector
unsigned long	firstSector;		//total sectors between MBR & the first sector of the partition
unsigned long	sectorsTotal;		//size of this partition in sectors
};

//Structure to access boot sector data
struct BS_Structure{
unsigned char SjumpBoot[3]; //default: 0x009000EB
unsigned char SOEMName[8];
unsigned int SbytesPerSector; //deafault: 512
unsigned char SsectorPerCluster;
unsigned int SreservedSectorCount;
unsigned char SnumberofFATs;
unsigned int SrootEntryCount;
unsigned int StotalSectors_F16; //must be 0 for FAT32
unsigned char SmediaType;
unsigned int SFATsize_F16; //must be 0 for FAT32
unsigned int SsectorsPerTrack;
unsigned int SnumberofHeads;
unsigned long ShiddenSectors;
unsigned long StotalSectors_F32;
unsigned long SFATsize_F32; //count of sectors occupied by one FAT
unsigned int SextFlags;
unsigned int SFSversion; //0x0000 (defines version 0.0)
unsigned long SrootCluster; //first cluster of root directory (=2)
unsigned int SFSinfo; //sector number of FSinfo structure (=1)
unsigned int SBackupBootSector;
unsigned char Sreserved[12];
unsigned char SdriveNumber;
unsigned char Sreserved1;
unsigned char SbootSignature;
unsigned long SvolumeID;
unsigned char SvolumeLabel[11]; //"NO NAME "
unsigned char SfileSystemType[8]; //"FAT32"
unsigned char SbootData[420];
unsigned int SbootEndSignature; //0xaa55
};


//Structure to access FSinfo sector data
struct FSInfo_Structure
{
unsigned long leadSignature; //0x41615252
unsigned char reserved1[480];
unsigned long structureSignature; //0x61417272
unsigned long freeClusterCount; //initial: 0xffffffff
unsigned long nextFreeCluster; //initial: 0xffffffff
unsigned char reserved2[12];
unsigned long trailSignature; //0xaa550000
};

//Structure to access Directory Entry in the FAT
struct dir_Structure{
unsigned char name[11];
unsigned char attrib; //file attributes
unsigned char NTreserved; //always 0
unsigned char timeTenth; //tenths of seconds, set to 0 here
unsigned int createTime; //time file was created
unsigned int createDate; //date file was created
unsigned int lastAccessDate;
unsigned int firstClusterHI; //higher word of the first cluster number
unsigned int writeTime; //time of last write
unsigned int writeDate; //date of last write
unsigned int firstClusterLO; //lower word of the first cluster number
unsigned long SfileSize; //size of file in bytes
};

class FAT32 {

public:
	FAT32();

	~FAT32();

	void init();


	void set_cardType(unsigned char val);

	//************* functions *************
	unsigned char getBootSectorData (void);
	unsigned long getFirstSector(unsigned long clusterNumber);
	unsigned long getSetFreeCluster(unsigned char totOrNext, unsigned char get_set, unsigned long FSEntry);
	struct dir_Structure* findFiles (unsigned char flag,char *fileName);
	unsigned long getSetNextCluster (unsigned long clusterNumber,unsigned char get_set,unsigned long clusterEntry);
	unsigned char readFile (unsigned char flag, char *fileName);
	unsigned char convertFileName (char *fileName);
	unsigned char writeFile (char *fileName, char *dataString);
	void appendFile (void);
	unsigned long searchNextFreeCluster (unsigned long startCluster);
	void displayMemory (unsigned char flag, unsigned long memory);
	void deleteFile (char *fileName);
	void freeMemoryUpdate (unsigned char flag, unsigned long size);


	//************* external variables *************
	volatile unsigned long firstDataSector, rootCluster, totalClusters;
	volatile unsigned int  bytesPerSector, sectorPerCluster, reservedSectorCount;
	unsigned long unusedSectors, appendFileSector, appendFileLocation, fileSize, appendStartCluster;

	//global flag to keep track of free cluster count updating in FSinfo sector
	unsigned char freeClusterCountUpdated;


	//Attribute definitions for file/directory
	const unsigned char ATTR_READ_ONLY   = 0x01;
	const unsigned char ATTR_HIDDEN      = 0x02;
	const unsigned char ATTR_SYSTEM      = 0x04;
	const unsigned char ATTR_VOLUME_ID   = 0x08;
	const unsigned char ATTR_DIRECTORY   = 0x10;
	const unsigned char ATTR_ARCHIVE     = 0x20;
	const unsigned char ATTR_LONG_NAME   = 0x0f;


	const unsigned char DIR_ENTRY_SIZE   =  0x32;
	const unsigned char EMPTY            =  0x00;
	const unsigned char DELETED          =  0xe5;
	const unsigned char GET     =0;
	const unsigned char SET     =1;
	const unsigned char READ	=0;
	const unsigned char VERIFY  =1;
	const unsigned char ADD		=0;
	const unsigned char REMOVE	=1;
	const unsigned char LOW		=0;
	const unsigned char HIGH	=1	;
	const unsigned char TOTAL_FREE  = 1;
	const unsigned char NEXT_FREE   = 2;
	const unsigned char GET_LIST    = 0;
	const unsigned char GET_FILE    = 1;
	const unsigned char DELETE		= 2;
	const unsigned long int EOF =	0x0fffffff;


	const unsigned char MAX_STRING_SIZE	=	100;	 //defining the maximum size of the dataString

protected:
	fct_SDCard fct;

};


#endif
