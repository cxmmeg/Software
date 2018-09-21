/****************************************************************************************************************/
//                    modbus通用驱动代码，只完成了从站0x03,0x06功能

//                    作为从站，只用3个函数,功能码0x03,0x06，0x93,0x96
//                    modbus_listen   和   modbus_reply   和   ModbusErr_reply
/****************************************************************************************************************/
#ifndef _USERS_MODBUS_H_
#define _USERS_MODBUS_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#define	MAX_BUFFER_SIZE			64
#define READ_BUF_SIZE			(9 * 1024)
#define WRITE_BUF_SIZE			(9 * 1024)

// ERROR CODE---MODBUS通信超限错误代码
#define ModbusErr_1				-1			// 未接收
#define ModbusErr_2				-2			// 接收地址错误或CRC错误
#define ModbusErr_3				-3			//
#define MODBUS_NO_ERR			0x00		// MODBUS正常
#define EXCEPTION_CODE_1		0x01		// 非法功能
#define EXCEPTION_CODE_2		0x02		// 非法数据地址
#define EXCEPTION_CODE_3		0x03		// 非法数据值
#define EXCEPTION_CODE_4		0x04		// 从站设备故障

#define ModbusASendByte 		SCIA_SendByte				//(uint8 dat)
#define ModbusASendStr  		SCIA_SendStr				//(unsigned char str[],unsigned char k)	//发送函数需要传递数组的第一个元素地址和发送的字节数
#define ModbusARcvTemp      	RX_BUFA						//接收缓存

#define ModbusBSendByte 		SCIB_SendByte				//(uint8 dat)
#define ModbusBSendStr  		SCIB_SendStr				//(unsigned char str[],unsigned char k)	//发送函数需要传递数组的第一个元素地址和发送的字节数
#define ModbusBRcvTemp      	RX_BUFB						//接收缓存

#define ModbusCSendByte 		SCIC_SendByte				//(uint8 dat)
#define ModbusCSendStr  		SCIC_SendStr				//(unsigned char str[],unsigned char k)	//发送函数需要传递数组的第一个元素地址和发送的字节数
#define ModbusCRcvTemp      	RX_BUFC						//接收缓存

struct MODBUS_REGS
{
	Uint16	ListenOk;		// 从站接听成功标志
	Uint16 	SlaveAddr; 		// 从站地址
	Uint16 	FuncCode; 		// 返回的功能码
	Uint16 	SubAddr;  		// 返回的从站的寄存器子地址(起始地址)
	Uint16 	Num;     		// 返回的读写寄存器的个数
	Uint16 	ModbusErr;		// 错误代码
	Uint16 	Buf[8];			// 如果是主站写命令，则成功返回表示保存一段写的数据区
};

extern struct MODBUS_REGS	ModbusBData;

/*---------------------------------------------------------------------
函数原型：void InitModbusBData(void)
函数名称：初始化MODBUS寄存器数据
函数功能：
入口参数：空
出口参数：空
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void InitModbusBData(void);

/******************************************************************************************************
*                                             MOBUSB
*                                        ====作为从站===

******************************************************************************************************/

/*---------------------------------------------------------------------
函数原型：int ModbusBListen(struct MODBUS_REGS *RcvData)
函数名称：ModbusB监听
函数功能：从站监听主站，返回1表示成功监听到一个有效帧，0表示数据错误
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
int ModbusBListen(struct MODBUS_REGS *RcvData);

/*---------------------------------------------------------------------
函数原型：void ModbusBReply(struct MODBUS_REGS *RcvData)
函数名称：ModbusB从站回复主站
函数功能：当监听判断数据无误后，从站回复主站，用于数据传递和从站地址设置
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
void ModbusBReply(struct MODBUS_REGS *RcvData);

/*---------------------------------------------------------------------
函数原型：void ModbusBErrReply(struct MODBUS_REGS *RcvData)
函数名称：ModbusB从站回复主站
函数功能：从站监听主站失败，错误回复
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
void ModbusBErrReply(struct MODBUS_REGS *RcvData);

/*---------------------------------------------------------------------
函数原型：void modbus_process(void)
函数功能：modbus接收处理
入口参数：
出口参数：
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void ModbusBProcess(void);

/*---------------------------------------------------------------------
函数原型：void StartSaveData(void)
函数功能：定时保存数据值RAM
入口参数：
出口参数：
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void StartSaveData(void);


/******************************************************************************************************
*                                             MOBUSC
*                                        ====作为从站===

******************************************************************************************************/

/*---------------------------------------------------------------------
函数原型：int ModbusCListen(struct MODBUS_REGS *RcvData)
函数名称：ModbusC监听
函数功能：从站监听主站，返回1表示成功监听到一个有效帧，0表示数据错误
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
int ModbusCListen(struct MODBUS_REGS *RcvData);

/*---------------------------------------------------------------------
函数原型：void ModbusCReply(struct MODBUS_REGS *RcvData)
函数名称：ModbusC从站回复主站
函数功能：当监听判断数据无误后，从站回复主站，用于数据传递和从站地址设置
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
void ModbusCReply(struct MODBUS_REGS *RcvData);

/*---------------------------------------------------------------------
函数原型：void ModbusCErrReply(struct MODBUS_REGS *RcvData)
函数名称：ModbusC从站回复主站
函数功能：从站监听主站失败，错误回复
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
void ModbusCErrReply(struct MODBUS_REGS *RcvData);

/*---------------------------------------------------------------------
函数原型：void modbus_process(void)
函数功能：modbus接收处理
入口参数：
出口参数：
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void ModbusCProcess(void);

#endif
