/*********************************************************************
DSP������˵����

0��ǰ׺��DSP2833x_���ļ������޸ģ�USERS_���ļ��������޸ģ������ļ�����������ģ�

1�������޸ĵ��ļ����Ը�Ϊֻ����

2��һ��.h�ļ���һ��.c�ļ���ϣ��������塢����������ļ�������ĺ���������.c�ļ�����ɣ�
  ȫ���������������Ϳ��ⲿʹ�õı���������.h����ɣ��ļ�ֻ�Ա�������ͺ��������������
  ��ʹ���ⲿ�ı����ͺ�������������Ӧ��ͷ�ļ���

3������ļ��Ŀ���ֲ�ԣ�

*********************************************************************/
/*********************************************************************
//	V1	2016-12-26 09:33:39 	
//	����������	1��ͨ�Ź�����ʱȥ��CANͨ�ţ�
			2�����������
			3������MODBUSЭ��
			4��������������Ľṹ

*********************************************************************/
//ͷ�ļ�����
#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h" 

#pragma DATA_SECTION(SinTab,".SinTab")
int16 SinTab[1024];

struct  AD_BUFS                 DataInst;
struct  AD_BUFS                 DataZero;
struct  AD_BUFS                 DataBuf;
struct  AD_BUFS                 DataFactor;
struct  AD_BUFS                 DataNormal;
struct  AD_BUFS                 DataMax;
union  	FLAG_GPIOIN   	 		FlagGpioIn;
union   FLAG_SYSTEM_INITERR 	FlagSysInitErr;
union  	FLAG_SYSTEM_RUNERR 		FlagSysRunErr;
union  	FLAG_SYSTEM_ALARM		FlagSysAlarm;
union	FLAG_SYSTEM				FlagSystem;
union	STATE_SYSTEM			StateSystem;
struct	COUNT_SYSTEM			CountSystem;
struct  PWMISR_REGS             PwmIsrReg;
struct 	CANFRAME  				CanTxPack1;
struct 	CANFRAME  				CanTxPack2;
struct 	CANFRAME  				CanRxPack;
enum  	SYSTEM_STATE 			SystemState;
enum  	CONVERT_STATE 			ConvertState;
enum    SCR_CHANNEL             ScrChannel;

#pragma DATA_SECTION(SpllRegNO1,".SPLLData")
struct SPLL_REGS SpllRegNO1;

#pragma DATA_SECTION(SpllRegNO2,".SPLLData")
struct SPLL_REGS SpllRegNO2;

#pragma DATA_SECTION(SpllRegOut,".SPLLData")
struct SPLL_REGS SpllRegOut;

#pragma DATA_SECTION(SpllRegUFO,".SPLLData")
struct SPLL_REGS SpllRegUFO;

#pragma DATA_SECTION(NO1IARegs,".URmsReg")
struct 	URMS_REGS NO1IARegs;

#pragma DATA_SECTION(NO1IBRegs,".URmsReg")
struct 	URMS_REGS NO1IBRegs;

#pragma DATA_SECTION(NO1ICRegs,".URmsReg")
struct 	URMS_REGS NO1ICRegs;

#pragma DATA_SECTION(NO2IARegs,".URmsReg")
struct 	URMS_REGS NO2IARegs;

#pragma DATA_SECTION(NO2IBRegs,".URmsReg")
struct 	URMS_REGS NO2IBRegs;

#pragma DATA_SECTION(NO2ICRegs,".URmsReg")
struct 	URMS_REGS NO2ICRegs;

#pragma DATA_SECTION(ModATempRegs,".URmsReg")
struct  URMS_REGS ModATempRegs;

#pragma DATA_SECTION(ModBTempRegs,".URmsReg")
struct  URMS_REGS ModBTempRegs;

#pragma DATA_SECTION(ModCTempRegs,".URmsReg")
struct  URMS_REGS ModCTempRegs;

extern  Uint16 ramLoadStart;
extern  Uint16 ramLoadEnd;
extern  Uint16 ramRunStart;

