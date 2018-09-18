//###########################################################################
//
// FILE:   flash.c
//
// TITLE:  DSP28335 flash驱动程序
//
//###########################################################################
//		V1.0
//		V2.0		2016.12.30
//###########################################################################
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "USERS_Flash.h"

/***************全局变量定义****************/
unsigned int *FlashStart = (unsigned int *)0x100000;//flash的首地址

/*---------------------------------------------------------------------
函数原型：Uint16 SectorErase(Uint16	SectorNum)
函数名称：Flash段擦除
函数功能：以段为单位擦除Flash，每次2K
入口参数：SectorNum---段号                    
出口参数：0---正确 1---超时 3---错误
作    者：
日    期：
----------------------------------------------------------------------*/ 
Uint16	SectorErase(Uint16 SectorNum)
{
	Uint16	i,Data;
	Uint32	TimeOut;

	*(FlashStart + 0x5555) = 0xAAAA;
	*(FlashStart + 0x2AAA) = 0x5555;
	*(FlashStart + 0x5555) = 0x8080;
	*(FlashStart + 0x5555) = 0xAAAA;
	*(FlashStart + 0x2AAA) = 0x5555;
	*(FlashStart + SectorSize * SectorNum) = 0x3030;

	i = 0;
	TimeOut = 0;
	while (i < 5)
	{
		Data = *(FlashStart + SectorSize * SectorNum);
		if (Data == 0xFFFF)     i++;
		else                    i = 0;

		if (++TimeOut > 0x1000000)      return (TimeOutErr);
	}

	for	(i = 0; i < SectorSize; i++)
	{
		Data = *(FlashStart + SectorSize * SectorNum +i);
		if (Data != 0xFFFF)             return (EraseErr);
	}

	return (EraseOK);
}

/*---------------------------------------------------------------------
函数原型：Uint16 BlockErase(Uint16 BlockrNum)
函数名称：Flash块擦除
函数功能：以块为单位擦除Flash，每次32K
入口参数：BlockNum---段号                    
出口参数：0---正确 1---超时 3---错误
作    者：
日    期：
----------------------------------------------------------------------*/		
Uint16	BlockErase(Uint16 BlockNum)
{
	Uint16	i,Data;
	Uint32	TimeOut;
			
	*(FlashStart + 0x5555) = 0xAAAA;
	*(FlashStart + 0x2AAA) = 0x5555;
	*(FlashStart + 0x5555) = 0x8080;
	*(FlashStart + 0x5555) = 0xAAAA;
	*(FlashStart + 0x2AAA) = 0x5555;
	*(FlashStart + BlockSize * BlockNum) = 0x5050;

	i = 0;
	TimeOut = 0;
	while(i<5)
	{
		Data = *(FlashStart +  BlockSize * BlockNum);
		if	(Data == 0xFFFF)	i++;
		else	i=0;
		if ( ++TimeOut>0x1000000)	
			return (TimeOutErr);
	}

	for	(i=0;i<SectorSize;i++)	
	{
		Data = *(FlashStart + BlockSize * BlockNum +i);
		if (Data !=0xFFFF)	return (EraseErr);
	}

	return  (EraseOK);															
}

/*---------------------------------------------------------------------
函数原型：Uint16 ChipErase(void)
函数名称：Flash全部擦除
函数功能：擦除Flash
入口参数：无                    
出口参数：0---正确 1---超时 3---错误
作    者：
日    期：
----------------------------------------------------------------------*/				
Uint16	ChipErase(void)
{
	Uint16	Data;
	Uint32	TimeOut,i;
					
	*(FlashStart + 0x5555) = 0xAAAA;
	*(FlashStart + 0x2AAA) = 0x5555;
	*(FlashStart + 0x5555) = 0x8080;
	*(FlashStart + 0x5555) = 0xAAAA;
	*(FlashStart + 0x2AAA) = 0x5555;
	*(FlashStart + 0x5555) = 0x1010;
		
	i = 0;
	TimeOut = 0;
	while(i<5)
	{
		Data = *(FlashStart +  0x3FFFF);
		if	(Data == 0xFFFF)	i++;
		else	i=0;
		if ( ++TimeOut>0x1000000)	
			return (TimeOutErr);
	}

	for	(i=0;i<0x40000;i++)	
	{
		Data = *(FlashStart + i);
		if (Data !=0xFFFF)	return (EraseErr);
	}

	return  (EraseOK);											
}

/*---------------------------------------------------------------------
函数原型：Uint16 FlashWrite(int *Ptr, Uint32 RomStart, Uint16 Length)
函数名称：写Flash
函数功能：将Ptr所指的Length个数据写入RomStart开始处
入口参数：Ptr---写入数据指针 RomStart---Flash起始位置  Length---数据长度                   
出口参数：0---正确 1---超时
作    者：
日    期：
----------------------------------------------------------------------*/
Uint16 FlashWrite(int *Ptr, Uint32 RomStart, Uint32 Length)
{
	Uint32	i,TimeOut;
	Uint16	Data1,Data2,j;

	for	(i=0;i<Length;i++)
	{
		*(FlashStart + 0x5555) = 0x00AA;
		*(FlashStart + 0x2AAA) = 0x0055;
		*(FlashStart + 0x5555) = 0x00A0;
		*(FlashStart + RomStart + i) = *(Ptr+i);
		TimeOut = 0;
		j=0;
		while(j<5)
		{
			Data1 = *(FlashStart + RomStart + i);
			Data2 = *(FlashStart + RomStart + i);							
			if	(Data1 == Data2)	j++;
			else	j=0;
			if ( ++TimeOut>0x1000000)	
				return (TimeOutErr);						
		}						
	}
	return  (WriteOK);				
}

/*---------------------------------------------------------------------
函数原型：Uint16 FlashRead(int *Ptr, Uint32 RomStart, Uint16 Length)
函数名称：Flash读
函数功能：将RomStart开始Length个数据写入Ptr
入口参数：Ptr---写入数据指针 RomStart---Flash起始位置  Length---数据长度                   
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void FlashRead(int *Ptr, Uint32  RomStart,	Uint32	Length)
{
	Uint32	i;

	for	(i=0;i<Length;i++)
	{
		*(Ptr+i) =  *(FlashStart + RomStart +i);
	}	
}




							
//=========================================NO MORE==============================

