/*********************************************************************
DSP程序框架说明：

0，前缀：DSP2833x_的文件不可修改，USERS_的文件不建议修改，其他文件根据需求更改；

1，不可修改的文件属性改为只读；

2，一个.h文件和一个.c文件组合，函数定义、变量定义和文件作用域的函数声明在.c文件内完成，
  全局作用域函数声明和可外部使用的变量声明在.h内完成，文件只对本身变量和函数定义和声明，
  若使用外部的变量和函数，需包含相对应的头文件；

3，提高文件的可移植性；

*********************************************************************/
/*********************************************************************
//	V1	2016-12-26 09:33:39 	
//	功能描述：	1，通信功能暂时去除CAN通信；
			2，整理参数表；
			3，增加MODBUS协议
			4，调整整个程序的结构

*********************************************************************/
//头文件包含
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

	InitSysCtrl();                  // 系统初始化
	MemCopy(&ramLoadStart,&ramLoadEnd,&ramRunStart);
	
	DataInit();                     // 数据初始化
	DelayUs(5000);
//	InitFlash();
	FlashRegs.FOPT.bit.ENPIPE = 1;

	InitXintf();
	for( i = 0; i < 100; i++)
	{
		DelayUs(5000);
	}

	InitGpio();                    	// GPIO初始化
	FlagSysInitErr.bit.GPIOInitErr = 0;
 	InitSCRGpio();                  // 可控硅驱动初始化
	FlagSysInitErr.bit.SCRInitErr = 0;

	InitExRam();                    // 外部RAM初始化
	FlagSysInitErr.bit.EXRAMInitErr = 0;
	InitModbusBData();

    ReadParaFromFlash();            // 读取外部FLASH参数
	DelayUs(5000);
	ReadEmDataFromFlash();          // 读取故障时重要保存数据
	DelayUs(5000);

	ParaTable.PowerReg.uParaTableVer = 0;		// 正式程序中屏蔽
	ParaTable.PowerReg.uHardwareVer = 0;		// 正式程序中屏蔽
	// 硬件版本校对完成或无版本并且参数表没有初始化,恢复参数表值默认值
	if(((ParaTable.PowerReg.uHardwareVer == 42) || (ParaTable.PowerReg.uHardwareVer == 0))
	&& (ParaTable.PowerReg.uParaTableVer == 0))
	{
		WtiteDefaultToFlash();
	}

	InitDMA();                      // DMA初始化
	FlagSysInitErr.bit.DMAInitErr = 0;

	InitADCOfDMA();                 // AD初始化，配置成DMA模式
	FlagSysInitErr.bit.ADCInitErr = 0;
	DelayUs(5000);

	IniteCANB();					// CANB初始化
	FlagSysInitErr.bit.CanBInitErr = 0;
	DelayUs(5000);

	SCIBInit(1, 115200); 			// SCIB初始化；
	FlagSysInitErr.bit.SCIBInitErr = 0;
   	DelayUs(5000);

	SCICInit(0, 115200); 			// SCIC初始化；
	FlagSysInitErr.bit.SCICInitErr = 0;
   	DelayUs(5000);

	InitCpuTimers();    			//3ms,定时器，外部运行中断； 对应中断程序interrupt void Timer1_ISR(void)在文件T4_PWM.c中；
	FlagSysInitErr.bit.Time1InitErr = 0;
	FlagSysInitErr.bit.Time2InitErr = 0;
	DelayUs(5000);

	InitEPWM();          			//epwm  初始化；开了PWM周期中断；对应中断程序在文件T1_PWM.c中
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

/*************************等待检测用户模式设置******************************/
	while (0 == ReadGPIOIn2());
	if (0 == FLAG_SOFT_START)					// 控制模式选择--软启动模式，针对单电机负载
	{
		FlagSystem.bit.SoftStart = 1;
		ControlGPIOOut(GPIO_LED1, ON);
	}
	else										// 控制模式选择--分相投入模式，针对多负载
	{
		FlagSystem.bit.SoftStart = 0;
		ControlGPIOOut(GPIO_LED1, OFF);
	}
	/******************************END******************************/

/***************************等待用户设置主备电******************************/
	while (0 == ReadGPIOIn5());					// 外部设置主备电
	if (0 == FLAG_PREFFERED)					// NO1电源为主路
	{
		FlagSystem.bit.NO1Preferred = 1;
		FlagSystem.bit.NO2Preferred = 0;
		ControlGPIOOut(GPIO_LED2, OFF);
	}
	else 										// NO2电源为主路
	{
		FlagSystem.bit.NO1Preferred = 0;
		FlagSystem.bit.NO2Preferred = 1;
		ControlGPIOOut(GPIO_LED2, ON);
	}
	/******************************END******************************/

#endif

	while(1)
	{
        ModbusBProcess();			//modbus接收处理
        DelayUs(3000);
        ModbusCProcess();
        DelayUs(3000);
        UpdataParaToFlash();		//更新参数到Flash

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

// 系统初始化故障的初始化
	FlagSysInitErr.all 			= 0xffff;
	FlagSysInitErr.bit.rsvd1   	= 0;

// 系统运行故障的初始化
	FlagSysRunErr.all = 0;

// 系统保护报警的初始化
	FlagSysAlarm.all = 0;

// 系统标志位初始化
	FlagSystem.all = 0;

// 系统状态位初始化
	StateSystem.all = 0;

// 正弦函数表初始化
	for (i=0; i<=1023; i++)
	{
		SinTab[i] = sin(2 * PI * i / 1024) * 4096;
	}

// 第一路电源锁相环初始化
    memset(&SpllRegNO1, 0x00, sizeof(SpllRegNO1));

// 第二路电源锁相环初始化
    memset(&SpllRegNO2, 0x00, sizeof(SpllRegNO2));

// 输出电源锁相环初始化
    memset(&SpllRegOut, 0x00, sizeof(SpllRegOut));

// 备电与输出差值锁相环初始化
    memset(&SpllRegUFO, 0x00, sizeof(SpllRegUFO));

// 系统计数器初始化
    memset(&CountSystem, 0x00, sizeof(CountSystem));

// PWM中断处理程序数据初始化
    memset(&PwmIsrReg, 0x00, sizeof(PwmIsrReg));

// NO1A相电流有效值
    memset(&NO1IARegs, 0x00, sizeof(NO1IARegs));

// NO1B相电流有效值
    memset(&NO1IBRegs, 0x00, sizeof(NO1IBRegs));

// NO1C相电流有效值
    memset(&NO1ICRegs, 0x00, sizeof(NO1ICRegs));

// NO2A相电流有效值
    memset(&NO2IARegs, 0x00, sizeof(NO2IARegs));

// NO2B相电流有效值
    memset(&NO2IBRegs, 0x00, sizeof(NO2IBRegs));

// NO2C相电流有效值
    memset(&NO2ICRegs, 0x00, sizeof(NO2ICRegs));

// A相温度有效值
    memset(&ModATempRegs, 0x00, sizeof(ModATempRegs));

// B相温度有效值
    memset(&ModBTempRegs, 0x00, sizeof(ModBTempRegs));

// C相温度有效值
    memset(&ModCTempRegs, 0x00, sizeof(ModCTempRegs));

// AD数据
    memset(&DataBuf, 0x00, sizeof(DataBuf));

// 状态表初始化
	SystemState = SystemWait;
	ConvertState = ConvertInit;
}
