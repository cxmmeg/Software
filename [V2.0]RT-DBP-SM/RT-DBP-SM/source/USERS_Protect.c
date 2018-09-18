// �û�����
// ���£�2015-12
//###########################################################################
//
// �ļ�:	USERS_Protects.c
//
// ����:	ϵͳ��������
//
// ����:	����ϵͳ�л�ģʽѡ������ϵͳ�����Ӻ���
//				
//########################################################################### 
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

extern struct AD_BUFS                   DataBuf;
extern union  FLAG_GPIOIN   	 		FlagGpioIn;
extern union  FLAG_SYSTEM_INITERR 		FlagSysInitErr;
extern union  FLAG_SYSTEM_RUNERR 		FlagSysRunErr;
extern union  FLAG_SYSTEM_ALARM			FlagSysAlarm;
extern union  FLAG_SYSTEM				FlagSystem;
extern union  STATE_SYSTEM				StateSystem;
extern struct COUNT_SYSTEM				CountSystem;
extern struct REGS_SYSTEM               RegSystem;
extern struct SPLL_REGS 				SpllRegOut;

/*---------------------------------------------------------------------
����ԭ�ͣ�void PhaseLackProtect(void)
�������ƣ�ϵͳ��������--ȱ�ౣ��
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2018��07��
----------------------------------------------------------------------*/
void PhaseLackProtect(void)
{
/**************************** ���ȱ���ж� ***************************/
//	if((RegSystem.OutVolRms > ParaTable.RunCtrlReg.uVolOutLPLow)
//	&& (RegSystem.OutVolRms < ParaTable.RunCtrlReg.uVolOutLPUp))
//	{
//		if(++CountSystem.DelayOutInLP >= ParaTable.RunCtrlReg.uVolOutLPDelay)
//		{
//			CountSystem.DelayOutInLP = ParaTable.RunCtrlReg.uVolOutLPDelay;
//			FlagSysAlarm.bit.OutPhaseLose = 1;
//		}
//	}
//	else
//	{
//		if(--CountSystem.DelayOutInLP <= 0)
//		{
//			CountSystem.DelayOutInLP = 0;
//			FlagSysAlarm.bit.OutPhaseLose = 0;
//		}
//	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InVolProtect(void)
�������ƣ�ϵͳ��������--�����ѹ����(��ѹ����ѹ����)
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2015��12��
----------------------------------------------------------------------*/
void InVolProtect(void)
{
//	if(RegSystem.OutVolRms < ParaTable.RunCtrlReg.uOutVolLower)
//	{
//		if(++CountSystem.OutUrmsLow > ParaTable.RunCtrlReg.uOutVolLowDelay)
//		{
//			CountSystem.OutUrmsLow = ParaTable.RunCtrlReg.uOutVolLowDelay;
//			FlagSysAlarm.bit.OutVolLowAlarm = 1;
//		}
//	}
//	else if(RegSystem.OutVolRms > ParaTable.RunCtrlReg.uOutVolUpper)
//	{
//		if(++CountSystem.OutUrmsHigh > ParaTable.RunCtrlReg.uOutVolUpDelay)
//		{
//			CountSystem.OutUrmsHigh = ParaTable.RunCtrlReg.uOutVolUpDelay;
//			FlagSysAlarm.bit.OutVolHighAlarm = 1;
//		}
//	}
//	else if((RegSystem.OutVolRms >= ParaTable.RunCtrlReg.uOutVolLower + AC_VOL_RESUM5)
//	&& (RegSystem.OutVolRms <= ParaTable.RunCtrlReg.uOutVolUpper - AC_VOL_RESUM5))
//	{
//		CountSystem.OutUrmsLow 				= 0;
//		CountSystem.OutUrmsHigh 			= 0;
//		FlagSysAlarm.bit.OutVolLowAlarm 	= 0;
//		FlagSysAlarm.bit.OutVolHighAlarm 	= 0;
//	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InCurProtect(void)
�������ƣ�ϵͳ��������--�����������
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2015��12��
----------------------------------------------------------------------*/
void InCurProtect(void)
{
//	if(RegSystem.NO2IRms > ParaTable.RunCtrlReg.uNO2CurAlarm)
//	{
//		if(++CountSystem.NO2CurHighAlarm > ParaTable.RunCtrlReg.uNO2CurAlarmDelay)
//		{
//			CountSystem.NO2CurHighAlarm = ParaTable.RunCtrlReg.uNO2CurAlarmDelay;
//			FlagSysAlarm.bit.NO2InCurHighAlarm = 1;
//		}
//		if(RegSystem.NO2IRms > ParaTable.RunCtrlReg.uNO2CurCut)
//		{
//			if(++CountSystem.NO2CurHighCut > ParaTable.RunCtrlReg.uNO2CurCutDelay)
//			{
//				CountSystem.NO2CurHighCut = ParaTable.RunCtrlReg.uNO2CurCutDelay;
//				FlagSysRunErr.bit.NO2InCurHighCut = 1;
//			}
//		}
//		else
//		{
//			CountSystem.NO2CurHighCut = 0;
//		}
//	}
//	else if((RegSystem.NO2IRms <= ParaTable.RunCtrlReg.uNO2CurAlarm - AC_CUR_RUSUM5))
//	{
//		FlagSysAlarm.bit.NO2InCurHighAlarm 	= 0;
//		FlagSysRunErr.bit.NO2InCurHighCut 	= 0;
//		CountSystem.NO2CurHighAlarm 		= 0;
//		CountSystem.NO2CurHighCut 			= 0;
//	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void TempProtect(void)
�������ƣ�ϵͳ��������--�¶ȱ���
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2015��12��
----------------------------------------------------------------------*/
void TempProtect(void)
{
//    Uint16 TempMax;
//
//    // �¶�ȡ���ֵ
//    if (ParaTable.Timer1Reg.uModATemp >= ParaTable.Timer1Reg.uModBTemp)
//    {
//        TempMax = ParaTable.Timer1Reg.uModATemp;
//    }
//    else
//    {
//        TempMax = ParaTable.Timer1Reg.uModBTemp;
//    }
//
//    if (ParaTable.Timer1Reg.uModCTemp >= TempMax)
//    {
//        TempMax = ParaTable.Timer1Reg.uModCTemp;
//    }
//
//    // �����¶ȿ�������
//    if (TempMax > ParaTable.RunCtrlReg.uFanOpenTemp)
//    {
//
//    }
//    else if (TempMax <= (ParaTable.RunCtrlReg.uFanOpenTemp - TEMP_RESUM5))
//    {
//
//    }
//
//    // ���±���
//    if(TempMax > ParaTable.RunCtrlReg.uTempAlarm)
//    {
//        if(++CountSystem.DelayTempAlarm > 1000)     // �ӳ�3S����
//        {
//            CountSystem.DelayTempAlarm = 1000;
//            FlagSysAlarm.bit.OverTempAlarm = 1;
//        }
//    }
//    else if(TempMax > ParaTable.RunCtrlReg.uTempCut)
//    {
//        if(++CountSystem.DelayTempCut > 1000)       // �ӳ�3S����
//        {
//            CountSystem.DelayTempCut = 1000;
//            FlagSysRunErr.bit.OverTempCut = 1;
//        }
//    }
//    else if (TempMax < ParaTable.RunCtrlReg.uTempCut - TEMP_RESUM5)
//    {
//        CountSystem.DelayTempCut      = 0;
//        FlagSysRunErr.bit.OverTempCut = 0;
//    }
//    else if (TempMax < ParaTable.RunCtrlReg.uTempAlarm - TEMP_RESUM5)
//    {
//        CountSystem.DelayTempAlarm     = 0;
//        FlagSysAlarm.bit.OverTempAlarm = 0;
//    }
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void PDPClearFunc(Uint16 ClearTime)
�������ƣ�
�������ܣ����PDP����
��ڲ�����ClearTime �ӳ��������ʱ�� @3ms
���ڲ�����PDPFlag ���������ǣ� 0-���  1-δ���
��    �ߣ�
��    �ڣ�2017��05��03��
----------------------------------------------------------------------*/
void PDPClearFunc(Uint16 ClearTime)
{
	static Uint16 PDPCountTemp = 0;
	Uint16 PDPErrTemp;

	if (++PDPCountTemp >= ClearTime)
	{
		ClrCPLDSTS();           				//���Ӳ�����ϣ�
		PDPCountTemp = 0;
		PDPErrTemp = ReadCPLDSTS();          	//��ȡError_Status��
		if((PDPErrTemp & 0xff) == 0xff)
		{
			FlagSysRunErr.bit.FlagPDP = 0;
			PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
		}
	}
}

