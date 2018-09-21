/*****************头文件********************/
#include "USERS_Arithmetic.h"

/*---------------------------------------------------------------------
函数原型：int16 SpllCalc(struct SPLL_REGS *spllreg, int16 Uab, int16 Ubc)
函数名称：三相锁相环算法
函数功能：带有DSC的锁相环计算
入口参数：*spllreg--运算寄存器，Uab--AB相线电压瞬时值，Ubc--BC相电压瞬时值
出口参数：是否锁相成功
作    者：
日    期：2015年06月26日
----------------------------------------------------------------------*/
#pragma CODE_SECTION(SpllCalc, "ramcode")

int16 SpllCalc(struct SPLL_REGS *spllreg, int16 Uab, int16 Ubc)
{
	int16 *p_alpha, *p_beta;

	p_alpha = spllreg->Ualpha;
	p_beta  = spllreg->Ubeta;

    *(p_alpha + spllreg->VsdqIndex) = ((int32)(2 * Uab + Ubc) * SQRT3INV3) >> 10;
    *(p_beta  + spllreg->VsdqIndex) = Ubc;

    *(p_alpha + spllreg->VsdqIndex) = ((int32)*(p_alpha + spllreg->VsdqIndex) * SQRT2INV2) >> 10;
    *(p_beta  + spllreg->VsdqIndex) = ((int32)*(p_beta  + spllreg->VsdqIndex) * SQRT2INV2) >> 10;

	if(spllreg->VsdqIndex < CONST_Nd)
	{
		spllreg->VsdqIndex1 = CONST_PNMem + spllreg->VsdqIndex - CONST_Nd;
	}
	else
	{
		spllreg->VsdqIndex1 = spllreg->VsdqIndex - CONST_Nd;
	}

	spllreg->SpllUmAlphaP = (*(p_alpha + spllreg->VsdqIndex)  - *(p_beta  + spllreg->VsdqIndex1)) >> 1;
	spllreg->SpllUmBetaP  = (*(p_beta  + spllreg->VsdqIndex)  + *(p_alpha + spllreg->VsdqIndex1)) >> 1;

	spllreg->SpllVcdP = ((int32)spllreg->SpllUmBetaP  * spllreg->SpllSina + (int32)spllreg->SpllUmAlphaP * spllreg->SpllCosa) >> 12;	//Q12*Q12>>12--->Q12
	spllreg->SpllVcqP = ((int32)spllreg->SpllUmBetaP  * spllreg->SpllCosa - (int32)spllreg->SpllUmAlphaP * spllreg->SpllSina) >> 12;	//vSd和vSq通过Park变换

	spllreg->SpllVcdFineP = (spllreg->SpllVcdFineP + spllreg->SpllVcdP) >> 1;
	spllreg->SpllVcqFineP = (spllreg->SpllVcqFineP + spllreg->SpllVcqP) >> 1;

	// PI部分
	spllreg->SpllErr	 = spllreg->SpllVcqFineP;
	//比例输出
    spllreg->SpllUp = ((int32)spllreg->SpllKp * spllreg->SpllErr) >> 12;

    //Q12,饱和前输出
    spllreg->SpllSat = spllreg->SpllUp + (spllreg->SpllUi >> 12);

    //Q12,输出饱和限制
    if		(spllreg->SpllSat > spllreg->SpllPIOutMax) 	spllreg->SpllPIOut = spllreg->SpllPIOutMax;
    else if	(spllreg->SpllSat < spllreg->SpllPIOutMin) 	spllreg->SpllPIOut = spllreg->SpllPIOutMin;
    else 	 spllreg->SpllPIOut = spllreg->SpllSat;

    //Q12,计算饱和误差
    spllreg->SpllSatErr = spllreg->SpllPIOut - spllreg->SpllSat;

	//Q22,计算带饱和校正的积分输出
    spllreg->SpllUi += ((int32)spllreg->SpllKi * spllreg->SpllErr + (long)spllreg->SpllKc * spllreg->SpllSatErr);

    if		(spllreg->SpllUi > (int32)spllreg->SpllPIOutMax << 12) 	spllreg->SpllUi = (int32)spllreg->SpllPIOutMax << 12;
    else if	(spllreg->SpllUi < (int32)spllreg->SpllPIOutMin << 12) 	spllreg->SpllUi = (int32)spllreg->SpllPIOutMin << 12;

	 //计算角频率；
	spllreg->SpllPIOut  =  (spllreg->SpllPIOut + SPLL_FREQBASE) >> 2;
	spllreg->SpllWe   	=  ((int32)3 * spllreg->SpllWe + spllreg->SpllPIOut) >> 2;

	if(++spllreg->VsdqIndex >= CONST_PNMem)
	{
		spllreg->VsdqIndex = 0; 		//这句位置不能提前到计算正负序之前
	}

	//计算角度值；
	spllreg->SpllThetaSum  	=  (spllreg->SpllThetaSum + spllreg->SpllWe);                 //Theta:Q10;
//	spllreg->SpllTheta    = (spllreg->SpllThetaSum / CONST_W) & 0x03ff;
	spllreg->SpllTheta    = (spllreg->SpllThetaSum >> CONST_YW) & 0x03ff;

	if (spllreg->SpllTheta < 0)
	{
		spllreg->SpllTheta = spllreg->SpllTheta + 1024;
	}

	spllreg->SpllSina = SinTab[spllreg->SpllTheta];               				//sin值：q12
	spllreg->SpllCosa = SinTab[(spllreg->SpllTheta + 256) & 0x03ff];

	//SPLL误差计数
	if(spllreg->SpllWe  < 1038 && spllreg->SpllWe  > 1010)
	{
		spllreg->SpllCnt--;
		if(spllreg->SpllCnt < 0)
		{
			spllreg->SpllCnt = 0;
			spllreg->SpllOk  = 1;
		}
	}
	else
	{
		spllreg->SpllCnt++;
		if(spllreg->SpllCnt > 9000)
		{
			spllreg->SpllCnt = 9000;
			spllreg->SpllOk  = 0;
		}
	}

	if(0 == spllreg->SpllOk)
	{
		if(spllreg->Count_timeout++ > SPLL_TIMEOUT)					// 反相序或断线故障判断
		{
			spllreg->Count_timeout = SPLL_TIMEOUT;
			spllreg->SpllErr_Flag = 1;
		}
	}
	else
	{
		spllreg->SpllErr_Flag = 0;
		spllreg->Count_timeout = 0;
	}

	return spllreg->SpllErr_Flag;
}

