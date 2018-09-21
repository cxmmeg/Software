#ifndef _USER_ARITHMETIC_H_
#define _USER_ARITHMETIC_H_

// 根据不同平台定义头文件
#include "DSP2833x_Device.h"    // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"  // DSP2833x Examples Include File

/*----------------------------------------------------------*
*              锁相环调整相关数据                                                            *
*---------------------------------------------------------- */
#define     SPLL_SAMP_FREQ      6400        //  锁相环采样频率，Hz

/* 重要：调制比取2的整数幂，计算角频率，用CONST_YW作移位*/
#define     CONST_YW            7                                       //  取2为底的对数,(Uint16)(logb(CONST_W))

/* 重要：调制比不是2的整数幂，计算角频率，用CONST_W作除法*/
#define     CONST_W             (SPLL_SAMP_FREQ / SPLL_BASIC_FREQ)      //  锁相环，Fpwm / 50Hz = 128
#define     CONST_Nd            (CONST_W >> 2)                          //  锁相环，Fpwm / 50Hz /4 = 32
#define     CONST_PNMem         (CONST_W * 2)                           //  锁相环，Fpwm / 50Hz * 2= 256,取两个周期的值

/*----------------------------------------------------------*
*              基础数据宏定义，无需修改               *
*-----------------------------------------------------------*/
#define 	PI 					3.14159265
#define     SQRT2_Q10           1448    //  根号2，Q10
#define     SQRT3_Q10           1773    //  根号3，Q10
#define     SQRT2INV2           724     //  2分之根号2，Q10
#define     SQRT3INV3           591     //  3分之根号3，Q10
#define     SQRT2DIV3           482     //  3分之根号2，Q10
#define     SQRT3DIV2           887     //  2分之根号3，Q10
#define		SPLL_FREQBASE		4040	//  锁相环，50Hz基波值	Q12
#define 	SPLL_TIMEOUT		10000	//  锁相环，锁相失败报警值 @Fpwm
#define		SPLL_BASIC_FREQ		50		//	基波频率，Hz

// CLARKE变换
#define  	ClarkAlfa(a)			(a)
#define  	ClarkBeta(a,b)			(((long)(2 * b + a) * sqrt3inv)>> 10)

// CLARKE逆变换，alfa,beta均为Q12数据
#define  	ClarkInvA(alfa,beta)	(beta)
#define  	ClarkInvB(alfa,beta)	(((long)SQRT3_Q10  * alfa - (long)beta * 1024) >> 11)
#define  	ClarkInvC(alfa,beta)	(((long)-SQRT3_Q10 * alfa - (long)beta * 1024) >> 11)

// Park变换
#define		ParkD(alfa,beta)		(((long)beta * sina + (long)alfa * cosa) >> 14)
#define		ParkQ(alfa,beta)		(((long)beta * cosa - (long)alfa * sina) >> 14)

// Park逆变换
#define		ParkInvAlfa(d,q)		(((long)d*cosa_theta - (long)q*sina_theta)>>14)
#define		ParkInvBeta(d,q)		(((long)d*sina_theta + (long)q*cosa_theta)>>14)

//------------------------------------------------------------
// SPLL锁相环参数表

//------------------------------------------------------------
struct SPLL_REGS
{
	int16   SpllSina;   			//锁相环路PT调节器输出；
	int16   SpllCosa;   			//用来判断否稳定的计数器；
	int16	SpllUmAlphaP;   		//α值正序分量
	int16   SpllUmBetaP;   			//β值正序分量
	int16   SpllVcdP;   			//d值正序分量
	int16   SpllVcqP;  		 		//q值正序分量
	int16   SpllVcdFineP;   		//d值
	int16   SpllVcqFineP;  		 	//q值
	int16   SpllKp;					//锁相环Kp;
	int16   SpllKi; 				//锁相环Ki

	int16   SpllKc; 				//锁相环KC
	int16   SpllErr;   				//锁相环路误差 ；
	int32   SpllUi;   				//锁相环路积分输出；
	int16   SpllUp;   				//锁相环路比例输出
	int16   SpllSat;   				//锁相环PI饱和输出；；
	int16   SpllSatErr;   			//锁相环路饱和误差输出；
	int16   SpllPIOut;   			//锁相环路PT调节器输出；
	int16	SpllPIOutMax;			//锁相环路PI调节器输出最大值；
	int16	SpllPIOutMin;			//锁相环路PI调节器输出最大值；
	int16   SpllWe;   				//锁相环路PT调节器输出；

	int16   SpllWeLpf;   			//锁相环路PT调节器输出；
	Uint32  SpllThetaSum;   		//锁相环路PT调节器输出；
	int16	SpllTheta;   			//锁相环路PT调节器输出；
	int16   SpllCnt;   				//用来判断否稳定的计数器；
	int16   SpllOk;
	int16	Count_timeout;			//缺相或反相计数
	Uint16	SpllErr_Flag;			//故障
	int16	VsdqIndex;
	int16	VsdqIndex1;
	int16	ThetaA;

