#ifndef	_USERS_PARA_TAB_H_
#define	_USERS_PARA_TAB_H_

//#define TESTCODE	1				// ���в��Գ���Ѵ˿��ش�
#define RUNCODE		2				// ������Ŀ����Ѵ˿��ش�

#define ON							0
#define OFF 						1
#define STANDBY_LED_ON				0
#define STANDBY_LED_OFF				1
#define RUN_LED_ON					0
#define RUN_LED_OFF					1
#define FAULT_LED_ON				0
#define FAULT_LED_OFF				1
#define ALARM_LED_ON				0
#define ALARM_LED_OFF				1

#define LPF_K1						3
#define	LPF_K2						2

#define FLAG_RUN_CMD				FlagGpioIn.bit.GpioIn1
#define FLAG_STOP_CMD				FlagGpioIn.bit.GpioIn2
#define FLAG_FORCE_CHANGE			FlagGpioIn.bit.GpioIn3
#define FLAG_RSVD2					FlagGpioIn.bit.GpioIn4
#define FLAG_RSVD3					FlagGpioIn.bit.GpioIn5
#define FLAG_RSVD4					FlagGpioIn.bit.GpioIn6
#define FLAG_RSVD5					FlagGpioIn.bit.GpioIn7
#define FLAG_RSVD6		            FlagGpioIn.bit.GpioIn8

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------ϵͳ����������----------------------------
*   ������ϵͳ����������
*   ˵����ÿ�����ݹ̶�64���Ĵ���
*
*-------------------------ϵͳ����������----------------------------*/

//------------------------------------------------------------
// ϵͳ״̬������

//------------------------------------------------------------
enum SYSTEM_STATE {
	SystemWait,
	SystemStandby,
	SystemRun,
	SystemFault
};

//------------------------------------------------------------
// ϵͳ��ʼ���쳣���λ

//------------------------------------------------------------
struct	FLAG_SYSTEM_INITERR_BITS{				// λ����	0Ϊ����	1Ϊ�쳣
	Uint16 GPIOInitErr 			: 1;			// ͨ��IO�ڳ�ʼ���쳣λ
	Uint16 SCIBInitErr			: 1;			// SCIB�ڳ�ʼ���쳣λ
	Uint16 PWM1InitErr			: 1;			// PWM1�����ʼ���쳣λ
	Uint16 Time1InitErr			: 1;			// ��ʱ��1�����ʼ���쳣λ

	Uint16 Time2InitErr			: 1;			// ��ʱ��1�����ʼ���쳣λ
	Uint16 CanBInitErr			: 1;			// CANB�����ʼ���쳣λ
	Uint16 ADCInitErr			: 1;			// ADC�����ʼ���쳣λ
	Uint16 EXRAMInitErr			: 1;			// �ⲿRAM��ʼ���쳣λ

	Uint16 DMAInitErr			: 1;			// DMA��ʼ���쳣λ

	Uint16 rsvd1				: 7;			//
};

union   FLAG_SYSTEM_INITERR{
	Uint16 all;
	struct FLAG_SYSTEM_INITERR_BITS bit;
};

//------------------------------------------------------------
// ϵͳ�����쳣���λ

//------------------------------------------------------------
struct	FLAG_SYSTEM_RUNERR_BITS{			// λ����	0Ϊ����	1Ϊ�쳣
	Uint16 ACInVolHigh			: 1;		// ����AC��ѹ����
	Uint16 ACInVolLow			: 1;		// ����ACǷѹ����
	Uint16 ACInCurHigh			: 1;		// ����AC��������
	Uint16 DCInVolHigh			: 1;		// ����DC��ѹ����

	Uint16 DCInVolLow			: 1;		// ����DCǷѹ����
	Uint16 DCInCurHigh			: 1;		// ����DC��������
	Uint16 BoostVolHigh			: 1;		// Boost��ѹ��ѹ����
	Uint16 Boost1CurHigh		: 1;		// Boost1��ѹ��������

