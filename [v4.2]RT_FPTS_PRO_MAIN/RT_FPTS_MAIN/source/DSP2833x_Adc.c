// TI初始化文件
// 更新：2014-07-10
//###########################################################################
//
// 文件:	DSP2833x_Adc.c
//
// 标题:	用于控制DSP外设AD
//
// 描述:	1. 初始化AD
//		2. 启动AD
//		3. AD采样
//				
//###########################################################################  

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

extern struct   AD_BUFS                 DataInst;
extern struct   AD_BUFS                 DataZero;
extern union    FLAG_SYSTEM_INITERR 	FlagSysInitErr;
extern union    FlagSystem				FlagSystem;

void ADAdjustOfSOC(Uint16 FlagEn);
void ADAdjustOfDMA(Uint16 FlagEn);
void DelayUs(volatile int Usec);

extern volatile Uint16 DMABuf[20];

/*---------------------------------------------------------------------
函数原型：void InitADCOfDMA(void)
函数名称：AD初始化
函数功能：配置成DMA模式直接读取，采用连续采样
入口参数：无
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void InitADCOfDMA(void)                         //配置成DMA模式
{
    Uint16 i, j;

    AdcRegs.ADCTRL3.bit.ADCBGRFDN   = 3;        // 参考电路上电
    for(i=0;i<10000;i++)    NOP;
    AdcRegs.ADCTRL3.bit.ADCPWDN     = 1;        // 模拟电路上电
    for(i=0;i<10000;i++)    NOP;

    AdcRegs.ADCREFSEL.bit.REF_SEL   = 0x00;     // 内部参考3V
    AdcRegs.ADCTRL3.bit.ADCCLKPS    = 3;        // 核3分频
    AdcRegs.ADCTRL3.bit.SMODE_SEL   = 1;        // 同步采样方式
    AdcRegs.ADCTRL1.bit.SEQ_CASC    = 1;        // 级联
    AdcRegs.ADCTRL1.bit.CONT_RUN    = 1;        // 连续采样模式
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;

    AdcRegs.ADCMAXCONV.all          = 0x07;
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x00;     // 设置ADCINA0&ADCINB0转换
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x01;     // 设置ADCINA1&ADCINB1转换
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x02;     // 设置ADCINA2&ADCINB2转换
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x03;     // 设置ADCINA3&ADCINB3转换
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x04;     // 设置ADCINA4&ADCINB4转换
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x05;     // 设置ADCINA5&ADCINB5转换
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x06;     // 设置ADCINA6&ADCINB6转换
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x07;     // 设置ADCINA7&ADCINB7转换

    for(i = 0; i < 1000; i++)
    {
        for(j = 0; j < 5000; j++)   NOP;
    }

    ADStart();

    ADAdjustOfDMA(1);
}

/*---------------------------------------------------------------------
函数原型：void InitADCOfSOC(void)
函数名称：AD初始化
函数功能：配置SOC触发方式启动ADC
入口参数：无
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void InitADCOfSOC(void)         // 配置成标准触发采样模式
{
    Uint16 i, j;

    AdcRegs.ADCTRL3.bit.ADCBGRFDN   = 3;        // 参考电路上电
    for(i=0;i<10000;i++)    NOP;
    AdcRegs.ADCTRL3.bit.ADCPWDN     = 1;        // 模拟电路上电
    for(i=0;i<10000;i++)    NOP;

    AdcRegs.ADCREFSEL.bit.REF_SEL   = 0x00;     // 内部参考3V
    AdcRegs.ADCTRL3.bit.ADCCLKPS    = 3;        // 核3分频
    AdcRegs.ADCTRL3.bit.SMODE_SEL   = 1;        // 同步采样方式
    AdcRegs.ADCTRL1.bit.SEQ_CASC    = 1;        // 级联

    AdcRegs.ADCMAXCONV.all          = 0x07;
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x00;     // 设置ADCINA0&ADCINB0转换
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x01;     // 设置ADCINA1&ADCINB1转换
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x02;     // 设置ADCINA2&ADCINB2转换
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x03;     // 设置ADCINA3&ADCINB3转换
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x04;     // 设置ADCINA4&ADCINB4转换
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x05;     // 设置ADCINA5&ADCINB5转换
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x06;     // 设置ADCINA6&ADCINB6转换
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x07;     // 设置ADCINA7&ADCINB7转换

    for(i = 0; i < 1000; i++)
    {
        for(j = 0; j < 5000; j++)   NOP;
    }
    ADAdjustOfSOC(1);
}

/*---------------------------------------------------------------------
函数原型：void ADStart(void)
函数名称：AD启动转换
函数功能：
入口参数：无
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void ADStart(void)
{
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;		// 软件启动
}

/*---------------------------------------------------------------------
函数原型：void ADSampleOfSOC(void)
函数名称：AD采样
函数功能：触发方式读AD转换原始数据
入口参数：无                 
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void ADSampleOfSOC(void)
{
    DataInst.NO1Ia = AdcRegs.ADCRESULT9	 >> 4;
    DataInst.NO1Ib = AdcRegs.ADCRESULT7  >> 4;
    DataInst.NO1Ic = AdcRegs.ADCRESULT5  >> 4;
    DataInst.NO2Ia = AdcRegs.ADCRESULT15 >> 4;
    DataInst.NO2Ib = AdcRegs.ADCRESULT13 >> 4;
    DataInst.NO2Ic = AdcRegs.ADCRESULT11 >> 4;

    DataInst.NO1Uab = AdcRegs.ADCRESULT8  >> 4;
    DataInst.NO1Ubc = AdcRegs.ADCRESULT10 >> 4;
    DataInst.NO2Uab = AdcRegs.ADCRESULT12 >> 4;
    DataInst.NO2Ubc = AdcRegs.ADCRESULT14 >> 4;
    DataInst.OutUab = AdcRegs.ADCRESULT1  >> 4;
    DataInst.OutUbc = AdcRegs.ADCRESULT3  >> 4;

    DataInst.TempA = AdcRegs.ADCRESULT4 >> 4;
    DataInst.TempB = AdcRegs.ADCRESULT0 >> 4;
    DataInst.TempC = AdcRegs.ADCRESULT6 >> 4;
}

/*---------------------------------------------------------------------
函数原型：void ADSampleOfDMA(void)
函数名称：AD采样
函数功能：读AD转换原始数据
入口参数：无
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
#pragma CODE_SECTION(ADSampleOfDMA, "ramcode")

void ADSampleOfDMA(void)
{
    DataInst.NO1Ia = DMABuf[9];
    DataInst.NO1Ib = DMABuf[7];
    DataInst.NO1Ic = DMABuf[5];
    DataInst.NO2Ia = DMABuf[15];
    DataInst.NO2Ib = DMABuf[13];
    DataInst.NO2Ic = DMABuf[11];

    DataInst.NO1Uab = DMABuf[8];
    DataInst.NO1Ubc = DMABuf[10];
    DataInst.NO2Uab = DMABuf[12];
    DataInst.NO2Ubc = DMABuf[14];
    DataInst.OutUab = DMABuf[1];
    DataInst.OutUbc = DMABuf[3];

    DataInst.TempA = DMABuf[4];
    DataInst.TempB = DMABuf[0];
    DataInst.TempC = DMABuf[6];
}

/*---------------------------------------------------------------------
函数原型：void ADAdjustOfSOC(void)
函数名称：AD采样
函数功能：上电AD通道校零
入口参数：FlagEn  	0 -- 禁用校零功能 , 直接赋粗值DataZero
					1 -- 启用校零
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void ADAdjustOfSOC(Uint16 FlagEn)
{
	Uint16 i;
	int32 temp0 = 0, temp1  = 0, temp2  = 0, temp3  = 0, temp4  = 0,temp5  = 0, temp6  = 0, temp7  = 0,temp8  = 0, temp9  = 0, temp10 = 0;
	int32 temp11 = 0,temp12 = 0, temp13 = 0, temp14 = 0, temp15 = 0;
	if (1 == FlagEn)
	{
		for(i = 0; i < 1024; i++)
		{
			AdcRegs.ADCTRL2.all	= 0x2000;
			DelayUs(1500);

	        while (AdcRegs.ADCASEQSR.bit.SEQ_CNTR > 0)  {;}

			temp0 += AdcRegs.ADCRESULT0 >> 4;
			temp1 += AdcRegs.ADCRESULT1 >> 4;
			temp2 += AdcRegs.ADCRESULT2 >> 4;
			temp3 += AdcRegs.ADCRESULT3 >> 4;
			temp4 += AdcRegs.ADCRESULT4 >> 4;
			temp5 += AdcRegs.ADCRESULT5 >> 4;
			temp6 += AdcRegs.ADCRESULT6 >> 4;
			temp7 += AdcRegs.ADCRESULT7 >> 4;
			temp8 += AdcRegs.ADCRESULT8 >> 4;
			temp9 += AdcRegs.ADCRESULT9 >> 4;
			temp10 += AdcRegs.ADCRESULT10 >> 4;
			temp11 += AdcRegs.ADCRESULT11 >> 4;
			temp12 += AdcRegs.ADCRESULT12 >> 4;
			temp13 += AdcRegs.ADCRESULT13 >> 4;
			temp14 += AdcRegs.ADCRESULT14 >> 4;
			temp15 += AdcRegs.ADCRESULT15 >> 4;
		}

		DataZero.NO1Ia = temp9  >> 10;
		DataZero.NO1Ib = temp7  >> 10;
		DataZero.NO1Ic = temp5  >> 10;
		DataZero.NO2Ia = temp15 >> 10;
		DataZero.NO2Ib = temp13 >> 10;
		DataZero.NO2Ic = temp11 >> 10;

		DataZero.NO1Uab = temp8  >> 10;
		DataZero.NO1Ubc = temp10 >> 10;
		DataZero.NO2Uab = temp12 >> 10;
		DataZero.NO2Ubc = temp14 >> 10;
		DataZero.OutUab = temp1  >> 10;
		DataZero.OutUbc = temp3  >> 10;

		DataZero.TempA = temp4 >> 10;
		DataZero.TempB = temp0 >> 10;
		DataZero.TempC = temp6 >> 10;
	}
	else
	{
        DataZero.NO1Ia = 2048;
        DataZero.NO1Ib = 2048;
        DataZero.NO1Ic = 2048;
        DataZero.NO2Ia = 2048;
        DataZero.NO2Ib = 2048;
        DataZero.NO2Ic = 2048;

        DataZero.NO1Uab = 2048;
        DataZero.NO1Ubc = 2048;
        DataZero.NO2Uab = 2048;
        DataZero.NO2Ubc = 2048;
        DataZero.OutUab = 2048;
        DataZero.OutUbc = 2048;

        DataZero.TempA = 0;
        DataZero.TempB = 0;
        DataZero.TempC = 0;
	}
}


void ADAdjustOfDMA(Uint16 FlagEn)
{
    Uint16 i;
    int32 temp0 = 0, temp1  = 0, temp2  = 0, temp3  = 0, temp4  = 0;
    int32 temp5 = 0, temp6  = 0, temp7  = 0, temp8  = 0, temp9  = 0;
    int32 temp10 = 0,temp11 = 0, temp12 = 0, temp13 = 0, temp14 = 0, temp15 = 0;

    if (1 == FlagEn)
    {
        for(i = 0; i < 1024; i++)
        {
            ADSampleOfDMA();

            temp0 += DMABuf[0];
            temp1 += DMABuf[1];
            temp2 += DMABuf[2];
            temp3 += DMABuf[3];
            temp4 += DMABuf[4];
            temp5 += DMABuf[5];
            temp6 += DMABuf[6];
            temp7 += DMABuf[7];
            temp8 += DMABuf[8];
            temp9 += DMABuf[9];
            temp10 += DMABuf[10];
            temp11 += DMABuf[11];
            temp12 += DMABuf[12];
            temp13 += DMABuf[13];
            temp14 += DMABuf[14];
            temp15 += DMABuf[15];

            DelayUs(5000);
        }

        DataZero.NO1Ia = temp9  >> 10;
        DataZero.NO1Ib = temp7  >> 10;
        DataZero.NO1Ic = temp5  >> 10;
        DataZero.NO2Ia = temp15 >> 10;
        DataZero.NO2Ib = temp13 >> 10;
        DataZero.NO2Ic = temp11 >> 10;

        DataZero.NO1Uab = temp8  >> 10;
        DataZero.NO1Ubc = temp10 >> 10;
        DataZero.NO2Uab = temp12 >> 10;
        DataZero.NO2Ubc = temp14 >> 10;
        DataZero.OutUab = temp1  >> 10;
        DataZero.OutUbc = temp3  >> 10;

        DataZero.TempA = 0;//temp4 >> 10;
        DataZero.TempB = 0;//temp0 >> 10;
        DataZero.TempC = 0;//temp6 >> 10;
    }
    else
    {
        DataZero.NO1Ia = 2048;
        DataZero.NO1Ib = 2048;
        DataZero.NO1Ic = 2048;
        DataZero.NO2Ia = 2048;
        DataZero.NO2Ib = 2048;
        DataZero.NO2Ic = 2048;

        DataZero.NO1Uab = 2048;
        DataZero.NO1Ubc = 2048;
        DataZero.NO2Uab = 2048;
        DataZero.NO2Ubc = 2048;
        DataZero.OutUab = 2048;
        DataZero.OutUbc = 2048;

        DataZero.TempA = 0;
        DataZero.TempB = 0;
        DataZero.TempC = 0;
    }
}


//===========================================================================
// End of file.
//===========================================================================
