#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include <string.h>

#pragma DATA_SECTION(ExRamCtrl,".ExRamCtrl")  	// 定义外部存储数据的控制数组
struct ExRamCtrl ExRamCtrl[80];

#pragma DATA_SECTION(EmgcRamCtrl,".ExRamCtrl")  // 定义外部存储数据的控制数组
struct ExRamCtrl EmgcRamCtrl[128];

#pragma DATA_SECTION(ExRamData,".ExRamData")  	// 定义外部存储数组；80KByte
int ExRamData[81920];

#pragma DATA_SECTION(EmRamData,".EmRamData")  	// 定义外部存储数组；128KByte
int EmRamData[131072];

#pragma DATA_SECTION(ExRamReg,".SciData")
struct SAVE_EXRAM_REGS ExRamReg;

#pragma DATA_SECTION(EmgcRamReg,".SciData")
struct SAVE_EXRAM_REGS EmgcRamReg;

extern union  FLAG_SYSTEM_RUNERR 		FlagSysRunErr;
extern int16 *EmDataAddrArray[8];
/*---------------------------------------------------------------------
函数原型：void InitExRam(void)
函数名称：初始化外部Ram
函数功能：将ExRam初始化为0
入口参数：
出口参数：
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void InitExRam(void)
{
	Uint16 i=0;

	memset((int *)0x180000, 0x00, sizeof(int)*1024*256);
	memset(&ExRamReg, 0x00, sizeof(ExRamReg));
	memset(&EmgcRamReg, 0x00, sizeof(EmgcRamReg));

	for(i = 0; i < 80; i++)
	{
		ExRamCtrl[i].pt_base_addr = (int *)(0x184000 + (int32)i * 0x400);
	}

	for(i = 0; i < 80; i++)
	{
		EmgcRamCtrl[i].pt_base_addr = (int *)(0x1A0000 + (int32)i * 0x400);
	}

	EmgcRamReg.FlagBlockSave = 1;
}

/*---------------------------------------------------------------------
函数原型：void CopyData(Uint16 lenth, int *ReadBuf, uchar *WriteBuf)
函数名称：从外部RAM读数据
函数功能：从外部RAM的缓冲区read_buf 读取数据复制到 write_buf
入口参数：	Uint16 lenth, 		数据长度
			int *ReadBuf, 		源地址
			uchar *WriteBuf	目的地址
出口参数：无
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void CopyData(Uint16 Lenth, int *ReadBuf, uchar *WriteBuf)
{
	Uint16 i = 0;

	for (i = 0; i < Lenth; i++)
	{
		WriteBuf[i*2] = (uchar)(ReadBuf[i]>>8);
		WriteBuf[i*2+1] = (uchar)ReadBuf[i];
	}
}

/*---------------------------------------------------------------------
函数原型：Uint16 T1VarSave(void)
数名称：存储上位机指定数据
函数功能：
入口参数：
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
#pragma CODE_SECTION(T1VarSave, "ramcode")

Uint16 T1VarSave(void)
{
	Uint16  i, flag;

	if(1 == ExRamReg.FlagBlockSave)
	{
		ExRamReg.FlagDataSaveFinish = 0;
		if(ExRamReg.CountRam < (ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->buf_len * 1024)
		{
			flag = 0;
			for(i = ExRamReg.CountBlock; i > 0; i--)
			{
				*((ExRamData + ExRamReg.ArrayBlock[i] * 0x400) + ExRamReg.CountRam) = *(ExRamCtrl + ExRamReg.ArrayBlock[i])->pt_addr;
			}
		}
		else
		{
			flag = 1;
			ExRamReg.CountRam	= 0;
			ExRamReg.FlagBlockSave	= 0;
			ExRamReg.CountBlock = 0;
			ExRamReg.FlagDataSaveFinish = 1;
		}
		ExRamReg.CountRam++;
	}
	else
	{
		flag = 0;
		ExRamReg.CountRam = 0;
	}

	return flag;
}

/*---------------------------------------------------------------------
函数原型：void T1EmergencySave(Uint16 FlagExFinsh)
函数名称：紧急状态保存数据
函数功能：当硬件保护时，保存重要参数信息，便于故障分析
入口参数：FlagExFinsh--停止保存数据
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
#pragma CODE_SECTION(T1EmergencySave, "ramcode")

void T1EmergencySave(Uint16 FlagExFinsh)
{
	Uint16  i;

	if (1 == EmgcRamReg.FlagBlockSave)
	{
		EmgcRamReg.FlagDataSaveFinish = 0;
		if(EmgcRamReg.CountRam < (EmgcRamCtrl + EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock])->buf_len * 1024)
		{
			for(i = EmgcRamReg.CountBlock; i > 0; i--)
			{
				*((EmRamData + (Uint32)EmgcRamReg.ArrayBlock[i] * 0x400) + EmgcRamReg.CountRam) = *(EmgcRamCtrl + EmgcRamReg.ArrayBlock[i])->pt_addr;
			}
			EmgcRamReg.CountRam++;
		}
		else
		{
			if (1 == FlagExFinsh)
			{
				if (EmgcRamReg.CountRam >= 16384)		// 16384 -- 16K
				{
					EmgcRamReg.CountRam = 0;
					EmgcRamReg.FlagBlockSave = 0;
					EmgcRamReg.CountBlock = 0;
					EmgcRamReg.FlagDataSaveFinish = 1;
				}
			}
			else
			{
				EmgcRamReg.CountRam = 0;
			}
		}
	}
	else
	{
		EmgcRamReg.CountRam = 0;
	}
}

/*---------------------------------------------------------------------
函数原型：Uint16 EmDataWriteToFlash(void)
函数名称：将保存至内存的重要数据写进Flash
函数功能：
入口参数：无
出口参数：WriteOK：1 -- 烧写成功
作    者：
日    期：
----------------------------------------------------------------------*/
Uint16 EmDataWriteToFlash(void)
{
	int *PointEm, *PointPara;

	PointEm = (int16 *)&EmRamData;							//获取参数表的首地址；
	PointPara =  (int16 *)&ParaTable;

	if ((1 == FlagSysRunErr.bit.FlagPDP) && (1 == EmgcRamReg.FlagDataSaveFinish))
	{
		PieCtrlRegs.PIEIER3.bit.INTx1 = 0;
/*		BlockErase(0x01);		// 每次擦出32K  0x8000  - 0x10000  问题:只擦除了前32K 为什么？
		DelayUs(100);
		BlockErase(0x02);		// 每次擦出32K  0x10000 - 0x18000
		DelayUs(100);
		BlockErase(0x03);		// 每次擦出32K  0x18000 - 0x20000
		DelayUs(100);
		BlockErase(0x04);		// 每次擦出32K  0x20000 - 0x28000
		DelayUs(100);
*/
		ChipErase();
		DelayUs(100);

		FlashWrite(PointPara, 0x00, sizeof(ParaTable));     // 整个参数表写入flash；
		DelayUs(100);

		FlashWrite(PointEm,0x8000,sizeof(EmRamData));		// 前32K空间存储参数表
		DelayUs(100);

		PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

		return (1);
	}
	else
	{
		return (0);
	}
}

