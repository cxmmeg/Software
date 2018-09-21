// 用户程序
// 更新：2015-12
//###########################################################################
//
// 文件:	USERS_Protects.c
//
// 标题:	系统保护函数
//
// 描述:	包含系统切换模式选择函数和系统保护子函数
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
extern struct PWMISR_REGS               PwmIsrReg;
extern struct SPLL_REGS 				SpllRegNO1;
extern struct SPLL_REGS 				SpllRegNO2;
extern struct SPLL_REGS 				SpllRegOut;
extern enum   CONVERT_STATE 			ConvertState;
/*---------------------------------------------------------------------
函数原型：void ForceModeChosen(void)
函数名称：切换模式选择--即时强制切换
函数功能：
入口参数：无
出口参数：无
作    者：
日    期：2015年12月
----------------------------------------------------------------------*/
void ForceModeChosen(void)
{
	if((1 == SpllRegNO1.SpllOk) && (1 == SpllRegNO2.SpllOk))
//	if(1)
	{
/**************************** 单键循环计数 ***************************/
		if(1 == FLAG_FORCE_CHANGE)
		{
			CountSystem.ForceChangeDelay++;
		}
		if((CountSystem.ForceChangeDelay > 50) && (0 == FLAG_FORCE_CHANGE))
		{
			CountSystem.ForceChangeTime++;
			CountSystem.ForceChangeDelay = 0;
		}

/**************************** 手动强制切换 ***************************/
		if(CountSystem.ForceChangeTime == 1)
		{
			FlagSystem.bit.ForceModeOn = 1;
			FlagSysAlarm.bit.FroceModeAlarm = 1;	// 手动强制模式报警，提醒用户切回主路运行

			CountSystem.ForceReset++;               // 等待两路电源相位差同步，如等待10秒后仍不同步，则自动推出手动切换模式
			if (CountSystem.ForceReset > 3333)
			{
			    CountSystem.ForceReset = 0;
			    CountSystem.ForceChangeTime = 0;
                FlagSystem.bit.ForceModeOn = 0;
                FlagSysAlarm.bit.FroceModeAlarm = 0;
			}

			if (1 == FlagSystem.bit.NO1Preferred)
			{
				if (abs(SpllRegNO2.SpllTheta - SpllRegOut.SpllTheta) < 28)
				{
					CountSystem.DelayForceChg++;
					if(CountSystem.DelayForceChg >= 2)
					{
						CountSystem.DelayForceChg = 0;
						CountSystem.ForceReset = 0;
						ConvertState = ConvertForce;
					}
				}
				else
				{
					CountSystem.DelayForceChg = 0;
				}
			}

			if (1 == FlagSystem.bit.NO2Preferred)
			{
				if (abs(SpllRegNO1.SpllTheta - SpllRegOut.SpllTheta) < 28)
				{
					CountSystem.DelayForceChg++;
					if(CountSystem.DelayForceChg >= 2)
					{
					    CountSystem.ForceReset = 0;
						CountSystem.DelayForceChg = 0;
						ConvertState = ConvertForce;
					}
				}
				else
				{
					CountSystem.DelayForceChg = 0;
				}
			}
		}

/**************************** 手动强制切回 ***************************/
		else if(CountSystem.ForceChangeTime == 2)
		{
			if (1 == FlagSystem.bit.NO1Preferred)
			{
				if (abs(SpllRegNO1.SpllTheta - SpllRegOut.SpllTheta) < 28)
				{
					CountSystem.DelayForceChg++;
					if(CountSystem.DelayForceChg >= 2)
					{
						CountSystem.DelayForceChg = 0;
						CountSystem.ForceChangeTime = 0;
						FlagSystem.bit.ForceModeOn = 0;
						FlagSysAlarm.bit.FroceModeAlarm = 0;
						ConvertState = ConvertReset;
					}
				}
				else
				{
					CountSystem.DelayForceChg = 0;
				}
			}

			if (1 == FlagSystem.bit.NO2Preferred)
			{
				if (abs(SpllRegNO2.SpllTheta - SpllRegOut.SpllTheta) < 28)
				{
					CountSystem.DelayForceChg++;
					if(CountSystem.DelayForceChg >= 2)
					{
						CountSystem.DelayForceChg = 0;
						CountSystem.ForceChangeTime = 0;
						FlagSystem.bit.ForceModeOn = 0;
						FlagSysAlarm.bit.FroceModeAlarm = 0;
						ConvertState = ConvertReset;
					}
				}
				else
				{
					CountSystem.DelayForceChg = 0;
				}
			}
		}
	}
}

