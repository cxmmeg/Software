/****************************************************************************************************************/
//                    DSP modbus驱动代码
//
/****************************************************************************************************************/
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include <string.h>

#pragma DATA_SECTION(ModbusBData,".SciData")
struct MODBUS_REGS	ModbusBData;

#pragma DATA_SECTION(ModbusCData,".SciData")
struct MODBUS_REGS	ModbusCData;

#pragma DATA_SECTION(EmDataAddrArray,".SciData")
int16 *EmDataAddrArray[8];

#pragma DATA_SECTION(CrcBTempBuf,".SciData")
uchar CrcBTempBuf[192];

#pragma DATA_SECTION(CrcCTempBuf,".SciData")
uchar CrcCTempBuf[192];

#pragma DATA_SECTION(ErrCrcBTempBuf,".SciData")
uchar  ErrCrcBTempBuf[5];

#pragma DATA_SECTION(ErrCrcCTempBuf,".SciData")
uchar  ErrCrcCTempBuf[5];

#pragma DATA_SECTION(auchCRCHi,".SciData")
uchar  auchCRCHi[256] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
}; 

#pragma DATA_SECTION(auchCRCLo,".SciData")
uchar  auchCRCLo[256] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
};

void ModbusBUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i);
void ModbusCUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i);

extern union FLAG_SYSTEM FlagSystem;
/*---------------------------------------------------------------------
函数原型：uint16 crc16(uchar *puchMsg, Uint16 usDataLen) 
函数名称：CRC校验计算
函数功能：
入口参数：uchar *puchMsg	: 需校验数据头
	   	   Uint16 usDataLen	: 数据长度
出口参数：校验值，1个字节
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
Uint16 crc16(uchar *puchMsg, Uint16 usDataLen)
{
	Uint16  uchCRCHi = 0xFF ; 					//高CRC字节初始化
	Uint16  uchCRCLo = 0xFF ; 					//低CRC 字节初始化
	Uint32  uIndex;          					// CRC循环中的索引

	while (usDataLen--)      					// 传输消息缓冲区
	{ 
		uIndex = uchCRCHi ^ * puchMsg++; 		// 计算CRC
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	} 
	return (((Uint16)uchCRCHi << 8) | uchCRCLo);

}

/*---------------------------------------------------------------------
函数原型：uchar CrcDetect(uchar *crc_buf, Uint16 len)
函数名称：CRC校验验证
函数功能：此CRC校验为MODBUS从机返回数据，len个字节，前len-2个是数据，后2个CRC校验
入口参数：uchar *crc_buf	: 需校验数据头
	   Uint16 len		: 数据长度
出口参数：返回1成功，0失败
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
uchar CrcDetect(uchar *CrcBuf, Uint16 Len)
{
    uchar  *p_buf = CrcBuf;
    Uint16 cal_crc;
    Uint16 rcv_crc;

    cal_crc = crc16(p_buf, Len - 2);
    rcv_crc = ((Uint16)p_buf[Len - 2]<<8) | ((Uint16)p_buf[Len - 1]);

    if (rcv_crc == cal_crc)
    {
        return 1;    
    }
    else
    {
        return 0;    
    }
}

/*---------------------------------------------------------------------
函数原型：void InitModbusBData(void)
函数名称：初始化MODBUS寄存器数据
函数功能：
入口参数：空
出口参数：空
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void InitModbusBData(void)
{
	memset(CrcBTempBuf, 0, sizeof(CrcBTempBuf));
	memset(ErrCrcBTempBuf, 0, sizeof(ErrCrcBTempBuf));
	memset(&ModbusBData, 0, sizeof(ModbusBData));

	memset(CrcCTempBuf, 0, sizeof(CrcCTempBuf));
	memset(ErrCrcCTempBuf, 0, sizeof(ErrCrcCTempBuf));
	memset(&ModbusCData, 0, sizeof(ModbusCData));

	ModbusBData.SlaveAddr = 0x01;
	ModbusCData.SlaveAddr = 0x01;

	memset(EmDataAddrArray, 0, sizeof(EmDataAddrArray));
}

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
int ModbusBListen(struct MODBUS_REGS *RcvData)
{
    if(1 == FlagSciBRcvOK)
	{
		FlagSciBRcvOK = 0;
		if ((ModbusBRcvTemp[0] == RcvData->SlaveAddr) && CrcDetect(ModbusBRcvTemp, 8))
		{
	    	RcvData->FuncCode = ModbusBRcvTemp[1];												// 功能码
	        
		    switch (ModbusBRcvTemp[1])
		    {
		        // 功能码    [1 byte]: 0x03  参数表查询
		        // 起始地址 [2 byte]: 0x0000 to 0x2400
		        // 读取数量 [2 byte]: 0x0001 to 0x0080(128)
		        case 0x03:
		        {
		            RcvData->SubAddr = ((Uint16)ModbusBRcvTemp[2] << 8) + ModbusBRcvTemp[3];	// Starting address
	                RcvData->Num   = ((Uint16)ModbusBRcvTemp[4] << 8) + ModbusBRcvTemp[5];	// Quantity of registers
	                if ((RcvData->SubAddr + RcvData->Num * 2) > READ_BUF_SIZE)				// 非法数据地址
	                {    
	                    RcvData->ModbusErr = EXCEPTION_CODE_2;
	                }
	                else if ((RcvData->Num <= 0) || ((RcvData->Num * 2) > 0x0080))			// 非法数据值
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;
					}
	                else
	                {
	                	RcvData->ModbusErr = MODBUS_NO_ERR;
	                }

	                return MODBUS_NO_ERR;
		        }

		        // 功能码 	[1 byte]: 0x06 参数表单寄存器设置
		        // 设置地址 [2 byte]: 0x0000 to 0xFFFF
		        // 设置值 	[2 byte]: 0x0000 to 0xFFFF
		        case 0x06:  // write single register
		        {
		            RcvData->SubAddr = ModbusBRcvTemp[3] + ((Uint16)ModbusBRcvTemp[2] << 8);
		            if (RcvData->SubAddr > WRITE_BUF_SIZE)
	                {
	                    RcvData->ModbusErr = EXCEPTION_CODE_2;								// 非法数据地址
	                }
		            else
		            {
		            	RcvData->ModbusErr = MODBUS_NO_ERR;
		            }
	                RcvData->Buf[0] = ModbusBRcvTemp[4];
	               	RcvData->Buf[1] = ModbusBRcvTemp[5];
					

	                return MODBUS_NO_ERR;
		        }

		        // 功能码     [1 byte]: 0x64  同步指令
		        // 内存区段 [1 byte]: 0x0000 to 0x0003
		        // 指令        [2 byte]: 0x0000 to 0x0001
		        case 0x64:  // write single register
		        {
	                RcvData->Buf[0] = ModbusBRcvTemp[2];
	                RcvData->Buf[1] = ModbusBRcvTemp[3];
	                RcvData->Buf[2] = ModbusBRcvTemp[4];
	               	RcvData->Buf[3] = ModbusBRcvTemp[5];
	               	if(RcvData->Buf[1] > 3)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;								// 非法指令
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
		        }

		        // 功能码 			[1 byte]: 0x76 设置监测点
		        // 设置寄存器号   	[2 byte]: 0x0000 to 0x2400
		        // 设置存储区域号	[1 byte]: 0x00 to 0x50(外部存储空间80K空间)
		        // 设置写入数量   	[1 byte]: 0x00 to 0x04(单位为KByte，最大写入数量4KByte)
		        case 0x76:
		        {
		            RcvData->SubAddr = ModbusBRcvTemp[3] + ((Uint16)ModbusBRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusBRcvTemp[4];			// 存储区域号
	               	RcvData->Buf[1] = ModbusBRcvTemp[5];			// 存储长度

	                if(RcvData->Buf[0] >= 0x50)
	                {
	                	RcvData->ModbusErr = EXCEPTION_CODE_1;	// 非法存储区域
	                }
	                else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// 非法数据地址
					}
	                else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// 非法数据长度
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
		        }

	            // 功能码 		[1 byte]: 0x73 读监测点数据
		        // 读取地址号 	[2 byte]: 0x0000 to 0x2400
		        // 读取的数量 	[2 byte]: 0x0000 to 0xFFFF(单位为64Byte)    
	            case 0x73:  	// 自定义，读取外部RAM数据，
	            {
		            RcvData->SubAddr = ModbusBRcvTemp[3] + ((Uint16)ModbusBRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusBRcvTemp[4];		// 读取存储区域
	               	RcvData->Buf[1] = ModbusBRcvTemp[5];		// 读取长度

	               	if(RcvData->Buf[0] >= 0x50)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_1;	// 非法存储区域
					}
					else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// 非法数据地址
					}
					else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// 非法数据长度
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}
					
	                return MODBUS_NO_ERR;
	            }

	            // 功能码 		[1 byte]: 0x63 读监测点数据
		        // 读取地址号 	[2 byte]: 0x0000 to 0x2400
		        // 读取的数量 	[2 byte]: 0x0000 to 0xFFFF(单位为64Byte)
	            case 0x63:  	// 自定义，读取重要RAM数据 共128K
	            {
		            RcvData->SubAddr = ModbusBRcvTemp[3] + ((Uint16)ModbusBRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusBRcvTemp[4];		// 读取存储区域
	               	RcvData->Buf[1] = ModbusBRcvTemp[5];		// 读取长度	1K

	               	if(RcvData->Buf[0] >= 0x80)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_1;	// 非法存储区域
					}
					else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// 非法数据地址
					}
					else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// 非法数据长度
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
	            }

		        default:
		        {
		        	RcvData->ModbusErr = EXCEPTION_CODE_1;					//异常码，非法功能码
		        	
		            return MODBUS_NO_ERR;
		        }
		    }
	    }
	    else
	    {
	        return ModbusErr_1;		// 地址或CRC校验错误
	    }
    }
    else
    {
        return ModbusErr_2;			// UART未接收或接收错误
    }
}

/*---------------------------------------------------------------------
函数原型：void ModbusBReply(struct MODBUS_REGS *RcvData)
函数名称：ModbusB从站回复主站
函数功能：当监听判断数据无误后，从站回复主站，用于数据传递和从站地址设置
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
void ModbusBReply(struct MODBUS_REGS *RcvData)
{
    Uint16 crcData = 0;
    Uint16 i = 0,j = 0;
	
    switch (RcvData->FuncCode)
    {
    	// 标准MODBUS协议，功能码：0x03, 读取数据，固定一帧64Byte
        case 0x03:
		{
			ModbusBUpdata(RcvData, &CrcBTempBuf[3],0);				//更新数据
			CrcBTempBuf[0] = RcvData->SlaveAddr;
			CrcBTempBuf[1] = 0x03;
			CrcBTempBuf[2] = RcvData->Num * 2;

			crcData = crc16(CrcBTempBuf, RcvData->Num * 2 + 3);
			CrcBTempBuf[RcvData->Num * 2 + 4] = (crcData >> 8) & 0xff;
			CrcBTempBuf[RcvData->Num * 2 + 5] = crcData & 0xff;

			for (i = 0; i < RcvData->Num * 2 + 5; i++)   //寄存器数据
			{
				ModbusBSendByte(CrcBTempBuf[i]);
			}

			break;
		}

        // 标准MODBUS协议，功能码：0x06, 设置一个寄存器数据
        // 原样数据回复
        case 0x06:
        {
        	ModbusBUpdata(RcvData, &CrcBTempBuf[3],0);				//更新数据
            CrcBTempBuf[0] = RcvData->SlaveAddr;
            CrcBTempBuf[1] = 0x06;
            CrcBTempBuf[2] = (uchar)(RcvData->SubAddr >> 8);
            CrcBTempBuf[3] = (uchar)RcvData->SubAddr;
            CrcBTempBuf[4] = RcvData->Buf[0];
            CrcBTempBuf[5] = RcvData->Buf[1];
			
			crcData = crc16(CrcBTempBuf, 6);
            CrcBTempBuf[6] = crcData >> 8;
            CrcBTempBuf[7] = crcData & 0xff;

            for (i = 0; i < 8; i++)         //寄存器数据
            {
            	ModbusBSendByte(CrcBTempBuf[i]);
            }

            break;
        }

        // 自定义MODBUS协议，功能码：0x64, 同步指令；
        // 原样数据回复
		case 0x64:
		{
			ModbusBUpdata(RcvData, &CrcBTempBuf[3],0);				//更新数据
			CrcBTempBuf[0] = RcvData->SlaveAddr;
			CrcBTempBuf[1] = 0x64;
			CrcBTempBuf[2] = RcvData->Buf[0];
			CrcBTempBuf[3] = RcvData->Buf[1];
			CrcBTempBuf[4] = ExRamReg.FlagDataSaveFinish;//point_save_finish_flag;//RcvData->Buf[2];
			CrcBTempBuf[5] = RcvData->Buf[3];

			crcData = crc16(CrcBTempBuf, 6);
			CrcBTempBuf[6] = (crcData >> 8) & 0xff;
			CrcBTempBuf[7] = crcData & 0xff;

			for (i = 0; i < 8; i++)         //寄存器数据
			{
				ModbusBSendByte(CrcBTempBuf[i]);
			}

			break;
		}

     	// 自定义MODBUS协议，功能码：0x76, 选择一个数据作为检测点；
     	// 原样数据回复
     	case 0x76:
     	{
     		ModbusBUpdata(RcvData, &CrcBTempBuf[3],0);				//更新数据
            CrcBTempBuf[0] = RcvData->SlaveAddr;
            CrcBTempBuf[1] = 0x76;
            CrcBTempBuf[2] = (uchar)(RcvData->SubAddr >> 8);
            CrcBTempBuf[3] = (uchar)RcvData->SubAddr;
            CrcBTempBuf[4] = RcvData->Buf[0];
            CrcBTempBuf[5] = RcvData->Buf[1];

            crcData = crc16(CrcBTempBuf, 6);
            CrcBTempBuf[6] = crcData >> 8;
            CrcBTempBuf[7] = crcData & 0xff;

            for (i = 0; i < 8; i++)
            {
                ModbusBSendByte(CrcBTempBuf[i]);
            }
            break;
     	}

        // 自定义MODBUS协议，功能码：0x73, 读取多帧数据；一帧64Byte
     	// 读取1K数据，回复16帧   
        case 0x73:
        {
        	for(i = RcvData->Buf[1] - 1; i < 16; i++)
        	{
        		ModbusBUpdata(RcvData, CrcBTempBuf + 4, i);			//更新数据

        		CrcBTempBuf[0] = 0x01;
	            CrcBTempBuf[1] = 0x73;
	            CrcBTempBuf[2] = RcvData->Buf[0];					//存储区域号；
	            CrcBTempBuf[3] = i;									//多帧数据的帧ID；

	        	crcData = crc16(CrcBTempBuf, (64 * 2 + 4));
	        	CrcBTempBuf[64 * 2 + 4] = (crcData >> 8) & 0xff;
	        	CrcBTempBuf[64 * 2 + 5] = crcData & 0xff;

	            for (j = 0; j < (64 * 2 + 6);j++)   				//寄存器数据
	            {
	                ModbusBSendByte(CrcBTempBuf[j]);
	            }
        	}

            break;
        }

        // 自定义MODBUS协议，读取重要数据功能码：0x63, 读取多帧数据；一帧64Byte
     	// 读取1K数据，回复16帧
        case 0x63:
        {
        	for(i = RcvData->Buf[1] - 1; i < 16; i++)
        	{
        		ModbusBUpdata(RcvData, CrcBTempBuf + 4, i);			//更新数据

        		CrcBTempBuf[0] = 0x01;
	            CrcBTempBuf[1] = 0x63;
	            CrcBTempBuf[2] = RcvData->Buf[0];					//存储区域号；
	            CrcBTempBuf[3] = i;									//多帧数据的帧ID；

	        	crcData = crc16(CrcBTempBuf, (64 * 2 + 4));
	        	CrcBTempBuf[64 * 2 + 4] = (crcData >> 8) & 0xff;
	        	CrcBTempBuf[64 * 2 + 5] = crcData & 0xff;
	        	
	            for (j = 0; j < (64 * 2 + 6);j++)   			//寄存器数据
	            {
	                ModbusBSendByte(CrcBTempBuf[j]);
	            }
        	}

            break;
        }

        default:
            break;    
    }
}

/*---------------------------------------------------------------------
函数原型：void ModbusBErrReply(struct MODBUS_REGS *RcvData)
函数名称：ModbusB从站回复主站
函数功能：从站监听主站失败，错误回复
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
void ModbusBErrReply(struct MODBUS_REGS *RcvData)
{
	Uint16 crcData = 0;
    Uint16 i = 0;
	
	ErrCrcBTempBuf[0] = RcvData->SlaveAddr;
	ErrCrcBTempBuf[1] = RcvData->FuncCode + 0x80;
	ErrCrcBTempBuf[2] = RcvData->ModbusErr;
	
	crcData = crc16(ErrCrcBTempBuf, 3);
    ErrCrcBTempBuf[3] = crcData >> 8;
    ErrCrcBTempBuf[4] = crcData & 0xff;
	
	for (i = 0; i < 5; i++)         							//寄存器数据
    {
        ModbusBSendByte(ErrCrcBTempBuf[i]);
    }
}

/*---------------------------------------------------------------------
函数原型：void ModbusBUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i)
函数名称：更新modbus数据
函数功能：完成相应功能码的数据更新
入口参数：	struct MODBUS_REGS *RcvData 	数据传递结构体
			uchar *Buf						写/读 缓冲
			Uint16 i						重复次数
出口参数：无
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void ModbusBUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i)
{
	int *ptParatab = (int *)&ParaTable;					//参数表首地址指针；

	switch (RcvData->FuncCode)
    {
    	//读取参数表，
        case 0x03:
        {
			CopyData(64, ptParatab + RcvData->SubAddr, Buf);
	     	break;
        }

	    //设置一个参数表
        case 0x06:
        {
			*(ptParatab + RcvData->SubAddr) = (int)(((Uint16)RcvData->Buf[0] << 8) | (Uint16)RcvData->Buf[1]);
			FlagSystem.bit.ParaDataChg = 1;
			ParaTable.PowerReg.uParaTableVer++;
			break;
        }

        //同步保存数据
		case 0x64:
		{
			if(RcvData->Buf[1] == 0x01)
			{
				ExRamReg.FlagBlockSave = 1;
			}

			break;
		}

	    //设置一个监测点
		case 0x76:
		{
			ExRamReg.ArrayBlock[ExRamReg.CountBlock] = RcvData->Buf[0];
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->pt_addr = ptParatab + RcvData->SubAddr;											// 指定存储数据地址
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->buf_len = RcvData->Buf[1];														// 地址存储数据长度
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->pt_base_addr = (int *)(ExRamData + ExRamReg.ArrayBlock[ExRamReg.CountBlock] * 0x400);	// 指定外部存储区域

			ExRamReg.FlagDataSaveFinish = 0;
			ExRamReg.CountBlock++;
			if(ExRamReg.CountBlock >= 20)
			{
				ExRamReg.CountBlock = 19;
				RcvData->ModbusErr = EXCEPTION_CODE_1;
			}

			if (1 == FlagSystem.bit.EmDataChgFlag)
			{
			    EmDataAddrArray[EmgcRamReg.CountBlock] = (int16 *)RcvData->SubAddr;

				EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock] = EmgcRamReg.CountBlock * 16;
				(EmgcRamCtrl + EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock])->pt_addr = ptParatab + RcvData->SubAddr;											// 指定存储数据地址
				(EmgcRamCtrl + EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock])->buf_len = 16;			//RcvData->Buf[1];											// 地址存储数据长度 固定16K
				(EmgcRamCtrl + EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock])->pt_base_addr = (int *)(EmRamData + EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock] * 0x400);	// 指定外部存储区域

				EmgcRamReg.FlagDataSaveFinish = 0;
				EmgcRamReg.CountBlock++;
				if(EmgcRamReg.CountBlock >= 8)
				{
				    EmgcRamReg.CountBlock = 8;
					RcvData->ModbusErr = EXCEPTION_CODE_1;
				}
			}

			break;
		}

		// 读取指定内存区EXRAM
		case 0x73:
		{
			CopyData(64, ExRamData + RcvData->Buf[0] * 1024 + i * 64, Buf);
			break;
		}

		// 读取指定内存区EMRAM
		case 0x63:
		{
			CopyData(64, EmRamData + ((Uint32)RcvData->Buf[0] * 1024 + i * 64), Buf);
			break;
		}

	    default:
	    	break;
	}
}

/*---------------------------------------------------------------------
函数原型：void modbus_process(void)
函数功能：modbus接收处理
入口参数：
出口参数：
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void ModbusBProcess(void)
{
	ControlGPIOOut(SCI_CONB, SCI_CON_RECV);
	ModbusBData.ListenOk = ModbusBListen(&ModbusBData);			//作为从站，等待消息
	if(MODBUS_NO_ERR == ModbusBData.ListenOk)
	{
		ControlGPIOOut(SCI_CONB, SCI_CON_SEND);
		if(MODBUS_NO_ERR == ModbusBData.ModbusErr)
		{
			ModbusBReply(&ModbusBData);
		}
		else
		{
			ModbusBErrReply(&ModbusBData);
		}
		ModbusBData.ListenOk = 1;
	}
}

/*---------------------------------------------------------------------
函数原型：void StartSaveData(void)
函数功能：定时保存数据值RAM
入口参数：
出口参数：
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void StartSaveData(void)
{
    ExRamReg.FlagBlockSave = 1;
}

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
int ModbusCListen(struct MODBUS_REGS *RcvData)
{
    if(1 == FlagSciCRcvOK)
	{
    	FlagSciCRcvOK = 0;
		if ((ModbusCRcvTemp[0] == RcvData->SlaveAddr) && CrcDetect(ModbusCRcvTemp, 8))
		{
	    	RcvData->FuncCode = ModbusCRcvTemp[1];												// 功能码

		    switch (ModbusCRcvTemp[1])
		    {
		        // 功能码    [1 byte]: 0x03  参数表查询
		        // 起始地址 [2 byte]: 0x0000 to 0x2400
		        // 读取数量 [2 byte]: 0x0001 to 0x0080(128)
		        case 0x03:
		        {
		            RcvData->SubAddr = ((Uint16)ModbusCRcvTemp[2] << 8) + ModbusCRcvTemp[3];	// Starting address
	                RcvData->Num   = ((Uint16)ModbusCRcvTemp[4] << 8) + ModbusCRcvTemp[5];	// Quantity of registers
	                if ((RcvData->SubAddr + RcvData->Num * 2) > READ_BUF_SIZE)				// 非法数据地址
	                {
	                    RcvData->ModbusErr = EXCEPTION_CODE_2;
	                }
	                else if ((RcvData->Num <= 0) || ((RcvData->Num * 2) > 0x0080))			// 非法数据值
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;
					}
	                else
	                {
	                	RcvData->ModbusErr = MODBUS_NO_ERR;
	                }

	                return MODBUS_NO_ERR;
		        }

		        // 功能码 	[1 byte]: 0x06 参数表单寄存器设置
		        // 设置地址   [2 byte]: 0x0000 to 0xFFFF
		        // 设置值 	[2 byte]: 0x0000 to 0xFFFF
		        case 0x06:  // write single register
		        {
		            RcvData->SubAddr = ModbusCRcvTemp[3] + ((Uint16)ModbusCRcvTemp[2] << 8);
		            if (RcvData->SubAddr > WRITE_BUF_SIZE)
	                {
	                    RcvData->ModbusErr = EXCEPTION_CODE_2;								// 非法数据地址
	                }
		            else
		            {
		            	RcvData->ModbusErr = MODBUS_NO_ERR;
		            }
	                RcvData->Buf[0] = ModbusCRcvTemp[4];
	               	RcvData->Buf[1] = ModbusCRcvTemp[5];


	                return MODBUS_NO_ERR;
		        }

		        // 功能码     [1 byte]: 0x64  同步指令
		        // 内存区段 [1 byte]: 0x0000 to 0x0003
		        // 指令        [2 byte]: 0x0000 to 0x0001
		        case 0x64:  // write single register
		        {
	                RcvData->Buf[0] = ModbusCRcvTemp[2];
	                RcvData->Buf[1] = ModbusCRcvTemp[3];
	                RcvData->Buf[2] = ModbusCRcvTemp[4];
	               	RcvData->Buf[3] = ModbusCRcvTemp[5];
	               	if(RcvData->Buf[1] > 3)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;								// 非法指令
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
		        }

		        // 功能码 			[1 byte]: 0x76 设置监测点
		        // 设置寄存器号   	[2 byte]: 0x0000 to 0x2400
		        // 设置存储区域号	[1 byte]: 0x00 to 0x50(外部存储空间80K空间)
		        // 设置写入数量   	[1 byte]: 0x00 to 0x04(单位为KByte，最大写入数量4KByte)
		        case 0x76:
		        {
		            RcvData->SubAddr = ModbusCRcvTemp[3] + ((Uint16)ModbusCRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusCRcvTemp[4];			// 存储区域号
	               	RcvData->Buf[1] = ModbusCRcvTemp[5];			// 存储长度

	                if(RcvData->Buf[0] >= 0x50)
	                {
	                	RcvData->ModbusErr = EXCEPTION_CODE_1;	// 非法存储区域
	                }
	                else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// 非法数据地址
					}
	                else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// 非法数据长度
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
		        }

	            // 功能码 		[1 byte]: 0x73 读监测点数据
		        // 读取地址号 	[2 byte]: 0x0000 to 0x2400
		        // 读取的数量 	[2 byte]: 0x0000 to 0xFFFF(单位为64Byte)
	            case 0x73:  	// 自定义，读取外部RAM数据，
	            {
		            RcvData->SubAddr = ModbusCRcvTemp[3] + ((Uint16)ModbusCRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusCRcvTemp[4];		// 读取存储区域
	               	RcvData->Buf[1] = ModbusCRcvTemp[5];		// 读取长度

	               	if(RcvData->Buf[0] >= 0x50)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_1;	// 非法存储区域
					}
					else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// 非法数据地址
					}
					else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// 非法数据长度
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
	            }

		        default:
		        {
		        	RcvData->ModbusErr = EXCEPTION_CODE_1;					//异常码，非法功能码

		            return MODBUS_NO_ERR;
		        }
		    }
	    }
	    else
	    {
	        return ModbusErr_1;		// 地址或CRC校验错误
	    }
    }
    else
    {
        return ModbusErr_2;			// UART未接收或接收错误
    }
}

/*---------------------------------------------------------------------
函数原型：void ModbusCReply(struct MODBUS_REGS *RcvData)
函数名称：ModbusC从站回复主站
函数功能：当监听判断数据无误后，从站回复主站，用于数据传递和从站地址设置
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
void ModbusCReply(struct MODBUS_REGS *RcvData)
{
    Uint16 crcData = 0;
    Uint16 i = 0,j = 0;

    switch (RcvData->FuncCode)
    {
    	// 标准MODBUS协议，功能码：0x03, 读取数据，固定一帧64Byte
        case 0x03:
		{
			ModbusCUpdata(RcvData, &CrcCTempBuf[3],0);				//更新数据
			CrcCTempBuf[0] = RcvData->SlaveAddr;
			CrcCTempBuf[1] = 0x03;
			CrcCTempBuf[2] = RcvData->Num * 2;

			crcData = crc16(CrcCTempBuf, RcvData->Num * 2 + 3);
			CrcCTempBuf[RcvData->Num * 2 + 4] = (crcData >> 8) & 0xff;
			CrcCTempBuf[RcvData->Num * 2 + 5] = crcData & 0xff;

			for (i = 0; i < RcvData->Num * 2 + 5; i++)   //寄存器数据
			{
				ModbusCSendByte(CrcCTempBuf[i]);
			}

			break;
		}

        // 标准MODBUS协议，功能码：0x06, 设置一个寄存器数据
        // 原样数据回复
        case 0x06:
        {
        	ModbusCUpdata(RcvData, &CrcCTempBuf[3],0);				//更新数据
        	CrcCTempBuf[0] = RcvData->SlaveAddr;
        	CrcCTempBuf[1] = 0x06;
        	CrcCTempBuf[2] = (uchar)(RcvData->SubAddr >> 8);
        	CrcCTempBuf[3] = (uchar)RcvData->SubAddr;
        	CrcCTempBuf[4] = RcvData->Buf[0];
        	CrcCTempBuf[5] = RcvData->Buf[1];

			crcData = crc16(CrcCTempBuf, 6);
			CrcCTempBuf[6] = crcData >> 8;
			CrcCTempBuf[7] = crcData & 0xff;

            for (i = 0; i < 8; i++)         //寄存器数据
            {
            	ModbusCSendByte(CrcCTempBuf[i]);
            }

            break;
        }

        // 自定义MODBUS协议，功能码：0x64, 同步指令；
        // 原样数据回复
		case 0x64:
		{
			ModbusCUpdata(RcvData, &CrcCTempBuf[3],0);				//更新数据
			CrcCTempBuf[0] = RcvData->SlaveAddr;
			CrcCTempBuf[1] = 0x64;
			CrcCTempBuf[2] = RcvData->Buf[0];
			CrcCTempBuf[3] = RcvData->Buf[1];
			CrcCTempBuf[4] = ExRamReg.FlagDataSaveFinish;//point_save_finish_flag;//RcvData->Buf[2];
			CrcCTempBuf[5] = RcvData->Buf[3];

			crcData = crc16(CrcCTempBuf, 6);
			CrcCTempBuf[6] = (crcData >> 8) & 0xff;
			CrcCTempBuf[7] = crcData & 0xff;

			for (i = 0; i < 8; i++)         //寄存器数据
			{
				ModbusCSendByte(CrcCTempBuf[i]);
			}

		break;
		}

     	// 自定义MODBUS协议，功能码：0x76, 选择一个数据作为检测点；
     	// 原样数据回复
     	case 0x76:
     	{
     		ModbusCUpdata(RcvData, &CrcCTempBuf[3],0);				//更新数据
     		CrcCTempBuf[0] = RcvData->SlaveAddr;
     		CrcCTempBuf[1] = 0x76;
     		CrcCTempBuf[2] = (uchar)(RcvData->SubAddr >> 8);
     		CrcCTempBuf[3] = (uchar)RcvData->SubAddr;
     		CrcCTempBuf[4] = RcvData->Buf[0];
     		CrcCTempBuf[5] = RcvData->Buf[1];

            crcData = crc16(CrcCTempBuf, 6);
            CrcCTempBuf[6] = crcData >> 8;
            CrcCTempBuf[7] = crcData & 0xff;

            for (i = 0; i < 8; i++)
            {
                ModbusCSendByte(CrcCTempBuf[i]);
            }
            break;
     	}

        // 自定义MODBUS协议，功能码：0x73, 读取多帧数据；一帧64Byte
     	// 读取1K数据，回复16帧
        case 0x73:
        {
        	for(i = RcvData->Buf[1] - 1; i < 16; i++)
        	{
        		ModbusCUpdata(RcvData, CrcCTempBuf + 4, i);			//更新数据

        		CrcCTempBuf[0] = 0x01;
        		CrcCTempBuf[1] = 0x73;
        		CrcCTempBuf[2] = RcvData->Buf[0];				//存储区域号；
        		CrcCTempBuf[3] = i;							//多帧数据的帧ID；

	        	crcData = crc16(CrcCTempBuf, (64 * 2 + 4));
	        	CrcCTempBuf[64 * 2 + 4] = (crcData >> 8) & 0xff;
	        	CrcCTempBuf[64 * 2 + 5] = crcData & 0xff;

	            for (j = 0; j < (64 * 2 + 6);j++)   			//寄存器数据
	            {
	                ModbusCSendByte(CrcCTempBuf[j]);
	            }
        	}

            break;
        }

        default:
            break;
    }
}

/*---------------------------------------------------------------------
函数原型：void ModbusCErrReply(struct MODBUS_REGS *RcvData)
函数名称：ModbusC从站回复主站
函数功能：从站监听主站失败，错误回复
入口参数：*RcvData  数据处理结构体
出口参数：空
作    者：
日    期：2018.01.13
----------------------------------------------------------------------*/
void ModbusCErrReply(struct MODBUS_REGS *RcvData)
{
	Uint16 crcData = 0;
    Uint16 i = 0;

	ErrCrcCTempBuf[0] = RcvData->SlaveAddr;
	ErrCrcCTempBuf[1] = RcvData->FuncCode + 0x80;
	ErrCrcCTempBuf[2] = RcvData->ModbusErr;

	crcData = crc16(ErrCrcCTempBuf, 3);
	ErrCrcCTempBuf[3] = crcData >> 8;
	ErrCrcCTempBuf[4] = crcData & 0xff;

	for (i = 0; i < 5; i++)         							//寄存器数据
    {
        ModbusCSendByte(ErrCrcCTempBuf[i]);
    }
}