/*---------------------------------------------------------------------
函数原型：void UinstCalc(struct CAL_UINST_REGS *UinstRegs, int16 Theta, int16 Uab, int16 Ubc)
函数名称：线电压瞬时值计算
函数功能：计算三相电压瞬时值
入口参数：
出口参数：无
作    者：
日    期：2015年12月
----------------------------------------------------------------------*/
void UinstCalc(struct CALC_UINST_REGS *UinstRegs, int16 Theta, int16 Uab, int16 Ubc)
{
	UinstRegs->Theta = Theta;
	UinstRegs->Sina = SinTab[UinstRegs->Theta];
	UinstRegs->Cosa = SinTab[(UinstRegs->Theta + 256) & 0x03ff];

    UinstRegs->Ualpha  = ((int32)(2 * Uab + Ubc) * SQRT3INV3) >> 10;
    UinstRegs->Ubeta   = Ubc;

    UinstRegs->Ualpha  = ((int32)UinstRegs->Ualpha * SQRT2INV2) >> 10;
    UinstRegs->Ubeta   = ((int32)UinstRegs->Ubeta  * SQRT2INV2) >> 10;

    UinstRegs->Usd     = ((int32)UinstRegs->Ubeta * UinstRegs->Sina + (int32)UinstRegs->Ualpha * UinstRegs->Cosa) >> 12;
    UinstRegs->Usq     = ((int32)UinstRegs->Ubeta * UinstRegs->Cosa - (int32)UinstRegs->Ualpha * UinstRegs->Sina) >> 12;

    UinstRegs->UsdFine = (UinstRegs->UsdFine + UinstRegs->Usd) >> 1;
    UinstRegs->UsqFine = (UinstRegs->UsqFine + UinstRegs->Usq) >> 1;

    UinstRegs->Urms    = sqrt(((int32)UinstRegs->UsdFine * UinstRegs->UsdFine) + \
                              ((int32)UinstRegs->UsqFine * UinstRegs->UsqFine));
}

