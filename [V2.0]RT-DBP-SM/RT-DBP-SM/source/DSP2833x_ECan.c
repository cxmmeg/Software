// TI File $Revision: /main/6 $
// Checkin $Date: May 7, 2007   16:26:05 $
//###########################################################################
//
// FILE:	DSP2833x_ECan.c
//
// TITLE:	DSP2833x Enhanced CAN Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP28 Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP28 Examples Include File

/***************全局变量定义****************/
struct ECAN_REGS ECanbShadow;

extern union 	FLAG_GPIOIN   	 		FlagGpioIn;
extern union  	FLAG_SYSTEM_INITERR 	FlagSysInitErr;
extern union  	FLAG_SYSTEM_RUNERR 		FlagSysRunErr;
extern union  	FLAG_SYSTEM_ALARM		FlagSysAlarm;
extern union  	FLAG_SYSTEM				FlagSystem;
extern union  	STATE_SYSTEM			StateSystem;
extern struct 	COUNT_SYSTEM			CountSystem;
extern struct   REGS_SYSTEM             RegSystem;

extern struct 	CANFRAME  				CanTxPack1;		//发送数据包
extern struct 	CANFRAME  				CanTxPack2;		//发送数据包
extern struct 	CANFRAME  				CanTxPack3;		//发送数据包
extern struct 	CANFRAME  				CanRxPack;		//接收到的数据包

void InitCANDATA(void);

void InitECanaGpio(Uint16 ch)
{
	EALLOW;
	if(0 == ch)
	{
		GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	    // Enable pull-up for GPIO18 (CANRXA)
		GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	    // Enable pull-up for GPIO19 (CANTXA)
		GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;   // Asynch qual for GPIO18 (CANRXA)
		GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;	// Configure GPIO18 for CANRXA operation
		GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;	// Configure GPIO19 for CANTXA operation
	}
	else if(1 == ch)
	{
		GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;	    // Enable pull-up for GPIO30 (CANRXA)
		GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;	    // Enable pull-up for GPIO31 (CANTXA)
	    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)
		GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// Configure GPIO30 for CANRXA operation
		GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// Configure GPIO31 for CANTXA operation
	}
    EDIS;
}

void InitECanbGpio(Uint16 ch)
{
	EALLOW;
	if(0 == ch)
	{
		GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;	  // Enable pull-up for GPIO8  (CANTXB)
		GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;	  // Enable pull-up for GPIO10 (CANRXB)
		GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 3; // Asynch qual for GPIO10 (CANRXB)
		GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 2;   // Configure GPIO8 for CANTXB operation
		GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 2;  // Configure GPIO10 for CANRXB operation
	}
	else if(1 == ch)
	{
		GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   // Enable pull-up for GPIO12 (CANTXB)
		GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   // Enable pull-up for GPIO13 (CANRXB)
		GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch qual for GPIO13 (CANRXB)
		GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;  // Configure GPIO12 for CANTXB operation
		GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 2;  // Configure GPIO13 for CANRXB operation
	}
	else if(2 == ch)
	{
		GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up for GPIO16 (CANTXB)
		GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up for GPIO17 (CANRXB)
		GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch qual for GPIO17 (CANRXB)
		GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;  // Configure GPIO16 for CANTXB operation
		GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;  // Configure GPIO17 for CANRXB operation
	}
	else if(3 == ch)
	{
		GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;   // Enable pull-up for GPIO20 (CANTXB)
		GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;   // Enable pull-up for GPIO21 (CANRXB)
		GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3; // Asynch qual for GPIO21 (CANRXB)
		GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 3;  // Configure GPIO20 for CANTXB operation
		GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 3;  // Configure GPIO21 for CANRXB operation
	}
    EDIS;
}

