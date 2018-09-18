/*****************ͷ�ļ�********************/
#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h"

extern struct   AD_BUFS                 DataInst;
extern struct   AD_BUFS                 DataZero;
extern struct   AD_BUFS                 DataBuf;
extern struct   AD_BUFS                 DataFactor;
extern struct   AD_BUFS                 DataFactorOrig;
extern struct   AD_BUFS                 DataNormal;
extern struct   AD_BUFS                 DataMax;

extern union  	FLAG_GPIOIN   	 		FlagGpioIn;		// GPIO��־λ�ṹ��λ����
extern union  	FLAG_SYSTEM_INITERR 	FlagSysInitErr;
extern union  	FLAG_SYSTEM_RUNERR 		FlagSysRunErr;
extern union  	FLAG_SYSTEM				FlagSystem;
extern union  	FLAG_SYSTEM_ALARM		FlagSysAlarm;
extern union  	FLAG_SYSTEM_PDPERR		FlagSysPDPErr;

extern struct 	COUNT_SYSTEM			CountSystem;
extern struct   REGS_SYSTEM             RegSystem;
extern enum   	SYSTEM_STATE 			SystemState;
extern struct 	CANFRAME  				CanRxPack;
extern struct 	INSTANT_POWER 			PowerInst;

void SaveT4VarToPara(void);
void SysStateLedDisplay(int16 StandByState, int16 RunState, int16 FaultState);
Uint16 PreCharge(Uint16 RealVol, Uint16 ChgVol, Uint16 *Count, Uint16 KM);
void RefSet(Uint16 *Real, Uint16 Ref, Uint16 Step);
void FlagInit(void);
void ReadT4VarFromPara(void);

void InitT4Var(void)
{
    NOP;
}

