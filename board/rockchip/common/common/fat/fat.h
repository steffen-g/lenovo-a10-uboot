/********************************************************************************
*********************************************************************************
			COPYRIGHT (c)   2004 BY ROCK-CHIP FUZHOU
				--  ALL RIGHTS RESERVED  --

File Name:	    fat.h
Author:		    XUESHAN LIN
Created:		1st JUL 2007
Modified:
Revision:		1.00
********************************************************************************
********************************************************************************/

#ifndef _FAT_H
#define _FAT_H

//1可配置参数
#define 	FAT_CACHE_NUM               10

//1常量定义
#undef      FAT12
#undef      FAT16
#undef      FAT32
#define	    FAT12	0
#define	    FAT16	1
#define	    FAT32	2


//1结构定义
/***************************************************************************
磁盘格式化参数
***************************************************************************/
typedef struct _DskSizeToSecPerClus
{
        uint32 	DiskSize;		/*磁盘大小(扇区)*/
        uint8 	SecPerClus;		/*每簇扇区数*/
}DskSizeToSecPerClus;


/***************************************************************************
文件系统CHCHE
***************************************************************************/
typedef struct _CACHE
{
	uint8	Buf[512];   //数据BUF
	uint8	Flag;		//读写标记
	uint8	Valid;		//有效标记
	uint8   Drive;      //所属驱动器
	uint16  Count;      //扇区访问记录
	uint32	Sec;        //扇区地址
}CACHE, *pCACHE;


//1常量定义
/* 簇类型 */
#define 	EMPTY_CLUS              0
#define 	EMPTY_CLUS_1           	1
#define 	BAD_CLUS                0x0ffffff7L
#define 	EOF_CLUS_1              0x0ffffff8L
#define 	EOF_CLUS_END            0x0fffffffL


//1全局变量
#undef	EXT
#ifdef	IN_FAT
		#define	EXT
#else
		#define	EXT		extern
#endif		
EXT		CACHE           FatCache[FAT_CACHE_NUM];

//1函数原型声明
extern	void 	Format(uint8 Drive, uint8 type);
extern	void 	WriteDBRSector(uint8 Drive, uint8 type);
extern	void 	WriteRootDir(uint8 Drive, uint8 type);
extern	void 	WriteFAT(uint8 Drive, uint8 type);
extern 	void    CacheInit(void);
extern 	uint8   ReadCache(uint8 Drive, uint32 Index, pCACHE *cache, uint8 type);
extern 	uint8   CacheWriteBack(pCACHE cache);
extern 	uint8   CacheWriteBackAll(void);
extern	uint8	CheckFileSystem(uint8 Drive);
extern	void	GetBootInfo(uint8 Drive, uint32 PBRSector, uint8 *DbrBuf);
extern	int16 	lg2(uint16 arg);
extern	bool 	CheckFatBootSector(uint8 *DbrBuf);
extern	uint32 	CheckMbr(uint8 *DbrBuf);
extern	void 	CopyFat(uint8 Drive, uint8 FatNum);
extern	void 	GetFATPosition(uint8 Drive, uint32 cluster, uint32 *FATSecNum, uint16 *FATEntOffset);
extern	bool 	CheckBadCluster(uint32 checkCluster);
extern	uint32 	GetFirstSectorofCluster(uint32 cluster);

extern	uint32  GetTotalMem(uint8 Drive);
extern 	uint32 	GetFreeMem(uint8 Drive);
extern 	void 	UpdataFreeMem(uint8 Drive, int32 size);
extern 	uint32 	FATGetNextClus(uint8 Drive, uint32 Index, uint32 Count);
extern 	void 	FATSetNextClus(uint8 Drive, uint32 Index, uint32 Next);
extern 	uint32 	FATAddClus(uint8 Drive, uint32 Index);
extern 	void 	FATDelClusChain(uint8 Drive, uint32 Index);


//1表格定义