/*---------------------------------------------------------------------
函数原型：void IinstCalculate(struct CAL_UINST_REGS *UinstRegs, int16 Theta, int16 Uab, int16 Ubc)
函数名称：线电流瞬时值计算
函数功能：计算三相电流瞬时值
入口参数：
出口参数：无
作    者：
日    期：2015年12月
----------------------------------------------------------------------*/
void IinstCalculate(struct CALC_UINST_REGS *IinstRegs, int16 Theta, int16 Ia, int16 Ic)
{
    int16 Ib = -Ia - Ic;

    IinstRegs->Theta   = Theta;
    IinstRegs->Sina    = SinTab[IinstRegs->Theta];
    IinstRegs->Cosa    = SinTab[(IinstRegs->Theta + 256) & 0x03ff];

    IinstRegs->Ualpha  = (Ia - (Ib >> 1) - (Ic >> 1));
    IinstRegs->Ubeta   = ((int32)(Ib - Ic) * SQRT3DIV2) >> 10;

    IinstRegs->Ualpha  = ((int32)IinstRegs->Ualpha * SQRT2DIV3) >> 10;
    IinstRegs->Ubeta   = ((int32)IinstRegs->Ubeta  * SQRT2DIV3) >> 10;

    IinstRegs->Usd     = ((int32)IinstRegs->Ubeta * IinstRegs->Sina + (int32)IinstRegs->Ualpha * IinstRegs->Cosa) >> 12;
    IinstRegs->Usq     = ((int32)IinstRegs->Ubeta * IinstRegs->Cosa - (int32)IinstRegs->Ualpha * IinstRegs->Sina) >> 12;

    IinstRegs->UsdFine = (IinstRegs->UsdFine + IinstRegs->Usd) >> 1;
    IinstRegs->UsqFine = (IinstRegs->UsqFine + IinstRegs->Usq) >> 1;

    IinstRegs->Urms    = sqrt(((int32)IinstRegs->UsdFine * IinstRegs->UsdFine) + \
                              ((int32)IinstRegs->UsqFine * IinstRegs->UsqFine));
}

/*---------------------------------------------------------------------
函数原型：Uint16 URmsCalc(int16 Vq, struct CAL_URMS_REGS *Cal_V, Uint16 nCycle)
函数名称：采用滑摸计算的单相电压/电流有效值
函数功能：计算半波电压/电流有效值,适用于实时性不高的场合，如显示。
入口参数：data-瞬时标幺值Q12，*Cal_V-结构体指针，nCycle-每个周期内的采样点数=fs/f0（fs-采样频率/f0-正弦波频率）
出口参数：
作    者：
日    期：2017年9月
----------------------------------------------------------------------*/
#pragma CODE_SECTION(URmsCalc, "ramcode")

int16 URmsCalc(int16 data, struct URMS_REGS *URmsRegs, Uint16 nCycle)
{
	if(1 == URmsRegs->FirstCycleCalc)				// 第二个周期开始保存被替换的数据
	{
		URmsRegs->ReplaceData = URmsRegs->Data[URmsRegs->DataIndex];
	}

	URmsRegs->Data[URmsRegs->DataIndex] = data;		// 新数据代替老数据

	if(0 == URmsRegs->FirstCycleCalc)				// 第一个周期做直接累加
	{
		URmsRegs->Ssum += (int32)URmsRegs->Data[URmsRegs->DataIndex] * URmsRegs->Data[URmsRegs->DataIndex];
		if(URmsRegs->DataIndex == (nCycle - 1))
		{
			URmsRegs->FirstCycleCalc = 1;
			URmsRegs->URms = sqrt(URmsRegs->Ssum / nCycle);
		}
	}
	else	// 第二个周期开始做替换累加
	{
		URmsRegs->Ssum += (int32)URmsRegs->Data[URmsRegs->DataIndex] * URmsRegs->Data[URmsRegs->DataIndex];
		URmsRegs->Ssum -= (int32)URmsRegs->ReplaceData * URmsRegs->ReplaceData;
		URmsRegs->URms = sqrt(URmsRegs->Ssum / nCycle);
	}

	URmsRegs->DataIndex++;		// 此句不能提前
	if(URmsRegs->DataIndex >= nCycle)
	{
		URmsRegs->DataIndex = 0;
	}

	return URmsRegs->URms;
}