/*---------------------------------------------------------------------
函数原型：void ForceModeChosen(void)
函数名称：切换模式选择--相控延迟转换
函数功能：
入口参数：无
出口参数：无
作    者：
日    期：2015年12月
----------------------------------------------------------------------*/
void DelayModeChosen(void)
{	
/**************************** NO1为主路模式 ***************************/
	if((1 == FlagSystem.bit.NO1Preferred) && (1 == SpllRegNO1.SpllOk) && (1 == SpllRegNO2.SpllOk))
	{
		if(1 == StateSystem.bit.NO1Active)
		{
			CountSystem.DelayProtectNO2 = 0;
			CountSystem.DelayProtectNO1++;		
			if(CountSystem.DelayProtectNO1 >= DELAY_PROTECT)
			{
				CountSystem.DelayProtectNO1 = DELAY_PROTECT;
				
				// 相控延迟转换		85%380V ~ 90%380V
				if((PwmIsrReg.NO1URms > AC_VOLLOW_DLC) && (PwmIsrReg.NO1URms < AC_VOLLOW_DLC + AC_VOL_RESUM5))
				{
					if (abs(SpllRegNO2.SpllTheta - SpllRegOut.SpllTheta) < 28)
					{
						CountSystem.DelayChgULow++;
						if(CountSystem.DelayChgULow >= 1)
						{
							CountSystem.DelayChgULow = 1;
							ConvertState = ConvertForce;
						}
					}
				}
				else
				{
					CountSystem.DelayChgULow = 0;
				}
				
				// 低压直接转换 	< 83%380V
				if(PwmIsrReg.NO1URms < AC_VOLLOW_DRC)
				{
					if(++CountSystem.DirectChg >= DELAY_DIRECT_CHANGE)
					{
						CountSystem.DirectChg = DELAY_DIRECT_CHANGE;
						ConvertState = ConvertForce;
					}
				}
				else
				{
					CountSystem.DirectChg = 0;
				}
			}
		}
		if(1 == StateSystem.bit.NO2Active)
		{
			CountSystem.DelayProtectNO1 = 0;		
			CountSystem.DelayProtectNO2++;
			if(CountSystem.DelayProtectNO2 >= DELAY_PROTECT)
			{
				CountSystem.DelayProtectNO2 = DELAY_PROTECT;

				// 复位转换		90%380V ~ 110%380V
				if((PwmIsrReg.NO1URms > AC_VOLLOW_NORMAL) && (PwmIsrReg.NO1URms < AC_VOLHIGH_NORMAL))
				{
					CountSystem.DelayNormDetc++;
					if(CountSystem.DelayNormDetc >= 1000)
					{
						CountSystem.DelayNormDetc = 1000;

						if (abs(SpllRegNO1.SpllTheta - SpllRegOut.SpllTheta) < 28)
						{
							CountSystem.DelayResetChg++;
							if(CountSystem.DelayResetChg >= 1)
							{
								CountSystem.DelayResetChg = 1;
								ConvertState = ConvertReset;
							}
						}
						else
						{
							CountSystem.DelayResetChg = 0;
						}
					}
				}
				else
				{
					CountSystem.DelayNormDetc = 0;
					CountSystem.DelayResetChg = 0;
				}
			}
		}
	}

/**************************** NO2为主路模式 ***************************/	
	if((1 == FlagSystem.bit.NO2Preferred) && (1 == SpllRegNO1.SpllOk) && (1 == SpllRegNO2.SpllOk))
	{
		if(1 == StateSystem.bit.NO2Active)
		{
			CountSystem.DelayProtectNO1 = 0;
			CountSystem.DelayProtectNO2++;		
			if(CountSystem.DelayProtectNO2 >= DELAY_PROTECT)
			{
				CountSystem.DelayProtectNO2 = DELAY_PROTECT;
				
				// 相控延迟转换		85%380V ~ 90%380V
				if((PwmIsrReg.NO2URms > AC_VOLLOW_DLC) && (PwmIsrReg.NO2URms < AC_VOLLOW_DLC + AC_VOL_RESUM5))
				{
					if (abs(SpllRegNO1.SpllTheta - SpllRegOut.SpllTheta) < 28)
					{
						CountSystem.DelayChgULow++;
						if(CountSystem.DelayChgULow >= 1)
						{
							CountSystem.DelayChgULow = 1;
							ConvertState = ConvertForce;
						}
					}
				}
				else
				{
					CountSystem.DelayChgULow = 0;
				}
				
				// 低压直接转换 	<83%380V
				if(PwmIsrReg.NO2URms < AC_VOLLOW_DRC)
				{
					if(++CountSystem.DirectChg >= DELAY_DIRECT_CHANGE)
					{
						CountSystem.DirectChg= DELAY_DIRECT_CHANGE;
						ConvertState = ConvertForce;
					}
				}
				else
				{
					CountSystem.DirectChg = 0;
				}
			}
		}
		
		if(1 == StateSystem.bit.NO1Active)
		{
			CountSystem.DelayProtectNO2 = 0;		
			CountSystem.DelayProtectNO1++;
			if(CountSystem.DelayProtectNO1 >= DELAY_PROTECT)
			{
				CountSystem.DelayProtectNO1 = DELAY_PROTECT;
				// 复位转换		90%380V ~ 110%380V
				if((PwmIsrReg.NO2URms > AC_VOLLOW_NORMAL) && (PwmIsrReg.NO2URms < AC_VOLHIGH_NORMAL))
				{
					CountSystem.DelayNormDetc++;
					if(CountSystem.DelayNormDetc >= 1000)
					{
						CountSystem.DelayNormDetc = 1000;

						if (abs(SpllRegNO2.SpllTheta - SpllRegOut.SpllTheta) < 28)
						{
							CountSystem.DelayResetChg++;
							if(CountSystem.DelayResetChg >= 1)
							{
								CountSystem.DelayResetChg = 1;
								ConvertState = ConvertReset;
							}
						}
						else
						{
							CountSystem.DelayResetChg = 0;
						}
					}
				}
				else
				{
					CountSystem.DelayNormDetc = 0;
					CountSystem.DelayResetChg = 0;
				}
			}
		}
	}
}