	int16	ThetaB;
	int16	ThetaC;
	int16	ThetaA_Zero;
	int16 	Ualpha[(2 * SPLL_SAMP_FREQ / SPLL_BASIC_FREQ) + 5];
	int16	Ubeta[(2 *SPLL_SAMP_FREQ / SPLL_BASIC_FREQ) + 5];
};

//------------------------------------------------------------
// 电压/电流瞬时值计算

//------------------------------------------------------------
struct CALC_UINST_REGS
{
	int16	Ualpha;					// clark变换计算α值
	int16	Ubeta;					// clark变换计算β值
	int16	Usd;					// Park变换计算d轴值
	int16	Usq;					// Park变换计算q轴值
	int16	UsdFine;				// d轴有效值
	int16	UsqFine;				// q轴有效值
	int16	Sina;					// sin值
	int16	Cosa;					// cos值
	int16	Theta;					// 当前角度值
    int16   Urms;                   // 有效值
};

//------------------------------------------------------------
// 有效值计算表

//------------------------------------------------------------
struct	URMS_REGS{
	int16  Data[SPLL_SAMP_FREQ / SPLL_BASIC_FREQ];		// 1	每个周期采样个数
	int16  DataIndex;		// 73	数据索引
	Uint64 Ssum;			// 74-75
	int16  ReplaceData;		// 76
	int16  FirstCycleCalc;	// 77
	int16  URms;			// 78
};

//------------------------------------------------------------
// 半波有效值计算表

//------------------------------------------------------------
struct	HALF_URMS_REGS{
	int16  Data[73];		// 1	每个周期采样个数
	int16  DataIndex;		// 74	数据索引
	int16  LargestNum;		// 75
	int16  LargestData;		// 76
	int16  DataSaveFlag;	// 77
	int16  LowLever;		// 78
	int16  HighLever;		// 79
	int16  BeginIndex1;		// 80
	int16  EndIndex1;		// 81
	int16  BeginIndex2;		// 82
	int16  EndIndex2;		// 83
	Uint64 Ssum;			// 84-87
	int16  URms;			// 88
	int32  VolBias;			// 89
};


//------------------------------------------------------------
// SVPWM寄存器表

//------------------------------------------------------------
struct	SVPWM_REGS{
	int16  vSal_ref;		// 1	alpha值
	int16  vSbe_ref;		// 2	beta值
	int16  sector;			// 3	扇区
	int16  Vref1;			// 4	A相扇区因子
	int16  Vref2;			// 5	B相扇区因子
	int16  Vref3;			// 6	C相扇区因子
	int16  X_sv;			// 7	X方向矢量
	int16  Y_sv;			// 8	Y方向矢量
	int16  Z_sv;			// 9	Z方向矢量
	int16  t1_sv;			// 10	t1方向矢量
	int16  t2_sv;			// 11	t2方向矢量
	int16  t1_sat;			// 12	t1方向饱和校正
	int16  t2_sat;			// 13	t2方向饱和校正
	int16  taon;			// 14	导通时间计算
	int16  tbon;			// 15	导通时间计算
	int16  tcon;			// 16	导通时间计算
	int16  T_CMPR1;			// 17	A相PWM1比较器比较值
	int16  T_CMPR2;			// 18	B相PWM2比较器比较值
	int16  T_CMPR3;			// 19	C相PWM3比较器比较值
	int16  EPwm1TBPRD;		// 20	PWM1定时器周期值
	int16  EPwm2TBPRD;		// 21	PWM2定时器周期值
	int16  EPwm3TBPRD;		// 22	PWM3定时器周期值
};

//------------------------------------------------------------
// PI寄存器组

//------------------------------------------------------------
struct PI_REGS{
	int16 PiRef;			// 1	参考值	Q12
	int16 Input;			// 2	采样值输入	Q12
	int16 Output;			// 3	计算输出	Q12
	int16 Kp;				// 4	比例系数	Q12
	int16 Ki;				// 5	积分系数	Q12
	int16 Kc;				// 6	校正系数	Q12
	int16 PIMax;			// 7	饱和限值	Q12
	int16 PIMin;			// 8			Q12
	int32 PiUi;				// 9	积分值
};

//------------------------------------------------------------
// PWM寄存器组

//------------------------------------------------------------
struct PWMREC_REGS{
	int16 Ialpha;
	int16 Ibeta;
	int16 Id;
	int16 Iq;
	int16 Sina;
	int16 Cosa;
	int16 IdRef;
	int16 IqRef;
	int16 Vd;
	int16 Vq;
	int16 SpllErr;
	int16 VdcFeedBack;
	int16 VdcInv;					// 13

