//###########################################################################
//
// FILE:   flash.h
//
// TITLE:  DSP28335 flash驱动程序头文件
//
//###########################################################################
//		V1.0
//		V2.0		2016.12.30
//###########################################################################
#ifndef _FLASH_H_
#define _FLASH_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

/****************宏定义*****************/
//#define 	SST_ID			0x00BF         /* SST Manufacturer's ID code   */
//#define 	SST_39VF800A	0x2780         /* SST39VF800/SST39VF800A device code */
#define		TimeOutErr		 1
#define		VerifyErr		 2
#define		WriteOK			 0
#define		EraseErr		 3
#define		EraseOK			 0
#define		SectorSize		 0x800
#define		BlockSize		 0x8000

/****************函数声明*******************/
/*---------------------------------------------------------------------
函数原型：Uint16 SectorErase(Uint16	SectorNum)
函数名称：Flash段擦除
函数功能：以段为单位擦除Flash
入口参数：SectorNum---段号
出口参数：0---正确 1---超时 3---错误
作    者：
日    期：
----------------------------------------------------------------------*/
Uint16	SectorErase(Uint16 SectorNum);


/*---------------------------------------------------------------------
函数原型：Uint16 BlockErase(Uint16 BlockrNum)
函数名称：Flash块擦除
函数功能：以块为单位擦除Flash
入口参数：BlockNum---段号
出口参数：0---正确 1---超时 3---错误
作    者：
日    期：
----------------------------------------------------------------------*/
Uint16	BlockErase(Uint16 BlockNum);


/*---------------------------------------------------------------------
函数原型：Uint16 ChipErase(void)
函数名称：Flash全部擦除
函数功能：擦除Flash
入口参数：无
出口参数：0---正确 1---超时 3---错误
作    者：
日    期：
----------------------------------------------------------------------*/
Uint16	ChipErase(void);


/*---------------------------------------------------------------------
函数原型：Uint16 FlashWrite(int *Ptr, Uint32 RomStart, Uint16 Length)
函数名称：写Flash
函数功能：将Ptr所指的Length个数据写入RomStart开始处
入口参数：Ptr---写入数据指针 RomStart---Flash起始位置  Length---数据长度
出口参数：0---正确 1---超时
作    者：
日    期：
----------------------------------------------------------------------*/
Uint16 FlashWrite(int *Ptr,Uint32 RomStart,Uint32 Length);


/*---------------------------------------------------------------------
函数原型：Uint16 FlashRead(int *Ptr, Uint32 RomStart, Uint16 Length)
函数名称：Flash读
函数功能：将RomStart开始Length个数据写入Ptr
入口参数：Ptr---写入数据指针 RomStart---Flash起始位置  Length---数据长度
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void FlashRead(int *Ptr, Uint32  RomStart,	Uint32	Length);


#endif