/*---------------------------------------------------------------------
函数原型：void IniteCANB(void)
函数名称：初始化eCAN模块
函数功能：
入口参数：无                 
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void IniteCANB(void)
{
	InitECanbGpio(2);

	/* Configure eCAN RX and TX pins for eCAN transmissions using eCAN regs*/
	EALLOW;
	ECanbShadow.CANTIOC.all = ECanbRegs.CANTIOC.all;
	ECanbShadow.CANTIOC.bit.TXFUNC = 1;
	ECanbRegs.CANTIOC.all = ECanbShadow.CANTIOC.all;

	ECanbShadow.CANRIOC.all = ECanbRegs.CANRIOC.all;
	ECanbShadow.CANRIOC.bit.RXFUNC = 1;
	ECanbRegs.CANRIOC.all = ECanbShadow.CANRIOC.all;
	EDIS;

	/* Configure eCAN for ECAN mode:29位地址 */
	EALLOW;
	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.SCB = 1;
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
	EDIS;

	/* Initialize all bits of ’Master Control Field’ to zero */
	ECanbMboxes.MBOX0.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX30.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX31.MSGCTRL.all = 0x00000000;

	// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
	ECanbRegs.CANTA.all 	= 0xFFFFFFFF;
	ECanbRegs.CANRMP.all 	= 0xFFFFFFFF;
	ECanbRegs.CANGIF0.all 	= 0xFFFFFFFF;
	ECanbRegs.CANGIF1.all 	= 0xFFFFFFFF;

	/* Configure bit timing parameters */
	EALLOW;
	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 1 ;				// Set CCR = 1
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
	EDIS;

	while(ECanbShadow.CANES.bit.CCE != 1 )
	{
		ECanbShadow.CANES.all = ECanbRegs.CANES.all;
	} 	// Wait for CCE bit to be set..
	
	EALLOW;
	ECanbShadow.CANBTC.all = ECanbRegs.CANBTC.all;
	ECanbShadow.CANBTC.bit.BRPREG 		= 99;    //19: 250KHz; 39:125KHZ; 99:50Khz
	ECanbShadow.CANBTC.bit.TSEG2REG 	= 4;
	ECanbShadow.CANBTC.bit.TSEG1REG 	= 8;
	ECanbShadow.CANBTC.bit.SJWREG     	= 2;
	ECanbRegs.CANBTC.all = ECanbShadow.CANBTC.all;
	EDIS;

	EALLOW;
	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 0 ;				// Set CCR = 0
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
	EDIS;

	while(ECanbShadow.CANES.bit.CCE != 0 )
	{
		ECanbShadow.CANES.all = ECanbRegs.CANES.all;
	} // Wait for CCE bit to be cleared..

	/* Disable all Mailboxes */
	ECanbRegs.CANME.all = 0x00000000; 			// 禁止所有的邮箱

	ECanbShadow.CANTRR.all = ECanbRegs.CANTRR.all;
    ECanbRegs.CANTRR.bit.TRR0   = 1;   			//请求复位邮箱0；
    ECanbRegs.CANTRR.bit.TRR30  = 1;   			//请求复位邮箱30；
    ECanbRegs.CANTRR.bit.TRR31  = 1;   			//请求复位邮箱31；
    ECanbRegs.CANTRR.all = ECanbShadow.CANTRR.all;

    while(ECanbShadow.CANTRS.bit.TRS0==1)  {ECanbShadow.CANTRS.all = ECanbRegs.CANTRS.all;}
    while(ECanbShadow.CANTRS.bit.TRS30==1) {ECanbShadow.CANTRS.all = ECanbRegs.CANTRS.all;}
    while(ECanbShadow.CANTRS.bit.TRS31==1) {ECanbShadow.CANTRS.all = ECanbRegs.CANTRS.all;}

	/* Write to the MSGID field */
    ECanbMboxes.MBOX0.MSGID.all = 0x80000000;  // 发送邮箱地址；

	/* Configure Mailbox under test as a Transmit mailbox */
	ECanbShadow.CANMD.all = ECanbRegs.CANMD.all; 
	ECanbShadow.CANMD.bit.MD0 = 0;            //设置为发送邮箱；
	ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;

	/* Enable Mailbox under test */
	ECanbShadow.CANME.all = ECanbRegs.CANME.all;
	ECanbShadow.CANME.bit.ME0 = 1;           //允许邮箱0；
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;

	/* Write to DLC field in Master Control reg */
    ECanbMboxes.MBOX0.MSGCTRL.bit.TPL = 1;
	ECanbMboxes.MBOX0.MSGCTRL.bit.RTR = 0;
	ECanbMboxes.MBOX0.MSGCTRL.bit.DLC = 8;

	/* Write to the MSGID as its MSGID */
	ECanbMboxes.MBOX30.MSGID.all     = 0x90ff6020; // 接收地址，接收设置帧
	ECanbMboxes.MBOX31.MSGID.all     = 0x91600f00; // 接收地址，接收查询帧

    ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
	ECanbShadow.CANMD.bit.MD30 = 1;            	//设置为接受邮箱
	ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;

	ECanbShadow.CANME.all = ECanbRegs.CANME.all;
	ECanbShadow.CANME.bit.ME30= 1;             	//允许邮箱30；
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;

   	ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
	ECanbShadow.CANMD.bit.MD31 = 1;            	//设置为接受邮箱
	ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;

	/* Enable Mailboxes */
	ECanbShadow.CANME.all = ECanbRegs.CANME.all;
	ECanbShadow.CANME.bit.ME31= 1;           	//允许邮箱31；
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;

	InitCANDATA();
}