	Uint16 Boost2CurHigh		: 1;		// Boost2��ѹ��������
	Uint16 Boost3CurHigh		: 1;		// Boost3��ѹ��������
	Uint16 ACOutVolHigh			: 1;		// ���AC��ѹ����
	Uint16 ACOutVolLow			: 1;		// ���ACǷѹ����

	Uint16 ACOutCurHigh			: 1;		// ���AC��������
	Uint16 OverTemp				: 1;		// ϵͳ���±���
	Uint16 FlagPDP				: 1;		// PDPӲ������

	Uint16 rsvd1				: 1;		// ����
};

struct  FLAG_SYSTEM_ALARM_BITS{				// λ����	0Ϊ����	1Ϊ�쳣
	Uint32 ACInVolHigh			: 1;		// ����AC��ѹ����
	Uint32 ACInVolLow			: 1;		// ����ACǷѹ����
	Uint32 ACInCurHigh			: 1;		// ����AC��������
	Uint32 ACInLossPhase		: 1;		// ����ACȱ�౨��

	Uint32 DCInVolHigh			: 1;		// ����DC��ѹ����
	Uint32 DCInVolLow			: 1;		// ����DCǷѹ����
	Uint32 DCInCurHigh			: 1;		// ����DC��������
	Uint32 BoostVolHigh			: 1;		// Boost��ѹ��ѹ����

	Uint32 Boost1CurHigh		: 1;		// Boost1��ѹ��������
	Uint32 Boost2CurHigh		: 1;		// Boost2��ѹ��������
	Uint32 Boost3CurHigh		: 1;		// Boost3��ѹ��������
	Uint32 ACOutVolHigh			: 1;		// ���AC��ѹ����

	Uint32 ACOutVolLow			: 1;		// ���ACǷѹ����
	Uint32 ACOutCurHigh			: 1;		// ���AC��������
	Uint32 OverTemp				: 1;		// ϵͳ���±���
	Uint32 CanSend				: 1;		// CAN����ʧ�ܱ���

	Uint32 OutSPLLAlarm			: 1;		// �������ʧ�ܱ���
	Uint32 OutPhaseLose			: 1;		// �����ȱ�౨��

	Uint32 rsvd1				: 19;		// ����
};

union   FLAG_SYSTEM_RUNERR{
	Uint16 all;
	struct FLAG_SYSTEM_RUNERR_BITS bit;
};

union	FLAG_SYSTEM_ALARM{
	Uint32 all;
	struct FLAG_SYSTEM_ALARM_BITS bit;
};

//------------------------------------------------------------
// ϵͳ����Ӳ�����ϱ��λ

//------------------------------------------------------------
struct  FLAG_SYSTEM_PDPERR_BITS{			// λ����	0Ϊ����	1Ϊ�쳣
	Uint16 IGBT123Err			: 1;		// IGBTP123_ERR		PWM123������IGBT��������
	Uint16 IGBT456Err			: 1;		// IGBTP456_ERR		PWM456������IGBT��������
	Uint16 IAErr				: 1;		// IAErr			A���е�����������
	Uint16 ICErr				: 1;		// ICErr		3	B���е�����������

	Uint16 IUErr				: 1;		// IUErr			U�����������������
	Uint16 IWErr				: 1;		// IWErr			W�����������������
	Uint16 VDCErr				: 1;		// VDCErr			ĸ�������ѹ��ѹ����
	Uint16 rsd					: 1;

	Uint16 rsd1					: 8;
};

union	FLAG_SYSTEM_PDPERR{
	Uint16 all;
	struct FLAG_SYSTEM_PDPERR_BITS bit;
};

//------------------------------------------------------------
// ��Ʒ������

