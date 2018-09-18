/****************************************************************************************************************/
//                    DSP modbus��������
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
����ԭ�ͣ�uint16 crc16(uchar *puchMsg, Uint16 usDataLen) 
�������ƣ�CRCУ�����
�������ܣ�
��ڲ�����uchar *puchMsg	: ��У������ͷ
	   	   Uint16 usDataLen	: ���ݳ���
���ڲ�����У��ֵ��1���ֽ�
��    �ߣ�
��    �ڣ�2017.04.13
----------------------------------------------------------------------*/
Uint16 crc16(uchar *puchMsg, Uint16 usDataLen)
{
	Uint16  uchCRCHi = 0xFF ; 					//��CRC�ֽڳ�ʼ��
	Uint16  uchCRCLo = 0xFF ; 					//��CRC �ֽڳ�ʼ��
	Uint32  uIndex;          					// CRCѭ���е�����

	while (usDataLen--)      					// ������Ϣ������
	{ 
		uIndex = uchCRCHi ^ * puchMsg++; 		// ����CRC
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	} 
	return (((Uint16)uchCRCHi << 8) | uchCRCLo);

}

/*---------------------------------------------------------------------
����ԭ�ͣ�uchar CrcDetect(uchar *crc_buf, Uint16 len)
�������ƣ�CRCУ����֤
�������ܣ���CRCУ��ΪMODBUS�ӻ��������ݣ�len���ֽڣ�ǰlen-2�������ݣ���2��CRCУ��
��ڲ�����uchar *crc_buf	: ��У������ͷ
	   Uint16 len		: ���ݳ���
���ڲ���������1�ɹ���0ʧ��
��    �ߣ�
��    �ڣ�2017.04.13
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
����ԭ�ͣ�void InitModbusBData(void)
�������ƣ���ʼ��MODBUS�Ĵ�������
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2017.04.13
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
*                                        ====��Ϊ��վ===

******************************************************************************************************/