/*---------------------------------------------------------------------
函数原型：void ReadEmDataFromFlash(void)
函数名称：从FLASH中读取上一次保存的重要数据
函数功能：上电时，读取上一次故障数据
入口参数：无
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void ReadEmDataFromFlash(void)
{
	int *Point, *EmAddrPoint, *ParaPoint;
	int i;

	Point = (int *)&EmRamData;					    //获取重要数据保存数组的首地址；
	FlashRead(Point,0x8000,sizeof(EmRamData));		//从flash读取保存的重要数据；

	ParaPoint = (int *)&ParaTable;                      //参数表首地址指针；
    EmAddrPoint = (int *)ParaTable.EmergencyReg.uEmData0Addr;
    for (i = 0; i < 8; i++, EmAddrPoint++)
    {
        EmDataAddrArray[i] = EmAddrPoint;

        EmgcRamReg.ArrayBlock[i] = i * 16;
        (EmgcRamCtrl + EmgcRamReg.ArrayBlock[i])->pt_addr = ParaPoint + (int16)(int32)EmDataAddrArray[i];                            // 指定存储数据地址
        (EmgcRamCtrl + EmgcRamReg.ArrayBlock[i])->buf_len = 16;                                                             // 地址存储数据长度 固定16K
        (EmgcRamCtrl + EmgcRamReg.ArrayBlock[i])->pt_base_addr = (int *)(EmRamData + EmgcRamReg.ArrayBlock[i] * 0x400);   // 指定外部存储区域
    }
}