//------------------------------------------------------------
//��Դ��������� 64
struct DBP_POWER_REG
{
	int16		uInputType;			// �����Դ����
    int16		uInputFre;			// �����Դ�Ƶ�ʣ���λ0.1Hz
	int16		uInputVol;			// �����Դ���ѹ����λ0.1V
	int16		uInputCur;			// �����Դ���������λ0.1A
	int16		uOutputType;    	// �����Դ����
    int16		uOutputFre;     	// �����Դ�Ƶ�ʣ���λ0.1Hz
	int16		uOutputVol;     	// �����Դ���ѹ����λ0.1V
	int16		uOutputCur;     	// �����Դ���������λ0.1A
	int16		uOutputPower;		// �����Դ���ʣ���λ1W
	int16		uBoostVol;			// Boost��ѹ���ѹ����λ0.1V

	int16		uBoostCur;			// Boost��ѹ���������λ0.1A
	int16		uSpeedMax;			// ��Ƶ�����ص�����ת�٣���λ1rpm
	int16		uSpeedMin;			// ��Ƶ�����ص�����ת�٣���λ1rpm
	int16		uSpeedSet;			// ��Ƶ�����ص���趨ת�٣���λ1rpm
	int16		uTempNom;		    // �����¶�ֵ����λ0.1��
	int16		uPWMSwitchFreq;		// PWM����Ƶ�ʣ���λ1Hz
	int16		uPWMSwitchPeriod;	// PWM�������ڣ���λ1us
	int16		uPWMSwitchDeadTime;	// PWM��������ʱ�䣬��λ1ns
	int16		uBoostSwitchFreq;	// Boost����Ƶ�ʣ���λ1Hz
	int16		uBoostSwitchPeriod;	// Boost�������ڣ���λ1us

	int16		uBoostSwitchDeadTime;// Boost��������ʱ�䣬��λ1ns
	int16		uHardwareVer;		// Ӳ���汾�ţ���Ӳ���汾ΪV4.2����ֵ��ʾ42
	int16		uParaTableVer;	 	// �������汾�ţ����޸Ĵ����ۼӣ���Ϊ0�����ж�Ϊ��Ҫ��ʼ��
    int16       uSoftwareVer;       // �����汾�ţ��������汾ΪV18041����ֵ��ʾ18��4�·ݵ�1���޸�

   	int16		nRrsev[40];
};

//PI���������� 64
struct  DBP_PI_REG
{
	int16   	uBoostVolKp;       	// Boost��ѹ��
	int16   	uBoostVolKi;
    int16  		uBoostVolKd;
	int16 		uBoostVolPIMax;
	int16 		uBoostVolPIMin;

	int16   	uOutVolKp;          // �����ѹ��
	int16   	uOutVolKi;
    int16  		uOutVolKd;
	int16 		uOutVolPIMax;
	int16 		uOutVolPIMin;

	int16   	uOutCurKp;          // ���������
	int16   	uOutCurKi;
    int16  		uOutCurKd;
	int16 		uOutCurPIMax;
	int16 		uOutCurPIMin;

	int16   	uOutPLLKp;          // �����ѹ���໷
	int16   	uOutPLLKi;
    int16  		uOutPLLKd;
	int16 		uOutPLLPIMax;
	int16 		uOutPLLPIMin;

   	int16     	nRrsev[44];       	//Ԥ����
};

// ���п��Ʋ����� 64
struct DBP_RUNCTRL_REG
{
	int16		uInVolPreChg;		// �����ѹԤ����ѹֵ�� ����ֵ��4096=100%  Q12
	int16		uInVolHighAlarm;	// �����ѹ����ֵ�� ����ֵ��4096=100%  Q12
	int16		uInVolHighCut;		// �����ѹ�ж�ֵ�� ����ֵ��4096=100%  Q12
	int16		uInVolLowAlarm;		// ����Ƿѹ����ֵ�� ����ֵ��4096=100%  Q12
	int16		uInVolLowCut;		// ����Ƿѹ�ж�ֵ�� ����ֵ��4096=100%  Q12
	int16		uInCurHighAlarm;	// �����������ֵ�� ����ֵ��4096=100%  Q12
	int16		uInCurHighCut;		// ��������ж�ֵ�� ����ֵ��4096=100%  Q12
	int16		uBoostVolHighAlarm;	// Boost��ѹ��ѹ����ֵ�� ����ֵ��4096=100%  Q12
	int16		uBoostVolHighCut;	// Boost��ѹ��ѹ�ж�ֵ�� ����ֵ��4096=100%  Q12
	int16		uBoostCurHighAlarm;	// Boost��ѹ��������ֵ�� ����ֵ��4096=100%  Q12