/*---------------------------------------------------------------------
函数原型：void ModbusCUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i)
函数名称：更新modbus数据
函数功能：完成相应功能码的数据更新
入口参数：	struct MODBUS_REGS *RcvData 	数据传递结构体
			uchar *Buf						写/读 缓冲
			Uint16 i						重复次数
出口参数：无
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void ModbusCUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i)
{
	int *ptParatab = (int *)&ParaTable;					//参数表首地址指针；

	switch (RcvData->FuncCode)
    {
    	//读取参数表，
        case 0x03:
        {
			CopyData(64, ptParatab + RcvData->SubAddr, Buf);
	     	break;
        }

	    //设置一个参数表
        case 0x06:
        {
			*(ptParatab + RcvData->SubAddr) = (int)(((Uint16)RcvData->Buf[0] << 8) | (Uint16)RcvData->Buf[1]);
			FlagSystem.bit.ParaDataChg = 1;
			ParaTable.PowerReg.uParaTableVer++;
			break;
        }

        //同步保存数据
		case 0x64:
		{
			if(RcvData->Buf[1] == 0x01)
			{
				ExRamReg.FlagBlockSave = 1;
			}

			break;
		}

	    //设置一个监测点
		case 0x76:
		{
			ExRamReg.ArrayBlock[ExRamReg.CountBlock] = RcvData->Buf[0];
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->pt_addr = ptParatab + RcvData->SubAddr;											// 指定存储数据地址
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->buf_len = RcvData->Buf[1];														// 地址存储数据长度
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->pt_base_addr = (int *)(ExRamData + ExRamReg.ArrayBlock[ExRamReg.CountBlock] * 0x400);	// 指定外部存储区域

			ExRamReg.FlagDataSaveFinish = 0;
			ExRamReg.CountBlock++;
			if(ExRamReg.CountBlock >= 20)
			{
				ExRamReg.CountBlock = 19;
				RcvData->ModbusErr = EXCEPTION_CODE_1;
			}

			break;
		}

		// 读取指定内存区
		case 0x73:
		{
			CopyData(64, ExRamData + RcvData->Buf[0] * 1024 + i * 64, Buf);
			break;
		}

	    default:
	    	break;
	}
}

/*---------------------------------------------------------------------
函数原型：void ModbusCProcess(void)
函数功能：ModbusC接收处理
入口参数：
出口参数：
作    者：
日    期：2017.04.13
----------------------------------------------------------------------*/
void ModbusCProcess(void)
{
	ControlGPIOOut(SCI_CONC, SCI_CON_RECV);
	ModbusCData.ListenOk = ModbusCListen(&ModbusCData);			//作为从站，等待消息
	if(MODBUS_NO_ERR == ModbusCData.ListenOk)
	{
		ControlGPIOOut(SCI_CONC, SCI_CON_SEND);
		if(MODBUS_NO_ERR == ModbusCData.ModbusErr)
		{
			ModbusCReply(&ModbusCData);
		}
		else
		{
			ModbusCErrReply(&ModbusCData);
		}
		ModbusCData.ListenOk = 1;
	}
}

