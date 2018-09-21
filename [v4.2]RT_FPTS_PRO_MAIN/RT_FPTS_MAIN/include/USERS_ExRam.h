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
	Uint16 WRen;			//��дʹ��
	Uint16 buf_len;			//���������ȣ�1��1K��2��2K��
	Uint16 addr_count;
	Uint16 buf_state;		//����ȥ�Ƿ����
	int *pt_base_addr;		//�������ĵ�ַ
	int *pt_addr;			//�������ĵ�ַ
	int *pt_data;			//���ݵ�ָ��
};

/*---------------------------------------------------------------------
����ԭ�ͣ�Uint16 T1VarSave(void)
�����ƣ��洢��λ��ָ������
�������ܣ�
��ڲ�����
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
Uint16 	T1VarSave(void);

/*---------------------------------------------------------------------
����ԭ�ͣ�void T1EmergencySave(Uint16 ExFinshSaveFlag)
�������ƣ�����״̬��������
�������ܣ���Ӳ������ʱ��������Ҫ������Ϣ�����ڹ��Ϸ���
��ڲ�����ExFinshSaveFlag--ֹͣ��������
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void 	T1EmergencySave(Uint16 ExFinshSaveFlag);

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitExRam(void)
�������ƣ���ʼ���ⲿRam
�������ܣ���ExRam��ʼ��Ϊ0
��ڲ�����
���ڲ�����
��    �ߣ�
��    �ڣ�2017.04.13
----------------------------------------------------------------------*/
void InitExRam(void);

/*---------------------------------------------------------------------
����ԭ�ͣ�void CopyData(Uint16 lenth, int *read_buf, uchar *write_buf)
�������ƣ����ⲿRAM������
�������ܣ����ⲿRAM�Ļ�����read_buf ��ȡ���ݸ��Ƶ� write_buf
��ڲ�����	Uint16 lenth, 		���ݳ���
			int *read_buf, 		Դ��ַ
			uchar *write_buf	Ŀ�ĵ�ַ
���ڲ�������
��    �ߣ�
��    �ڣ�2017.04.13
----------------------------------------------------------------------*/
void CopyData(Uint16 lenth, int *read_buf, uchar *write_buf);

/*---------------------------------------------------------------------
����ԭ�ͣ�Uint16 EmDataWriteToFlash(void)
�������ƣ����������ڴ����Ҫ����д��Flash
�������ܣ�
��ڲ�������
���ڲ�����WriteOK��1 -- ��д�ɹ�
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
Uint16 EmDataWriteToFlash(void);

/*---------------------------------------------------------------------
����ԭ�ͣ�void ReadEmDataFromFlash(void)
�������ƣ���FLASH�ж�ȡ��һ�α������Ҫ����
�������ܣ��ϵ�ʱ����ȡ��һ�ι�������
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void ReadEmDataFromFlash(void);


#endif
