// TI��ʼ���ļ�
// ���£�2014-07-10
//###########################################################################
//
// �ļ�:	DSP2833x_CpuTimers.c
//
// ����:	����32λCPU��ʱ���ĳ�ʼ�����书�ܺ���
//
// ����:	1. ��ʱ����ʼ��
//			2. ��ʱ������
//
// ע�⣻	��ʹ��Ƕ��ʽϵͳʱ����ʹ��CPU1��2��ʱ��				
//########################################################################### 

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "DSP2833x_CpuTimers.h"   // Headerfile
#include "USERS_T4.h"
// ��ʱ��0��1��Ϊ��ͨ��ʱ�����û�ʹ��
struct CPUTIMER_VARS CpuTimer0;
struct CPUTIMER_VARS CpuTimer1;

// ��ʱ��2ϵͳ������ΪǶ��ʽϵͳʱʹ��
struct CPUTIMER_VARS CpuTimer2;

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitCpuTimers(void)
�������ƣ���ʼ��CPU��ʱ��
�������ܣ�
��ڲ�������                  
���ڲ�������
��    �ߣ�
��    �ڣ�2014��07��10��
----------------------------------------------------------------------*/ 
void InitCpuTimers(void)
{
/*--------------	��ʼ��CPUTIME0	---------------------*/	
	CpuTimer0.RegsAddr = &CpuTimer0Regs;							// ��ʼ��CPUTIME0�Ĵ�����ַ

	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;							// ��ʼ��CPUTIME0��ʱ������ֵ�����ֵ
	CpuTimer0Regs.TPR.all  = 0;										// ��ʼ��CPU��ʱ��Ԥ���������
	CpuTimer0Regs.TPRH.all = 0;										// ��ʼ��CPU��ʱ����Ƶ�Ĵ���

	CpuTimer0Regs.TCR.bit.TSS = 1;									// ֹͣ��ʱ��
	CpuTimer0Regs.TCR.bit.TRB = 1;									// ����װ��

	CpuTimer0.InterruptCount = 0;									// ��ʼ���жϼ���

/*--------------	��ʼ��CPUTIME1	---------------------*/	
	CpuTimer1.RegsAddr = &CpuTimer1Regs;							// ��ʼ��CPUTIME0�Ĵ�����ַ

	CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;							// ��ʼ��CPUTIME0��ʱ������ֵ�����ֵ
	CpuTimer1Regs.TPR.all  = 0;										// ��ʼ��CPU��ʱ��Ԥ���������
	CpuTimer1Regs.TPRH.all = 0;										// ��ʼ��CPU��ʱ����Ƶ�Ĵ���

	CpuTimer1Regs.TCR.bit.TSS = 1;									// ֹͣ��ʱ��
	CpuTimer1Regs.TCR.bit.TRB = 1;									// ����װ��

	CpuTimer1.InterruptCount = 0;									// ��ʼ���жϼ���

/*--------------	��ʼ��CPUTIME2	---------------------*/	
	CpuTimer2.RegsAddr = &CpuTimer2Regs;							// ��ʼ��CPUTIME0�Ĵ�����ַ
	
	CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;							// ��ʼ��CPUTIME0��ʱ������ֵ�����ֵ
	CpuTimer2Regs.TPR.all  = 0;										// ��ʼ��CPU��ʱ��Ԥ���������
	CpuTimer2Regs.TPRH.all = 0;										// ��ʼ��CPU��ʱ����Ƶ�Ĵ���
	
	CpuTimer2Regs.TCR.bit.TSS = 1;									// ֹͣ��ʱ��
	CpuTimer2Regs.TCR.bit.TRB = 1;									// ����װ��

	CpuTimer2.InterruptCount = 0;									// ��ʼ���жϼ���

// ��TIM1�жϣ�����ʼ��������
	EALLOW;  
   	PieVectTable.XINT13 = &Timer1_ISR;							// ��ʼ��XINT13�ж�������
   	EDIS;
	
	IER |= M_INT13;												// ��CPU�ж�ԴINT13
	
//	EINT;
//	ERTM;

// ����CPUTIM1ʱ��	
	ConfigCpuTimer(&CpuTimer1, 150, 3000);						// 3ms�ж�
	StartCpuTimer1(); 

	ConfigCpuTimer(&CpuTimer2, 150, 3000);						// 3ms/cycle
	StartCpuTimer2();
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void ConfigCpuTimer(void)
�������ƣ���ʼ��CPU��ʱ��
�������ܣ�
��ڲ�����CPUTIMER_VARS *Timer -- ����ѡ�������ö�ʱ�� 
		Freq	-- ����ѡ��CPUʱ��Ƶ��
		Period  -- ����ѡ���Ƶ����
���ڲ�������
��    �ߣ�
��    �ڣ�2014��07��10��
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
