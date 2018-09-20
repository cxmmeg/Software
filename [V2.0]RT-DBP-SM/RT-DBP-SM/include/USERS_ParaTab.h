#ifndef	_USERS_PARA_TAB_H_
#define	_USERS_PARA_TAB_H_

//#define TESTCODE	1				// 运行测试程序把此开关打开
#define RUNCODE		2				// 运行项目程序把此开关打开

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

/*-------------------------系统参数表定义----------------------------
*   包含：系统参数表定义
*   说明：每组数据固定64个寄存器
*
*-------------------------系统参数表定义----------------------------*/

//------------------------------------------------------------
// 系统状态表定义

//------------------------------------------------------------
enum SYSTEM_STATE {
	SystemWait,
	SystemStandby,
	SystemRun,
	SystemFault
};

//------------------------------------------------------------
// 系统初始化异常标记位

//------------------------------------------------------------
struct	FLAG_SYSTEM_INITERR_BITS{				// 位定义	0为正常	1为异常
	Uint16 GPIOInitErr 			: 1;			// 通用IO口初始化异常位
	Uint16 SCIBInitErr			: 1;			// SCIB口初始化异常位
	Uint16 PWM1InitErr			: 1;			// PWM1外设初始化异常位
	Uint16 Time1InitErr			: 1;			// 定时器1外设初始化异常位

	Uint16 Time2InitErr			: 1;			// 定时器1外设初始化异常位
	Uint16 CanBInitErr			: 1;			// CANB外设初始化异常位
	Uint16 ADCInitErr			: 1;			// ADC外设初始化异常位
	Uint16 EXRAMInitErr			: 1;			// 外部RAM初始化异常位

	Uint16 DMAInitErr			: 1;			// DMA初始化异常位

	Uint16 rsvd1				: 7;			//
};

union   FLAG_SYSTEM_INITERR{
	Uint16 all;
	struct FLAG_SYSTEM_INITERR_BITS bit;
};

//------------------------------------------------------------
// 系统运行异常标记位

//------------------------------------------------------------
struct	FLAG_SYSTEM_RUNERR_BITS{			// 位定义	0为正常	1为异常
	Uint16 ACInVolHigh			: 1;		// 输入AC过压保护
	Uint16 ACInVolLow			: 1;		// 输入AC欠压保护
	Uint16 ACInCurHigh			: 1;		// 输入AC过流保护
	Uint16 DCInVolHigh			: 1;		// 输入DC过压保护

	Uint16 DCInVolLow			: 1;		// 输入DC欠压保护
	Uint16 DCInCurHigh			: 1;		// 输入DC过流保护
	Uint16 BoostVolHigh			: 1;		// Boost升压过压保护
	Uint16 Boost1CurHigh		: 1;		// Boost1升压过流保护

	Uint16 Boost2CurHigh		: 1;		// Boost2升压过流保护
	Uint16 Boost3CurHigh		: 1;		// Boost3升压过流保护
	Uint16 ACOutVolHigh			: 1;		// 输出AC过压保护
	Uint16 ACOutVolLow			: 1;		// 输出AC欠压保护

	Uint16 ACOutCurHigh			: 1;		// 输出AC过流保护
	Uint16 OverTemp				: 1;		// 系统过温保护
	Uint16 FlagPDP				: 1;		// PDP硬件保护

	Uint16 rsvd1				: 1;		// 备用
};

struct  FLAG_SYSTEM_ALARM_BITS{				// 位定义	0为正常	1为异常
	Uint32 ACInVolHigh			: 1;		// 输入AC过压报警
	Uint32 ACInVolLow			: 1;		// 输入AC欠压报警
	Uint32 ACInCurHigh			: 1;		// 输入AC过流报警
	Uint32 ACInLossPhase		: 1;		// 输入AC缺相报警

	Uint32 DCInVolHigh			: 1;		// 输入DC过压报警
	Uint32 DCInVolLow			: 1;		// 输入DC欠压报警
	Uint32 DCInCurHigh			: 1;		// 输入DC过流报警
	Uint32 BoostVolHigh			: 1;		// Boost升压过压报警

	Uint32 Boost1CurHigh		: 1;		// Boost1升压过流报警
	Uint32 Boost2CurHigh		: 1;		// Boost2升压过流报警
	Uint32 Boost3CurHigh		: 1;		// Boost3升压过流报警
	Uint32 ACOutVolHigh			: 1;		// 输出AC过压报警