/*---------------------------------------------------------------------
函数原型：void HalfCycleUrmsCalc(struct HALF_URMS_REGS *URmsRegs, int16 data, Uint16 nCycle)
函数名称：半波有效值计算
函数功能：计算带有直流偏置的半波有效值
入口参数：*URmsRegs--数据处理结构体，data--Q12瞬时值，nCycle--周期采样数
出口参数：
作    者：
日    期：2017年1月
----------------------------------------------------------------------*/
#pragma CODE_SECTION(HalfCycleUrmsCalc, "ramcode")

Uint16 HalfCycleUrmsCalc(struct HALF_URMS_REGS *URmsRegs, int16 data, Uint16 nCycle)
{
	Uint16 i;
	int32 temp;

	URmsRegs->LowLever = nCycle >> 2;
	URmsRegs->HighLever = nCycle - URmsRegs->LowLever;

	if(0 == URmsRegs->DataSaveFlag)
	{
		URmsRegs->DataIndex++;
		URmsRegs->Data[URmsRegs->DataIndex] = data;
		if(URmsRegs->Data[URmsRegs->DataIndex] >= URmsRegs->LargestData)
		{
			URmsRegs->LargestData = URmsRegs->Data[URmsRegs->DataIndex];
			URmsRegs->LargestNum = URmsRegs->DataIndex;
		}

		if(URmsRegs->DataIndex == nCycle)
		{
			URmsRegs->DataIndex = 0;
			URmsRegs->DataSaveFlag = 1;
		}
	}
	else
	{
		if((URmsRegs->LargestNum > URmsRegs->LowLever) && (URmsRegs->LargestNum <= URmsRegs->HighLever))
		{
			URmsRegs->BeginIndex1 = 1;
			URmsRegs->EndIndex1 = URmsRegs->LargestNum - URmsRegs->LowLever;
			URmsRegs->BeginIndex2 = URmsRegs->LargestNum + URmsRegs->LowLever;
			URmsRegs->EndIndex2 = nCycle;

			for(i = URmsRegs->BeginIndex1; i <= URmsRegs->EndIndex1; i++)
			{
				URmsRegs->VolBias += URmsRegs->Data[i];
			}
			for(i = URmsRegs->BeginIndex2; i < URmsRegs->EndIndex2; i++)
			{
				URmsRegs->VolBias += URmsRegs->Data[i];
			}
			URmsRegs->VolBias = URmsRegs->VolBias * 2 / nCycle;

			for(i = (URmsRegs->EndIndex1 + 1); i < URmsRegs->BeginIndex2; i++)
			{
				temp = URmsRegs->Data[i] - URmsRegs->VolBias;
				URmsRegs->Ssum += temp * temp;
			}
			URmsRegs->URms = sqrt(URmsRegs->Ssum * 2 / nCycle);
			URmsRegs->Ssum = 0;
			URmsRegs->VolBias = 0;
			URmsRegs->LargestData = 0;
			URmsRegs->DataSaveFlag = 0;
		}
		else if(URmsRegs->LargestNum <= URmsRegs->LowLever)
		{
			URmsRegs->BeginIndex1 = URmsRegs->LargestNum + URmsRegs->LowLever;
			URmsRegs->EndIndex1 = URmsRegs->BeginIndex1 + (nCycle >> 1) - 1;

			for(i = URmsRegs->BeginIndex1; i <= URmsRegs->EndIndex1; i++)
			{
				URmsRegs->VolBias += URmsRegs->Data[i];
			}
			URmsRegs->VolBias = URmsRegs->VolBias * 2 / nCycle;

			for(i = 1; i <= (URmsRegs->BeginIndex1 - 1); i++)
			{
				temp = URmsRegs->Data[i] - URmsRegs->VolBias;
				URmsRegs->Ssum += temp * temp;
			}
			for(i = (URmsRegs->EndIndex1 + 1); i <= nCycle; i++)
			{
				temp = URmsRegs->Data[i] - URmsRegs->VolBias;
				URmsRegs->Ssum += temp * temp;
			}
			URmsRegs->URms = sqrt(URmsRegs->Ssum * 2 / nCycle);
			URmsRegs->Ssum = 0;
			URmsRegs->VolBias = 0;
			URmsRegs->LargestData = 0;
			URmsRegs->DataSaveFlag = 0;
		}
		else if(URmsRegs->LargestNum > URmsRegs->HighLever)
		{
			URmsRegs->EndIndex1 = URmsRegs->LargestNum - URmsRegs->LowLever - 1;
			URmsRegs->BeginIndex1 = URmsRegs->EndIndex1 - (nCycle >> 1) + 1;

			for(i = URmsRegs->BeginIndex1; i <= URmsRegs->EndIndex1; i++)
			{
				URmsRegs->VolBias += URmsRegs->Data[i];
			}
			URmsRegs->VolBias = URmsRegs->VolBias * 2 / nCycle;

			for(i = 1; i <= (URmsRegs->BeginIndex1 - 1); i++)
			{
				temp = URmsRegs->Data[i] - URmsRegs->VolBias;
				URmsRegs->Ssum += temp * temp;
			}
			for(i = (URmsRegs->EndIndex1 + 1); i <= nCycle; i++)
			{
				temp = URmsRegs->Data[i] - URmsRegs->VolBias;
				URmsRegs->Ssum += temp * temp;
			}
			URmsRegs->URms = sqrt(URmsRegs->Ssum * 2 / nCycle);
			URmsRegs->Ssum = 0;
			URmsRegs->VolBias = 0;
			URmsRegs->LargestData = 0;
			URmsRegs->DataSaveFlag = 0;
		}
	}
	return URmsRegs->URms;
}