	int16		uBoostCurHighCut;	// Boost��ѹ�����ж�ֵ�� ����ֵ��4096=100%  Q12
	int16		uOutVolHighAlarm;	// �����ѹ����ֵ�� ����ֵ��4096=100%  Q12
	int16		uOutVolHighCut;		// �����ѹ�ж�ֵ�� ����ֵ��4096=100%  Q12
	int16		uOutVolLowAlarm;	// ���Ƿѹ����ֵ�� ����ֵ��4096=100%  Q12
	int16		uOutVolLowCut;		// ���Ƿѹ�ж�ֵ�� ����ֵ��4096=100%  Q12
	int16		uOutCurHighAlarm;	// �����������ֵ�� ����ֵ��4096=100%  Q12
	int16		uOutCurHighCut;		// ��������ж�ֵ�� ����ֵ��4096=100%  Q12
	int16       uTempFanOpen;       // ���ȿ����¶ȣ���λ0.1��
	int16       uTempAlarm;         // ���±���ֵ����λ0.1��
	int16       uTempCut;           // �����ж�ֵ����λ0.1��

    int16		uVolOutLPLow;		// ���ȱ��������ֵ�� ����ֵ��4096=100%  Q12
    int16		uVolOutLPUp;		// ���ȱ��������ֵ�� ����ֵ��4096=100%  Q12
    int16		uFreUnusualLow;		// Ƶ���쳣����ֵ�� ����ֵ��4096=100%  Q12
    int16		uFreUnusualUp;		// Ƶ���쳣����ֵ�� ����ֵ��4096=100%  Q12
    int16		bCtrlModeSet;		// ����ģʽ�趨
    int16		bDirectionSet;		// �����ת�����趨
    int16		bFreSourceSet;		// �����Ƶ����Դ�趨
    int16     	XSMaxInVol;			// �����ѹ������ֵ
    int16     	XSMaxInCur;			// �������������ֵ
    int16		XSMaxBoostVol;		// Boost��ѹ��ѹ������ֵ

    int16     	XSMaxBoostCur;		// Boost��ѹ����������ֵ
    int16     	XSMaxOutVol;		// �����ѹ������ֵ
    int16     	XSMaxOutCur;		// �������������ֵ
    int16 		XSMaxNTCTemp;		// �¶�������ֵ
    int16		XSMaxSpeed;			// ת��������ֵ
    int16		uSpeedRatio;		// ��/��Ƶ����
    int16		uVVVFVfSet;			// VVVF��V/Fֵ���ã���λ0.1

 	int16     	nRrsev[27];			// Ԥ��
};

// �ӳٱ��������� 64
struct DBP_DELAYCTRL_REG
{
	int16		uInVolAlarm;		// �����ѹ�����ӳ�ֵ	@3ms
	int16		uInVolCut;			// �����ѹ�ж��ӳ�ֵ	@3ms
	int16		uInCurAlarm;		// ������������ӳ�ֵ	@3ms
	int16		uInCurCut;			// ��������ж��ӳ�ֵ	@3ms
	int16		uBoostVolAlarm;		// Boost��ѹ��ѹ�����ӳ�ֵ	@3ms
	int16		uBoostVolCut;		// Boost��ѹ��ѹ�ж��ӳ�ֵ	@3ms
	int16		uBoostCurAlarm;		// Boost��ѹ���������ӳ�ֵ	@3ms
	int16		uBoostCurCut;		// Boost��ѹ�����ж��ӳ�ֵ	@3ms
	int16		uOutVolAlarm;		// �����ѹ�����ӳ�ֵ	@3ms
	int16		uOutVolCut;			// �����ѹ�ж��ӳ�ֵ	@3ms