	Uint32 ACOutVolLow			: 1;		// 输出AC欠压报警
	Uint32 ACOutCurHigh			: 1;		// 输出AC过流报警
	Uint32 OverTemp				: 1;		// 系统过温报警
	Uint32 CanSend				: 1;		// CAN发送失败报警

	Uint32 OutSPLLAlarm			: 1;		// 输出锁相失败报警
	Uint32 OutPhaseLose			: 1;		// 输出锁缺相报警

	Uint32 rsvd1				: 19;		// 备用
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
// 系统运行硬件故障标记位

//------------------------------------------------------------
struct  FLAG_SYSTEM_PDPERR_BITS{			// 位定义	0为正常	1为异常
	Uint16 IGBT123Err			: 1;		// IGBTP123_ERR		PWM123驱动侧IGBT驱动保护
	Uint16 IGBT456Err			: 1;		// IGBTP456_ERR		PWM456驱动侧IGBT驱动保护
	Uint16 IAErr				: 1;		// IAErr			A相电感电流过流保护
	Uint16 ICErr				: 1;		// ICErr		3	B相电感电流过流保护

	Uint16 IUErr				: 1;		// IUErr			U相输出电流过流保护
	Uint16 IWErr				: 1;		// IWErr			W相输出电流过流保护
	Uint16 VDCErr				: 1;		// VDCErr			母线输入电压过压保护
	Uint16 rsd					: 1;

	Uint16 rsd1					: 8;
};

union	FLAG_SYSTEM_PDPERR{
	Uint16 all;
	struct FLAG_SYSTEM_PDPERR_BITS bit;
};

//------------------------------------------------------------
// 产品参数表

//------------------------------------------------------------
//电源常规参数组 64
struct DBP_POWER_REG
{
	int16		uInputType;			// 输入电源类型
    int16		uInputFre;			// 输入电源额定频率，单位0.1Hz
	int16		uInputVol;			// 输入电源额定电压，单位0.1V
	int16		uInputCur;			// 输入电源额定电流，单位0.1A
	int16		uOutputType;    	// 输出电源类型
    int16		uOutputFre;     	// 输出电源额定频率，单位0.1Hz
	int16		uOutputVol;     	// 输出电源额定电压，单位0.1V
	int16		uOutputCur;     	// 输出电源额定电流，单位0.1A
	int16		uOutputPower;		// 输出电源功率，单位1W
	int16		uBoostVol;			// Boost升压额定电压，单位0.1V

	int16		uBoostCur;			// Boost升压额定电流，单位0.1A
	int16		uSpeedMax;			// 变频器负载电机最高转速，单位1rpm
	int16		uSpeedMin;			// 变频器负载电机最低转速，单位1rpm
	int16		uSpeedSet;			// 变频器负载电机设定转速，单位1rpm
	int16		uTempNom;		    // 正常温度值，单位0.1℃
	int16		uPWMSwitchFreq;		// PWM开关频率，单位1Hz
	int16		uPWMSwitchPeriod;	// PWM开关周期，单位1us
	int16		uPWMSwitchDeadTime;	// PWM开关死区时间，单位1ns
	int16		uBoostSwitchFreq;	// Boost开关频率，单位1Hz
	int16		uBoostSwitchPeriod;	// Boost开关周期，单位1us

	int16		uBoostSwitchDeadTime;// Boost开关死区时间，单位1ns
	int16		uHardwareVer;		// 硬件版本号，例硬件版本为V4.2，该值显示42
	int16		uParaTableVer;	 	// 参数表版本号，按修改次数累加，如为0，则判断为需要初始化
    int16       uSoftwareVer;       // 软件版本号，例软件版本为V18041，该值表示18年4月份第1次修改

   	int16		nRrsev[40];
};

//PI参数设置组 64
struct  DBP_PI_REG
{
	int16   	uBoostVolKp;       	// Boost电压环
	int16   	uBoostVolKi;
    int16  		uBoostVolKd;
	int16 		uBoostVolPIMax;
	int16 		uBoostVolPIMin;

	int16   	uOutVolKp;          // 输出电压环
	int16   	uOutVolKi;
    int16  		uOutVolKd;
	int16 		uOutVolPIMax;
	int16 		uOutVolPIMin;

	int16   	uOutCurKp;          // 输出电流环
	int16   	uOutCurKi;
    int16  		uOutCurKd;
	int16 		uOutCurPIMax;
	int16 		uOutCurPIMin;