/*---------------------------------------------------------------------
函数原型：void svpwm(struct SVPWM_REGS *svpwm, int16 Vsd, int16 Vsq, int16 Sina, int16 Cosa)
函数名称：SVPWM计算
函数功能：计算SVPWM空间矢量PWM定时器比较器值
入口参数：*svpwm--数据处理结构体， Vsd--d轴瞬时值		Q12
							Vsq--q轴瞬时值	Q12
							Cosa--cos_a值    	Q12
							Sina--sin_a值    	Q12
出口参数：svpwm->T_CMPR1、T_CMPR2、T_CMPR3赋予定时期比较值
作    者：
日    期：2018年1月
----------------------------------------------------------------------*/
//#pragma CODE_SECTION(svpwm, "ramcode")

void svpwm(struct SVPWM_REGS *svpwm, int16 Vsd, int16 Vsq, int16 Sina, int16 Cosa)
{
	long t_sat;

	svpwm->vSal_ref = ((long)Vsd * Cosa - (long)Vsq * Sina) >> 12;	//Q12*Q12>>12
	svpwm->vSbe_ref = ((long)Vsq * Cosa + (long)Vsd * Sina) >> 12;	//Q12*Q12>>12

	svpwm->sector = 0;

	//CLARKE逆变换,由Vref1,Vref2,Vref3来决定扇区
	svpwm->Vref1 = ClarkInvA(svpwm->vSal_ref, svpwm->vSbe_ref);
	svpwm->Vref2 = ClarkInvB(svpwm->vSal_ref, svpwm->vSbe_ref);
	svpwm->Vref3 = ClarkInvC(svpwm->vSal_ref, svpwm->vSbe_ref);

	//first step
	if(svpwm->Vref1 > 0)
	{
		svpwm->sector += 1;
	}

	if(svpwm->Vref2 > 0)
	{
		svpwm->sector += 2;
	}

	if(svpwm->Vref3 > 0)
	{
		svpwm->sector += 4;
	}

	//second step
	svpwm->X_sv = svpwm->vSbe_ref;
	svpwm->Y_sv = ((long)svpwm->vSbe_ref * 1024 + (long)svpwm->vSal_ref * SQRT3_Q10) >> 11;
	svpwm->Z_sv = ((long)svpwm->vSbe_ref * 1024 - (long)svpwm->vSal_ref * SQRT3_Q10) >> 11;

	switch(svpwm->sector)
	{
		case 1:
			svpwm->t1_sv = svpwm->Z_sv;
			svpwm->t2_sv = svpwm->Y_sv;
			break;
		case 2:
			svpwm->t1_sv = svpwm->Y_sv;
			svpwm->t2_sv = -svpwm->X_sv;
			break;
		case 3:
			svpwm->t1_sv = -svpwm->Z_sv;
			svpwm->t2_sv = svpwm->X_sv;
			break;
		case 4:
			svpwm->t1_sv = -svpwm->X_sv;
			svpwm->t2_sv = svpwm->Z_sv;
			break;
		case 5:
			svpwm->t1_sv = svpwm->X_sv;
			svpwm->t2_sv = -svpwm->Y_sv;
			break;
		case 6:
			svpwm->t1_sv = -svpwm->Y_sv;
			svpwm->t2_sv = -svpwm->Z_sv;
			break;
		default:
			svpwm->t1_sv = 0;
			svpwm->t2_sv = 0;
			break;
	}

	//饱和计算，防止超调，超出内切圆
	if((svpwm->t1_sv + svpwm->t2_sv) > 4096)
	{
		t_sat  = ((long)4096  * 4096) / (svpwm->t1_sv + svpwm->t2_sv);
		svpwm->t1_sat = ((long)svpwm->t1_sv * t_sat) >> 12;
		svpwm->t2_sat = ((long)svpwm->t2_sv * t_sat) >> 12;
	}
	else
	{
		svpwm->t1_sat = svpwm->t1_sv;
		svpwm->t2_sat = svpwm->t2_sv;
	}

	//step3
	svpwm->taon	= (4096 - svpwm->t1_sat - svpwm->t2_sat) / 2;
	svpwm->tbon	= svpwm->taon + svpwm->t1_sat;
	svpwm->tcon	= svpwm->tbon + svpwm->t2_sat;

	//step4
	switch(svpwm->sector)
	{
		case 1:
			svpwm->T_CMPR1 = ((long)svpwm->tbon * svpwm->EPwm1TBPRD) >> 12;
			svpwm->T_CMPR2 = ((long)svpwm->taon * svpwm->EPwm2TBPRD) >> 12;
			svpwm->T_CMPR3 = ((long)svpwm->tcon * svpwm->EPwm3TBPRD) >> 12;
			break;
		case 2:
			svpwm->T_CMPR1 = ((long)svpwm->taon * svpwm->EPwm1TBPRD) >> 12;
			svpwm->T_CMPR2 = ((long)svpwm->tcon * svpwm->EPwm2TBPRD) >> 12;
			svpwm->T_CMPR3 = ((long)svpwm->tbon * svpwm->EPwm3TBPRD) >> 12;
			break;
		case 3:
			svpwm->T_CMPR1 = ((long)svpwm->taon * svpwm->EPwm1TBPRD) >> 12;
			svpwm->T_CMPR2 = ((long)svpwm->tbon * svpwm->EPwm2TBPRD) >> 12;
			svpwm->T_CMPR3 = ((long)svpwm->tcon * svpwm->EPwm3TBPRD) >> 12;
			break;
		case 4:
			svpwm->T_CMPR1 = ((long)svpwm->tcon * svpwm->EPwm1TBPRD) >> 12;
			svpwm->T_CMPR2 = ((long)svpwm->tbon * svpwm->EPwm2TBPRD) >> 12;
			svpwm->T_CMPR3 = ((long)svpwm->taon * svpwm->EPwm3TBPRD) >> 12;
			break;
		case 5:
			svpwm->T_CMPR1 = ((long)svpwm->tcon * svpwm->EPwm1TBPRD) >> 12;
			svpwm->T_CMPR2 = ((long)svpwm->taon * svpwm->EPwm2TBPRD) >> 12;
			svpwm->T_CMPR3 = ((long)svpwm->tbon * svpwm->EPwm3TBPRD) >> 12;
			break;
		case 6:
			svpwm->T_CMPR1 = ((long)svpwm->tbon * svpwm->EPwm1TBPRD) >> 12;
			svpwm->T_CMPR2 = ((long)svpwm->tcon * svpwm->EPwm2TBPRD) >> 12;
			svpwm->T_CMPR3 = ((long)svpwm->taon * svpwm->EPwm3TBPRD) >> 12;
			break;
	}

	if(svpwm->T_CMPR1 >= svpwm->EPwm1TBPRD)
	{
		svpwm->T_CMPR1 = svpwm->EPwm1TBPRD - 1;
	}
	else if(svpwm->T_CMPR1 <= 0)
	{
		svpwm->T_CMPR1 = 1;
	}
	else
	{
		NOP;
	}

	if(svpwm->T_CMPR2 >= svpwm->EPwm2TBPRD)
	{
		svpwm->T_CMPR2 = svpwm->EPwm2TBPRD - 1;
	}
	else if(svpwm->T_CMPR2 <= 0)
	{
		svpwm->T_CMPR2 = 1;
	}
	else
	{
		NOP;
	}

	if(svpwm->T_CMPR3 >= svpwm->EPwm3TBPRD)
	{
		svpwm->T_CMPR3 = svpwm->EPwm3TBPRD - 1;
	}
	else if(svpwm->T_CMPR3 <= 0)
	{
		svpwm->T_CMPR3 = 1;
	}
	else
	{
		NOP;
	}
}

