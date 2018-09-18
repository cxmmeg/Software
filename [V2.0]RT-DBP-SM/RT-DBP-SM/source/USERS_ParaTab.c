/*****************ͷ�ļ�********************/ 
#include <string.h>
#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h"   

#pragma DATA_SECTION(ParaTable,".ParaTab")
struct PARAMETER_REGS ParaTable;

extern enum   SYSTEM_STATE 	SystemState;
extern union  FLAG_SYSTEM   FlagSystem;
/*---------------------------------------------------------------------
����ԭ�ͣ�ReadParaFromFlash
�������ƣ�read the parameter from the flash;
if the flash is not empty, then read the parameter from the flash and then run;
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void ReadParaFromFlash(void)
{
	int *ParPoint;
	ParPoint = (int *)&ParaTable;					//��ȡ���������׵�ַ��
	FlashRead(ParPoint,0x0,sizeof(ParaTable));		//��flash��ȡ������������
}

/*---------------------------------------------------------------------
����ԭ�ͣ�WtiteDefaultToFlash
�������ƣ���ʼ����������������������д��flash
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void WtiteDefaultToFlash(void)
{
	int *ParPoint;
	int32 temp;

	ParPoint = (int *)&ParaTable;
	memset(&ParaTable,0x00,sizeof(ParaTable));

	// ����������
	ParaTable.PowerReg.uInputType 		= 1;			// �����Դ���� 0-AC��1-DC
	ParaTable.PowerReg.uInputFre 		= 0;			// �����Դ�Ƶ�ʣ���λ0.1Hz
	ParaTable.PowerReg.uInputVol		= 5380;			// �����Դ���ѹ����λ0.1V
	ParaTable.PowerReg.uInputCur		= 7000;			// �����Դ���������λ0.1A

	ParaTable.PowerReg.uOutputType 		= 0;			// �����Դ����
	ParaTable.PowerReg.uOutputFre 		= 500;			// �����Դ�Ƶ�ʣ���λ0.1Hz
	ParaTable.PowerReg.uOutputVol		= 4400;			// �����Դ���ѹ����λ0.1V
	ParaTable.PowerReg.uOutputCur		= 3280;			// �����Դ���������λ0.1A
	temp = (int32)ParaTable.PowerReg.uOutputVol * ParaTable.PowerReg.uOutputCur;
	ParaTable.PowerReg.uOutputPower 	= temp * 0.01732;// �����Դ���� ʵ��ֵ����λ1W

	ParaTable.PowerReg.uBoostVol 		= 6600;			// Boost��ѹ���ѹ����λ0.1V
	ParaTable.PowerReg.uBoostCur		= 2800;			// Boost��ѹ���������λ0.1A
	ParaTable.PowerReg.uSpeedMax		= 1000;			// ��Ƶ�����ص�����ת�٣���λ1r/s
	ParaTable.PowerReg.uSpeedMin		= 100;			// ��Ƶ�����ص�����ת�٣���λ1r/s
	ParaTable.PowerReg.uSpeedSet		= 800;			// ��Ƶ�����ص���趨ת�٣���λ1r/s
	ParaTable.PowerReg.uTempNom			= 750;			// �����¶�ֵ����λ0.1��

	ParaTable.PowerReg.uPWMSwitchFreq 		= 6400;			// PWM����Ƶ�ʣ���λ1Hz
	ParaTable.PowerReg.uPWMSwitchPeriod		= 1E6 / 6400;	// PWM�������ڣ���λ1us
	ParaTable.PowerReg.uPWMSwitchDeadTime	= 2200;			// PWM��������ʱ�䣬��λ1ns

	ParaTable.PowerReg.uBoostSwitchFreq		= 25600;		// Boost����Ƶ�ʣ���λ1Hz
	ParaTable.PowerReg.uBoostSwitchPeriod	= 1E6 / 25600;	// Boost�������ڣ���λ1us
	ParaTable.PowerReg.uBoostSwitchDeadTime	= 500;			// Boost��������ʱ�䣬��λ1ns

	ParaTable.PowerReg.uParaTableVer		= 1;			// ��ʼ��Ϊ1
    ParaTable.PowerReg.uHardwareVer         = 20;           // Ӳ���汾�ţ����V2.0�汾
	ParaTable.PowerReg.uSoftwareVer		    = 18081;		// �����汾�ţ�xx��xx�µ�x���޸�

	// PI�������
	ParaTable.PIReg.uBoostVolKp		= 3000;			// Boost��ѹ��
	ParaTable.PIReg.uBoostVolKi		= 10;			// Q12
	ParaTable.PIReg.uBoostVolKd 	= 10; 			// Q12
	ParaTable.PIReg.uBoostVolPIMax	= 4000; 		// Q12
	ParaTable.PIReg.uBoostVolPIMin	= 10; 			// Q12

	ParaTable.PIReg.uOutVolKp 		= 3000; 		// �����ѹ��
	ParaTable.PIReg.uOutVolKi 		= 10;			// Q12
	ParaTable.PIReg.uOutVolKd 		= 10;			// Q12
	ParaTable.PIReg.uOutVolPIMax 	= 4000;			// Q12
	ParaTable.PIReg.uOutVolPIMin 	= -4000; 		// Q12

	ParaTable.PIReg.uOutCurKp 		= 3000; 		// ���������
	ParaTable.PIReg.uOutCurKi 		= 10;			// Q12
	ParaTable.PIReg.uOutCurKd 		= 10;			// Q12
	ParaTable.PIReg.uOutCurPIMax 	= 4000;			// Q12
	ParaTable.PIReg.uOutCurPIMin 	= -4000; 		// Q12

	ParaTable.PIReg.uOutPLLKp 		= 3000; 		// �����ѹ���໷
	ParaTable.PIReg.uOutPLLKi 		= 10;			// Q12
	ParaTable.PIReg.uOutPLLKd 		= 10;			// Q12
	ParaTable.PIReg.uOutPLLPIMax 	= 4000;			// Q12
	ParaTable.PIReg.uOutPLLPIMin 	= -4000; 		// Q12

	// ���п��Ʋ���Ĭ��ֵ
	ParaTable.RunCtrlReg.uInVolPreChg		= 3481;		// �����ѹԤ����ѹֵ��85%380V   Q12
	ParaTable.RunCtrlReg.uInVolHighAlarm	= 4505;		// �����ѹ����ֵ��110%380V  Q12
	ParaTable.RunCtrlReg.uInVolHighCut		= 5324;		// �����ѹ�ж�ֵ��130%380V  Q12
	ParaTable.RunCtrlReg.uInVolLowAlarm		= 3686;		// ����Ƿѹ����ֵ��90%380V   Q12
	ParaTable.RunCtrlReg.uInVolLowCut		= 3276;		// ����Ƿѹ�ж�ֵ��80%380V   Q12
	ParaTable.RunCtrlReg.uInCurHighAlarm	= 4505;		// �����������ֵ��110%380V  Q12
	ParaTable.RunCtrlReg.uInCurHighCut		= 5324;		// ��������ж�ֵ��130%380V  Q12

	ParaTable.RunCtrlReg.uBoostVolHighAlarm	= 4505;		// Boost��ѹ��ѹ����ֵ��110%380V  Q12
	ParaTable.RunCtrlReg.uBoostVolHighCut	= 5324;		// Boost��ѹ��ѹ�ж�ֵ��130%380V  Q12
	ParaTable.RunCtrlReg.uBoostCurHighAlarm	= 4505;		// Boost��ѹ��������ֵ��110%380V  Q12
	ParaTable.RunCtrlReg.uBoostCurHighCut	= 5324;		// Boost��ѹ�����ж�ֵ��130%380V  Q12

	ParaTable.RunCtrlReg.uOutVolHighAlarm	= 4505;		// �����ѹ����ֵ��110%380V  Q12
	ParaTable.RunCtrlReg.uOutVolHighCut	    = 5324;		// �����ѹ�ж�ֵ��130%380V  Q12
	ParaTable.RunCtrlReg.uOutVolLowAlarm	= 3686;		// ���Ƿѹ����ֵ��90%380V   Q12
	ParaTable.RunCtrlReg.uOutVolLowCut		= 3276;		// ���Ƿѹ�ж�ֵ��80%380V   Q12
	ParaTable.RunCtrlReg.uOutCurHighAlarm	= 4505;		// �����������ֵ��110%380V  Q12
	ParaTable.RunCtrlReg.uOutCurHighCut		= 1000;		// ��������ж�ֵ��130%380V  Q12

	ParaTable.RunCtrlReg.uTempFanOpen		= 500;		// ���ȿ����¶� ����λ0.1��
	ParaTable.RunCtrlReg.uTempAlarm			= 900;		// ���±���ֵ����λ0.1��
	ParaTable.RunCtrlReg.uTempCut			= 1000;		// �����ж�ֵ����λ0.1��

    ParaTable.RunCtrlReg.uVolOutLPLow       = 1400;     // ���ȱ��������ֵ
    ParaTable.RunCtrlReg.uVolOutLPUp		= 2800;		// ���ȱ��������ֵ
    ParaTable.RunCtrlReg.uFreUnusualLow		= 972;		// Ƶ���쳣����ֵ
    ParaTable.RunCtrlReg.uFreUnusualUp		= 1076;		// Ƶ���쳣����ֵ

	ParaTable.RunCtrlReg.bCtrlModeSet		= 0;		// ����ģʽ�趨 0--����ģʽ��1--ң��ģʽ
	ParaTable.RunCtrlReg.bDirectionSet		= 0;		// �����ת�����趨 0--��ת��1--��ת
	ParaTable.RunCtrlReg.bFreSourceSet		= 0;		// �����Ƶ����Դ�趨 0--ģ������1--������

	ParaTable.RunCtrlReg.XSMaxInVol			= 10000;	// �����ѹ������ֵ
	ParaTable.RunCtrlReg.XSMaxInCur			= 6800;		// �������������ֵ
	ParaTable.RunCtrlReg.XSMaxBoostCur		= 6800;		// Bosst��ѹ����������ֵ
	ParaTable.RunCtrlReg.XSMaxOutVol		= 10000;	// �����ѹ������ֵ
	ParaTable.RunCtrlReg.XSMaxOutCur		= 10000;	// �������������ֵ
	ParaTable.RunCtrlReg.XSMaxNTCTemp		= 6800;		// �¶�������ֵ
	ParaTable.RunCtrlReg.XSMaxSpeed			= 1024;		// �ٶ�������ֵ

	ParaTable.RunCtrlReg.uSpeedRatio		= 20;		// ��/��Ƶ�ٶȣ�1~100��Ӧ��~��
	ParaTable.RunCtrlReg.uVVVFVfSet			= 44;		// VVVF��V/Fֵ���ã���λ0.1

	// �ӳٱ���/��������Ĭ��ֵ
	ParaTable.DelayCtrlReg.uInVolAlarm		= 1000;		// �����ѹ�����ӳ�ֵ	@3ms
	ParaTable.DelayCtrlReg.uInVolCut		= 200;		// �����ѹ�ж��ӳ�ֵ	@3ms
	ParaTable.DelayCtrlReg.uInCurAlarm		= 1000;		// ������������ӳ�ֵ	@3ms
	ParaTable.DelayCtrlReg.uInCurCut		= 200;		// ��������ж��ӳ�ֵ	@3ms

	ParaTable.DelayCtrlReg.uBoostVolAlarm	= 1000;		// Boost��ѹ��ѹ�����ӳ�ֵ	@3ms
	ParaTable.DelayCtrlReg.uBoostVolCut		= 200;		// Boost��ѹ��ѹ�ж��ӳ�ֵ	@3ms
	ParaTable.DelayCtrlReg.uBoostCurAlarm	= 1000;		// Boost��ѹ���������ӳ�ֵ	@3ms
	ParaTable.DelayCtrlReg.uBoostCurCut		= 200;		// Boost��ѹ�����ж��ӳ�ֵ	@3ms

	ParaTable.DelayCtrlReg.uOutVolAlarm		= 1000;		// �����ѹ�����ӳ�ֵ	@3ms
	ParaTable.DelayCtrlReg.uOutVolCut		= 200;		// �����ѹ�ж��ӳ�ֵ	@3ms
	ParaTable.DelayCtrlReg.uOutCurAlarm		= 1000;		// ������������ӳ�ֵ	@3ms
	ParaTable.DelayCtrlReg.uOutCurCut		= 200;		// ��������ж��ӳ�ֵ	@3ms

    ParaTable.DelayCtrlReg.uTempAlarm    	= 1000;     // ���±����ӳ�ֵ	@3ms
    ParaTable.DelayCtrlReg.uTempCut      	= 200;      // �����ж��ӳ�ֵ	@3ms

    ParaTable.DelayCtrlReg.uVolOutLP		= 500;		// ���ȱ���ӳٱ���ֵ	@3ms
	ParaTable.DelayCtrlReg.uFreUnusual		= 200;		// Ƶ���쳣�����ӳ�ֵ	@3ms

    // ��Ҫ���ݱ�������� 64
	ParaTable.EmergencyReg.uEmDataSize      = 128;      // ��Ҫ���ݴ洢�����С128K
	ParaTable.EmergencyReg.uEmDataNumMax    = 8;        // ��Ҫ�������洢����8��
	ParaTable.EmergencyReg.uEmDataSaveTime  = 16 * 1024 / 3600; // ��Ҫ���ݱ���ʱ�� 4.55s
    ParaTable.EmergencyReg.uEmData0Addr     = 1;
    ParaTable.EmergencyReg.uEmData1Addr     = 2;
    ParaTable.EmergencyReg.uEmData2Addr     = 3;
    ParaTable.EmergencyReg.uEmData3Addr     = 4;
    ParaTable.EmergencyReg.uEmData4Addr     = 5;
    ParaTable.EmergencyReg.uEmData5Addr     = 6;
    ParaTable.EmergencyReg.uEmData6Addr     = 7;
    ParaTable.EmergencyReg.uEmData7Addr     = 8;

	SectorErase(0x00);
	DelayUs(1000);
	FlashWrite(ParPoint,0x00,sizeof(ParaTable));					// ����������д��flash��
}

/*---------------------------------------------------------------------
����ԭ�ͣ�
�������ƣ�
�������ܣ����±��޸ĵĲ�����flash;
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
int UpdataParaToFlash(void)
{
	//�ж���δ���в��Ҳ������޸�ʱ��������������д��flash
	if ((SystemState != SystemRun) && (FlagSystem.bit.ParaDataChg == 1))
	{
		FlagSystem.bit.ParaDataChg = 0;
		SectorErase(0x00);

		FlashWrite((int *)&ParaTable, 0x00, sizeof(ParaTable));		//����������д��flash��
		return 1;
	}
	return 0;
}