#ifdef IN_FAT
DskSizeToSecPerClus DskTableFAT16[]=
{
	{8400, 0},			//磁盘容量最大为4.1MB, SecPerClus=0表示这是个错误
	{32680, 2},			//磁盘容量最大为16MB, 1KB clus
	{262144, 4},		//磁盘容量最大为128MB, 2KB clus
	{524288, 8},		//磁盘容量最大为256MB, 4KB clus
	{1048576, 16},		//磁盘容量最大为512MB, 8KB clus
	{2097152, 32},		//磁盘容量最大为1GB, 16KB clus
	{4194304, 64},		//磁盘容量最大为2GB, 32KB clus
	{-1, 0}				//磁盘容量超过2GB, SecPerClus=0表示这是个错误
};
DskSizeToSecPerClus DskTableFAT32[]=
{
	{0x10428, 0},		//磁盘容量最大为32MB, SecPerClus=0表示这是个错误
	{0x82000, 1},		//磁盘容量最大为260MB, 0.5KB clus
	{0x1000000, 8},		//磁盘容量最大为8GB, 4KB clus
	{0x2000000, 16},	//磁盘容量最大为16GB, 8KB clus
	{0x4000000, 32},	//磁盘容量最大为32GB, 16KB clus
	{-1, 64}			//磁盘容量超过32GB, 32KB clus
};