void DataInit(void);
void eCANSend(Uint32 boxID, struct CANFRAME CANTXPACK1);
void main(void)
{
	Uint16 i;

	InitSysCtrl();                  // ϵͳ��ʼ��
	MemCopy(&ramLoadStart,&ramLoadEnd,&ramRunStart);
	
	DataInit();                     // ���ݳ�ʼ��
	DelayUs(5000);
//	InitFlash();
	FlashRegs.FOPT.bit.ENPIPE = 1;

	InitXintf();
	for( i = 0; i < 100; i++)
	{
		DelayUs(5000);
	}

	InitGpio();                    	// GPIO��ʼ��
	FlagSysInitErr.bit.GPIOInitErr = 0;
 	InitSCRGpio();                  // �ɿع�������ʼ��
	FlagSysInitErr.bit.SCRInitErr = 0;

	InitExRam();                    // �ⲿRAM��ʼ��
	FlagSysInitErr.bit.EXRAMInitErr = 0;
	InitModbusBData();

    ReadParaFromFlash();            // ��ȡ�ⲿFLASH����
	DelayUs(5000);
	ReadEmDataFromFlash();          // ��ȡ����ʱ��Ҫ��������
	DelayUs(5000);

	ParaTable.PowerReg.uParaTableVer = 0;		// ��ʽ����������
	ParaTable.PowerReg.uHardwareVer = 0;		// ��ʽ����������
	// Ӳ���汾У����ɻ��ް汾���Ҳ�����û�г�ʼ��,�ָ�������ֵĬ��ֵ
	if(((ParaTable.PowerReg.uHardwareVer == 42) || (ParaTable.PowerReg.uHardwareVer == 0))
	&& (ParaTable.PowerReg.uParaTableVer == 0))
	{
		WtiteDefaultToFlash();
	}

	InitDMA();                      // DMA��ʼ��
	FlagSysInitErr.bit.DMAInitErr = 0;

	InitADCOfDMA();                 // AD��ʼ�������ó�DMAģʽ
	FlagSysInitErr.bit.ADCInitErr = 0;
	DelayUs(5000);

	IniteCANB();					// CANB��ʼ��
	FlagSysInitErr.bit.CanBInitErr = 0;
	DelayUs(5000);

	SCIBInit(1, 115200); 			// SCIB��ʼ����
	FlagSysInitErr.bit.SCIBInitErr = 0;
   	DelayUs(5000);

	SCICInit(0, 115200); 			// SCIC��ʼ����
	FlagSysInitErr.bit.SCICInitErr = 0;
   	DelayUs(5000);

	InitCpuTimers();    			//3ms,��ʱ�����ⲿ�����жϣ� ��Ӧ�жϳ���interrupt void Timer1_ISR(void)���ļ�T4_PWM.c�У�
	FlagSysInitErr.bit.Time1InitErr = 0;
	FlagSysInitErr.bit.Time2InitErr = 0;
	DelayUs(5000);

	InitEPWM();          			//epwm  ��ʼ��������PWM�����жϣ���Ӧ�жϳ������ļ�T1_PWM.c��
   	FlagSysInitErr.bit.PWM1InitErr = 0;
	DisablePWM(EPWM_ALL);

	EnableTZ();
	DelayUs(5000);

	ClrCPLD_STS();

    InitT1Var();
    InitT4Var();

	EINT;
	ERTM;

#ifdef RUNCODE

/*************************�ȴ�����û�ģʽ����******************************/
	while (0 == ReadGPIOIn2());
	if (0 == FLAG_SOFT_START)					// ����ģʽѡ��--������ģʽ����Ե��������
	{
		FlagSystem.bit.SoftStart = 1;
		ControlGPIOOut(GPIO_LED1, ON);
	}
	else										// ����ģʽѡ��--����Ͷ��ģʽ����Զฺ��
	{
		FlagSystem.bit.SoftStart = 0;
		ControlGPIOOut(GPIO_LED1, OFF);
	}
	/******************************END******************************/

/***************************�ȴ��û�����������******************************/
	while (0 == ReadGPIOIn5());					// �ⲿ����������
	if (0 == FLAG_PREFFERED)					// NO1��ԴΪ��·
	{
		FlagSystem.bit.NO1Preferred = 1;
		FlagSystem.bit.NO2Preferred = 0;
		ControlGPIOOut(GPIO_LED2, OFF);
	}
	else 										// NO2��ԴΪ��·
	{
		FlagSystem.bit.NO1Preferred = 0;
		FlagSystem.bit.NO2Preferred = 1;
		ControlGPIOOut(GPIO_LED2, ON);
	}
	/******************************END******************************/

#endif

	while(1)
	{
        ModbusBProcess();			//modbus���մ���
        DelayUs(3000);
        ModbusCProcess();
        DelayUs(3000);
        UpdataParaToFlash();		//���²�����Flash

        if(1 == FlagSystem.bit.CanSend)
        {
            FlagSystem.bit.CanSend = 0;
            eCANSend(0x82100f00, CanTxPack1);
            eCANSend(0x82100f01, CanTxPack2);
        }
    }
}