/*---------------------------------------------------------------------
函数原型：void InVolProtect(void)
函数名称：系统保护类型--输入电压保护(低压、高压保护)
函数功能：
入口参数：无
出口参数：无
作    者：
日    期：2015年12月
----------------------------------------------------------------------*/
void InVolProtect(void)
{
/* ----------------NO1电路输入电压保护--------------------*/
	if(PwmIsrReg.NO1URms < ParaTable.RunCtrlReg.uNO1VolLower)
	{
		if(++CountSystem.NO1UrmsLow > ParaTable.RunCtrlReg.uNO1VolLowDelay)
		{
			CountSystem.NO1UrmsLow = ParaTable.RunCtrlReg.uNO1VolLowDelay;
			FlagSysAlarm.bit.NO1InVolLowAlarm = 1;
		}
	}
	else if(PwmIsrReg.NO1URms > ParaTable.RunCtrlReg.uNO1VolUpper)
	{
		if(++CountSystem.NO1UrmsHigh > ParaTable.RunCtrlReg.uNO1VolUpDelay)
		{
			CountSystem.NO1UrmsHigh = ParaTable.RunCtrlReg.uNO1VolUpDelay;
			FlagSysAlarm.bit.NO1InVolHighAlarm = 1;
		}
	}
	else if((PwmIsrReg.NO1URms >= ParaTable.RunCtrlReg.uNO1VolLower + AC_VOL_RESUM5)
	&& (PwmIsrReg.NO1URms <= ParaTable.RunCtrlReg.uNO1VolUpper - AC_VOL_RESUM5))
	{
		CountSystem.NO1UrmsLow 		= 0;
		CountSystem.NO1UrmsHigh 		= 0;
		FlagSysAlarm.bit.NO1InVolLowAlarm = 0;
		FlagSysAlarm.bit.NO1InVolHighAlarm = 0;
	}

/* ----------------NO2电路输入电压保护--------------------*/
	if(PwmIsrReg.NO2URms < ParaTable.RunCtrlReg.uNO2VolLower)
	{
		if(++CountSystem.NO2UrmsLow > ParaTable.RunCtrlReg.uNO2VolLowDelay)
		{
			CountSystem.NO2UrmsLow = ParaTable.RunCtrlReg.uNO2VolLowDelay;
			FlagSysAlarm.bit.NO2InVolLowAlarm = 1;
		}
	}
	else if(PwmIsrReg.NO2URms > ParaTable.RunCtrlReg.uNO2VolUpper)
	{
		if(++CountSystem.NO2UrmsHigh > ParaTable.RunCtrlReg.uNO2VolUpDelay)
		{
			CountSystem.NO2UrmsHigh = ParaTable.RunCtrlReg.uNO2VolUpDelay;
			FlagSysAlarm.bit.NO2InVolHighAlarm = 1;
		}
	}
	else if((PwmIsrReg.NO2URms >= ParaTable.RunCtrlReg.uNO2VolLower + AC_VOL_RESUM5)
	&& (PwmIsrReg.NO2URms <= ParaTable.RunCtrlReg.uNO2VolUpper - AC_VOL_RESUM5))
	{
		CountSystem.NO2UrmsLow 		= 0;
		CountSystem.NO2UrmsHigh 		= 0;
		FlagSysAlarm.bit.NO2InVolLowAlarm = 0;
		FlagSysAlarm.bit.NO2InVolHighAlarm = 0;
	}
}