void InitCANDATA(void)
{
    CountSystem.CanRst  = 0;
  	CanRxPack.Data1		= 0; 
	CanRxPack.Data2		= 0;
	CanRxPack.Data3		= 0;
	CanRxPack.Data4	    = 0;  
	CanRxPack.Data5	    = 0;
	CanRxPack.Data6	    = 0;
	CanRxPack.Data7	    = 0;
	CanRxPack.Data8		= 0;
} 

void eCAN_Reset(void)
{
   ECanbRegs.CANMC.bit.SRES  =  0;  									//软件复位CAN总线；
   IniteCANB();                     									//初始化CAN总线配置；同时清除初始化接受到的CAN命令数据；
}

/*---------------------------------------------------------------------
函数原型：void eCANSend(Uint32 boxID, struct Frame TxPack1)
函数名称：eCAN发送函数
函数功能：邮箱0发送数据
入口参数：boxID---设置发送邮箱ID；TxPack1---8Byte邮箱数据                 
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void eCANSend(Uint32 boxID, struct CANFRAME CANTXPACK1)
{
    Uint16  errcnt; 
  	
	ECanbMboxes.MBOX0.MDL.byte.BYTE0    =  CANTXPACK1.Data1;	
	ECanbMboxes.MBOX0.MDL.byte.BYTE1    =  CANTXPACK1.Data2;
    ECanbMboxes.MBOX0.MDL.byte.BYTE2    =  CANTXPACK1.Data3;
    ECanbMboxes.MBOX0.MDL.byte.BYTE3    =  CANTXPACK1.Data4;
	
    ECanbMboxes.MBOX0.MDH.byte.BYTE4    =  CANTXPACK1.Data5;
    ECanbMboxes.MBOX0.MDH.byte.BYTE5    =  CANTXPACK1.Data6;
    ECanbMboxes.MBOX0.MDH.byte.BYTE6    =  CANTXPACK1.Data7;
    ECanbMboxes.MBOX0.MDH.byte.BYTE7    =  CANTXPACK1.Data8;

    ECanbShadow.CANME.all = ECanbRegs.CANME.all;
    ECanbShadow.CANME.bit.ME0= 0;           // Disable mail-box0
    ECanbRegs.CANME.all = ECanbShadow.CANME.all;

	ECanbMboxes.MBOX0.MSGID.all = boxID;

	ECanbShadow.CANME.all = ECanbRegs.CANME.all;
	ECanbShadow.CANME.bit.ME0 = 1;			// enable mail-box0
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;

	/* Begin transmitting */
	ECanbShadow.CANTRS.all 		= ECanbRegs.CANTRS.all;
	ECanbShadow.CANTRS.bit.TRS0 = 1; 		// Set TRS for mailbox under test
	ECanbRegs.CANTRS.all 		= ECanbShadow.CANTRS.all;

	errcnt = 0;
	while(ECanbRegs.CANTA.bit.TA0 == 0)		// Wait for TA0 bit to be set..
	{  
		errcnt++;
	   	if(errcnt > 20000) 
		{
			FlagSysAlarm.bit.CanSend = 1;
			break;
	   	}
	}

	ECanbShadow.CANTA.all		= ECanbRegs.CANTA.all;
	ECanbShadow.CANTA.bit.TA0	= 1;					// Clear TA0
	ECanbRegs.CANTA.all 		= ECanbShadow.CANTA.all;
}