/*---------------------------------------------------------------------
函数原型：int16 PICal(struct PI_REGS *PIReg, int16 input, int16 ref)
函数名称：通用PI计算
函数功能：
入口参数：*PIReg--数据处理结构体，input--控制量，ref--基准量
出口参数：PIReg->Output
作    者：
日    期：2018年1月
----------------------------------------------------------------------*/
#pragma CODE_SECTION(PICal, "ramcode")

int16 PICal(struct PI_REGS *PIReg, int16 input, int16 ref)
{
	int Err, PIUp, PISat, PISatErr;

	PIReg->PiRef = ref;
	PIReg->Input = input;

	Err = PIReg->PiRef - PIReg->Input;		//Q12,误差计算
	PIUp  = ((long)PIReg->Kp * Err) >> 12;	//Q12,比例输出(Q12=Q12*Q12>>12)
	PISat = PIUp + (PIReg->PiUi >> 12);		//Q12,饱和前输出

    if		(PISat > PIReg->PIMax) 	PIReg->Output = PIReg->PIMax;
    else if	(PISat < PIReg->PIMin) 	PIReg->Output = PIReg->PIMin;
    else 							PIReg->Output = PISat;  	 //Q12,输出饱和限制

    PISatErr = PIReg->Output - PISat;		//Q12,计算饱和误差
    PIReg->PiUi += (long)PIReg->Ki * Err + (long)PIReg->Kc * PISatErr; //Q24,计算带饱和校正的积分输出
    if		(PIReg->PiUi > ((long)PIReg->PIMax << 12)) 	PIReg->PiUi = (long)PIReg->PIMax << 12;
    else if	(PIReg->PiUi < ((long)PIReg->PIMin << 12)) 	PIReg->PiUi = (long)PIReg->PIMin << 12;

    return PIReg->Output;
}