#if 0
uint8 defaultBootSector[512] =
{
	//2 0x00-0x02:跳转指令,跳到操作系统引导程序代码
	0xEB,0x3E,0x90,

	//2 0x03-0x0a:OEM ID,操作系统名称和版本
	'M','S','W','I','N','4','.','1',

	//2 0x0b-0x23:BPB
	0x00,0x02,					//3 0x0b:每扇区的字节数
	0x01,						//3 0x0d:每簇扇区数
	0x01,0x00,					//3 0x0e:保留扇区数
	0x02,						//3 0x10:FAT备份数
	0x00,0x02,					//3 0x11:根目录项数
	0x00,0x00,					//3 0x13:该分区上的扇区数
	0xF8,						//3 0x15:媒体描述符(0xf8=硬盘)
	0x3F,0x00,					//3 0x16:每FAT占用的扇区数
	0x3F,0x00,					//3 0x18:每磁道扇区数
	0xFF,0x00,					//3 0x1a:磁头数
	0x00,0x00,0x00,0x00,			//3 0x1c:隐藏扇区数
	0x00,0x00,0x00,0x00,			//3 0x20:总扇区数

	//2 0x24-0x3d:扩展BPB
	0x00,						//3 0x24:物理软盘
	0x00,						//3 0x25:保留
	0x29,						//3 0x26:扩展引导标签,必须是0x28或0x29
	0x77,0xE2,0xCB,0x94,			//3 0x27:卷序号(格式化时产生随机数)
	'N','O','N','A','M','E',' ',' ',' ',' ',' ',	//3 0x2b:卷标名(11B)
	'F','A','T','1','6',' ',' ',' ',			//3 0x36:文件系统类型(8B)

	//2 0x3e-0x01fd:引导程序代码
	0xf1,0x7d,0xfa,0x33,0xc9,0x8e,0xd1,0xbc,
	0xfc,0x7b,0x16,0x07,0xbd,0x78,0x00,0xc5,
	0x76,0x00,0x1e,0x56,0x16,0x55,0xbf,0x22,
	0x05,0x89,0x7E,0x00,0x89,0x4e,0x02,0xb1,
	0x0b,0xfc,0x1F,0xBD,0x00,0x7C,0xC6,0x45,
	0xFE,0x0F,0x38,0x4E,0x24,0x7D,0x20,0x8B,
	0xC1,0x99,0xE8,0x7E,0x01,0x83,0xEB,0x3A,
	0x66,0xA1,0x1C,0x7C,0x66,0x3B,0x07,0x8A,
	0x57,0xFC,0x75,0x06,0x80,0xCA,0x02,0x88,
	0x56,0x02,0x80,0xC3,0x10,0x73,0xED,0x33,
	0xC9,0xFE,0x06,0xD8,0x7D,0x8A,0x46,0x10,
	0x98,0xF7,0x66,0x16,0x03,0x46,0x1C,0x13,
	0x56,0x1E,0x03,0x46,0x0E,0x13,0xD1,0x8B,
	0x76,0x11,0x60,0x89,0x46,0xFC,0x89,0x56,
	0xFE,0xB8,0x20,0x00,0xF7,0xE6,0x8B,0x5E,
	0x0B,0x03,0xC3,0x48,0xF7,0xF3,0x01,0x46,
	0xFC,0x11,0x4E,0xFE,0x61,0xBF,0x00,0x07,
	0xE8,0x28,0x01,0x72,0x3E,0x38,0x2D,0x74,
	0x17,0x60,0xB1,0x0B,0xBE,0xD8,0x7D,0xF3,
	0xA6,0x61,0x74,0x3D,0x4E,0x74,0x09,0x83,
	0xC7,0x20,0x3B,0xFB,0x72,0xE7,0xEB,0xDD,
	0xFE,0x0E,0xD8,0x7D,0x7B,0xA7,0xBE,0x7F,
	0x7D,0xAC,0x98,0x03,0xF0,0xAC,0x98,0x40,
	0x74,0x0C,0x48,0x74,0x13,0xB4,0x0E,0xBB,
	0x07,0x00,0xCD,0x10,0xEB,0xEF,0xBE,0x82,
	0x7D,0xEB,0xE6,0xBE,0x80,0x7D,0xEB,0xE1,
	0xCD,0x16,0x5E,0x1F,0x66,0x8F,0x04,0xCD,
	0x19,0xBE,0x81,0x7D,0x8B,0x7D,0x1A,0x8D,
	0x45,0xFE,0x8A,0x4E,0x0D,0xF7,0xE1,0x03,
	0x46,0xFC,0x13,0x56,0xFE,0xB1,0x04,0xE8,
	0xC2,0x00,0x72,0xD7,0xEA,0x00,0x02,0x70,
	0x00,0x52,0x50,0x06,0x53,0x6A,0x01,0x6A,
	0x10,0x91,0x8B,0x46,0x18,0xA2,0x26,0x05,
	0x96,0x92,0x33,0xD2,0xF7,0xF6,0x91,0xF7,
	0xF6,0x42,0x87,0xCA,0xF7,0x76,0x1A,0x8A,
	0xF2,0x8A,0xE8,0xC0,0xCC,0x02,0x0A,0xCC,
	0xB8,0x01,0x02,0x80,0x7E,0x02,0x0E,0x75,
	0x04,0xB4,0x42,0x8B,0xF4,0x8A,0x56,0x24,
	0xCD,0x13,0x61,0x61,0x72,0x0A,0x40,0x75,
	0x01,0x42,0x03,0x5E,0x0B,0x49,0x75,0x77,
	0xC3,0x03,0x18,0x01,0x27,0x0D,0x0A,0x49,
	0x6E,0x76,0x61,0x6C,0x69,0x64,0x20,0x73,
	0x79,0x73,0x74,0x65,0x6D,0x20,0x64,0x69,
	0x73,0x6B,0xFF,0x0D,0x0A,0x44,0x69,0x73,
	0x6B,0x20,0x49,0x2F,0x4F,0x20,0x65,0x72,
	0x72,0x6F,0x72,0xFF,0x0D,0x0A,0x52,0x65,
	0x70,0x6C,0x61,0x63,0x65,0x20,0x74,0x68,
	0x65,0x20,0x64,0x69,0x73,0x6B,0x2C,0x20,
	0x61,0x6E,0x64,0x20,0x74,0x68,0x65,0x6E,
	0x20,0x70,0x72,0x65,0x73,0x73,0x20,0x61,
	0x6E,0x79,0x20,0x6B,0x65,0x79,0x0D,0x0A,
	0x00,0x00,0x49,0x4F,0x20,0x20,0x20,0x20,
	0x20,0x20,0x53,0x59,0x53,0x4D,0x53,0x44,
	0x4F,0x53,0x20,0x20,0x20,0x53,0x59,0x53,
	0x7F,0x01,0x00,0x41,0xBB,0x00,0x07,0x60,
	0x66,0x6A,0x00,0xE9,0x3B,0xFF,0x00,0x00,

	//2 0x01fe-0x01ff:有效结束标志
	0x55,0xAA
};
#endif
#else
	extern	uint8 defaultBootSector[512];
	extern	DskSizeToSecPerClus DskTableFAT16[];
    extern	DskSizeToSecPerClus DskTableFAT32[];
#endif
#endif