	int16		uOutCurAlarm;		// ������������ӳ�ֵ	@3ms
	int16		uOutCurCut;			// ��������ж��ӳ�ֵ	@3ms
	int16		uTempAlarm;			// ���±����ӳ�ֵ		@3ms
	int16		uTempCut;			// �����ж��ӳ�ֵ		@3ms
    int16		uVolOutLP;			// ���ȱ���ӳٱ���ֵ	@3ms
    int16		uFreUnusual;		// Ƶ���쳣�����ӳ�ֵ	@3ms

 	int16     	nRrsev[48];			// Ԥ��
};


// CPU�жϲ����� 64
struct DBP_TIMER1ISR_REG
{
	Uint16		uSystemStatus;		// ϵͳ����״̬��
	Uint16		uSystemInitErr;		// ϵͳ��ʼ������״̬��
	Uint16		uSystemRunErr;		// ϵͳ���й���״̬��
	Uint16		uSystemAlarmH;		// ϵͳ���б���״̬�������ֽ�
	Uint16		uSystemAlarmL;		// ϵͳ���б���״̬�������ֽ�
	Uint16		uSystemPDPErr;		// ϵͳӲ�����ϱ�
	Uint16		uGpioIn;			// ����������
	Uint16		uGpioOut;			// ���������
	int16		uCtrlMode;			// ����ģʽ
	int16 		uDirection;			// �����ת����

	int16		uFreSource;			// ��Ƶ����Դ
	int16		uFreSet;			// ��Ƶ���趨ֵ
	int16		uInVolRms;			// �����ѹ��Чֵ
	int16		uInCurRms;			// ���������Чֵ
	int16		uBoostVolRms;		// Boost1��ѹ��ѹ��Чֵ
	int16		uBoost1CurRms;		// Boost1��ѹ������Чֵ
	int16		uBoost2CurRms;		// Boost2��ѹ������Чֵ
	int16		uBoost3CurRms;		// Boost3��ѹ������Чֵ
	int16		uOutVolRms;			// �����ѹ��Чֵ
	int16		uOutCurRms;			// ���������Чֵ

	int16		uOutFre;			// ���Ƶ��
	int16		uOutActivePower;	// ����й�����ʵ��ֵ����λ1W
	int16		uOutReactivePower;	// ����޹�����ʵ��ֵ����λ1W
	int16		uOutApparentPower;	// ������ڹ���ʵ��ֵ����λ1W
	int16		uOutPowerFactor;	// �����������,��λ0.01
	int16		uTemp1Rms;			// NTC1�¶�ֵ
	int16		uTemp2Rms;			// NTC2�¶�ֵ
	int16		uTemp3Rms;			// NTC3�¶�ֵ
	int16		uTemp4Rms;			// NTC4�¶�ֵ
	int16		uTemp5Rms;			// NTC5�¶�ֵ

	int16		uTemp6Rms;			// NTC6�¶�ֵ
	int16		uTemp7Rms;			// NTC7�¶�ֵ
	int16		uTemp8Rms;			// NTC8�¶�ֵ
	int16		uT4UseTime;		    // Timer1�ж�����ʱ�� @3ms/cycle
	int16		uInstReg0;			// ���üĴ���0
	int16		uInstReg1;			// ���üĴ���1
	int16		uInstReg2;			// ���üĴ���2
	int16		uInstReg3;			// ���üĴ���3
	int16		uInstReg4;			// ���üĴ���4
	int16		uInstReg5;			// ���üĴ���5