/*---------------------------------------------------------------------
函数原型：void PWMRectifier(struct PWMREC_REGS *PwmRegs, int16 Uab, int16 Ubc, int16 ia, int16 ib, int16 Vd, int16 VdcRef)
函数名称：PMW整流
函数功能：DSC锁相环、PI控制器、SVPWM输出
入口参数：*PwmRegs--数据处理结构体，	Uab、Ubc--输入线电压Q12
								Ia、Ib  --输入线电流Q12
								Vd--当前输出直流电压值Q12
								VdcRef--直流电压值参考值  Q12
出口参数：svpwm->T_CMPR1、T_CMPR2、T_CMPR3赋予定时期比较值
作    者：
日    期：2018年1月
----------------------------------------------------------------------*/
//#pragma CODE_SECTION(PWMRectifier, "ramcode")

void PWMRectifier(struct PWMREC_REGS *PwmRegs, int16 Uab, int16 Ubc, int16 Ia, int16 Ib, int16 Vdc, int16 VdcRef)
{
	PwmRegs->SpllErr = SpllCalc(&PwmRegs->SpllReg, Uab, Ubc);
	PwmRegs->Sina = PwmRegs->SpllReg.SpllSina;
	PwmRegs->Cosa = PwmRegs->SpllReg.SpllCosa;

	PwmRegs->Ialpha = Ia;			// Q12
	PwmRegs->Ibeta  = (((long)2 * Ib + Ia) * SQRT3INV3) >> 10;		// Q12

	PwmRegs->Id = ((long)PwmRegs->Ibeta * PwmRegs->Sina + (long)PwmRegs->Ialpha * PwmRegs->Cosa) >> 12; //Q12*Q12>>12-->Q12
	PwmRegs->Iq = ((long)PwmRegs->Ibeta * PwmRegs->Cosa - (long)PwmRegs->Ialpha * PwmRegs->Sina) >> 12; //Q12*Q12>>12-->Q12

	PwmRegs->IdRef = PICal(&PwmRegs->VdcPI, Vdc, VdcRef);
	PwmRegs->IqRef = 0;

    if(Vdc > 2048)
    {
    	PwmRegs->VdcInv = ((long) 4096 * 4096 / Vdc);    // VDC 的倒数计算  Q12
    }
	else
	{
		PwmRegs->VdcInv = 8192;            				// 如果直流电压过低，则直接赋值200%；
	}

	PwmRegs->Vd = -(PICal(&PwmRegs->IdPI, PwmRegs->Id, PwmRegs->IdRef) - PwmRegs->VdcFeedBack);
	PwmRegs->Vq = -(PICal(&PwmRegs->IqPI, PwmRegs->Iq, PwmRegs->IqRef));

	svpwm(&PwmRegs->Svpwm, PwmRegs->Vd, PwmRegs->Vq, PwmRegs->Sina, PwmRegs->Cosa);
}