/*---------------------------------------------------------------------
函数原型：void InCurProtect(void)
函数名称：系统保护类型--输出过流保护
函数功能：
入口参数：无
出口参数：无
作    者：
日    期：2015年12月
----------------------------------------------------------------------*/
void InCurProtect(void)
{
/* ----------------主电路输出过流保护--------------------*/
	if(PwmIsrReg.NO1IRms > ParaTable.RunCtrlReg.uNO1CurAlarm)
	{
		if(++CountSystem.NO1CurHighAlarm > ParaTable.RunCtrlReg.uNO1CurAlarmDelay)
		{
			CountSystem.NO1CurHighAlarm = ParaTable.RunCtrlReg.uNO1CurAlarmDelay;
			FlagSysAlarm.bit.NO1InCurHighAlarm = 1;
		}
		if(PwmIsrReg.NO1IRms > ParaTable.RunCtrlReg.uNO1CurCut)
		{
			if(++CountSystem.NO1CurHighCut > ParaTable.RunCtrlReg.uNO1CurCutDelay)
			{
				CountSystem.NO1CurHighCut = ParaTable.RunCtrlReg.uNO1CurCutDelay;
				FlagSysRunErr.bit.NO1InCurHighCut = 1;
			}
		}
		else
		{
			CountSystem.NO1CurHighCut = 0;
		}
	}
	else if((PwmIsrReg.NO1IRms <= ParaTable.RunCtrlReg.uNO1CurAlarm - AC_CUR_RUSUM5))
	{
		FlagSysAlarm.bit.NO1InCurHighAlarm = 0;
		FlagSysRunErr.bit.NO1InCurHighCut = 0;
		CountSystem.NO1CurHighAlarm = 0;
		CountSystem.NO1CurHighCut = 0;
	}

/* ----------------辅电路输出过流保护--------------------*/
	if(PwmIsrReg.NO2IRms > ParaTable.RunCtrlReg.uNO2CurAlarm)
	{
		if(++CountSystem.NO2CurHighAlarm > ParaTable.RunCtrlReg.uNO2CurAlarmDelay)
		{
			CountSystem.NO2CurHighAlarm = ParaTable.RunCtrlReg.uNO2CurAlarmDelay;
			FlagSysAlarm.bit.NO2InCurHighAlarm = 1;
		}
		if(PwmIsrReg.NO2IRms > ParaTable.RunCtrlReg.uNO2CurCut)
		{
			if(++CountSystem.NO2CurHighCut > ParaTable.RunCtrlReg.uNO2CurCutDelay)
			{
				CountSystem.NO2CurHighCut = ParaTable.RunCtrlReg.uNO2CurCutDelay;
				FlagSysRunErr.bit.NO2InCurHighCut = 1;
			}
		}
		else
		{
			CountSystem.NO2CurHighCut = 0;
		}
	}
	else if((PwmIsrReg.NO2IRms <= ParaTable.RunCtrlReg.uNO2CurAlarm - AC_CUR_RUSUM5))
	{
		FlagSysAlarm.bit.NO2InCurHighAlarm = 0;
		FlagSysRunErr.bit.NO2InCurHighCut = 0;
		CountSystem.NO2CurHighAlarm = 0;
		CountSystem.NO2CurHighCut = 0;
	}
}