/*---------------------------------------------------------------------
函数原型：void eCAN_Recv(void)
函数名称：eCAN接受函数
函数功能：邮箱31接收数据
入口参数：无                 
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void CanDataRecv(void)
{
//	if(ECanbRegs.CANRMP.bit.RMP31 == 1)     // 回复查询帧
//	{
//		ECanbRegs.CANRMP.bit.RMP31 = 1; 									// Clear RMPn bits and start
//		CountSystem.CanRst = 0;
//		FlagSystem.bit.CanSend = 1;
//	}
//	else
//	{
//		CountSystem.CanRst++;
//		if(CountSystem.CanRst >= 600)
//		{
//			eCAN_Reset();
//		}
//	}
//
//	if(ECanbRegs.CANRMP.bit.RMP30 == 1)     // 回复设置帧
//	{
//		ECanbRegs.CANRMP.bit.RMP30 = 1; 									// Clear RMPn bits and start
//
//		CanRxPack.Data1 =  ECanbMboxes.MBOX30.MDL.byte.BYTE0;				// 0：本地，1：遥控
//	    CanRxPack.Data2  = ECanbMboxes.MBOX30.MDL.byte.BYTE1;				// 0：停止，1：启动
//	    CanRxPack.Data3  = ECanbMboxes.MBOX30.MDL.byte.BYTE2;				// 0：不动作，1：NO.1路为主电
//	    CanRxPack.Data4  = ECanbMboxes.MBOX30.MDL.byte.BYTE3;				// 0：不动作，1：NO.2路为主电
//	    CanRxPack.Data5  = ECanbMboxes.MBOX30.MDH.byte.BYTE4;				// 0：不动作，1：查询
//	    CanRxPack.Data6  = ECanbMboxes.MBOX30.MDH.byte.BYTE5;
//	    CanRxPack.Data7  = ECanbMboxes.MBOX30.MDH.byte.BYTE6;
//	    CanRxPack.Data8  = ECanbMboxes.MBOX30.MDH.byte.BYTE7;
//
//	}

	if(++CountSystem.CanRst >= 100)
	{
		CountSystem.CanRst = 0;
		FlagSystem.bit.CanSend = 1;
	}
}

/*---------------------------------------------------------------------
函数原型：void CanDataSave(void)
函数名称：保存CAN数据
函数功能: 
入口参数：无                 
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void CanDataSave(void)
{
	static Uint16 CountSum = 0;
	static Uint16 UdcVolIn = 0, 	UacVolOut = 0, 		UacCurOut = 0, 		UacFreOut = 0;
	static Uint32 SumUdcVolIn = 0, 	SumUacVolOut = 0, 	SumUacCurOut = 0, 	SumUacFreOut = 0;

	SumUdcVolIn  += (ParaTable.Timer1Reg.uInVolRms / 10);
	SumUacVolOut += (ParaTable.Timer1Reg.uOutVolRms / 10);
	SumUacCurOut += ParaTable.Timer1Reg.uOutCurRms;
	SumUacFreOut += ParaTable.Timer1Reg.uOutFre;

	CountSum++;
	if(CountSum >= 256)
	{
		UdcVolIn 	= SumUdcVolIn >> 8;
		UacVolOut 	= SumUacVolOut >> 8;
		UacCurOut 	= SumUacCurOut >> 8;
		UacFreOut 	= SumUacFreOut >> 8;

		SumUdcVolIn = 0;
		SumUacVolOut = 0;
		SumUacCurOut = 0;
		SumUacFreOut = 0;
		CountSum = 0;
	}

// 数据帧
	CanTxPack1.Data2 = UdcVolIn  & 0xff;
	CanTxPack1.Data1 = (UdcVolIn  >> 8) & 0xff;
	CanTxPack1.Data4 = UacVolOut & 0xff;
	CanTxPack1.Data3 = (UacVolOut >> 8) & 0xff;
	CanTxPack1.Data6 = UacCurOut & 0xff;
	CanTxPack1.Data5 = (UacCurOut >> 8) & 0xff;
	CanTxPack1.Data8 = UacFreOut & 0xff;
	CanTxPack1.Data7 = (UacFreOut >> 8) & 0xff;

// 状态帧1
	CanTxPack2.Data1 = 1;
	CanTxPack2.Data2 = 1;
	CanTxPack2.Data3 = 0;
	CanTxPack2.Data4 = 0;
	CanTxPack2.Data5 = 0;
	CanTxPack2.Data6 = 0;
	CanTxPack2.Data7 = 0;
	CanTxPack2.Data8 = 0;

// 状态帧2
	CanTxPack3.Data1 = 0;
	CanTxPack3.Data2 = 0;
	CanTxPack3.Data3 = 0;
	CanTxPack3.Data4 = 0;
	CanTxPack3.Data5 = 0;
	CanTxPack3.Data6 = 0;
	CanTxPack3.Data7 = 0;
	CanTxPack3.Data8 = 0;
}

//===========================================================================
// End of file.
//===========================================================================