	struct SPLL_REGS 	SpllReg;	// 1243
	struct PI_REGS 		VdcPI;		// 9
	struct PI_REGS 		IdPI;		// 9
	struct PI_REGS 		IqPI;		// 9
	struct SVPWM_REGS 	Svpwm;		// 21
};


//------------------------------------------------------------
// 最大电流追踪寄存器组

//------------------------------------------------------------
struct LLC_REGS{
	int16 VolPIOut;
	int16 Cur1Ref;
	int16 Cur1PIOut;
	int16 Cur2Ref;
	int16 Cur2PIOut;
	int16 Fre1PIOut;
	int16 Fre1;
	int16 Fre2PIOut;
	int16 Fre2;
	int16 FreNom;
	int16 FreMax;
	int16 FreMin;

	struct PI_REGS 	VolPI;
	struct PI_REGS	Cur1PI;
	struct PI_REGS	Cur2PI;
};

//------------------------------------------------------------
// 外部变量声明

//------------------------------------------------------------
extern int16 SinTab[1024];		// 每个周期1024个点，Q12数据存储


/*---------------------------------------------------------------------
函数原型：int16 SpllCalc(struct SPLL_REG *spllreg, int16 Uab, int16 Ubc)
函数名称：三相锁相环算法
函数功能：带有DSC的锁相环计算
入口参数：*spllreg--运算寄存器，Uab--AB相线电压瞬时值，Ubc--BC相线电压瞬时值
出口参数：是否锁相成功
作    者：
日    期：2015年06月26日
----------------------------------------------------------------------*/
int16 SpllCalc(struct SPLL_REGS *spllreg, int16 Uab, int16 Ubc);

/*---------------------------------------------------------------------
函数原型：void UinstCalc(struct CAL_UINST_REGS *UinstRegs, int16 Theta, int16 Uab, int16 Ubc)
函数名称：线电压瞬时值计算
函数功能：计算三相电压瞬时值
入口参数：*UinstRegs--运算寄存器，Theta--当前A相相电压角度值，Uab-线电压瞬时值，Ubc-线电压瞬时值
出口参数：无
作    者：
日    期：2015年12月
----------------------------------------------------------------------*/
void UinstCalc(struct CALC_UINST_REGS *UinstRegs, int16 Theta, int16 Uab, int16 Ubc);

/*---------------------------------------------------------------------
函数原型：Uint16 URmsCalc(int16 Vq, struct CAL_URMS_REGS *Cal_V, Uint16 nCycle)
函数名称：单相电压/电流有效值计算
函数功能：计算半波电压/电流有效值,适用于实时性不高的场合，如显示。
入口参数：data-瞬时标幺值Q12，*Cal_V-结构体指针，nCycle-每个周期内的采样点数=fs/f0（fs-采样频率/f0-正弦波频率），wave-0全波 1半波
出口参数：
注意：        使用前注意结构体初始化
作    者：
日    期：2017年9月
----------------------------------------------------------------------*/
int16 URmsCalc(int16 data, struct URMS_REGS *URmsRegs, Uint16 nCycle);

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
void svpwm(struct SVPWM_REGS *svpwm, int16 Vsd, int16 Vsq, int16 Sina, int16 Cosa);

/*---------------------------------------------------------------------
函数原型：int16 PICal(struct PI_REGS *PIReg, int16 input, int16 ref)
函数名称：通用PI计算
函数功能：
入口参数：*PIReg--数据处理结构体，input--控制量，ref--基准量
出口参数：PIReg->Output
作    者：
日    期：2018年1月
----------------------------------------------------------------------*/
int16 PICal(struct PI_REGS *PIReg, int16 input, int16 ref);

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
void PWMRectifier(struct PWMREC_REGS *PwmRegs, int16 Uab, int16 Ubc, int16 ia, int16 ib, int16 Vd, int16 VdcRef);

/*---------------------------------------------------------------------
函数原型：void LLCConvertor(struct LLC_REGS *CurReg, int16 Cur1, int16 Cur1Ref, int16 Cur2, int16 Cur2Ref, int16 Vdc, int16 VdcRef)
函数名称：LLC变换器
函数功能：计算LLC脉冲，恒压恒流控制
入口参数：*CurReg--数据处理结构体，	Cur -- 谐振腔电流有效值Q12
								Vdc -- 当前输出直流电压值Q12
								VdcRef--直流电压值参考值  Q12
出口参数：CurReg->Fre赋予定时期比较值
作    者：
日    期：2018年4月
----------------------------------------------------------------------*/
void LLCConvertor(struct LLC_REGS *CurReg, int16 Cur1, int16 Cur2, int16 Vdc, int16 VdcRef);

#endif