void ReadT4VarFromPara(void)
{
    Uint16 i;
    Uint16 *pDataNormal, *pDataMax, *pDataFactor;

//    // ��ȡ�ⲿ�趨ֵ
//    DataNormal.Uab  	= ParaTable.PowerReg.uOutputVol;
//    DataNormal.Ubc  	= ParaTable.PowerReg.uOutputVol;
//    DataNormal.Ial  	= ParaTable.PowerReg.uOutputCur;
//    DataNormal.Ibl 		= ParaTable.PowerReg.uOutputCur;
//    DataNormal.Icl 		= ParaTable.PowerReg.uOutputCur;
//    DataNormal.Iac  	= ParaTable.PowerReg.uOutputCur;
//    DataNormal.Ibc  	= ParaTable.PowerReg.uOutputCur;
//    DataNormal.Icc  	= ParaTable.PowerReg.uOutputCur;
//    DataNormal.Udc 		= ParaTable.PowerReg.uInputVol;
//    DataNormal.Idc		= ParaTable.PowerReg.uInputCur;
//    DataNormal.Iboost1 	= ParaTable.PowerReg.uBoostCur;
//    DataNormal.Iboost2 	= ParaTable.PowerReg.uBoostCur;
//    DataNormal.Iboost3  = ParaTable.PowerReg.uBoostCur;
//    DataNormal.TempA  	= ParaTable.PowerReg.uTempNom;
//    DataNormal.TempB  	= ParaTable.PowerReg.uTempNom;
//    DataNormal.URFre  	= ParaTable.PowerReg.uOutputFre;
//
    //    // ��ȡ�ⲿ������ϵ��
    //    DataMax.Uab  		= ParaTable.RunCtrlReg.XSMaxOutVol;
    //    DataMax.Ubc  		= ParaTable.RunCtrlReg.XSMaxOutVol;
    //    DataMax.Ial  		= ParaTable.RunCtrlReg.XSMaxOutCur;
    //    DataMax.Ibl 		= ParaTable.RunCtrlReg.XSMaxOutCur;
    //    DataMax.Icl 		= ParaTable.RunCtrlReg.XSMaxOutCur;
    //    DataMax.Iac  		= ParaTable.RunCtrlReg.XSMaxOutCur;
    //    DataMax.Ibc  		= ParaTable.RunCtrlReg.XSMaxOutCur;
    //    DataMax.Icc  		= ParaTable.RunCtrlReg.XSMaxOutCur;
    //    DataMax.Udc 		= ParaTable.RunCtrlReg.XSMaxInVol >> 1;
    //    DataMax.Idc 		= ParaTable.RunCtrlReg.XSMaxInCur >> 1;
    //    DataMax.Iboost1 	= ParaTable.RunCtrlReg.XSMaxBoostCur >> 1;
    //    DataMax.Iboost2 	= ParaTable.RunCtrlReg.XSMaxBoostCur >> 1;
    //    DataMax.Iboost3  	= ParaTable.RunCtrlReg.XSMaxBoostCur >> 1;
    //    DataMax.TempA  		= ParaTable.RunCtrlReg.XSMaxNTCTemp;
    //    DataMax.TempB  		= ParaTable.RunCtrlReg.XSMaxNTCTemp;
    //    DataMax.URFre  		= ParaTable.RunCtrlReg.XSMaxFre >> 1;

    RegSystem.SpeedRatio = ParaTable.RunCtrlReg.uSpeedRatio;
    RegSystem.VVVFVfSet  = ParaTable.RunCtrlReg.uVVVFVfSet;

    // ��ȡ�ⲿ�趨ֵ
    DataNormal.Ia  	= ParaTable.PowerReg.uOutputCur;
    DataNormal.Ic  	= ParaTable.PowerReg.uOutputCur;
    DataNormal.Iu  	= ParaTable.PowerReg.uOutputCur;
    DataNormal.Iw 	= ParaTable.PowerReg.uOutputCur;
    DataNormal.Uab 	= ParaTable.PowerReg.uOutputVol;
    DataNormal.Ubc  = ParaTable.PowerReg.uOutputVol;
    DataNormal.Udc1 = ParaTable.PowerReg.uInputVol;
    DataNormal.Udc2 = ParaTable.PowerReg.uInputCur;
    DataNormal.Uai1	= ParaTable.PowerReg.uSpeedSet;
    DataNormal.Uai2 = ParaTable.PowerReg.uSpeedSet;

    // ��ȡ�ⲿ������ϵ��
    DataMax.Ia  	= ParaTable.RunCtrlReg.XSMaxOutCur;
    DataMax.Ic  	= ParaTable.RunCtrlReg.XSMaxOutCur;
    DataMax.Iu  	= ParaTable.RunCtrlReg.XSMaxOutCur;
    DataMax.Iw 		= ParaTable.RunCtrlReg.XSMaxOutCur;
    DataMax.Uab 	= ParaTable.RunCtrlReg.XSMaxOutVol;
    DataMax.Ubc  	= ParaTable.RunCtrlReg.XSMaxOutVol;
    DataMax.Udc1  	= ParaTable.RunCtrlReg.XSMaxInVol >> 1;
    DataMax.Udc2 	= ParaTable.RunCtrlReg.XSMaxInCur >> 1;
    DataMax.Uai1 	= ParaTable.RunCtrlReg.XSMaxSpeed >> 1;
    DataMax.Uai2 	= ParaTable.RunCtrlReg.XSMaxSpeed >> 1;

    // �������ϵ��
    pDataNormal = (Uint16 *)&DataNormal;
    pDataMax = (Uint16 *)&DataMax;
    pDataFactor = (Uint16 *)&DataFactor;
    for (i = 0; i < 10; i++, pDataNormal++, pDataMax++, pDataFactor++)
    {
        *pDataFactor     = ((Uint32)*pDataMax * 1024) / (*pDataNormal);     // Q10
        if (*pDataFactor >= 8192)     // ��ֹ������������Ʊ���ϵ��
        {
            *pDataFactor = 8192;
        }
    }
}