	int16   	uOutPLLKp;          // 输出电压锁相环
	int16   	uOutPLLKi;
    int16  		uOutPLLKd;
	int16 		uOutPLLPIMax;
	int16 		uOutPLLPIMin;

   	int16     	nRrsev[44];       	//预留；
};

// 运行控制参数组 64
struct DBP_RUNCTRL_REG
{
	int16		uInVolPreChg;		// 输入电压预充电电压值： 标幺值；4096=100%  Q12
	int16		uInVolHighAlarm;	// 输入过压报警值： 标幺值；4096=100%  Q12
	int16		uInVolHighCut;		// 输入过压切断值： 标幺值；4096=100%  Q12
	int16		uInVolLowAlarm;		// 输入欠压报警值： 标幺值；4096=100%  Q12
	int16		uInVolLowCut;		// 输入欠压切断值： 标幺值；4096=100%  Q12
	int16		uInCurHighAlarm;	// 输入过流报警值： 标幺值；4096=100%  Q12
	int16		uInCurHighCut;		// 输入过流切断值： 标幺值；4096=100%  Q12
	int16		uBoostVolHighAlarm;	// Boost升压过压报警值： 标幺值；4096=100%  Q12
	int16		uBoostVolHighCut;	// Boost升压过压切断值： 标幺值；4096=100%  Q12
	int16		uBoostCurHighAlarm;	// Boost升压过流报警值： 标幺值；4096=100%  Q12

	int16		uBoostCurHighCut;	// Boost升压过流切断值： 标幺值；4096=100%  Q12
	int16		uOutVolHighAlarm;	// 输出过压报警值： 标幺值；4096=100%  Q12
	int16		uOutVolHighCut;		// 输出过压切断值： 标幺值；4096=100%  Q12
	int16		uOutVolLowAlarm;	// 输出欠压报警值： 标幺值；4096=100%  Q12
	int16		uOutVolLowCut;		// 输出欠压切断值： 标幺值；4096=100%  Q12
	int16		uOutCurHighAlarm;	// 输出过流报警值： 标幺值；4096=100%  Q12
	int16		uOutCurHighCut;		// 输出过流切断值： 标幺值；4096=100%  Q12
	int16       uTempFanOpen;       // 风扇开启温度，单位0.1℃
	int16       uTempAlarm;         // 过温报警值，单位0.1℃
	int16       uTempCut;           // 过温切断值，单位0.1℃

    int16		uVolOutLPLow;		// 输出缺相检测下限值： 标幺值；4096=100%  Q12
    int16		uVolOutLPUp;		// 输出缺相检测上限值： 标幺值；4096=100%  Q12
    int16		uFreUnusualLow;		// 频率异常下限值： 标幺值；4096=100%  Q12
    int16		uFreUnusualUp;		// 频率异常上限值： 标幺值；4096=100%  Q12
    int16		bCtrlModeSet;		// 控制模式设定
    int16		bDirectionSet;		// 电机运转方向设定
    int16		bFreSourceSet;		// 输出主频率来源设定
    int16     	XSMaxInVol;			// 输入电压满量程值
    int16     	XSMaxInCur;			// 输入电流满量程值
    int16		XSMaxBoostVol;		// Boost升压电压满量程值

    int16     	XSMaxBoostCur;		// Boost升压电流满量程值
    int16     	XSMaxOutVol;		// 输出电压满量程值
    int16     	XSMaxOutCur;		// 输出电流满量程值
    int16 		XSMaxNTCTemp;		// 温度满量程值
    int16		XSMaxSpeed;			// 转速满量程值
    int16		uSpeedRatio;		// 升/降频速率
    int16		uVVVFVfSet;			// VVVF的V/F值设置，单位0.1

 	int16     	nRrsev[27];			// 预留
};

// 延迟报警参数组 64
struct DBP_DELAYCTRL_REG
{
	int16		uInVolAlarm;		// 输入电压报警延迟值	@3ms
	int16		uInVolCut;			// 输入电压切断延迟值	@3ms
	int16		uInCurAlarm;		// 输入电流报警延迟值	@3ms
	int16		uInCurCut;			// 输入电流切断延迟值	@3ms
	int16		uBoostVolAlarm;		// Boost升压电压报警延迟值	@3ms
	int16		uBoostVolCut;		// Boost升压电压切断延迟值	@3ms
	int16		uBoostCurAlarm;		// Boost升压电流报警延迟值	@3ms
	int16		uBoostCurCut;		// Boost升压电流切断延迟值	@3ms
	int16		uOutVolAlarm;		// 输出电压报警延迟值	@3ms
	int16		uOutVolCut;			// 输出电压切断延迟值	@3ms