/*---------------------------------------------------------------------
函数原型：void SCRProtect(void)
函数名称：系统保护类型--SCR损坏保护
函数功能：
入口参数：无
出口参数：无
作    者：
日    期：2015年12月
----------------------------------------------------------------------*/
void SCRProtect(void)
{
	Uint16 NO1IaTemp, NO1IbTemp, NO1IcTemp, NO2IaTemp, NO2IbTemp, NO2IcTemp;

/*********************** 第一路SCR短路、断路判断  ***********************/
	// 当第一路当前正运行，且输出电流大于额定电流的30%
	if ((1 == StateSystem.bit.NO1Active) && (PwmIsrReg.NO1IRms >= 1228))
	{
	    /*********************** 短路判断  ***********************/
	    // A相   SCR短路检测
	    if ((PwmIsrReg.NO1IaRms > ((PwmIsrReg.NO1IbRms + PwmIsrReg.NO1IcRms) >> 1) * 1.4))
	    {
	        CountSystem.NO1ASCRShortDetc++;
	        if(CountSystem.NO1ASCRShortDetc > DELAY_SCR_SHORT_DETC)
	        {
	            CountSystem.NO1ASCRShortDetc = DELAY_SCR_SHORT_DETC;
	            FlagSysAlarm.bit.NO1ASCRShort = 1;
	        }
	    }

	    //  B相  SCR短路检测
	    if ((PwmIsrReg.NO1IbRms > ((PwmIsrReg.NO1IaRms + PwmIsrReg.NO1IcRms) >> 1) * 1.4))
	    {
	        CountSystem.NO1BSCRShortDetc++;
	        if(CountSystem.NO1BSCRShortDetc > DELAY_SCR_SHORT_DETC)
	        {
	            CountSystem.NO1BSCRShortDetc = DELAY_SCR_SHORT_DETC;
	            FlagSysAlarm.bit.NO1BSCRShort = 1;
	        }
	    }

	    // C相   SCR短路检测
	    if ((PwmIsrReg.NO1IcRms > ((PwmIsrReg.NO1IaRms + PwmIsrReg.NO1IbRms) >> 1) * 1.4))
	    {
	        CountSystem.NO1CSCRShortDetc++;
	        if(CountSystem.NO1CSCRShortDetc > DELAY_SCR_SHORT_DETC)
	        {
	            CountSystem.NO1CSCRShortDetc = DELAY_SCR_SHORT_DETC;
	            FlagSysAlarm.bit.NO1CSCRShort = 1;
	        }
	    }

	    /*********************** 断路判断  ***********************/
	    NO1IaTemp = (Uint16)((Uint32)PwmIsrReg.NO1IaRms * 51 >> 8);       // %20
        NO1IbTemp = (Uint16)((Uint32)PwmIsrReg.NO1IbRms * 51 >> 8);       // %20
        NO1IcTemp = (Uint16)((Uint32)PwmIsrReg.NO1IcRms * 51 >> 8);       // %20

        // A相   SCR断路检测
        if((PwmIsrReg.NO1IaRms < NO1IbTemp) || (PwmIsrReg.NO1IaRms < NO1IcTemp))
        {
            CountSystem.NO1ASCRBrokenDetc ++;
            if(CountSystem.NO1ASCRBrokenDetc >= DELAY_SCR_BROKEN_DETC)
            {
                CountSystem.NO1ASCRBrokenDetc = DELAY_SCR_BROKEN_DETC;
                FlagSysRunErr.bit.NO1ASCRBroken = 1;
            }
        }
        else
        {
            CountSystem.NO1ASCRBrokenDetc = 0;
        }

        // B相   SCR断路检测
        if((PwmIsrReg.NO1IbRms < NO1IaTemp) || (PwmIsrReg.NO1IbRms < NO1IcTemp))
        {
            CountSystem.NO1BSCRBrokenDetc ++;
            if(CountSystem.NO1BSCRBrokenDetc >= DELAY_SCR_BROKEN_DETC)
            {
                CountSystem.NO1BSCRBrokenDetc = DELAY_SCR_BROKEN_DETC;
                FlagSysRunErr.bit.NO1BSCRBroken = 1;
            }
        }
        else
        {
            CountSystem.NO1BSCRBrokenDetc = 0;
        }

        // C相   SCR断路检测
        if((PwmIsrReg.NO1IcRms < NO1IaTemp) || (PwmIsrReg.NO1IcRms < NO1IbTemp))
        {
            CountSystem.NO1CSCRBrokenDetc ++;
            if(CountSystem.NO1CSCRBrokenDetc >= DELAY_SCR_BROKEN_DETC)
            {
                CountSystem.NO1CSCRBrokenDetc = DELAY_SCR_BROKEN_DETC;
                FlagSysRunErr.bit.NO1CSCRBroken = 1;
            }
        }
        else
        {
            CountSystem.NO1CSCRBrokenDetc = 0;
        }
	}


/*********************** 第二路SCR短路、断路判断  ***********************/
    // 当第二路当前正运行，且输出电流大于额定电流的30%
    if ((1 == StateSystem.bit.NO2Active) && (PwmIsrReg.NO2IRms >= 1228))
    {
        /*********************** 短路判断  ***********************/
	    // A相   SCR短路检测
        if ((PwmIsrReg.NO2IaRms > ((PwmIsrReg.NO2IbRms + PwmIsrReg.NO2IcRms) >> 1) * 1.4))
        {
            CountSystem.NO2ASCRShortDetc++;
            if(CountSystem.NO2ASCRShortDetc > DELAY_SCR_SHORT_DETC)
            {
                CountSystem.NO2ASCRShortDetc = DELAY_SCR_SHORT_DETC;
                FlagSysAlarm.bit.NO2ASCRShort = 1;
            }
        }

        // B相   SCR短路检测
        if ((PwmIsrReg.NO2IbRms > ((PwmIsrReg.NO2IaRms + PwmIsrReg.NO2IcRms) >> 1) * 1.4))
        {
            CountSystem.NO2BSCRShortDetc++;
            if(CountSystem.NO2BSCRShortDetc > DELAY_SCR_SHORT_DETC)
            {
                CountSystem.NO2BSCRShortDetc = DELAY_SCR_SHORT_DETC;
                FlagSysAlarm.bit.NO2BSCRShort = 1;
            }
        }

        // C相   SCR短路检测
        if ((PwmIsrReg.NO2IcRms > ((PwmIsrReg.NO2IaRms + PwmIsrReg.NO2IbRms) >> 1) * 1.4))
        {
            CountSystem.NO2CSCRShortDetc++;
            if(CountSystem.NO2CSCRShortDetc > DELAY_SCR_SHORT_DETC)
            {
                CountSystem.NO2CSCRShortDetc = DELAY_SCR_SHORT_DETC;
                FlagSysAlarm.bit.NO2CSCRShort = 1;
            }
        }

        /*********************** 断路判断  ***********************/
		NO2IaTemp = (Uint16)((Uint32)PwmIsrReg.NO2IaRms * 51 >> 8);		// %20
		NO2IbTemp = (Uint16)((Uint32)PwmIsrReg.NO2IbRms * 51 >> 8);		// %20
		NO2IcTemp = (Uint16)((Uint32)PwmIsrReg.NO2IcRms * 51 >> 8);		// %20

		// A相   SCR断路检测
		if((PwmIsrReg.NO2IaRms < NO2IbTemp) || (PwmIsrReg.NO2IaRms < NO2IcTemp))
		{
			CountSystem.NO2ASCRBrokenDetc ++;
			if(CountSystem.NO2ASCRBrokenDetc >= DELAY_SCR_BROKEN_DETC)
			{
				CountSystem.NO2ASCRBrokenDetc = DELAY_SCR_BROKEN_DETC;
				FlagSysRunErr.bit.NO2ASCRBroken = 1;
			}
		}
		else
		{
			CountSystem.NO2ASCRBrokenDetc = 0;
		}

		// B相   SCR断路检测
		if((PwmIsrReg.NO2IbRms < NO2IaTemp) || (PwmIsrReg.NO2IbRms < NO2IcTemp))
		{
			CountSystem.NO2BSCRBrokenDetc ++;
			if(CountSystem.NO2BSCRBrokenDetc >= DELAY_SCR_BROKEN_DETC)
			{
				CountSystem.NO2BSCRBrokenDetc = DELAY_SCR_BROKEN_DETC;
				FlagSysRunErr.bit.NO2BSCRBroken = 1;
			}
		}
		else
		{
			CountSystem.NO2BSCRBrokenDetc = 0;
		}

		// C相   SCR断路检测
		if((PwmIsrReg.NO2IcRms < NO2IaTemp) || (PwmIsrReg.NO2IcRms < NO2IbTemp))
		{
			CountSystem.NO2CSCRBrokenDetc ++;
			if(CountSystem.NO2CSCRBrokenDetc >= DELAY_SCR_BROKEN_DETC)
			{
				CountSystem.NO2CSCRBrokenDetc = DELAY_SCR_BROKEN_DETC;
				FlagSysRunErr.bit.NO2CSCRBroken = 1;
			}
		}
		else
		{
			CountSystem.NO2CSCRBrokenDetc = 0;
		}
	}
}