/*---------------------------------------------------------------------
����ԭ�ͣ�interrupt void Timer0_ISR(void)
�������ƣ�Timer0�ж�
�������ܣ�3ms�ж�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
interrupt void Timer1_ISR(void)
{
	DINT;

	// Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all	    = M_INT13;
    CpuTimer1Regs.TCR.bit.TIF	= 1;
    CpuTimer1Regs.TCR.bit.TRB	= 1;

	//save data from the PWM;
    ReadT4VarFromPara();  				// ��ȡ�������е�����
    if(0 == FlagSystem.bit.EmDataFlashWriteOK)
    {
    	FlagSystem.bit.EmDataFlashWriteOK = EmDataWriteToFlash();
    }
  	EINT;

/*******************************�û�������**********************************/
	GpioRead();

	CanDataRecv();
	CanDataSave();

	SCIBRcvDeal();
	SCICRcvDeal();
	SaveT4VarToPara();

	/*------------------------״̬����������--------------------------*/
	switch (SystemState)
	{
		case SystemWait:									// ϵͳ��ʼ���Լ�
			{
				SysStateLedDisplay(STANDBY_LED_OFF, RUN_LED_OFF, FAULT_LED_OFF);
				GPIOOutOfDSP(MAIN_CIRCUIT_KM, OFF);			// ���Ӵ����Ͽ�
				GPIOOutOfDSP(PRE_CHARGE_KM, OFF);			// Ԥ���Ӵ����Ͽ�
				GPIOOutOfDSP(DISCHARGE_KM, OFF);

				DisablePWM(EPWM_ALL);
				if (FlagSysInitErr.all != 0)			// �ж�ϵͳ��ʼ���Ƿ��й���
				{
					SystemState = SystemFault;
				}
				else
				{
					CountSystem.FaultReset = 0;
					CountSystem.DelaySysWait++;
					if (CountSystem.DelaySysWait >= 1000)	// �ӳ�3��
					{
						CountSystem.DelaySysWait = 1000;
						SystemState = SystemStandby;
					}
				}
				break;
			}

		case SystemStandby:
			{
				SysStateLedDisplay(STANDBY_LED_OFF, RUN_LED_OFF, FAULT_LED_OFF);
				GPIOOutOfDSP(DISCHARGE_KM, OFF);
				DisablePWM(EPWM_ALL);
				FlagInit();

				EALLOW;
			   	EPwm1Regs.TZCLR.all = 0x07;
			    EPwm2Regs.TZCLR.all = 0x07;
			    EPwm3Regs.TZCLR.all = 0x07;
				EPwm4Regs.TZCLR.all = 0x07;
			    EPwm5Regs.TZCLR.all = 0x07;
			    EPwm6Regs.TZCLR.all = 0x07;
				EDIS;

				if (0 == FlagGpioIn.bit.GpioIn1)		// ��ֹPDP��λ�Ժ��Զ���������״̬
				{
					FlagSystem.bit.LastErr = 0;
				}

				InVolProtect();

				FlagSystem.bit.PreChgOk = PreCharge(DataBuf.Udc1, ParaTable.RunCtrlReg.uInVolPreChg, (Uint16 *)&CountSystem.ACPreChg, PRE_CHARGE_KM);
//				FlagSystem.bit.PreChgOk = 1;
				if (1 == FlagSystem.bit.PreChgOk)		// Ԥ�����ɺ��������Ӵ���
				{
					GPIOOutOfDSP(MAIN_CIRCUIT_KM, ON);
				}

				if ((1 == FlagGpioIn.bit.GpioIn1) && (0 == FlagSystem.bit.LastErr))	//  && (1 == FlagGpioIn.bit.GpioIn3) && (1 == FlagGpioIn.bit.GpioIn4)
				{
					SystemState = SystemRun;
				}

				if (1 == FlagSysRunErr.bit.FlagPDP)
				{
					SystemState = SystemFault;
					DisablePWM(EPWM_ALL);
				}
				break;
			}
		case SystemRun:									// ϵͳ��������
			{
				SysStateLedDisplay(STANDBY_LED_ON, RUN_LED_ON, FAULT_LED_OFF);
				GPIOOutOfDSP(MAIN_CIRCUIT_KM, ON);		// ���Ӵ����պ�
				GPIOOutOfDSP(PRE_CHARGE_KM, ON);		// Ԥ���Ӵ����պ�
				GPIOOutOfDSP(DISCHARGE_KM, OFF);

				RefSet((Uint16 *)&RegSystem.OutVolWeRef, DataBuf.Uai1 >> 2, 10);

				//----- �������� ------//
				// �����Ӻ���
//				PhaseLackProtect();
//				InVolProtect();
//				InCurProtect();

				if (0 == FlagGpioIn.bit.GpioIn1)
				{
					SystemState = SystemStandby;
					DisablePWM(EPWM_ALL);
				}

				if (1 == FlagSysRunErr.bit.FlagPDP)
				{
					SystemState = SystemFault;
					DisablePWM(EPWM_ALL);
				}

				//---------------------//
				break;
			}
		case SystemFault:								// ���ϴ���
			{
				SysStateLedDisplay(STANDBY_LED_OFF, RUN_LED_OFF, FAULT_LED_ON);
				GPIOOutOfDSP(MAIN_CIRCUIT_KM, OFF);			// ���Ӵ����Ͽ�
				GPIOOutOfDSP(PRE_CHARGE_KM, OFF);			// Ԥ���Ӵ����Ͽ�
				GPIOOutOfDSP(DISCHARGE_KM, ON);

				FlagSystem.bit.LastErr = 1;

				if (1 == FlagSysRunErr.bit.FlagPDP)
				{
					PDPClearFunc(4000);		// Ӳ�����ϣ��ӳ�4.5s�ָ�
				}
				else
				{
					SystemState = SystemWait;
				}

				break;
			}
	}

	CountSystem.T4Cycle = (CpuTimer1Regs.PRD.all - CpuTimer1Regs.TIM.all) * 0.0066;			// us
}


