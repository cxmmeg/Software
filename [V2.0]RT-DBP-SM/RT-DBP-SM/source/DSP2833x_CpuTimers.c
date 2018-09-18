// TI初始化文件
// 更新：2014-07-10
//###########################################################################
//
// 文件:	DSP2833x_CpuTimers.c
//
// 标题:	用于32位CPU定时器的初始化与其功能函数
//
// 描述:	1. 定时器初始化
//			2. 定时器配置
//
// 注意；	当使用嵌入式系统时不用使用CPU1和2定时器				
//########################################################################### 

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "DSP2833x_CpuTimers.h"   // Headerfile
#include "USERS_T4.h"
// 定时器0和1作为普通定时器供用户使用
struct CPUTIMER_VARS CpuTimer0;
struct CPUTIMER_VARS CpuTimer1;

// 定时器2系统保留作为嵌入式系统时使用
struct CPUTIMER_VARS CpuTimer2;

/*---------------------------------------------------------------------
函数原型：void InitCpuTimers(void)
函数名称：初始化CPU定时器
函数功能：
入口参数：无                  
出口参数：无
作    者：
日    期：2014年07月10日
----------------------------------------------------------------------*/ 
void InitCpuTimers(void)
{
/*--------------	初始化CPUTIME0	---------------------*/	
	CpuTimer0.RegsAddr = &CpuTimer0Regs;							// 初始化CPUTIME0寄存器地址

	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;							// 初始化CPUTIME0定时器周期值到最大值
	CpuTimer0Regs.TPR.all  = 0;										// 初始化CPU定时器预定标计数器
	CpuTimer0Regs.TPRH.all = 0;										// 初始化CPU定时器分频寄存器

	CpuTimer0Regs.TCR.bit.TSS = 1;									// 停止定时器
	CpuTimer0Regs.TCR.bit.TRB = 1;									// 重新装载

	CpuTimer0.InterruptCount = 0;									// 初始化中断计数

/*--------------	初始化CPUTIME1	---------------------*/	
	CpuTimer1.RegsAddr = &CpuTimer1Regs;							// 初始化CPUTIME0寄存器地址

	CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;							// 初始化CPUTIME0定时器周期值到最大值
	CpuTimer1Regs.TPR.all  = 0;										// 初始化CPU定时器预定标计数器
	CpuTimer1Regs.TPRH.all = 0;										// 初始化CPU定时器分频寄存器

	CpuTimer1Regs.TCR.bit.TSS = 1;									// 停止定时器
	CpuTimer1Regs.TCR.bit.TRB = 1;									// 重新装载

	CpuTimer1.InterruptCount = 0;									// 初始化中断计数

/*--------------	初始化CPUTIME2	---------------------*/	
	CpuTimer2.RegsAddr = &CpuTimer2Regs;							// 初始化CPUTIME0寄存器地址
	
	CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;							// 初始化CPUTIME0定时器周期值到最大值
	CpuTimer2Regs.TPR.all  = 0;										// 初始化CPU定时器预定标计数器
	CpuTimer2Regs.TPRH.all = 0;										// 初始化CPU定时器分频寄存器
	
	CpuTimer2Regs.TCR.bit.TSS = 1;									// 停止定时器
	CpuTimer2Regs.TCR.bit.TRB = 1;									// 重新装载

	CpuTimer2.InterruptCount = 0;									// 初始化中断计数

// 打开TIM1中断，并初始化向量表
	EALLOW;  
   	PieVectTable.XINT13 = &Timer1_ISR;							// 初始化XINT13中断向量表
   	EDIS;
	
	IER |= M_INT13;												// 打开CPU中断源INT13
	
//	EINT;
//	ERTM;

// 配置CPUTIM1时钟	
	ConfigCpuTimer(&CpuTimer1, 150, 3000);						// 3ms中断
	StartCpuTimer1(); 

	ConfigCpuTimer(&CpuTimer2, 150, 3000);						// 3ms/cycle
	StartCpuTimer2();
}

/*---------------------------------------------------------------------
函数原型：void ConfigCpuTimer(void)
函数名称：初始化CPU定时器
函数功能：
入口参数：CPUTIMER_VARS *Timer -- 输入选择需配置定时器 
		Freq	-- 输入选择CPU时钟频率
		Period  -- 输入选择分频因子
出口参数：无
作    者：
日    期：2014年07月10日
----------------------------------------------------------------------*/ 
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	Uint32 	temp;

	Timer->CPUFreqInMHz = Freq;
	Timer->PeriodInUSec = Period - 1;
	temp = (long) (Freq * Period);
	Timer->RegsAddr->PRD.all = temp;

	// Set pre-scale counter to divide by 1 (SYSCLKOUT):
	Timer->RegsAddr->TPR.all  = 0;
	Timer->RegsAddr->TPRH.all  = 0;

	// Initialize timer control register:
	Timer->RegsAddr->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer
	Timer->RegsAddr->TCR.bit.TRB = 1;      // 1 = reload timer
	Timer->RegsAddr->TCR.bit.SOFT = 0;
	Timer->RegsAddr->TCR.bit.FREE = 0;     // Timer Free Run Disabled
	Timer->RegsAddr->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt

	// Reset interrupt counter:
	Timer->InterruptCount = 0;
}

//===========================================================================
// End of file.
//===========================================================================
