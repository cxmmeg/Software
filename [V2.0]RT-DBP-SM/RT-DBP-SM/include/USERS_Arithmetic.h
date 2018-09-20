#ifndef _USER_ARITHMETIC_H_
#define _USER_ARITHMETIC_H_

// ���ݲ�ͬƽ̨����ͷ�ļ�
#include "DSP2833x_Device.h"    // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"  // DSP2833x Examples Include File

/*----------------------------------------------------------*
*              ���໷�����������                                                            *
*---------------------------------------------------------- */
#define     SPLL_SAMP_FREQ      6400        //  ���໷����Ƶ�ʣ�Hz

/* ��Ҫ�����Ʊ�ȡ2�������ݣ������Ƶ�ʣ���CONST_YW����λ*/
#define     CONST_YW            7                                       //  ȡ2Ϊ�׵Ķ���,(Uint16)(logb(CONST_W))

/* ��Ҫ�����ƱȲ���2�������ݣ������Ƶ�ʣ���CONST_W������*/
#define     CONST_W             (SPLL_SAMP_FREQ / SPLL_BASIC_FREQ)      //  ���໷��Fpwm / 50Hz = 180
#define     CONST_Nd            (CONST_W >> 2)                          //  ���໷��Fpwm / 50Hz /4 = 45
#define     CONST_PNMem         (CONST_W * 2)                           //  ���໷��Fpwm / 50Hz * 2= 360,ȡ�������ڵ�ֵ

/*----------------------------------------------------------*
*              �������ݺ궨�壬�����޸�               *
*-----------------------------------------------------------*/
#define 	PI 					3.14159265
#define     SQRT2_Q10           1448    //  ����2��Q10
#define     SQRT3_Q10           1773    //  ����3��Q10
#define     SQRT2INV2           724     //  2��֮����2��Q10
#define     SQRT3INV3           591     //  3��֮����3��Q10
#define     SQRT2DIV3           482     //  3��֮����2��Q10
#define     SQRT3DIV2           887     //  2��֮����3��Q10
#define		SPLL_FREQBASE		4040	//  ���໷��50Hz����ֵ	Q12
#define 	SPLL_TIMEOUT		10000	//  ���໷������ʧ�ܱ���ֵ @Fpwm
#define		SPLL_BASIC_FREQ		50		//	����Ƶ�ʣ�Hz

// CLARKE�任
#define  	ClarkAlfa(a)			(a)
#define  	ClarkBeta(a,b)			(((long)(2 * b + a) * sqrt3inv)>> 10)

// CLARKE��任��alfa,beta��ΪQ12����
#define  	ClarkInvA(alfa,beta)	(beta)
#define  	ClarkInvB(alfa,beta)	(((long)SQRT3_Q10  * alfa - (long)beta * 1024) >> 11)
#define  	ClarkInvC(alfa,beta)	(((long)-SQRT3_Q10 * alfa - (long)beta * 1024) >> 11)

// Park�任
#define		ParkD(alfa,beta)		(((long)beta * sina + (long)alfa * cosa) >> 14)
#define		ParkQ(alfa,beta)		(((long)beta * cosa - (long)alfa * sina) >> 14)

// Park��任
#define		ParkInvAlfa(d,q)		(((long)d*cosa_theta - (long)q*sina_theta)>>14)
#define		ParkInvBeta(d,q)		(((long)d*sina_theta + (long)q*cosa_theta)>>14)

//------------------------------------------------------------
// SPLL���໷������

//------------------------------------------------------------
struct SPLL_REGS
{
	int16   SpllSina;   			//���໷·PT�����������
	int16   SpllCosa;   			//�����жϷ��ȶ��ļ�������
	int16	SpllUmAlphaP;   		//��ֵ�������
	int16   SpllUmBetaP;   			//��ֵ�������
	int16   SpllVcdP;   			//dֵ�������
	int16   SpllVcqP;  		 		//qֵ�������
	int16   SpllVcdFineP;   		//dֵ
	int16   SpllVcqFineP;  		 	//qֵ
	int16   SpllKp;					//���໷Kp;
	int16   SpllKi; 				//���໷Ki

	int16   SpllKc; 				//���໷KC
	int16   SpllErr;   				//���໷·��� ��
	int32   SpllUi;   				//���໷·���������
	int16   SpllUp;   				//���໷·�������
	int16   SpllSat;   				//���໷PI�����������
	int16   SpllSatErr;   			//���໷·������������
	int16   SpllPIOut;   			//���໷·PT�����������
	int16	SpllPIOutMax;			//���໷·PI������������ֵ��
	int16	SpllPIOutMin;			//���໷·PI������������ֵ��
	int16   SpllWe;   				//���໷·PT�����������

	int16   SpllWeLpf;   			//���໷·PT�����������
	Uint32  SpllThetaSum;   		//���໷·PT�����������
	int16	SpllTheta;   			//���໷·PT�����������
	int16   SpllCnt;   				//�����жϷ��ȶ��ļ�������
	int16   SpllOk;
	int16	Count_timeout;			//ȱ��������
	Uint16	SpllErr_Flag;			//����
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
// ��ѹ/����˲ʱֵ����

//------------------------------------------------------------
struct CALC_UINST_REGS
{
	int16	Ualpha;					// clark�任�����ֵ
	int16	Ubeta;					// clark�任�����ֵ
	int16	Usd;					// Park�任����d��ֵ
	int16	Usq;					// Park�任����q��ֵ
	int16	UsdFine;				// d����Чֵ
	int16	UsqFine;				// q����Чֵ
	int16	Sina;					// sinֵ
	int16	Cosa;					// cosֵ
	int16	Theta;					// ��ǰ�Ƕ�ֵ
    int16   Urms;                   // ��Чֵ
};

//------------------------------------------------------------
// ��Чֵ�����

//------------------------------------------------------------
struct	URMS_REGS{
	int16  Data[SPLL_SAMP_FREQ / SPLL_BASIC_FREQ];		// 1	ÿ�����ڲ�������
	int16  DataIndex;		// 73	��������
	Uint32 Ssum;			// 74-75
	int16  ReplaceData;		// 76
	int16  FirstCycleCalc;	// 77
	int16  URms;			// 78
};

//------------------------------------------------------------
// �벨��Чֵ�����

//------------------------------------------------------------
struct	HALF_URMS_REGS{
	int16  Data[73];		// 1	ÿ�����ڲ�������
	int16  DataIndex;		// 74	��������
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
// SVPWM�Ĵ�����

//------------------------------------------------------------
struct	SVPWM_REGS{
	int16  vSal_ref;		// 1	alphaֵ
	int16  vSbe_ref;		// 2	betaֵ
	int16  sector;			// 3	����
	int16  Vref1;			// 4	A����������
	int16  Vref2;			// 5	B����������
	int16  Vref3;			// 6	C����������
	int16  X_sv;			// 7	X����ʸ��
	int16  Y_sv;			// 8	Y����ʸ��
	int16  Z_sv;			// 9	Z����ʸ��
	int16  t1_sv;			// 10	t1����ʸ��
	int16  t2_sv;			// 11	t2����ʸ��
	int16  t1_sat;			// 12	t1���򱥺�У��
	int16  t2_sat;			// 13	t2���򱥺�У��
	int16  taon;			// 14	��ͨʱ�����
	int16  tbon;			// 15	��ͨʱ�����
	int16  tcon;			// 16	��ͨʱ�����
	int16  T_CMPR1;			// 17	A��PWM1�Ƚ����Ƚ�ֵ
	int16  T_CMPR2;			// 18	B��PWM2�Ƚ����Ƚ�ֵ
	int16  T_CMPR3;			// 19	C��PWM3�Ƚ����Ƚ�ֵ
	int16  EPwm1TBPRD;		// 20	PWM1��ʱ������ֵ
	int16  EPwm2TBPRD;		// 21	PWM2��ʱ������ֵ
	int16  EPwm3TBPRD;		// 22	PWM3��ʱ������ֵ
};

//------------------------------------------------------------
// PI�Ĵ�����

//------------------------------------------------------------
struct PI_REGS{
	int16 PiRef;			// 1	�ο�ֵ	Q12
	int16 Input;			// 2	����ֵ����	Q12
	int16 Output;			// 3	�������	Q12
	int16 Kp;				// 4	����ϵ��	Q12
	int16 Ki;				// 5	����ϵ��	Q12
	int16 Kc;				// 6	У��ϵ��	Q12
	int16 PIMax;			// 7	������ֵ	Q12
	int16 PIMin;			// 8			Q12
	int32 PiUi;				// 9	����ֵ
};

//------------------------------------------------------------
// PWM�Ĵ�����

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
// ������׷�ټĴ�����

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
// ���ʼ���

//------------------------------------------------------------
struct INSTANT_POWER{
	int16 Period;						// ��������ֵ
	int16 PowerInstP;					// �й�����˲ʱֵ
	int16 PowerInstQ;					// �޹�����˲ʱֵ
	int16 PowerAvgP;					// �й�����ƽ��ֵ
	int16 PowerAvgQ;					// �޹�����ƽ��ֵ