	int16		uInstReg6;		    // ���üĴ���6
	int16		uInstReg7;		    // ���üĴ���7
	int16		uInstReg8;		    // ���üĴ���8
	int16		uInstReg9;		    // ���üĴ���9

    int16		nRrsev[20];			// Ԥ��
};

// PWM�жϲ����� 64
struct DBP_PWMISR_REG
{
	int16 uInVol;					// �����ѹ  			Q12
	int16 uInCur;					// �������  			Q12
	int16 uBoost1Cur;				// Boost��ѹ����1  	Q12
	int16 uBoost2Cur;				// Boost��ѹ����2  	Q12
	int16 uBoost3Cur;				// Boost��ѹ����3  	Q12
	int16 uOutUab;					// �����ԴAB��ѹ        	Q12
	int16 uOutUbc;					// �����ԴBC��ѹ        	Q12
	int16 uOutIAl;				    // ���A���ܵ���	 	Q12
	int16 uOutIBl;				    // ���B���ܵ���	 	Q12
	int16 uOutICl;				    // ���C���ܵ���	 	Q12

	int16 uOutIAc;				    // ���A�ฺ�ص���	 	Q12
	int16 uOutIBc;				    // ���B�ฺ�ص���		Q12
	int16 uOutICc;				    // ���C�ฺ�ص���	 	Q12
	int16 uOutUd;					// �����ѹD��˲ʱֵ   	Q12
	int16 uOutUq;					// �����ѹQ��˲ʱֵ	Q12
	int16 uOutTheta;				// �����ѹ�Ƕ�ֵ		Q12
	int16 uOutInstActivePower;		// ����й�����˲ʱֵ	Q12
	int16 uOutInstReactivePower;	// ����޹�����˲ʱֵ	Q12
	int16 uOutAvgActivePower;		// ����й�����ƽ��ֵ	Q12
	int16 uOutAvgReactivePower;		// ����޹�����ƽ��ֵ	Q12

	int16 uModATemp;				// A��ģ���¶���Чֵ	Q12
	int16 uModBTemp;				// B��ģ���¶���Чֵ	Q12
	int16 uT1UseTime;				// T1�ж�����ʱ��
    int16 uActiveReg0;              // ���üĴ���0
	int16 uActiveReg1;				// ���üĴ���1
	int16 uActiveReg2;				// ���üĴ���2
	int16 uActiveReg3;				// ���üĴ���3
	int16 uActiveReg4;				// ���üĴ���4
	int16 uActiveReg5;			    // ���üĴ���5
	int16 uActiveReg6;			    // ���üĴ���6

	int16 uActiveReg7;			    // ���üĴ���7
	int16 uActiveReg8;			    // ���üĴ���8
	int16 uActiveReg9;			    // ���üĴ���9

    int16 nRrsev[31];				// Ԥ��
};

// ��Ҫ���ݱ�������� 64
struct EMERGENCY_REG
{
    int16 uEmDataSize;              // ��Ҫ���ݴ洢�����С
    int16 uEmDataNumMax;            // ��Ҫ�������洢����
    int16 uEmDataSaveTime;          // ��Ҫ���ݱ���ʱ��
    int16 uEmDataChgEnable;         // ��Ҫ���ݴ洢ʹ�ܿ��أ�д0x5555ʹ��
    int16 uEmDataChgFlag;           // ��Ҫ�����������ı��
    int16 uEmData0Addr;             // ��Ҫ����0��ַ
    int16 uEmData1Addr;             // ��Ҫ����1��ַ
    int16 uEmData2Addr;             // ��Ҫ����2��ַ
    int16 uEmData3Addr;             // ��Ҫ����3��ַ
    int16 uEmData4Addr;             // ��Ҫ����4��ַ