/*---------------------------------------------------------------------
����ԭ�ͣ�Uint16 PreCharge(Uint16 RealVol, Uint16 ChgVol, Uint16 *Count, Uint16 KM)
�������ƣ�PreCharge
�������ܣ�Ԥ������
��ڲ�����RealVol--ʵ�ʵ�ѹֵ��ChgVol--Ԥ����ѹֵ��*Count--�ӳټ�������KM--�̵���
���ڲ�����flag--Ԥ���ɹ����
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
Uint16 PreCharge(Uint16 RealVol, Uint16 ChgVol, Uint16 *Count, Uint16 KM)
{
	Uint16 flag;

	if (RealVol > ChgVol)
	{
		(*Count)++;
		if ((*Count) > 2000)		// @3ms
		{
			(*Count) = 2000;
			flag = 1;
		}
		else
		{
			GPIOOutOfDSP(KM, ON);
			flag = 0;
		}
	}
	else
	{
		GPIOOutOfDSP(KM, ON);
		(*Count) = 0;
		flag = 0;
	}

	return (flag);
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void RefSet(Uint16 *Real, Uint16 Ref, Uint16 Step)
�������ƣ�RefSet
�������ܣ���׼ֵ����
��ڲ�����*Real--ʵ��ֵ��Ref--��׼ֵ��Step--����ֵ
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void RefSet(Uint16 *Real, Uint16 Ref, Uint16 Step)
{
	if ((*Real) < Ref)
	{
		if (Ref - (*Real) >= Step)
		{
			(*Real) += Step;
		}
		else
		{
			(*Real) = Ref;
		}
	}
	else
	{
		(*Real) = Ref;
	}
}

void SysStateLedDisplay(int16 StandByState, int16 RunState, int16 FaultState)
{
	GPIOOutOfDSP(STANDBY_LED, StandByState);		// ϵͳSTAND BY״ָ̬ʾ
	GPIOOutOfDSP(RUN_LED, RunState);			// ϵͳSYSTEM_RUN״ָ̬ʾ
	GPIOOutOfDSP(FAULT_LED, FaultState);		// ϵͳSYSTEM_FAULT״ָ̬ʾ
}

void FlagInit(void)
{
	FlagSysAlarm.bit.OutSPLLAlarm = 0;
	RegSystem.OutVolWeRef = 1;
}

void SaveT4VarToPara(void)
{
	Uint32 ApparentPower;
	static Uint16 i;

	if(++i >= 2000)
	{
		i = 0;
	}

	ParaTable.Timer1Reg.uSystemStatus 	= SystemState;
	ParaTable.Timer1Reg.uSystemInitErr	= FlagSysInitErr.all;
	ParaTable.Timer1Reg.uSystemRunErr 	= FlagSysRunErr.all;
	ParaTable.Timer1Reg.uSystemAlarmH	= FlagSysAlarm.all >> 16;
	ParaTable.Timer1Reg.uSystemAlarmL	= FlagSysAlarm.all & 0x0000ffff;
	ParaTable.Timer1Reg.uSystemPDPErr	= FlagSysPDPErr.all;

	ParaTable.Timer1Reg.uGpioIn			= FlagGpioIn.all;					// ����������
    ParaTable.Timer1Reg.uGpioOut 		= 0;								// ���������

    ParaTable.Timer1Reg.uCtrlMode 		= 0;								// ����ģʽ
    ParaTable.Timer1Reg.uDirection 		= 0;							// �����ת����

    ParaTable.Timer1Reg.uFreSource 		= 0;						// ��Ƶ����Դ
    ParaTable.Timer1Reg.uFreSet    		= ((Uint32)(DataBuf.Uai1 >> 2) * ParaTable.PowerReg.uOutputFre) >> 10;	// ��Ƶ���趨ֵ

    ParaTable.Timer1Reg.uInVolRms   	= ((Uint32)DataBuf.Udc1 * ParaTable.PowerReg.uInputVol) >> 12;			// �����ѹ��Чֵ
    ParaTable.Timer1Reg.uInCurRms		= 1;	// ���������Чֵ

    ParaTable.Timer1Reg.uBoostVolRms 	= 2;	// Boost1��ѹ��ѹ��Чֵ
	ParaTable.Timer1Reg.uBoost1CurRms	= 3;	// Boost1��ѹ������Чֵ
	ParaTable.Timer1Reg.uBoost2CurRms	= 4;	// Boost2��ѹ������Чֵ
	ParaTable.Timer1Reg.uBoost3CurRms	= 52;	// Boost3��ѹ������Чֵ

    ParaTable.Timer1Reg.uOutVolRms 		= 6;	// �����ѹ��Чֵ
	ParaTable.Timer1Reg.uOutCurRms	   	= 7;	// ���������Чֵ
	ParaTable.Timer1Reg.uOutFre	    	= ((Uint32)RegSystem.OutVolWeReal * ParaTable.PowerReg.uOutputFre) >> 10;	// ���Ƶ��

	ParaTable.Timer1Reg.uOutActivePower 	= ((int32)PowerInst.PowerAvgP * ParaTable.PowerReg.uOutputPower) >> 12;
	ParaTable.Timer1Reg.uOutReactivePower 	= ((int32)PowerInst.PowerAvgQ * ParaTable.PowerReg.uOutputPower) >> 12;
	ApparentPower = sqrt(((Uint32)PowerInst.PowerAvgP * PowerInst.PowerAvgP) + ((Uint32)PowerInst.PowerAvgQ * PowerInst.PowerAvgQ));
	ParaTable.Timer1Reg.uOutApparentPower 	= ((int32)ApparentPower * ParaTable.PowerReg.uOutputPower) >> 12;
	ParaTable.Timer1Reg.uOutPowerFactor		= ((Uint32)ParaTable.Timer1Reg.uOutActivePower * 100) / ParaTable.Timer1Reg.uOutApparentPower;

	ParaTable.Timer1Reg.uTemp1Rms		= 1;		// NTC1�¶�ֵ
	ParaTable.Timer1Reg.uTemp2Rms		= 1;		// NTC2�¶�ֵ
	ParaTable.Timer1Reg.uTemp3Rms		= 1;		// NTC3�¶�ֵ
	ParaTable.Timer1Reg.uTemp4Rms		= 1;		// NTC4�¶�ֵ
	ParaTable.Timer1Reg.uTemp5Rms		= 1;		// NTC5�¶�ֵ
	ParaTable.Timer1Reg.uTemp6Rms		= 1;		// NTC6�¶�ֵ
	ParaTable.Timer1Reg.uTemp7Rms		= 1;		// NTC7�¶�ֵ
	ParaTable.Timer1Reg.uTemp8Rms		= 1;		// NTC8�¶�ֵ

	ParaTable.Timer1Reg.uT4UseTime		= CountSystem.T4Cycle;

	ParaTable.Timer1Reg.uInstReg0		= i;
	ParaTable.Timer1Reg.uInstReg1		= 0;
	ParaTable.Timer1Reg.uInstReg2		= 0;
	ParaTable.Timer1Reg.uInstReg3		= 0;
	ParaTable.Timer1Reg.uInstReg4		= 0;
	ParaTable.Timer1Reg.uInstReg5		= 0;
	ParaTable.Timer1Reg.uInstReg6		= 0;
	ParaTable.Timer1Reg.uInstReg7		= 0;
	ParaTable.Timer1Reg.uInstReg8		= 0;
	ParaTable.Timer1Reg.uInstReg9		= 0;
}