	int16		uOutCurAlarm;		// 输出电流报警延迟值	@3ms
	int16		uOutCurCut;			// 输出电流切断延迟值	@3ms
	int16		uTempAlarm;			// 过温报警延迟值		@3ms
	int16		uTempCut;			// 过温切断延迟值		@3ms
    int16		uVolOutLP;			// 输出缺相延迟报警值	@3ms
    int16		uFreUnusual;		// 频率异常报警延迟值	@3ms

 	int16     	nRrsev[48];			// 预留
};


// CPU中断参数组 64
struct DBP_TIMER1ISR_REG
{
	Uint16		uSystemStatus;		// 系统运行状态表
	Uint16		uSystemInitErr;		// 系统初始化错误状态表
	Uint16		uSystemRunErr;		// 系统运行故障状态表
	Uint16		uSystemAlarmH;		// 系统运行报警状态表，高字节
	Uint16		uSystemAlarmL;		// 系统运行报警状态表，低字节
	Uint16		uSystemPDPErr;		// 系统硬件故障表
	Uint16		uGpioIn;			// 数字量输入
	Uint16		uGpioOut;			// 数字量输出
	int16		uCtrlMode;			// 控制模式
	int16 		uDirection;			// 电机运转方向

	int16		uFreSource;			// 主频率来源
	int16		uFreSet;			// 主频率设定值
	int16		uInVolRms;			// 输入电压有效值
	int16		uInCurRms;			// 输入电流有效值
	int16		uBoostVolRms;		// Boost1升压电压有效值
	int16		uBoost1CurRms;		// Boost1升压电流有效值
	int16		uBoost2CurRms;		// Boost2升压电流有效值
	int16		uBoost3CurRms;		// Boost3升压电流有效值
	int16		uOutVolRms;			// 输出电压有效值
	int16		uOutCurRms;			// 输出电流有效值

	int16		uOutFre;			// 输出频率
	int16		uOutActivePower;	// 输出有功功率实际值，单位1W
	int16		uOutReactivePower;	// 输出无功功率实际值，单位1W
	int16		uOutApparentPower;	// 输出视在功率实际值，单位1W
	int16		uOutPowerFactor;	// 输出功率因数,单位0.01
	int16		uTemp1Rms;			// NTC1温度值
	int16		uTemp2Rms;			// NTC2温度值
	int16		uTemp3Rms;			// NTC3温度值
	int16		uTemp4Rms;			// NTC4温度值
	int16		uTemp5Rms;			// NTC5温度值

	int16		uTemp6Rms;			// NTC6温度值
	int16		uTemp7Rms;			// NTC7温度值
	int16		uTemp8Rms;			// NTC8温度值
	int16		uT4UseTime;		    // Timer1中断运行时间 @3ms/cycle
	int16		uInstReg0;			// 备用寄存器0
	int16		uInstReg1;			// 备用寄存器1
	int16		uInstReg2;			// 备用寄存器2
	int16		uInstReg3;			// 备用寄存器3
	int16		uInstReg4;			// 备用寄存器4
	int16		uInstReg5;			// 备用寄存器5

	int16		uInstReg6;		    // 备用寄存器6
	int16		uInstReg7;		    // 备用寄存器7
	int16		uInstReg8;		    // 备用寄存器8
	int16		uInstReg9;		    // 备用寄存器9

    int16		nRrsev[20];			// 预留
};

// PWM中断参数组 64
struct DBP_PWMISR_REG
{
	int16 uInVol;					// 输入电压  			Q12
	int16 uInCur;					// 输入电流  			Q12
	int16 uBoost1Cur;				// Boost升压电流1  	Q12
	int16 uBoost2Cur;				// Boost升压电流2  	Q12
	int16 uBoost3Cur;				// Boost升压电流3  	Q12
	int16 uOutUab;					// 输出电源AB电压        	Q12
	int16 uOutUbc;					// 输出电源BC电压        	Q12
	int16 uOutIAl;				    // 输出A相总电流	 	Q12
	int16 uOutIBl;				    // 输出B相总电流	 	Q12
	int16 uOutICl;				    // 输出C相总电流	 	Q12