	struct CALC_UINST_REGS UinstReg;	// ��ѹ˲ʱֵ����
	struct CALC_UINST_REGS IinstReg;	// ����˲ʱֵ����
	struct URMS_REGS AvgPReg;			// �й�����ƽ��ֵ����
	struct URMS_REGS AvgQReg;			// �޹�����ƽ��ֵ����
};

//------------------------------------------------------------
// VVVF���ƼĴ�����

//------------------------------------------------------------
struct VVVF_REGS{
	int16 AccelerationMax;
	int16 FreNom;
	int16 VolNom;
	int16 Count;
	int16 WeReal;
	int16 Uref;

	struct PI_REGS 	VolPI;
};

//------------------------------------------------------------
// �ⲿ��������

//------------------------------------------------------------
extern int16 SinTab[1024];		// ÿ������1024���㣬Q12���ݴ洢


/*---------------------------------------------------------------------
����ԭ�ͣ�int16 SpllCalc(struct SPLL_REG *spllreg, int16 Uab, int16 Ubc)
�������ƣ��������໷�㷨
�������ܣ�����DSC�����໷����
��ڲ�����*spllreg--����Ĵ�����Uab--AB���ߵ�ѹ˲ʱֵ��Ubc--BC���ߵ�ѹ˲ʱֵ
���ڲ������Ƿ�����ɹ�
��    �ߣ�
��    �ڣ�2015��06��26��
----------------------------------------------------------------------*/
int16 SpllCalc(struct SPLL_REGS *spllreg, int16 Uab, int16 Ubc);

/*---------------------------------------------------------------------
����ԭ�ͣ�void UinstCalc(struct CAL_UINST_REGS *UinstRegs, int16 Theta, int16 Uab, int16 Ubc)
�������ƣ��ߵ�ѹ˲ʱֵ����
�������ܣ����������ѹ˲ʱֵ
��ڲ�����*UinstRegs--����Ĵ�����Theta--��ǰA�����ѹ�Ƕ�ֵ��Uab-�ߵ�ѹ˲ʱֵ��Ubc-�ߵ�ѹ˲ʱֵ
���ڲ�������
��    �ߣ�
��    �ڣ�2015��12��
----------------------------------------------------------------------*/
void UinstCalc(struct CALC_UINST_REGS *UinstRegs, int16 Theta, int16 Uab, int16 Ubc);

/*---------------------------------------------------------------------
����ԭ�ͣ�Uint16 URmsCalc(int16 Vq, struct CAL_URMS_REGS *Cal_V, Uint16 nCycle)
�������ƣ������ѹ/������Чֵ����
�������ܣ�����벨��ѹ/������Чֵ,������ʵʱ�Բ��ߵĳ��ϣ�����ʾ��
��ڲ�����data-˲ʱ����ֵQ12��*Cal_V-�ṹ��ָ�룬nCycle-ÿ�������ڵĲ�������=fs/f0��fs-����Ƶ��/f0-���Ҳ�Ƶ�ʣ���wave-0ȫ�� 1�벨
���ڲ�����
ע�⣺        ʹ��ǰע��ṹ���ʼ��
��    �ߣ�
��    �ڣ�2017��9��
----------------------------------------------------------------------*/
int16 URmsCalc(int16 data, struct URMS_REGS *URmsRegs, Uint16 nCycle);

/*---------------------------------------------------------------------
����ԭ�ͣ�void svpwm(struct SVPWM_REGS *svpwm, int16 Vsd, int16 Vsq, int16 Sina, int16 Cosa)
�������ƣ�SVPWM����
�������ܣ�����SVPWM�ռ�ʸ��PWM��ʱ���Ƚ���ֵ
��ڲ�����*svpwm--���ݴ���ṹ�壬 Vsd--d��˲ʱֵ		Q12
							Vsq--q��˲ʱֵ	Q12
							Cosa--cos_aֵ    	Q12
							Sina--sin_aֵ    	Q12
���ڲ�����svpwm->T_CMPR1��T_CMPR2��T_CMPR3���趨ʱ�ڱȽ�ֵ
��    �ߣ�
��    �ڣ�2018��1��
----------------------------------------------------------------------*/
void svpwm(struct SVPWM_REGS *svpwm, int16 Vsd, int16 Vsq, int16 Sina, int16 Cosa);

/*---------------------------------------------------------------------
����ԭ�ͣ�int16 PICal(struct PI_REGS *PIReg, int16 input, int16 ref)
�������ƣ�ͨ��PI����
�������ܣ�
��ڲ�����*PIReg--���ݴ���ṹ�壬input--��������ref--��׼��
���ڲ�����PIReg->Output
��    �ߣ�
��    �ڣ�2018��1��
----------------------------------------------------------------------*/
int16 PICal(struct PI_REGS *PIReg, int16 input, int16 ref);

/*---------------------------------------------------------------------
����ԭ�ͣ�void PWMRectifier(struct PWMREC_REGS *PwmRegs, int16 Uab, int16 Ubc, int16 ia, int16 ib, int16 Vd, int16 VdcRef)
�������ƣ�PMW����
�������ܣ�DSC���໷��PI��������SVPWM���
��ڲ�����*PwmRegs--���ݴ���ṹ�壬	Uab��Ubc--�����ߵ�ѹQ12
								Ia��Ib  --�����ߵ���Q12
								Vd--��ǰ���ֱ����ѹֵQ12
								VdcRef--ֱ����ѹֵ�ο�ֵ  Q12
���ڲ�����svpwm->T_CMPR1��T_CMPR2��T_CMPR3���趨ʱ�ڱȽ�ֵ
��    �ߣ�
��    �ڣ�2018��1��
----------------------------------------------------------------------*/
void PWMRectifier(struct PWMREC_REGS *PwmRegs, int16 Uab, int16 Ubc, int16 ia, int16 ib, int16 Vd, int16 VdcRef);

/*---------------------------------------------------------------------
����ԭ�ͣ�void LLCConvertor(struct LLC_REGS *CurReg, int16 Cur1, int16 Cur1Ref, int16 Cur2, int16 Cur2Ref, int16 Vdc, int16 VdcRef)
�������ƣ�LLC�任��
�������ܣ�����LLC���壬��ѹ��������
��ڲ�����*CurReg--���ݴ���ṹ�壬	Cur -- г��ǻ������ЧֵQ12
								Vdc -- ��ǰ���ֱ����ѹֵQ12
								VdcRef--ֱ����ѹֵ�ο�ֵ  Q12
���ڲ�����CurReg->Fre���趨ʱ�ڱȽ�ֵ
��    �ߣ�
��    �ڣ�2018��4��
----------------------------------------------------------------------*/
void LLCConvertor(struct LLC_REGS *CurReg, int16 Cur1, int16 Cur2, int16 Vdc, int16 VdcRef);

/*---------------------------------------------------------------------
����ԭ�ͣ�void PowerCal(struct INSTANT_POWER *Power, int16 Theta, int16 Uab, int16 Ubc, int16 Ia, int16 Ic)
�������ƣ�����繦�ʼ���
�������ܣ����������˲ʱ�й����ʺ��޹����ʣ�������һ���ڵ�ƽ��ֵ
��ڲ�����Power--���ݴ���ṹ�壬Theta--A��Ƕ�ֵ��Uab--AB�ߵ�ѹ��Ia--A���ߵ���
										  Ubc--BC�ߵ�ѹ��Ic--C���ߵ���
���ڲ�������
��    �ߣ�
��    �ڣ�2018��07��16��
----------------------------------------------------------------------*/
void PowerCal(struct INSTANT_POWER *Power, int16 Theta, int16 Uab, int16 Ubc, int16 Ia, int16 Ic);

/*---------------------------------------------------------------------
����ԭ�ͣ�void VVVFCal(struct VVVF_REGS *VVVFRegs, int16 VFSet, int16 WeRef, int16 Speed)
�������ƣ���Ƶ��VVVF����
�������ܣ�����V/F����
��ڲ����� VVVFReg -- ���ݴ���ṹ��
		VFSet -- V/Fֵ����λ0.1
		WeRef -- ��Ƶ�ʻ�׼		Q10
		Speed -- �ٶ�1~100������~���٣�
���ڲ�������
��    �ߣ�
��    �ڣ�2018��08��16��
----------------------------------------------------------------------*/
void VVVFCal(struct VVVF_REGS *VVVFRegs, int16 VFSet, int16 WeRef, int16 Speed);

#endif
