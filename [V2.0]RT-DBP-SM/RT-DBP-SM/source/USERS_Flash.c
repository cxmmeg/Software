//###########################################################################
//
// FILE:   flash.c
//
// TITLE:  DSP28335 flash��������
//
//###########################################################################
//		V1.0
//		V2.0		2016.12.30
//###########################################################################
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "USERS_Flash.h"

/***************ȫ�ֱ�������****************/
unsigned int *FlashStart = (unsigned int *)0x100000;//flash���׵�ַ

/*---------------------------------------------------------------------
����ԭ�ͣ�Uint16 SectorErase(Uint16	SectorNum)
�������ƣ�Flash�β���
�������ܣ��Զ�Ϊ��λ����Flash��ÿ��2K
��ڲ�����SectorNum---�κ�                    
���ڲ�����0---��ȷ 1---��ʱ 3---����
��    �ߣ�
��    �ڣ�
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
����ԭ�ͣ�Uint16 BlockErase(Uint16 BlockrNum)
�������ƣ�Flash�����
�������ܣ��Կ�Ϊ��λ����Flash��ÿ��32K
��ڲ�����BlockNum---�κ�                    
���ڲ�����0---��ȷ 1---��ʱ 3---����
��    �ߣ�
��    �ڣ�
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
����ԭ�ͣ�Uint16 ChipErase(void)
�������ƣ�Flashȫ������
�������ܣ�����Flash
��ڲ�������                    
���ڲ�����0---��ȷ 1---��ʱ 3---����
��    �ߣ�
��    �ڣ�
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
����ԭ�ͣ�Uint16 FlashWrite(int *Ptr, Uint32 RomStart, Uint16 Length)
�������ƣ�дFlash
�������ܣ���Ptr��ָ��Length������д��RomStart��ʼ��
��ڲ�����Ptr---д������ָ�� RomStart---Flash��ʼλ��  Length---���ݳ���                   
���ڲ�����0---��ȷ 1---��ʱ
��    �ߣ�
��    �ڣ�
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
����ԭ�ͣ�Uint16 FlashRead(int *Ptr, Uint32 RomStart, Uint16 Length)
�������ƣ�Flash��
�������ܣ���RomStart��ʼLength������д��Ptr
��ڲ�����Ptr---д������ָ�� RomStart---Flash��ʼλ��  Length---���ݳ���                   
���ڲ�������
��    �ߣ�
��    �ڣ�
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