	int16 uOutIAc;				    // 输出A相负载电流	 	Q12
	int16 uOutIBc;				    // 输出B相负载电流		Q12
	int16 uOutICc;				    // 输出C相负载电流	 	Q12
	int16 uOutUd;					// 输出电压D轴瞬时值   	Q12
	int16 uOutUq;					// 输出电压Q轴瞬时值	Q12
	int16 uOutTheta;				// 输出电压角度值		Q12
	int16 uOutInstActivePower;		// 输出有功功率瞬时值	Q12
	int16 uOutInstReactivePower;	// 输出无功功率瞬时值	Q12
	int16 uOutAvgActivePower;		// 输出有功功率平均值	Q12
	int16 uOutAvgReactivePower;		// 输出无功功率平均值	Q12

	int16 uModATemp;				// A相模块温度有效值	Q12
	int16 uModBTemp;				// B相模块温度有效值	Q12
	int16 uT1UseTime;				// T1中断运行时间
    int16 uActiveReg0;              // 备用寄存器0
	int16 uActiveReg1;				// 备用寄存器1
	int16 uActiveReg2;				// 备用寄存器2
	int16 uActiveReg3;				// 备用寄存器3
	int16 uActiveReg4;				// 备用寄存器4
	int16 uActiveReg5;			    // 备用寄存器5
	int16 uActiveReg6;			    // 备用寄存器6

	int16 uActiveReg7;			    // 备用寄存器7
	int16 uActiveReg8;			    // 备用寄存器8
	int16 uActiveReg9;			    // 备用寄存器9

    int16 nRrsev[31];				// 预留
};

// 重要数据保存参数组 64
struct EMERGENCY_REG
{
    int16 uEmDataSize;              // 重要数据存储区域大小
    int16 uEmDataNumMax;            // 重要数据最大存储个数
    int16 uEmDataSaveTime;          // 重要数据保存时间
    int16 uEmDataChgEnable;         // 重要数据存储使能开关，写0x5555使能
    int16 uEmDataChgFlag;           // 重要数据允许更改标记
    int16 uEmData0Addr;             // 重要数据0地址
    int16 uEmData1Addr;             // 重要数据1地址
    int16 uEmData2Addr;             // 重要数据2地址
    int16 uEmData3Addr;             // 重要数据3地址
    int16 uEmData4Addr;             // 重要数据4地址

    int16 uEmData5Addr;             // 重要数据5地址
    int16 uEmData6Addr;             // 重要数据6地址
    int16 uEmData7Addr;             // 重要数据7地址
    int16 uEmCountRam;              // 重要数据保存计数器
    int16 uEmCountBlock;            // 重要数据块计数器
    int16 uEmDataSaveFinishFlag;    // 重要数据保存成功标记 0-保存未完成 1-保存完成
    int16 uEmDataFlashWriteOK;      // 重要数据写入FLASH成功标记 0-未成功写入 1-写入成功

    int16 nRrsev[47];               // 预留
};

struct PARAMETER_REGS
{
   	struct DBP_POWER_REG       	PowerReg;	    // 电源参数表			64
   	struct DBP_PI_REG          	PIReg;       	// PI参数表			64
   	struct DBP_RUNCTRL_REG    	RunCtrlReg;		// 运行控制参数表  		64
   	struct DBP_DELAYCTRL_REG	DelayCtrlReg;	// 延迟报警/保护参数表	64
	struct DBP_TIMER1ISR_REG   	Timer1Reg;      // 静态运行参数     		64
	struct DBP_PWMISR_REG		PWMReg;		    // 动态参数表        		64
	struct EMERGENCY_REG   		EmergencyReg;   // 重要数据保存参数组 	64
};

extern struct PARAMETER_REGS ParaTable;


/*-------------------------系统控制参数定义----------------------------
*   包含：系统控制参数定义
*   说明：标记、状态寄存器固定32个BIT，计数寄存器无固定大小
*
*-------------------------系统参数表定义----------------------------*/
//------------------------------------------------------------
// 系统运行标记、状态位

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
// 系统运行计数寄存器

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
	Uint16 PwmInvRun;
};

struct  REGS_SYSTEM{
	int32 OutVolThetaSum;
	int16 OutVolTheta;
	int16 OutVolWeReal;
	int16 OutVolWeRef;
	int16 OutVolSina;
	int16 OutVolCosa;
	int16 OutVolRms;
	int16 SpeedRatio;
	int16 VVVFVfSet;
};

/******************函数声明*********************/
void ReadParaFromFlash(void);
void WtiteDefaultToFlash(void);
int UpdataParaToFlash(void);

#endif