    int16 uEmData5Addr;             // ��Ҫ����5��ַ
    int16 uEmData6Addr;             // ��Ҫ����6��ַ
    int16 uEmData7Addr;             // ��Ҫ����7��ַ
    int16 uEmCountRam;              // ��Ҫ���ݱ��������
    int16 uEmCountBlock;            // ��Ҫ���ݿ������
    int16 uEmDataSaveFinishFlag;    // ��Ҫ���ݱ���ɹ���� 0-����δ��� 1-�������
    int16 uEmDataFlashWriteOK;      // ��Ҫ����д��FLASH�ɹ���� 0-δ�ɹ�д�� 1-д��ɹ�

    int16 nRrsev[47];               // Ԥ��
};

struct PARAMETER_REGS
{
   	struct DBP_POWER_REG       	PowerReg;	    // ��Դ������			64
   	struct DBP_PI_REG          	PIReg;       	// PI������			64
   	struct DBP_RUNCTRL_REG    	RunCtrlReg;		// ���п��Ʋ�����  		64
   	struct DBP_DELAYCTRL_REG	DelayCtrlReg;	// �ӳٱ���/����������	64
	struct DBP_TIMER1ISR_REG   	Timer1Reg;      // ��̬���в���     		64
	struct DBP_PWMISR_REG		PWMReg;		    // ��̬������        		64
	struct EMERGENCY_REG   		EmergencyReg;   // ��Ҫ���ݱ�������� 	64
};

extern struct PARAMETER_REGS ParaTable;


/*-------------------------ϵͳ���Ʋ�������----------------------------
*   ������ϵͳ���Ʋ�������
*   ˵������ǡ�״̬�Ĵ����̶�32��BIT�������Ĵ����޹̶���С
*
*-------------------------ϵͳ����������----------------------------*/
//------------------------------------------------------------
// ϵͳ���б�ǡ�״̬λ

//------------------------------------------------------------
struct  FLAG_SYSTEM_BITS{
    Uint16 CanSend              : 1;
    Uint16 EmDataFlashWriteOK   : 1;
    Uint16 EmDataChgFlag        : 1;
    Uint16 ParaDataChg          : 1;

    Uint16 PreChgOk          	: 1;
    Uint16 RmtMode              : 1;
    Uint16 RmtStart             : 1;
    Uint16 LastErr				: 1;

    Uint32 rsvd                 : 24;
};

union   FLAG_SYSTEM{
    Uint32 all;
    struct FLAG_SYSTEM_BITS bit;
};

//------------------------------------------------------------
// ϵͳ���м����Ĵ���

//------------------------------------------------------------
struct  COUNT_SYSTEM{
	Uint16 FaultReset;
	Uint16 ParaTableReset;
	Uint16 PDPInitCount;
	Uint16 CanRst;
	Uint16 T1Cycle;
	Uint16 T4Cycle;
	Uint16 DelaySysWait;
	Uint16 ACInLowAlarm;
	Uint16 ACInLowCut;
	Uint16 ACInHighAlarm;
	Uint16 ACInResume;
	Uint16 DCInLowAlarm;
	Uint16 DCInLowCut;
	Uint16 DCInHighAlarm;
	Uint16 DCInResume;
	Uint16 ACPreChg;
	Uint16 DCPreChg;
	Uint16 PDP;
	Uint16 DelayMainLLCRun;
	Uint16 DelaySpareLLCRun;
	Uint16 DelayPWMRecRun;
	Uint16 DelaySpareLLCPulse;
	Uint16 DelayOutInLP;
};

struct  REGS_SYSTEM{
	int16 OutVolThetaSum;
	int16 OutVolTheta;
	int16 OutVolWeReal;
	int16 OutVolWeRef;
	int16 OutVolSina;
	int16 OutVolCosa;
	int16 OutVolRms;
	int16 SpeedRatio;
	int16 VVVFVfSet;
};

/******************��������*********************/
void ReadParaFromFlash(void);
void WtiteDefaultToFlash(void);
int UpdataParaToFlash(void);

#endif