void DataInit(void)
{
	Uint16 i;

// ϵͳ��ʼ�����ϵĳ�ʼ��
	FlagSysInitErr.all 			= 0xffff;
	FlagSysInitErr.bit.rsvd1   	= 0;

// ϵͳ���й��ϵĳ�ʼ��
	FlagSysRunErr.all = 0;

// ϵͳ���������ĳ�ʼ��
	FlagSysAlarm.all = 0;

// ϵͳ��־λ��ʼ��
	FlagSystem.all = 0;

// ϵͳ״̬λ��ʼ��
	StateSystem.all = 0;

// ���Һ������ʼ��
	for (i=0; i<=1023; i++)
	{
		SinTab[i] = sin(2 * PI * i / 1024) * 4096;
	}

// ��һ·��Դ���໷��ʼ��
    memset(&SpllRegNO1, 0x00, sizeof(SpllRegNO1));

// �ڶ�·��Դ���໷��ʼ��
    memset(&SpllRegNO2, 0x00, sizeof(SpllRegNO2));

// �����Դ���໷��ʼ��
    memset(&SpllRegOut, 0x00, sizeof(SpllRegOut));

// �����������ֵ���໷��ʼ��
    memset(&SpllRegUFO, 0x00, sizeof(SpllRegUFO));

// ϵͳ��������ʼ��
    memset(&CountSystem, 0x00, sizeof(CountSystem));

// PWM�жϴ���������ݳ�ʼ��
    memset(&PwmIsrReg, 0x00, sizeof(PwmIsrReg));

// NO1A�������Чֵ
    memset(&NO1IARegs, 0x00, sizeof(NO1IARegs));

// NO1B�������Чֵ
    memset(&NO1IBRegs, 0x00, sizeof(NO1IBRegs));

// NO1C�������Чֵ
    memset(&NO1ICRegs, 0x00, sizeof(NO1ICRegs));

// NO2A�������Чֵ
    memset(&NO2IARegs, 0x00, sizeof(NO2IARegs));

// NO2B�������Чֵ
    memset(&NO2IBRegs, 0x00, sizeof(NO2IBRegs));

// NO2C�������Чֵ
    memset(&NO2ICRegs, 0x00, sizeof(NO2ICRegs));

// A���¶���Чֵ
    memset(&ModATempRegs, 0x00, sizeof(ModATempRegs));

// B���¶���Чֵ
    memset(&ModBTempRegs, 0x00, sizeof(ModBTempRegs));

// C���¶���Чֵ
    memset(&ModCTempRegs, 0x00, sizeof(ModCTempRegs));

// AD����
    memset(&DataBuf, 0x00, sizeof(DataBuf));

// ״̬���ʼ��
	SystemState = SystemWait;
	ConvertState = ConvertInit;
}
