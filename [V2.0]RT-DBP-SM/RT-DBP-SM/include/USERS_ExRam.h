#ifndef  _USERS_EXRAM_H_
#define  _USERS_EXRAM_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

extern int ExRamData[81920];
extern int EmRamData[131072];

extern struct ExRamCtrl ExRamCtrl[];
extern struct ExRamCtrl EmgcRamCtrl[];

extern struct SAVE_EXRAM_REGS EmgcRamReg;
extern struct SAVE_EXRAM_REGS ExRamReg;

struct SAVE_EXRAM_REGS
{
	Uint16 FlagStart;
	Uint16 FlagParaSave;
	Uint16 FlagBlockSave;
	Uint16 FlagDataSaveFinish;
	Uint16 CountPara;
	Uint16 CountBlock;
	Uint16 CountRam;
	Uint16 ArrayBlock[20];
};

struct ExRamCtrl
{
	Uint16 WRen;			//读写使能
	Uint16 buf_len;			//缓存区长度，1：1K，2：2K；
	Uint16 addr_count;
	Uint16 buf_state;		//缓存去是否存满
	int *pt_base_addr;		//缓存区的地址
	int *pt_addr;			//缓存区的地址
	int *pt_data;			//数据的指针
};

/*---------------------------------------------------------------------
函数原型：Uint16 T1VarSave(void)
数名称：存储上位机指定数据
函数功能：
入口参数：
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
Uint16 	T1VarSave(void);

/*---------------------------------------------------------------------
函数原型：void T1EmergencySave(Uint16 ExFinshSaveFlag)
函数名称：紧急状态保存数据
函数功能：当硬件保护时，保存重要参数信息，便于故障分析
入口参数：ExFinshSaveFlag--停止保存数据
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void 	T1EmergencySave(Uint16 ExFinshSaveFlag);

/*---------------------------------------------------------------------
函数原型：void InitExRam(void)
函数名称：初始化外部Ram
函数功能：将ExRam初始化为0
入口参数：
出口参数：
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void InitExRam(void);

/*---------------------------------------------------------------------
函数原型：void CopyData(Uint16 lenth, int *read_buf, uchar *write_buf)
函数名称：从外部RAM读数据
函数功能：从外部RAM的缓冲区read_buf 读取数据复制到 write_buf
入口参数：	Uint16 lenth, 		数据长度
			int *read_buf, 		源地址
			uchar *write_buf	目的地址
出口参数：无
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void CopyData(Uint16 lenth, int *read_buf, uchar *write_buf);

/*---------------------------------------------------------------------
函数原型：Uint16 EmDataWriteToFlash(void)
函数名称：将保存至内存的重要数据写进Flash
函数功能：
入口参数：无
出口参数：WriteOK：1 -- 烧写成功
作    者：
日    期：
----------------------------------------------------------------------*/
Uint16 EmDataWriteToFlash(void);

/*---------------------------------------------------------------------
函数原型：void ReadEmDataFromFlash(void)
函数名称：从FLASH中读取上一次保存的重要数据
函数功能：上电时，读取上一次故障数据
入口参数：无
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void ReadEmDataFromFlash(void);


#endif
