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
#ifdef EQUIPMENT_250A
void WtiteDefaultToFlash(void)
{
	int *ParPoint;

	ParPoint = (int *)&ParaTable;
	memset(&ParaTable,0x00,sizeof(ParaTable));

	// ����������
	ParaTable.PowerReg.uNO1PowerType 		= 0;
	ParaTable.PowerReg.uNO1PowerFre 		= 500;			// ��·�����Դ�Ƶ�� 0.1Hz
	ParaTable.PowerReg.uNO1PowerVol		    = 3800;			// ��·�����Դ���ѹ 0.1V
	ParaTable.PowerReg.uNO1PowerCur		    = 2500;			// ��·�����Դ����� 0.1A,Irms

	ParaTable.PowerReg.uNO2PowerType 		= 0;
	ParaTable.PowerReg.uNO2PowerFre 		= 500;			// ��·�����Դ�Ƶ��
	ParaTable.PowerReg.uNO2PowerVol		    = 3800;			// ��·�����Դ���ѹ
	ParaTable.PowerReg.uNO2PowerCur		    = 2500;			// ��·�����Դ�����

	ParaTable.PowerReg.uOutPowerType 		= 0;
	ParaTable.PowerReg.uOutPowerFre 		= 500;			// ���·�����Դ�Ƶ��
	ParaTable.PowerReg.uOutPowerVol		    = 3800;			// ���·�����Դ���ѹ
	ParaTable.PowerReg.uOutPowerCur		    = 2500;			// ���·�����Դ�����

	ParaTable.PowerReg.uModATemp			= 600;			// A�������¶�
	ParaTable.PowerReg.uModBTemp			= 600;			// B�������¶�
	ParaTable.PowerReg.uModCTemp			= 600;			// C�������¶�

	ParaTable.PowerReg.uParaTableVer		= 1;			// ��ʼ��Ϊ1
    ParaTable.PowerReg.uHardwareVer         = 42;           // Ӳ���汾�ţ����V4.2�汾
	ParaTable.PowerReg.uSoftwareVer		    = 18051;		// �����汾�ţ�xx��xx�µ�x���޸�

	// SPLL��PI�������
	ParaTable.PIReg.uNO1Kp					= 1024;			// Q12
	ParaTable.PIReg.uNO1Ki					= 10;			// Q12
	ParaTable.PIReg.uNO1Kd 					= 10; 			// Q12
	ParaTable.PIReg.uNO1PIMax				= 4000; 		// Q12
	ParaTable.PIReg.uNO1PIMin				= -4000; 		// Q12

	ParaTable.PIReg.uNO2Kp 					= 1024; 		// Q12
	ParaTable.PIReg.uNO2Ki 					= 10;			// Q12
	ParaTable.PIReg.uNO2Kd 					= 10;			// Q12
	ParaTable.PIReg.uNO2PIMax 				= 4000;			// Q12
	ParaTable.PIReg.uNO2PIMin 				= -4000; 		// Q12

	ParaTable.PIReg.uOutKp 					= 1024; 		// Q12
	ParaTable.PIReg.uOutKi 					= 10;			// Q12
	ParaTable.PIReg.uOutKd 					= 10;			// Q12
	ParaTable.PIReg.uOutPIMax 				= 4000;			// Q12
	ParaTable.PIReg.uOutPIMin 				= -4000; 		// Q12

	// ���п��Ʋ���Ĭ��ֵ
	ParaTable.RunCtrlReg.XSMaxNO1Vol		= 6094;			// AD:0~3V�� 2.5mA * 240K����λ0.1V
	ParaTable.RunCtrlReg.XSMaxNO2Vol		= 6094;			//
	ParaTable.RunCtrlReg.XSMaxOutVol		= 6000;			//
	ParaTable.RunCtrlReg.XSMaxNO1Cur		= 5000;			// AD:0~3V, 3V-500A����λ0.1A
	ParaTable.RunCtrlReg.XSMaxNO2Cur		= 5000;			//
	ParaTable.RunCtrlReg.XSMaxModATemp		= 1312;			// Is:4mA~20mA,AD:0~3V, 3V-131.2�棬��λ0.1��
	ParaTable.RunCtrlReg.XSMaxModBTemp		= 1312;			//
	ParaTable.RunCtrlReg.XSMaxModCTemp		= 1312;			//

	ParaTable.RunCtrlReg.uNO1VolLower		= 3686;			// �����ѹ��ѹ����ֵ Q12 342v-- 90%380V
	ParaTable.RunCtrlReg.uNO1VolUpper		= 4505;			// �����ѹ��ѹ����ֵ Q12 418v--110%380V
	ParaTable.RunCtrlReg.uNO1CurAlarm		= 4505;			// ��������ֵ 		 Q12 250A * 1.1
	ParaTable.RunCtrlReg.uNO1CurCut			= 5324;			// �����ж�ֵ		 Q12 250A * 1.3
	ParaTable.RunCtrlReg.uNO1VolUpDelay		= 1000;			// �����ѹ�����ӳ�ֵ  3ms * 1000 	= 3s
	ParaTable.RunCtrlReg.uNO1VolLowDelay	= 1000;			// �����ѹ�����ӳ�ֵ  3ms * 1000 	= 3s
	ParaTable.RunCtrlReg.uNO1CurAlarmDelay	= 1000;			// ������������ӳ�ֵ  3ms * 1000 	= 3s
	ParaTable.RunCtrlReg.uNO1CurCutDelay	= 100;			// ��������ж���ʱֵ  3ms * 1000 	= 3s

	ParaTable.RunCtrlReg.uNO2VolLower	    = 3686;			// �����ѹ��ѹ����ֵ Q12 342v-- 90%380V
	ParaTable.RunCtrlReg.uNO2VolUpper		= 4505;			// �����ѹ��ѹ����ֵ Q12 418v--110%380V
	ParaTable.RunCtrlReg.uNO2CurAlarm		= 4505;			// ��������ֵ 		Q12 250A * 1.1
	ParaTable.RunCtrlReg.uNO2CurCut			= 5324;			// �����ж�ֵ		Q12 250A * 1.3
	ParaTable.RunCtrlReg.uNO2VolUpDelay		= 1000;			// �����ѹ�����ӳ�ֵ  3ms * 1000 	= 3s
	ParaTable.RunCtrlReg.uNO2VolLowDelay	= 1000;			// �����ѹ�����ӳ�ֵ  3ms * 1000 	= 3s
	ParaTable.RunCtrlReg.uNO2CurAlarmDelay	= 1000;			// ������������ӳ�ֵ  3ms * 1000 	= 3s
	ParaTable.RunCtrlReg.uNO2CurCutDelay	= 100;			// ��������ж���ʱֵ  3ms * 1000 	= 3s

    ParaTable.RunCtrlReg.uFanOpenTemp       = 350;          // ���ȿ����¶�ֵ      35.0��
    ParaTable.RunCtrlReg.uTempAlarm         = 750;          // �¶ȱ���ֵ             75.0��
    ParaTable.RunCtrlReg.uTempCut           = 1000;         // �¶��ж�                 100.0��

    // ������ϵ�����Ʋ�����
    ParaTable.ADCoefReg.uCoefNO1Ia          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO1Ib          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO1Ic          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Ia          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Ib          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Ic          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO1Uab         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO1Ubc         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Uab         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Ubc         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefOutUab         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefOutUbc         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefTempA          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefTempB          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefTempC          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefrsvd           = 4096;         // Q12

    // ��Ҫ���ݱ�������� 64
	ParaTable.EmergencyReg.uEmDataSize      = 128;          // ��Ҫ���ݴ洢�����С128K
	ParaTable.EmergencyReg.uEmDataNumMax    = 8;            // ��Ҫ�������洢����8��
	ParaTable.EmergencyReg.uEmDataSaveTime  = 25;           // ��Ҫ���ݱ���ʱ�� 4.55s
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
#endif

#ifdef EQUIPMENT_400A
void WtiteDefaultToFlash(void)
{
    int *ParPoint;

    ParPoint = (int *)&ParaTable;
    memset(&ParaTable,0x00,sizeof(ParaTable));

    // ����������
    ParaTable.PowerReg.uNO1PowerType        = 0;
    ParaTable.PowerReg.uNO1PowerFre         = 500;          // ��·�����Դ�Ƶ�� 0.1Hz
    ParaTable.PowerReg.uNO1PowerVol         = 3800;         // ��·�����Դ���ѹ 0.1V
    ParaTable.PowerReg.uNO1PowerCur         = 4000;         // ��·�����Դ����� 0.1A,Irms

    ParaTable.PowerReg.uNO2PowerType        = 0;
    ParaTable.PowerReg.uNO2PowerFre         = 500;          // ��·�����Դ�Ƶ��
    ParaTable.PowerReg.uNO2PowerVol         = 3800;         // ��·�����Դ���ѹ
    ParaTable.PowerReg.uNO2PowerCur         = 4000;         // ��·�����Դ�����

    ParaTable.PowerReg.uOutPowerType        = 0;
    ParaTable.PowerReg.uOutPowerFre         = 500;          // ���·�����Դ�Ƶ��
    ParaTable.PowerReg.uOutPowerVol         = 3800;         // ���·�����Դ���ѹ
    ParaTable.PowerReg.uOutPowerCur         = 4000;         // ���·�����Դ�����

    ParaTable.PowerReg.uModATemp            = 600;          // A�������¶�
    ParaTable.PowerReg.uModBTemp            = 600;          // B�������¶�
    ParaTable.PowerReg.uModCTemp            = 600;          // C�������¶�

    ParaTable.PowerReg.uParaTableVer        = 1;            // ��ʼ��Ϊ1
    ParaTable.PowerReg.uHardwareVer         = 42;           // Ӳ���汾�ţ����V4.2�汾
    ParaTable.PowerReg.uSoftwareVer         = 18051;        // �����汾�ţ�xx��xx�µ�x���޸�

    // SPLL��PI�������
    ParaTable.PIReg.uNO1Kp                  = 1024;         // Q12
    ParaTable.PIReg.uNO1Ki                  = 10;           // Q12
    ParaTable.PIReg.uNO1Kd                  = 10;           // Q12
    ParaTable.PIReg.uNO1PIMax               = 4000;         // Q12
    ParaTable.PIReg.uNO1PIMin               = -4000;        // Q12

    ParaTable.PIReg.uNO2Kp                  = 1024;         // Q12
    ParaTable.PIReg.uNO2Ki                  = 10;           // Q12
    ParaTable.PIReg.uNO2Kd                  = 10;           // Q12
    ParaTable.PIReg.uNO2PIMax               = 4000;         // Q12
    ParaTable.PIReg.uNO2PIMin               = -4000;        // Q12

    ParaTable.PIReg.uOutKp                  = 1024;         // Q12
    ParaTable.PIReg.uOutKi                  = 10;           // Q12
    ParaTable.PIReg.uOutKd                  = 10;           // Q12
    ParaTable.PIReg.uOutPIMax               = 4000;         // Q12
    ParaTable.PIReg.uOutPIMin               = -4000;        // Q12

    // ���п��Ʋ���Ĭ��ֵ
    ParaTable.RunCtrlReg.XSMaxNO1Vol        = 6094;         // AD:0~3V�� 2.5mA * 240K����λ0.1V
    ParaTable.RunCtrlReg.XSMaxNO2Vol        = 6094;         //
    ParaTable.RunCtrlReg.XSMaxOutVol        = 6000;         //
    ParaTable.RunCtrlReg.XSMaxNO1Cur        = 7000;         // AD:0~3V, 3V-500A����λ0.1A
    ParaTable.RunCtrlReg.XSMaxNO2Cur        = 7000;         //
    ParaTable.RunCtrlReg.XSMaxModATemp      = 1312;         // Is:4mA~20mA,AD:0~3V, 3V-131.2�棬��λ0.1��
    ParaTable.RunCtrlReg.XSMaxModBTemp      = 1312;         //
    ParaTable.RunCtrlReg.XSMaxModCTemp      = 1312;         //

    ParaTable.RunCtrlReg.uNO1VolLower       = 3686;         // �����ѹ��ѹ����ֵ Q12 342v-- 90%380V
    ParaTable.RunCtrlReg.uNO1VolUpper       = 4505;         // �����ѹ��ѹ����ֵ Q12 418v--110%380V
    ParaTable.RunCtrlReg.uNO1CurAlarm       = 4505;         // ��������ֵ         Q12 400A * 1.1
    ParaTable.RunCtrlReg.uNO1CurCut         = 5324;         // �����ж�ֵ         Q12 400A * 1.3
    ParaTable.RunCtrlReg.uNO1VolUpDelay     = 1000;         // �����ѹ�����ӳ�ֵ  3ms * 1000    = 3s
    ParaTable.RunCtrlReg.uNO1VolLowDelay    = 1000;         // �����ѹ�����ӳ�ֵ  3ms * 1000    = 3s
    ParaTable.RunCtrlReg.uNO1CurAlarmDelay  = 1000;         // ������������ӳ�ֵ  3ms * 1000    = 3s
    ParaTable.RunCtrlReg.uNO1CurCutDelay    = 100;          // ��������ж���ʱֵ  3ms * 1000    = 3s

    ParaTable.RunCtrlReg.uNO2VolLower       = 3686;         // �����ѹ��ѹ����ֵ Q12 342v-- 90%380V
    ParaTable.RunCtrlReg.uNO2VolUpper       = 4505;         // �����ѹ��ѹ����ֵ Q12 418v--110%380V
    ParaTable.RunCtrlReg.uNO2CurAlarm       = 4505;         // ��������ֵ        Q12 400A * 1.1
    ParaTable.RunCtrlReg.uNO2CurCut         = 5324;         // �����ж�ֵ        Q12 400A * 1.3
    ParaTable.RunCtrlReg.uNO2VolUpDelay     = 1000;         // �����ѹ�����ӳ�ֵ  3ms * 1000    = 3s
    ParaTable.RunCtrlReg.uNO2VolLowDelay    = 1000;         // �����ѹ�����ӳ�ֵ  3ms * 1000    = 3s
    ParaTable.RunCtrlReg.uNO2CurAlarmDelay  = 1000;         // ������������ӳ�ֵ  3ms * 1000    = 3s
    ParaTable.RunCtrlReg.uNO2CurCutDelay    = 100;          // ��������ж���ʱֵ  3ms * 1000    = 3s

    ParaTable.RunCtrlReg.uFanOpenTemp       = 350;          // ���ȿ����¶�ֵ      35.0��
    ParaTable.RunCtrlReg.uTempAlarm         = 750;          // �¶ȱ���ֵ             75.0��
    ParaTable.RunCtrlReg.uTempCut           = 1000;         // �¶��ж�                 100.0��

    // ������ϵ�����Ʋ�����
    ParaTable.ADCoefReg.uCoefNO1Ia          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO1Ib          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO1Ic          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Ia          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Ib          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Ic          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO1Uab         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO1Ubc         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Uab         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefNO2Ubc         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefOutUab         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefOutUbc         = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefTempA          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefTempB          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefTempC          = 4096;         // Q12
    ParaTable.ADCoefReg.uCoefrsvd           = 4096;         // Q12

    // ��Ҫ���ݱ�������� 64
    ParaTable.EmergencyReg.uEmDataSize      = 128;          // ��Ҫ���ݴ洢�����С128K
    ParaTable.EmergencyReg.uEmDataNumMax    = 8;            // ��Ҫ�������洢����8��
    ParaTable.EmergencyReg.uEmDataSaveTime  = 25;           // ��Ҫ���ݱ���ʱ�� 4.55s
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
    FlashWrite(ParPoint,0x00,sizeof(ParaTable));                    // ����������д��flash��
}
#endif

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