/*---------------------------------------------------------------------
函数原型：void LLCConvertor(struct LLC_REGS *CurReg, int16 Cur1, int16 Cur2, int16 Vdc, int16 VdcRef)
函数名称：LLC变换器
函数功能：计算LLC脉冲，恒压恒流控制
入口参数：*CurReg--数据处理结构体，	Cur -- 谐振腔电流有效值Q12
								Vdc -- 当前输出直流电压值Q12
								VdcRef--直流电压值参考值  Q12
出口参数：CurReg->Fre赋予定时期比较值
作    者：
日    期：2018年4月
----------------------------------------------------------------------*/
//#pragma CODE_SECTION(LLCConvertor, "ramcode")

void LLCConvertor(struct LLC_REGS *CurReg, int16 Cur1, int16 Cur2, int16 Vdc, int16 VdcRef)
{
	CurReg->VolPIOut = PICal(&CurReg->VolPI, Vdc, VdcRef);

	// T1变压器恒流控制
	CurReg->Cur1PIOut = PICal(&CurReg->Cur1PI, Cur1, CurReg->Cur1Ref);
	if (CurReg->Cur1PIOut <= CurReg->VolPIOut)
	{
		CurReg->Fre1PIOut = CurReg->Cur1PIOut;
	}
	else
	{
		CurReg->Fre1PIOut = CurReg->VolPIOut;
	}

	CurReg->Fre1 = ((int32)CurReg->FreNom * CurReg->Fre1PIOut) >> 12;
	if (CurReg->Fre1 >= CurReg->FreMax)
	{
		CurReg->Fre1 = CurReg->FreMax;
	}
	else if (CurReg->Fre1 <= CurReg->FreMin)
	{
		CurReg->Fre1 = CurReg->FreMin;
	}

	// T2变压器恒流控制
	CurReg->Cur2PIOut = PICal(&CurReg->Cur2PI, Cur2, CurReg->Cur2Ref);
	if (CurReg->Cur2PIOut <= CurReg->VolPIOut)
	{
		CurReg->Fre2PIOut = CurReg->Cur2PIOut;
	}
	else
	{
		CurReg->Fre2PIOut = CurReg->VolPIOut;
	}

	CurReg->Fre2 = ((int32)CurReg->FreNom * CurReg->Fre2PIOut) >> 12;
	if (CurReg->Fre2 >= CurReg->FreMax)
	{
		CurReg->Fre2 = CurReg->FreMax;
	}
	else if (CurReg->Fre2 <= CurReg->FreMin)
	{
		CurReg->Fre2 = CurReg->FreMin;
	}
}