/*---------------------------------------------------------------------
����ԭ�ͣ�int ModbusBListen(struct MODBUS_REGS *RcvData)
�������ƣ�ModbusB����
�������ܣ���վ������վ������1��ʾ�ɹ�������һ����Ч֡��0��ʾ���ݴ���
��ڲ�����*RcvData  ���ݴ���ṹ��
���ڲ�������
��    �ߣ�
��    �ڣ�2018.01.13
----------------------------------------------------------------------*/
int ModbusBListen(struct MODBUS_REGS *RcvData)
{
    if(1 == FlagSciBRcvOK)
	{
		FlagSciBRcvOK = 0;
		if ((ModbusBRcvTemp[0] == RcvData->SlaveAddr) && CrcDetect(ModbusBRcvTemp, 8))
		{
	    	RcvData->FuncCode = ModbusBRcvTemp[1];												// ������
	        
		    switch (ModbusBRcvTemp[1])
		    {
		        // ������    [1 byte]: 0x03  �������ѯ
		        // ��ʼ��ַ [2 byte]: 0x0000 to 0x2400
		        // ��ȡ���� [2 byte]: 0x0001 to 0x0080(128)
		        case 0x03:
		        {
		            RcvData->SubAddr = ((Uint16)ModbusBRcvTemp[2] << 8) + ModbusBRcvTemp[3];	// Starting address
	                RcvData->Num   = ((Uint16)ModbusBRcvTemp[4] << 8) + ModbusBRcvTemp[5];	// Quantity of registers
	                if ((RcvData->SubAddr + RcvData->Num * 2) > READ_BUF_SIZE)				// �Ƿ����ݵ�ַ
	                {    
	                    RcvData->ModbusErr = EXCEPTION_CODE_2;
	                }
	                else if ((RcvData->Num <= 0) || ((RcvData->Num * 2) > 0x0080))			// �Ƿ�����ֵ
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;
					}
	                else
	                {
	                	RcvData->ModbusErr = MODBUS_NO_ERR;
	                }

	                return MODBUS_NO_ERR;
		        }

		        // ������ 	[1 byte]: 0x06 �������Ĵ�������
		        // ���õ�ַ [2 byte]: 0x0000 to 0xFFFF
		        // ����ֵ 	[2 byte]: 0x0000 to 0xFFFF
		        case 0x06:  // write single register
		        {
		            RcvData->SubAddr = ModbusBRcvTemp[3] + ((Uint16)ModbusBRcvTemp[2] << 8);
		            if (RcvData->SubAddr > WRITE_BUF_SIZE)
	                {
	                    RcvData->ModbusErr = EXCEPTION_CODE_2;								// �Ƿ����ݵ�ַ
	                }
		            else
		            {
		            	RcvData->ModbusErr = MODBUS_NO_ERR;
		            }
	                RcvData->Buf[0] = ModbusBRcvTemp[4];
	               	RcvData->Buf[1] = ModbusBRcvTemp[5];
					

	                return MODBUS_NO_ERR;
		        }

		        // ������     [1 byte]: 0x64  ͬ��ָ��
		        // �ڴ����� [1 byte]: 0x0000 to 0x0003
		        // ָ��        [2 byte]: 0x0000 to 0x0001
		        case 0x64:  // write single register
		        {
	                RcvData->Buf[0] = ModbusBRcvTemp[2];
	                RcvData->Buf[1] = ModbusBRcvTemp[3];
	                RcvData->Buf[2] = ModbusBRcvTemp[4];
	               	RcvData->Buf[3] = ModbusBRcvTemp[5];
	               	if(RcvData->Buf[1] > 3)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;								// �Ƿ�ָ��
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
		        }

		        // ������ 			[1 byte]: 0x76 ���ü���
		        // ���üĴ�����   	[2 byte]: 0x0000 to 0x2400
		        // ���ô洢�����	[1 byte]: 0x00 to 0x50(�ⲿ�洢�ռ�80K�ռ�)
		        // ����д������   	[1 byte]: 0x00 to 0x04(��λΪKByte�����д������4KByte)
		        case 0x76:
		        {
		            RcvData->SubAddr = ModbusBRcvTemp[3] + ((Uint16)ModbusBRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusBRcvTemp[4];			// �洢�����
	               	RcvData->Buf[1] = ModbusBRcvTemp[5];			// �洢����

	                if(RcvData->Buf[0] >= 0x50)
	                {
	                	RcvData->ModbusErr = EXCEPTION_CODE_1;	// �Ƿ��洢����
	                }
	                else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// �Ƿ����ݵ�ַ
					}
	                else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// �Ƿ����ݳ���
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
		        }

	            // ������ 		[1 byte]: 0x73 ����������
		        // ��ȡ��ַ�� 	[2 byte]: 0x0000 to 0x2400
		        // ��ȡ������ 	[2 byte]: 0x0000 to 0xFFFF(��λΪ64Byte)    
	            case 0x73:  	// �Զ��壬��ȡ�ⲿRAM���ݣ�
	            {
		            RcvData->SubAddr = ModbusBRcvTemp[3] + ((Uint16)ModbusBRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusBRcvTemp[4];		// ��ȡ�洢����
	               	RcvData->Buf[1] = ModbusBRcvTemp[5];		// ��ȡ����

	               	if(RcvData->Buf[0] >= 0x50)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_1;	// �Ƿ��洢����
					}
					else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// �Ƿ����ݵ�ַ
					}
					else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// �Ƿ����ݳ���
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}
					
	                return MODBUS_NO_ERR;
	            }

	            // ������ 		[1 byte]: 0x63 ����������
		        // ��ȡ��ַ�� 	[2 byte]: 0x0000 to 0x2400
		        // ��ȡ������ 	[2 byte]: 0x0000 to 0xFFFF(��λΪ64Byte)
	            case 0x63:  	// �Զ��壬��ȡ��ҪRAM���� ��128K
	            {
		            RcvData->SubAddr = ModbusBRcvTemp[3] + ((Uint16)ModbusBRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusBRcvTemp[4];		// ��ȡ�洢����
	               	RcvData->Buf[1] = ModbusBRcvTemp[5];		// ��ȡ����	1K

	               	if(RcvData->Buf[0] >= 0x80)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_1;	// �Ƿ��洢����
					}
					else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// �Ƿ����ݵ�ַ
					}
					else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// �Ƿ����ݳ���
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
	            }

		        default:
		        {
		        	RcvData->ModbusErr = EXCEPTION_CODE_1;					//�쳣�룬�Ƿ�������
		        	
		            return MODBUS_NO_ERR;
		        }
		    }
	    }
	    else
	    {
	        return ModbusErr_1;		// ��ַ��CRCУ�����
	    }
    }
    else
    {
        return ModbusErr_2;			// UARTδ���ջ���մ���
    }
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void ModbusBReply(struct MODBUS_REGS *RcvData)
�������ƣ�ModbusB��վ�ظ���վ
�������ܣ��������ж���������󣬴�վ�ظ���վ���������ݴ��ݺʹ�վ��ַ����
��ڲ�����*RcvData  ���ݴ���ṹ��
���ڲ�������
��    �ߣ�
��    �ڣ�2018.01.13
----------------------------------------------------------------------*/
void ModbusBReply(struct MODBUS_REGS *RcvData)
{
    Uint16 crcData = 0;
    Uint16 i = 0,j = 0;
	
    switch (RcvData->FuncCode)
    {
    	// ��׼MODBUSЭ�飬�����룺0x03, ��ȡ���ݣ��̶�һ֡64Byte
        case 0x03:
		{
			ModbusBUpdata(RcvData, &CrcBTempBuf[3],0);				//��������
			CrcBTempBuf[0] = RcvData->SlaveAddr;
			CrcBTempBuf[1] = 0x03;
			CrcBTempBuf[2] = RcvData->Num * 2;

			crcData = crc16(CrcBTempBuf, RcvData->Num * 2 + 3);
			CrcBTempBuf[RcvData->Num * 2 + 4] = (crcData >> 8) & 0xff;
			CrcBTempBuf[RcvData->Num * 2 + 5] = crcData & 0xff;

			for (i = 0; i < RcvData->Num * 2 + 5; i++)   //�Ĵ�������
			{
				ModbusBSendByte(CrcBTempBuf[i]);
			}

			break;
		}

        // ��׼MODBUSЭ�飬�����룺0x06, ����һ���Ĵ�������
        // ԭ�����ݻظ�
        case 0x06:
        {
        	ModbusBUpdata(RcvData, &CrcBTempBuf[3],0);				//��������
            CrcBTempBuf[0] = RcvData->SlaveAddr;
            CrcBTempBuf[1] = 0x06;
            CrcBTempBuf[2] = (uchar)(RcvData->SubAddr >> 8);
            CrcBTempBuf[3] = (uchar)RcvData->SubAddr;
            CrcBTempBuf[4] = RcvData->Buf[0];
            CrcBTempBuf[5] = RcvData->Buf[1];
			
			crcData = crc16(CrcBTempBuf, 6);
            CrcBTempBuf[6] = crcData >> 8;
            CrcBTempBuf[7] = crcData & 0xff;

            for (i = 0; i < 8; i++)         //�Ĵ�������
            {
            	ModbusBSendByte(CrcBTempBuf[i]);
            }

            break;
        }

        // �Զ���MODBUSЭ�飬�����룺0x64, ͬ��ָ�
        // ԭ�����ݻظ�
		case 0x64:
		{
			ModbusBUpdata(RcvData, &CrcBTempBuf[3],0);				//��������
			CrcBTempBuf[0] = RcvData->SlaveAddr;
			CrcBTempBuf[1] = 0x64;
			CrcBTempBuf[2] = RcvData->Buf[0];
			CrcBTempBuf[3] = RcvData->Buf[1];
			CrcBTempBuf[4] = ExRamReg.FlagDataSaveFinish;//point_save_finish_flag;//RcvData->Buf[2];
			CrcBTempBuf[5] = RcvData->Buf[3];

			crcData = crc16(CrcBTempBuf, 6);
			CrcBTempBuf[6] = (crcData >> 8) & 0xff;
			CrcBTempBuf[7] = crcData & 0xff;

			for (i = 0; i < 8; i++)         //�Ĵ�������
			{
				ModbusBSendByte(CrcBTempBuf[i]);
			}

			break;
		}

     	// �Զ���MODBUSЭ�飬�����룺0x76, ѡ��һ��������Ϊ���㣻
     	// ԭ�����ݻظ�
     	case 0x76:
     	{
     		ModbusBUpdata(RcvData, &CrcBTempBuf[3],0);				//��������
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

        // �Զ���MODBUSЭ�飬�����룺0x73, ��ȡ��֡���ݣ�һ֡64Byte
     	// ��ȡ1K���ݣ��ظ�16֡   
        case 0x73:
        {
        	for(i = RcvData->Buf[1] - 1; i < 16; i++)
        	{
        		ModbusBUpdata(RcvData, CrcBTempBuf + 4, i);			//��������

        		CrcBTempBuf[0] = 0x01;
	            CrcBTempBuf[1] = 0x73;
	            CrcBTempBuf[2] = RcvData->Buf[0];					//�洢����ţ�
	            CrcBTempBuf[3] = i;									//��֡���ݵ�֡ID��

	        	crcData = crc16(CrcBTempBuf, (64 * 2 + 4));
	        	CrcBTempBuf[64 * 2 + 4] = (crcData >> 8) & 0xff;
	        	CrcBTempBuf[64 * 2 + 5] = crcData & 0xff;

	            for (j = 0; j < (64 * 2 + 6);j++)   				//�Ĵ�������
	            {
	                ModbusBSendByte(CrcBTempBuf[j]);
	            }
        	}

            break;
        }

        // �Զ���MODBUSЭ�飬��ȡ��Ҫ���ݹ����룺0x63, ��ȡ��֡���ݣ�һ֡64Byte
     	// ��ȡ1K���ݣ��ظ�16֡
        case 0x63:
        {
        	for(i = RcvData->Buf[1] - 1; i < 16; i++)
        	{
        		ModbusBUpdata(RcvData, CrcBTempBuf + 4, i);			//��������

        		CrcBTempBuf[0] = 0x01;
	            CrcBTempBuf[1] = 0x63;
	            CrcBTempBuf[2] = RcvData->Buf[0];					//�洢����ţ�
	            CrcBTempBuf[3] = i;									//��֡���ݵ�֡ID��

	        	crcData = crc16(CrcBTempBuf, (64 * 2 + 4));
	        	CrcBTempBuf[64 * 2 + 4] = (crcData >> 8) & 0xff;
	        	CrcBTempBuf[64 * 2 + 5] = crcData & 0xff;
	        	
	            for (j = 0; j < (64 * 2 + 6);j++)   			//�Ĵ�������
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
����ԭ�ͣ�void ModbusBErrReply(struct MODBUS_REGS *RcvData)
�������ƣ�ModbusB��վ�ظ���վ
�������ܣ���վ������վʧ�ܣ�����ظ�
��ڲ�����*RcvData  ���ݴ���ṹ��
���ڲ�������
��    �ߣ�
��    �ڣ�2018.01.13
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
	
	for (i = 0; i < 5; i++)         							//�Ĵ�������
    {
        ModbusBSendByte(ErrCrcBTempBuf[i]);
    }
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void ModbusBUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i)
�������ƣ�����modbus����
�������ܣ������Ӧ����������ݸ���
��ڲ�����	struct MODBUS_REGS *RcvData 	���ݴ��ݽṹ��
			uchar *Buf						д/�� ����
			Uint16 i						�ظ�����
���ڲ�������
��    �ߣ�
��    �ڣ�2017.04.13
----------------------------------------------------------------------*/
void ModbusBUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i)
{
	int *ptParatab = (int *)&ParaTable;					//�������׵�ַָ�룻

	switch (RcvData->FuncCode)
    {
    	//��ȡ������
        case 0x03:
        {
			CopyData(64, ptParatab + RcvData->SubAddr, Buf);
	     	break;
        }

	    //����һ��������
        case 0x06:
        {
			*(ptParatab + RcvData->SubAddr) = (int)(((Uint16)RcvData->Buf[0] << 8) | (Uint16)RcvData->Buf[1]);
			FlagSystem.bit.ParaDataChg = 1;
			ParaTable.PowerReg.uParaTableVer++;
			break;
        }

        //ͬ����������
		case 0x64:
		{
			if(RcvData->Buf[1] == 0x01)
			{
				ExRamReg.FlagBlockSave = 1;
			}

			break;
		}

	    //����һ������
		case 0x76:
		{
			ExRamReg.ArrayBlock[ExRamReg.CountBlock] = RcvData->Buf[0];
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->pt_addr = ptParatab + RcvData->SubAddr;											// ָ���洢���ݵ�ַ
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->buf_len = RcvData->Buf[1];														// ��ַ�洢���ݳ���
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->pt_base_addr = (int *)(ExRamData + ExRamReg.ArrayBlock[ExRamReg.CountBlock] * 0x400);	// ָ���ⲿ�洢����

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
				(EmgcRamCtrl + EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock])->pt_addr = ptParatab + RcvData->SubAddr;											// ָ���洢���ݵ�ַ
				(EmgcRamCtrl + EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock])->buf_len = 16;			//RcvData->Buf[1];											// ��ַ�洢���ݳ��� �̶�16K
				(EmgcRamCtrl + EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock])->pt_base_addr = (int *)(EmRamData + EmgcRamReg.ArrayBlock[EmgcRamReg.CountBlock] * 0x400);	// ָ���ⲿ�洢����

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

		// ��ȡָ���ڴ���EXRAM
		case 0x73:
		{
			CopyData(64, ExRamData + RcvData->Buf[0] * 1024 + i * 64, Buf);
			break;
		}

		// ��ȡָ���ڴ���EMRAM
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
����ԭ�ͣ�void modbus_process(void)
�������ܣ�modbus���մ���
��ڲ�����
���ڲ�����
��    �ߣ�
��    �ڣ�2017.04.13
----------------------------------------------------------------------*/
void ModbusBProcess(void)
{
	GPIOOutOfDSP(SCI_CONB, SCI_CON_RECV);
	ModbusBData.ListenOk = ModbusBListen(&ModbusBData);			//��Ϊ��վ���ȴ���Ϣ
	if(MODBUS_NO_ERR == ModbusBData.ListenOk)
	{
		GPIOOutOfDSP(SCI_CONB, SCI_CON_SEND);
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

/******************************************************************************************************
*                                             MOBUSC
*                                        ====��Ϊ��վ===

******************************************************************************************************/
/*---------------------------------------------------------------------
����ԭ�ͣ�int ModbusCListen(struct MODBUS_REGS *RcvData)
�������ƣ�ModbusC����
�������ܣ���վ������վ������1��ʾ�ɹ�������һ����Ч֡��0��ʾ���ݴ���
��ڲ�����*RcvData  ���ݴ���ṹ��
���ڲ�������
��    �ߣ�
��    �ڣ�2018.01.13
----------------------------------------------------------------------*/
int ModbusCListen(struct MODBUS_REGS *RcvData)
{
    if(1 == FlagSciCRcvOK)
	{
    	FlagSciCRcvOK = 0;
		if ((ModbusCRcvTemp[0] == RcvData->SlaveAddr) && CrcDetect(ModbusCRcvTemp, 8))
		{
	    	RcvData->FuncCode = ModbusCRcvTemp[1];												// ������

		    switch (ModbusCRcvTemp[1])
		    {
		        // ������    [1 byte]: 0x03  �������ѯ
		        // ��ʼ��ַ [2 byte]: 0x0000 to 0x2400
		        // ��ȡ���� [2 byte]: 0x0001 to 0x0080(128)
		        case 0x03:
		        {
		            RcvData->SubAddr = ((Uint16)ModbusCRcvTemp[2] << 8) + ModbusCRcvTemp[3];	// Starting address
	                RcvData->Num   = ((Uint16)ModbusCRcvTemp[4] << 8) + ModbusCRcvTemp[5];	// Quantity of registers
	                if ((RcvData->SubAddr + RcvData->Num * 2) > READ_BUF_SIZE)				// �Ƿ����ݵ�ַ
	                {
	                    RcvData->ModbusErr = EXCEPTION_CODE_2;
	                }
	                else if ((RcvData->Num <= 0) || ((RcvData->Num * 2) > 0x0080))			// �Ƿ�����ֵ
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;
					}
	                else
	                {
	                	RcvData->ModbusErr = MODBUS_NO_ERR;
	                }

	                return MODBUS_NO_ERR;
		        }

		        // ������ 	[1 byte]: 0x06 �������Ĵ�������
		        // ���õ�ַ   [2 byte]: 0x0000 to 0xFFFF
		        // ����ֵ 	[2 byte]: 0x0000 to 0xFFFF
		        case 0x06:  // write single register
		        {
		            RcvData->SubAddr = ModbusCRcvTemp[3] + ((Uint16)ModbusCRcvTemp[2] << 8);
		            if (RcvData->SubAddr > WRITE_BUF_SIZE)
	                {
	                    RcvData->ModbusErr = EXCEPTION_CODE_2;								// �Ƿ����ݵ�ַ
	                }
		            else
		            {
		            	RcvData->ModbusErr = MODBUS_NO_ERR;
		            }
	                RcvData->Buf[0] = ModbusCRcvTemp[4];
	               	RcvData->Buf[1] = ModbusCRcvTemp[5];


	                return MODBUS_NO_ERR;
		        }

		        // ������     [1 byte]: 0x64  ͬ��ָ��
		        // �ڴ����� [1 byte]: 0x0000 to 0x0003
		        // ָ��        [2 byte]: 0x0000 to 0x0001
		        case 0x64:  // write single register
		        {
	                RcvData->Buf[0] = ModbusCRcvTemp[2];
	                RcvData->Buf[1] = ModbusCRcvTemp[3];
	                RcvData->Buf[2] = ModbusCRcvTemp[4];
	               	RcvData->Buf[3] = ModbusCRcvTemp[5];
	               	if(RcvData->Buf[1] > 3)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;								// �Ƿ�ָ��
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
		        }

		        // ������ 			[1 byte]: 0x76 ���ü���
		        // ���üĴ�����   	[2 byte]: 0x0000 to 0x2400
		        // ���ô洢�����	[1 byte]: 0x00 to 0x50(�ⲿ�洢�ռ�80K�ռ�)
		        // ����д������   	[1 byte]: 0x00 to 0x04(��λΪKByte�����д������4KByte)
		        case 0x76:
		        {
		            RcvData->SubAddr = ModbusCRcvTemp[3] + ((Uint16)ModbusCRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusCRcvTemp[4];			// �洢�����
	               	RcvData->Buf[1] = ModbusCRcvTemp[5];			// �洢����

	                if(RcvData->Buf[0] >= 0x50)
	                {
	                	RcvData->ModbusErr = EXCEPTION_CODE_1;	// �Ƿ��洢����
	                }
	                else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// �Ƿ����ݵ�ַ
					}
	                else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// �Ƿ����ݳ���
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
		        }

	            // ������ 		[1 byte]: 0x73 ����������
		        // ��ȡ��ַ�� 	[2 byte]: 0x0000 to 0x2400
		        // ��ȡ������ 	[2 byte]: 0x0000 to 0xFFFF(��λΪ64Byte)
	            case 0x73:  	// �Զ��壬��ȡ�ⲿRAM���ݣ�
	            {
		            RcvData->SubAddr = ModbusCRcvTemp[3] + ((Uint16)ModbusCRcvTemp[2] << 8);
	                RcvData->Buf[0] = ModbusCRcvTemp[4];		// ��ȡ�洢����
	               	RcvData->Buf[1] = ModbusCRcvTemp[5];		// ��ȡ����

	               	if(RcvData->Buf[0] >= 0x50)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_1;	// �Ƿ��洢����
					}
					else if (RcvData->SubAddr > WRITE_BUF_SIZE)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_2;	// �Ƿ����ݵ�ַ
					}
					else if(RcvData->Buf[1] > 0x10)
					{
						RcvData->ModbusErr = EXCEPTION_CODE_3;	// �Ƿ����ݳ���
					}
					else
					{
						RcvData->ModbusErr = MODBUS_NO_ERR;
					}

	                return MODBUS_NO_ERR;
	            }

		        default:
		        {
		        	RcvData->ModbusErr = EXCEPTION_CODE_1;					//�쳣�룬�Ƿ�������

		            return MODBUS_NO_ERR;
		        }
		    }
	    }
	    else
	    {
	        return ModbusErr_1;		// ��ַ��CRCУ�����
	    }
    }
    else
    {
        return ModbusErr_2;			// UARTδ���ջ���մ���
    }
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void ModbusCReply(struct MODBUS_REGS *RcvData)
�������ƣ�ModbusC��վ�ظ���վ
�������ܣ��������ж���������󣬴�վ�ظ���վ���������ݴ��ݺʹ�վ��ַ����
��ڲ�����*RcvData  ���ݴ���ṹ��
���ڲ�������
��    �ߣ�
��    �ڣ�2018.01.13
----------------------------------------------------------------------*/
void ModbusCReply(struct MODBUS_REGS *RcvData)
{
    Uint16 crcData = 0;
    Uint16 i = 0,j = 0;

    switch (RcvData->FuncCode)
    {
    	// ��׼MODBUSЭ�飬�����룺0x03, ��ȡ���ݣ��̶�һ֡64Byte
        case 0x03:
		{
			ModbusCUpdata(RcvData, &CrcCTempBuf[3],0);				//��������
			CrcCTempBuf[0] = RcvData->SlaveAddr;
			CrcCTempBuf[1] = 0x03;
			CrcCTempBuf[2] = RcvData->Num * 2;

			crcData = crc16(CrcCTempBuf, RcvData->Num * 2 + 3);
			CrcCTempBuf[RcvData->Num * 2 + 4] = (crcData >> 8) & 0xff;
			CrcCTempBuf[RcvData->Num * 2 + 5] = crcData & 0xff;

			for (i = 0; i < RcvData->Num * 2 + 5; i++)   //�Ĵ�������
			{
				ModbusCSendByte(CrcCTempBuf[i]);
			}

			break;
		}

        // ��׼MODBUSЭ�飬�����룺0x06, ����һ���Ĵ�������
        // ԭ�����ݻظ�
        case 0x06:
        {
        	ModbusCUpdata(RcvData, &CrcCTempBuf[3],0);				//��������
        	CrcCTempBuf[0] = RcvData->SlaveAddr;
        	CrcCTempBuf[1] = 0x06;
        	CrcCTempBuf[2] = (uchar)(RcvData->SubAddr >> 8);
        	CrcCTempBuf[3] = (uchar)RcvData->SubAddr;
        	CrcCTempBuf[4] = RcvData->Buf[0];
        	CrcCTempBuf[5] = RcvData->Buf[1];

			crcData = crc16(CrcCTempBuf, 6);
			CrcCTempBuf[6] = crcData >> 8;
			CrcCTempBuf[7] = crcData & 0xff;

            for (i = 0; i < 8; i++)         //�Ĵ�������
            {
            	ModbusCSendByte(CrcCTempBuf[i]);
            }

            break;
        }

        // �Զ���MODBUSЭ�飬�����룺0x64, ͬ��ָ�
        // ԭ�����ݻظ�
		case 0x64:
		{
			ModbusCUpdata(RcvData, &CrcCTempBuf[3],0);				//��������
			CrcCTempBuf[0] = RcvData->SlaveAddr;
			CrcCTempBuf[1] = 0x64;
			CrcCTempBuf[2] = RcvData->Buf[0];
			CrcCTempBuf[3] = RcvData->Buf[1];
			CrcCTempBuf[4] = ExRamReg.FlagDataSaveFinish;//point_save_finish_flag;//RcvData->Buf[2];
			CrcCTempBuf[5] = RcvData->Buf[3];

			crcData = crc16(CrcCTempBuf, 6);
			CrcCTempBuf[6] = (crcData >> 8) & 0xff;
			CrcCTempBuf[7] = crcData & 0xff;

			for (i = 0; i < 8; i++)         //�Ĵ�������
			{
				ModbusCSendByte(CrcCTempBuf[i]);
			}

		break;
		}

     	// �Զ���MODBUSЭ�飬�����룺0x76, ѡ��һ��������Ϊ���㣻
     	// ԭ�����ݻظ�
     	case 0x76:
     	{
     		ModbusCUpdata(RcvData, &CrcCTempBuf[3],0);				//��������
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

        // �Զ���MODBUSЭ�飬�����룺0x73, ��ȡ��֡���ݣ�һ֡64Byte
     	// ��ȡ1K���ݣ��ظ�16֡
        case 0x73:
        {
        	for(i = RcvData->Buf[1] - 1; i < 16; i++)
        	{
        		ModbusCUpdata(RcvData, CrcCTempBuf + 4, i);			//��������

        		CrcCTempBuf[0] = 0x01;
        		CrcCTempBuf[1] = 0x73;
        		CrcCTempBuf[2] = RcvData->Buf[0];				//�洢����ţ�
        		CrcCTempBuf[3] = i;							//��֡���ݵ�֡ID��

	        	crcData = crc16(CrcCTempBuf, (64 * 2 + 4));
	        	CrcCTempBuf[64 * 2 + 4] = (crcData >> 8) & 0xff;
	        	CrcCTempBuf[64 * 2 + 5] = crcData & 0xff;

	            for (j = 0; j < (64 * 2 + 6);j++)   			//�Ĵ�������
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
����ԭ�ͣ�void ModbusCErrReply(struct MODBUS_REGS *RcvData)
�������ƣ�ModbusC��վ�ظ���վ
�������ܣ���վ������վʧ�ܣ�����ظ�
��ڲ�����*RcvData  ���ݴ���ṹ��
���ڲ�������
��    �ߣ�
��    �ڣ�2018.01.13
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

	for (i = 0; i < 5; i++)         							//�Ĵ�������
    {
        ModbusCSendByte(ErrCrcCTempBuf[i]);
    }
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void ModbusCUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i)
�������ƣ�����modbus����
�������ܣ������Ӧ����������ݸ���
��ڲ�����	struct MODBUS_REGS *RcvData 	���ݴ��ݽṹ��
			uchar *Buf						д/�� ����
			Uint16 i						�ظ�����
���ڲ�������
��    �ߣ�
��    �ڣ�2017.04.13
----------------------------------------------------------------------*/
void ModbusCUpdata(struct MODBUS_REGS *RcvData, uchar *Buf, Uint16 i)
{
	int *ptParatab = (int *)&ParaTable;					//�������׵�ַָ�룻

	switch (RcvData->FuncCode)
    {
    	//��ȡ������
        case 0x03:
        {
			CopyData(64, ptParatab + RcvData->SubAddr, Buf);
	     	break;
        }

	    //����һ��������
        case 0x06:
        {
			*(ptParatab + RcvData->SubAddr) = (int)(((Uint16)RcvData->Buf[0] << 8) | (Uint16)RcvData->Buf[1]);
			FlagSystem.bit.ParaDataChg = 1;
			ParaTable.PowerReg.uParaTableVer++;
			break;
        }

        //ͬ����������
		case 0x64:
		{
			if(RcvData->Buf[1] == 0x01)
			{
				ExRamReg.FlagBlockSave = 1;
			}

			break;
		}

	    //����һ������
		case 0x76:
		{
			ExRamReg.ArrayBlock[ExRamReg.CountBlock] = RcvData->Buf[0];
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->pt_addr = ptParatab + RcvData->SubAddr;											// ָ���洢���ݵ�ַ
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->buf_len = RcvData->Buf[1];														// ��ַ�洢���ݳ���
			(ExRamCtrl + ExRamReg.ArrayBlock[ExRamReg.CountBlock])->pt_base_addr = (int *)(ExRamData + ExRamReg.ArrayBlock[ExRamReg.CountBlock] * 0x400);	// ָ���ⲿ�洢����

			ExRamReg.FlagDataSaveFinish = 0;
			ExRamReg.CountBlock++;
			if(ExRamReg.CountBlock >= 20)
			{
				ExRamReg.CountBlock = 19;
				RcvData->ModbusErr = EXCEPTION_CODE_1;
			}

			break;
		}

		// ��ȡָ���ڴ���
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
����ԭ�ͣ�void ModbusCProcess(void)
�������ܣ�ModbusC���մ���
��ڲ�����
���ڲ�����
��    �ߣ�
��    �ڣ�2017.04.13
----------------------------------------------------------------------*/
void ModbusCProcess(void)
{
//	ControlGPIOOut(SCI_CONC, SCI_CON_RECV);
	ModbusCData.ListenOk = ModbusCListen(&ModbusCData);			//��Ϊ��վ���ȴ���Ϣ
	if(MODBUS_NO_ERR == ModbusCData.ListenOk)
	{
//		ControlGPIOOut(SCI_CONC, SCI_CON_SEND);
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