/*---------------------------------------------------------------------
函数原型：void TempProtect(void)
函数名称：系统保护类型--温度保护
函数功能：
入口参数：无
出口参数：无
作    者：
日    期：2015年12月
----------------------------------------------------------------------*/
void TempProtect(void)
{
    Uint16 TempMax;

    // 温度取最大值
    if (ParaTable.Timer1Reg.uModATemp >= ParaTable.Timer1Reg.uModBTemp)
      {
       TempMax = ParaTable.Timer1Reg.uModATemp;
     }
     else
    {
        TempMax = ParaTable.Timer1Reg.uModBTemp;
    }

    if (ParaTable.Timer1Reg.uModCTemp >= TempMax)
    {
        TempMax = ParaTable.Timer1Reg.uModCTemp;
    }

    // 根据温度开启风扇
    if (TempMax > ParaTable.RunCtrlReg.uFanOpenTemp)
    {
        ControlGPIOOut(GPIO_OUT1, ON);
    }
    else if (TempMax <= (ParaTable.RunCtrlReg.uFanOpenTemp - TEMP_RESUM5))
    {
        ControlGPIOOut(GPIO_OUT1, OFF);
    }


    // 过温保护

    if(TempMax > ParaTable.RunCtrlReg.uTempAlarm)
       {
           if(++CountSystem.DelayTempAlarm > 1000)     // 延迟3S报警
           {
               CountSystem.DelayTempAlarm = 1000;
               FlagSysAlarm.bit.OverTempAlarm = 1;
           }
        if(TempMax > ParaTable.RunCtrlReg.uTempCut)
         {
           if(++CountSystem.DelayTempCut > 1000)       // 延迟3S保护
               {
               CountSystem.DelayTempCut = 1000;
               FlagSysRunErr.bit.OverTempCut = 1;
               }
         }
        else
            {
            FlagSysRunErr.bit.OverTempCut = 0;
            }
       }
       else if (TempMax < ParaTable.RunCtrlReg.uTempAlarm - TEMP_RESUM5)
       {
           CountSystem.DelayTempAlarm     = 0;
           FlagSysAlarm.bit.OverTempAlarm = 0;
           CountSystem.DelayTempCut       = 0;
           FlagSysRunErr.bit.OverTempCut  = 0;
       }

}


/*---------------------------------------------------------------------
函数原型：void PDPClearFunc(Uint16 ClearTime)
函数名称：
函数功能：清除PDP故障
入口参数：ClearTime 延迟清除故障时间 @3ms
出口参数：PDPFlag 故障清除标记： 0-清除  1-未清除
作    者：
日    期：2017年05月03日
----------------------------------------------------------------------*/
void PDPClearFunc(Uint16 ClearTime)
{
	static Uint16 PDPCountTemp = 0;
	Uint16 PDPErrTemp;

	if (++PDPCountTemp >= ClearTime)
	{
		ClrCPLD_STS();           				//清除硬件故障；
		PDPCountTemp = 0;
		PDPErrTemp = ReadCPLD_STS();          	//读取Error_Status；
		if((PDPErrTemp & 0xff) == 0xff)
		{
			FlagSysRunErr.bit.FlagPDP = 0;
			PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
		}
	}
}

