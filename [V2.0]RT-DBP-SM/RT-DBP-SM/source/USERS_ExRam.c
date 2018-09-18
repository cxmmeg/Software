#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include <string.h>

#pragma DATA_SECTION(ExRamCtrl,".ExRamCtrl")  	// �����ⲿ�洢���ݵĿ�������
struct ExRamCtrl ExRamCtrl[80];

#pragma DATA_SECTION(EmgcRamCtrl,".ExRamCtrl")  // �����ⲿ�洢���ݵĿ�������
struct ExRamCtrl EmgcRamCtrl[128];

#pragma DATA_SECTION(ExRamData,".ExRamData")  	// �����ⲿ�洢���飻80KByte
int ExRamData[81920];

#pragma DATA_SECTION(EmRamData,".EmRamData")  	// �����ⲿ�洢���飻128KByte
int EmRamData[131072];

#pragma DATA_SECTION(ExRamReg,".SciData")
struct SAVE_EXRAM_REGS ExRamReg;

#pragma DATA_SECTION(EmgcRamReg,".SciData")
struct SAVE_EXRAM_REGS EmgcRamReg;

extern union  FLAG_SYSTEM_RUNERR 		FlagSysRunErr;
extern int16 *EmDataAddrArray[8];
/*---------------------------------------------------------------------
����ԭ�ͣ�void InitExRam(void)
�������ƣ���ʼ���ⲿRam
�������ܣ���ExRam��ʼ��Ϊ0
��ڲ�����
���ڲ�����
��    �ߣ�
��    �ڣ�2017.04.13
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
����ԭ�ͣ�void CopyData(Uint16 lenth, int *ReadBuf, uchar *WriteBuf)
�������ƣ����ⲿRAM������
�������ܣ����ⲿRAM�Ļ�����read_buf ��ȡ���ݸ��Ƶ� write_buf
��ڲ�����	Uint16 lenth, 		���ݳ���
			int *ReadBuf, 		Դ��ַ
			uchar *WriteBuf	Ŀ�ĵ�ַ
���ڲ�������
��    �ߣ�
��    �ڣ�2017.04.13
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
����ԭ�ͣ�Uint16 T1VarSave(void)
�����ƣ��洢��λ��ָ������
�������ܣ�
��ڲ�����
���ڲ�������
��    �ߣ�
��    �ڣ�
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
����ԭ�ͣ�void T1EmergencySave(Uint16 FlagExFinsh)
�������ƣ�����״̬��������
�������ܣ���Ӳ������ʱ��������Ҫ������Ϣ�����ڹ��Ϸ���
��ڲ�����FlagExFinsh--ֹͣ��������
���ڲ�������
��    �ߣ�
��    �ڣ�
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
����ԭ�ͣ�Uint16 EmDataWriteToFlash(void)
�������ƣ����������ڴ����Ҫ����д��Flash
�������ܣ�
��ڲ�������
���ڲ�����WriteOK��1 -- ��д�ɹ�
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
Uint16 EmDataWriteToFlash(void)
{
	int *PointEm, *PointPara;

	PointEm = (int16 *)&EmRamData;							//��ȡ��������׵�ַ��
	PointPara =  (int16 *)&ParaTable;

	if ((1 == FlagSysRunErr.bit.FlagPDP) && (1 == EmgcRamReg.FlagDataSaveFinish))
	{
		PieCtrlRegs.PIEIER3.bit.INTx1 = 0;
/*		BlockErase(0x01);		// ÿ�β���32K  0x8000  - 0x10000  ����:ֻ������ǰ32K Ϊʲô��
		DelayUs(100);
		BlockErase(0x02);		// ÿ�β���32K  0x10000 - 0x18000
		DelayUs(100);
		BlockErase(0x03);		// ÿ�β���32K  0x18000 - 0x20000
		DelayUs(100);
		BlockErase(0x04);		// ÿ�β���32K  0x20000 - 0x28000
		DelayUs(100);
*/
		ChipErase();
		DelayUs(100);

		FlashWrite(PointPara, 0x00, sizeof(ParaTable));     // ����������д��flash��
		DelayUs(100);

		FlashWrite(PointEm,0x8000,sizeof(EmRamData));		// ǰ32K�ռ�洢������
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
����ԭ�ͣ�void ReadEmDataFromFlash(void)
�������ƣ���FLASH�ж�ȡ��һ�α������Ҫ����
�������ܣ��ϵ�ʱ����ȡ��һ�ι�������
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void ReadEmDataFromFlash(void)
{
	int *Point, *EmAddrPoint, *ParaPoint;
	int i;

	Point = (int *)&EmRamData;					    //��ȡ��Ҫ���ݱ���������׵�ַ��
	FlashRead(Point,0x8000,sizeof(EmRamData));		//��flash��ȡ�������Ҫ���ݣ�

	ParaPoint = (int *)&ParaTable;                      //�������׵�ַָ�룻
    EmAddrPoint = (int *)ParaTable.EmergencyReg.uEmData0Addr;
    for (i = 0; i < 8; i++, EmAddrPoint++)
    {
        EmDataAddrArray[i] = EmAddrPoint;

        EmgcRamReg.ArrayBlock[i] = i * 16;
        (EmgcRamCtrl + EmgcRamReg.ArrayBlock[i])->pt_addr = ParaPoint + (int16)(int32)EmDataAddrArray[i];                            // ָ���洢���ݵ�ַ
        (EmgcRamCtrl + EmgcRamReg.ArrayBlock[i])->buf_len = 16;                                                             // ��ַ�洢���ݳ��� �̶�16K
        (EmgcRamCtrl + EmgcRamReg.ArrayBlock[i])->pt_base_addr = (int *)(EmRamData + EmgcRamReg.ArrayBlock[i] * 0x400);   // ָ���ⲿ�洢����
    }
}
