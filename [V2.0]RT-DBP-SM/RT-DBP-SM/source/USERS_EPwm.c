/*****************ͷ�ļ�********************/
#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h"

extern Uint16 PDPData;
extern interrupt void ePWM1_ISR(void);

void InitEPwm1(int freq);
void InitEPwm2(int freq);
void InitEPwm3(int freq);
void InitEPwm4(int32 freq);
void InitEPwm5(int32 freq);
void InitEPwm6(int32 freq);

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitEPWM(void)
�������ƣ���ʼ��EPWMģ��
�������ܣ�
��ڲ�������                    
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void InitEPWM(void)
{	
	InitTzGpio();            //����TZ1;
	DisablePWM(EPWM_ALL);

	EALLOW;
   	PieVectTable.EPWM1_INT = &ePWM1ISR;  //����PWM�ж�
	PieVectTable.EPWM1_TZINT = &TZ1ISR;  //����PDP��tz�жϣ�
   	EDIS; 

   	EALLOW;
   	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   	EDIS;

   	InitEPwm1(6400);
	InitEPwm2(6400);
	InitEPwm3(6400);
//	InitEPwm4(80000);
//	InitEPwm5(80000);
//	InitEPwm6(80000);

   	EALLOW;
   	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   //����PWM����PWM2��PWM3��PWM1ͬ����
   	EDIS;
	
// Enable CPU INT2 AND INT3    	
	IER |= M_INT2;
	IER |= M_INT3;

// Enable PIE: Group 3 interrupt 1 
	PieCtrlRegs.PIEIER2.bit.INTx1 = 1;	
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

// Enable global Interrupts and higher priority real-time debug events
//	EINT;
//	ERTM;
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void EnableTZ(void)
�������ƣ�ʹ��TZ1
�������ܣ�
��ڲ�������                    
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void EnableTZ(void)
{	
	EALLOW;
	EPwm1Regs.TZEINT.bit.OST = 1;
	EPwm2Regs.TZEINT.bit.OST = 1;
	EPwm3Regs.TZEINT.bit.OST = 1;
	EPwm4Regs.TZEINT.bit.OST = 1;
	EPwm5Regs.TZEINT.bit.OST = 1;
	EPwm6Regs.TZEINT.bit.OST = 1;
	EDIS; 
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void Disable_TZ(void)
�������ƣ���ֹTZ1
�������ܣ�
��ڲ�������                    
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void Disable_TZ(void)
{
	EALLOW;
	EPwm1Regs.TZEINT.all = 0;
	EPwm2Regs.TZEINT.all = 0;
	EPwm3Regs.TZEINT.all = 0;
	EPwm4Regs.TZEINT.all = 0;
	EPwm5Regs.TZEINT.all = 0;
	EPwm6Regs.TZEINT.all = 0;
	EDIS;
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitEPwm1(int freq)
�������ƣ�epwm1��ʼ��
�������ܣ�
��ڲ�����freq---����Ƶ��                    
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void InitEPwm1(int freq)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO0	= 1; // PWM1
	GpioCtrlRegs.GPAMUX1.bit.GPIO1	= 1; // PWM2
    EDIS;
   	
    EPwm1Regs.AQCTLA.bit.CAU        = AQ_SET ;
    EPwm1Regs.AQCTLA.bit.CAD        = AQ_CLEAR ;

   	EPwm1Regs.TBPRD 			    = 37500000/freq;        	// TBCLK counts
	EPwm1Regs.TBPHS.half.TBPHS	    = 0; 			        	// Set Phase register to zero
   	EPwm1Regs.TBCTR 			    = 0;			        	// Clear counter

   // Setup TBCLK
   	EPwm1Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 		// Count up down
   	EPwm1Regs.TBCTL.bit.PHSEN 		= TB_DISABLE;        	// Master module
   	EPwm1Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;       		// Clock ratio to SYSCLKOUT
   	EPwm1Regs.TBCTL.bit.CLKDIV 		= TB_DIV2;
	EPwm1Regs.TBCTL.bit.PRDLD 		= TB_SHADOW;
	EPwm1Regs.TBCTL.bit.SYNCOSEL 	= TB_CTR_ZERO; 			// Sync down-stream module

   	EPwm1Regs.CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;    		// Load registers every ZERO
   	EPwm1Regs.CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;
   	EPwm1Regs.CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO;
   	EPwm1Regs.CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO;

   // Setup compare
   	EPwm1Regs.CMPA.half.CMPA 		= EPwm1Regs.TBPRD / 2;

   // Active Low PWMs - Setup Deadband
  	EPwm1Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
   	EPwm1Regs.DBCTL.bit.POLSEL 		= DB_ACTV_LOC;
   	EPwm1Regs.DBRED 				= 350;
   	EPwm1Regs.DBFED 				= 350;

	//PWM-chopping Disable
	EPwm1Regs.PCCTL.bit.CHPEN		= 0;

	//PWM-Trip-Zone Submodule
	EALLOW;
	EPwm1Regs.TZSEL.bit.OSHT1		= 1;	//CBC Trip1
	EPwm1Regs.TZCTL.bit.TZA			= 1;	//Force High
	EPwm1Regs.TZCTL.bit.TZB			= 1;
	EDIS;	

   // Interrupt where we will change the Deadband
  	EPwm1Regs.ETSEL.bit.INTSEL 		= ET_CTR_ZERO;     // Select INT on Zero event
   	EPwm1Regs.ETSEL.bit.INTEN 		= 1;               // Enable INT
   	EPwm1Regs.ETPS.bit.INTPRD 		= ET_1ST;          // Generate INT on first event
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitEPwm2(int freq)
�������ƣ�epwm2��ʼ��
�������ܣ�
��ڲ�����freq---����Ƶ��                     
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void InitEPwm2(int freq)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO2	= 1; 				// PWM3 
	GpioCtrlRegs.GPAMUX1.bit.GPIO3	= 1; 				// PWM4
    EDIS;

   	EPwm2Regs.AQCTLA.bit.CAU 		= AQ_SET;          	// Set PWM2A
   	EPwm2Regs.AQCTLA.bit.CAD 		= AQ_CLEAR    ;    	// Set PWM2B  
   
   	EPwm2Regs.TBPRD 				= 37500000/freq; 	// TBCLK counts
   	EPwm2Regs.TBPHS.half.TBPHS 		= 0;           		// Phase is 0
   	EPwm2Regs.TBCTR 				= 0;				// Clear counter
  
   // Setup TBCLK
   	EPwm2Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 	// Count up
   	EPwm2Regs.TBCTL.bit.PHSEN 		= TB_ENABLE; 		// Slave module
   	EPwm2Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;       	// Clock ratio to SYSCLKOUT
   	EPwm2Regs.TBCTL.bit.CLKDIV 		= TB_DIV2;          // Slow just to observe on the scope
	EPwm2Regs.TBCTL.bit.PRDLD 		= TB_SHADOW;
	EPwm2Regs.TBCTL.bit.SYNCOSEL 	= TB_SYNC_IN; 		// sync flow-through

   // Setup compare   	
	EPwm2Regs.CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero
	EPwm2Regs.CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero

	EPwm2Regs.CMPA.half.CMPA 		= EPwm2Regs.TBPRD/2;

    // Active Low complementary PWMs - setup the deadband
   	EPwm2Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE; 	// enable Dead-band module
	EPwm2Regs.DBCTL.bit.POLSEL 		= DB_ACTV_LOC; 		// Active Hi complementary

   	EPwm2Regs.DBRED 				= 350;
   	EPwm2Regs.DBFED 				= 350;

	//PWM-chopping Disable
	EPwm2Regs.PCCTL.bit.CHPEN		= 0;	

	//PWM-Trip-Zone Submodule
	EALLOW;
	EPwm2Regs.TZSEL.bit.OSHT1		= 1;	//one-shot trip1
	EPwm2Regs.TZCTL.bit.TZA			= 1;
	EPwm2Regs.TZCTL.bit.TZB			= 1;
	EDIS;

   // Interrupt where we will modify the deadband
   	EPwm2Regs.ETSEL.bit.INTEN 		= 0;                 // Disable INT
}


/*---------------------------------------------------------------------
����ԭ�ͣ�void InitEPwm3(int freq)
�����ƣ�epwm3��ʼ��
�������ܣ�
��ڲ���int freq---����Ƶ��                     
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void InitEPwm3(int freq)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO4	= 1; // PWM5 
	GpioCtrlRegs.GPAMUX1.bit.GPIO5	= 1; // PWM6
    EDIS;

   	EPwm3Regs.AQCTLA.bit.CAU 		= AQ_SET;          // Set PWM3A
   	EPwm3Regs.AQCTLA.bit.CAD 		= AQ_CLEAR  ;        // Set PWM3B  
   
   	EPwm3Regs.TBPRD 			= 37500000/freq; 	// TBCLK counts
   	EPwm3Regs.TBPHS.half.TBPHS	= 0;				// Phase is 0
   	EPwm3Regs.TBCTR 			= 0;				// Clear counter

   // Setup TBCLK
   	EPwm3Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 	// Count up
   	EPwm3Regs.TBCTL.bit.PHSEN 		= TB_ENABLE; 		// Slave module
   	EPwm3Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;			// Clock ratio to SYSCLKOUT
   	EPwm3Regs.TBCTL.bit.CLKDIV 		= TB_DIV2;			// Slow so we can observe on the scope
	EPwm3Regs.TBCTL.bit.PRDLD 		= TB_SHADOW;
	EPwm3Regs.TBCTL.bit.SYNCOSEL 	= TB_SYNC_IN; 		// sync flow-through

   // Setup compare
	EPwm3Regs.CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero
	EPwm3Regs.CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero
   	EPwm3Regs.CMPA.half.CMPA 		= EPwm3Regs.TBPRD/2;
   
   // Active high complementary PWMs - Setup the deadband	
	EPwm3Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE; 	// enable Dead-band module
	EPwm3Regs.DBCTL.bit.POLSEL 		= DB_ACTV_LOC; 		// Active Hi complementary
   
   	EPwm3Regs.DBRED 				= 350;
   	EPwm3Regs.DBFED 				= 350;

	//PWM-chopping Disable
	EPwm3Regs.PCCTL.bit.CHPEN		= 0;	

	//PWM-Trip-Zone Submodule
	EALLOW;
	EPwm3Regs.TZSEL.bit.OSHT1		= 1;	//one-shot trip1
	EPwm3Regs.TZCTL.bit.TZA			= 1;
	EPwm3Regs.TZCTL.bit.TZB			= 1;
	EDIS;

   // Interrupt where we will change the deadband
   	EPwm3Regs.ETSEL.bit.INTEN 		= 0;                  // Disable INT
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitEPwm4(int freq)
�����ƣ�epwm4��ʼ��
�������ܣ�
��ڲ���int freq---����Ƶ��
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void InitEPwm4(int32 freq)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO6	= 1; // PWM7
	GpioCtrlRegs.GPAMUX1.bit.GPIO7	= 1; // PWM8
	EDIS;

	EPwm4Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;			// Set PWM4A
	EPwm4Regs.AQCTLA.bit.CAD 		= AQ_SET;
	EPwm4Regs.AQCTLB.bit.CBU 		= AQ_SET;			// Set PWM4B
	EPwm4Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;

	EPwm4Regs.TBPRD 				= 37500000/freq; 	// TBCLK counts
	EPwm4Regs.TBPHS.half.TBPHS		= 0;				// Phase is 0
	EPwm4Regs.TBCTR 				= 0;				// Clear counter

// Setup TBCLK
	EPwm4Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 	// Count up
	EPwm4Regs.TBCTL.bit.PHSEN 		= TB_DISABLE; 		// Slave module
	EPwm4Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;			// Clock ratio to SYSCLKOUT
	EPwm4Regs.TBCTL.bit.CLKDIV 		= TB_DIV2;			// Slow so we can observe on the scope
	EPwm4Regs.TBCTL.bit.PRDLD 		= TB_SHADOW;
	EPwm4Regs.TBCTL.bit.SYNCOSEL 	= TB_SYNC_DISABLE; 		// sync flow-through

// Setup compare
	EPwm4Regs.CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero
	EPwm4Regs.CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero

	EPwm4Regs.CMPA.half.CMPA 		= EPwm4Regs.TBPRD/2;

// Active high complementary PWMs - Setup the deadband
	EPwm4Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE; 	// enable Dead-band module
	EPwm4Regs.DBCTL.bit.POLSEL 		= DB_ACTV_LOC; 		// Active Hi complementary

	EPwm4Regs.DBRED 				= 50;
	EPwm4Regs.DBFED 				= 50;

//PWM-chopping Disable
	EPwm4Regs.PCCTL.bit.CHPEN		= 0;

//PWM-Trip-Zone Submodule
	EALLOW;
	EPwm4Regs.TZSEL.bit.OSHT1		= 1;	//one-shot trip1
	EPwm4Regs.TZCTL.bit.TZA			= 1;
	EPwm4Regs.TZCTL.bit.TZB			= 1;
	EDIS;

// Interrupt where we will change the deadband
	EPwm4Regs.ETSEL.bit.INTEN 		= 0;                  // Disable INT
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitEPwm5(int freq)
�����ƣ�epwm5��ʼ��
�������ܣ�
��ڲ���int freq---����Ƶ��
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void InitEPwm5(int32 freq)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO8	= 1; // PWM9
	GpioCtrlRegs.GPAMUX1.bit.GPIO9	= 1; // PWM10
	EDIS;

	EPwm5Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;			// Set PWM5A
	EPwm5Regs.AQCTLA.bit.CAD 		= AQ_SET;
	EPwm5Regs.AQCTLB.bit.CBU 		= AQ_SET;			// Set PWM5B
	EPwm5Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;

	EPwm5Regs.TBPRD 				= 37500000/freq; 	// TBCLK counts
	EPwm5Regs.TBPHS.half.TBPHS		= 0;				// Phase is 0
	EPwm5Regs.TBCTR 				= 0;				// Clear counter

// Setup TBCLK
	EPwm5Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 	// Count up
	EPwm5Regs.TBCTL.bit.PHSEN 		= TB_DISABLE; 		// Slave module
	EPwm5Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;			// Clock ratio to SYSCLKOUT
	EPwm5Regs.TBCTL.bit.CLKDIV 		= TB_DIV2;			// Slow so we can observe on the scope
	EPwm5Regs.TBCTL.bit.PRDLD 		= TB_SHADOW;
	EPwm5Regs.TBCTL.bit.SYNCOSEL 	= TB_SYNC_DISABLE;//TB_SYNC_DISABLE; 		// sync flow-through

// Setup compare
	EPwm5Regs.CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero
	EPwm5Regs.CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero

	EPwm5Regs.CMPA.half.CMPA 		= EPwm5Regs.TBPRD/2;

// Active high complementary PWMs - Setup the deadband
	EPwm5Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE; 	// enable Dead-band module
	EPwm5Regs.DBCTL.bit.POLSEL 		= DB_ACTV_LOC; 		// Active Hi complementary

	EPwm5Regs.DBRED 				= 50;
	EPwm5Regs.DBFED 				= 50;

//PWM-chopping Disable
	EPwm5Regs.PCCTL.bit.CHPEN		= 0;

//PWM-Trip-Zone Submodule
	EALLOW;
	EPwm5Regs.TZSEL.bit.OSHT1		= 1;	//one-shot trip1
	EPwm5Regs.TZCTL.bit.TZA			= 1;
	EPwm5Regs.TZCTL.bit.TZB			= 1;
	EDIS;

// Interrupt where we will change the deadband
	EPwm5Regs.ETSEL.bit.INTEN 		= 0;                  // Disable INT
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitEPwm6(int freq)
�����ƣ�epwm6��ʼ��
�������ܣ�
��ڲ���int freq---����Ƶ��
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void InitEPwm6(int32 freq)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO10	= 1; 			// PWM11
	GpioCtrlRegs.GPAMUX1.bit.GPIO11	= 1; 			// PWM12
	EDIS;

	EPwm6Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;		// Set PWM6A
	EPwm6Regs.AQCTLA.bit.CAD 		= AQ_SET;
	EPwm6Regs.AQCTLB.bit.CBU 		= AQ_SET;		// Set PWM6B
	EPwm6Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;

	EPwm6Regs.TBPRD 				= 37500000/freq; 	// TBCLK counts
	EPwm6Regs.TBPHS.half.TBPHS		= 0;				// Phase is 0
	EPwm6Regs.TBCTR 				= 0;				// Clear counter

// Setup TBCLK
	EPwm6Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 	// Count up
	EPwm6Regs.TBCTL.bit.PHSEN 		= TB_DISABLE; 		// Slave module
	EPwm6Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;			// Clock ratio to SYSCLKOUT
	EPwm6Regs.TBCTL.bit.CLKDIV 		= TB_DIV2;			// Slow so we can observe on the scope
	EPwm6Regs.TBCTL.bit.PRDLD 		= TB_SHADOW;
	EPwm6Regs.TBCTL.bit.SYNCOSEL 	= TB_SYNC_DISABLE;//TB_CTR_ZERO; 		// sync flow-through

// Setup compare
	EPwm6Regs.CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero
	EPwm6Regs.CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO; 		// load on CTR=Zero

	EPwm6Regs.CMPA.half.CMPA 		= EPwm6Regs.TBPRD/2;
	EPwm6Regs.CMPB 					= EPwm6Regs.TBPRD/2;

// Active high complementary PWMs - Setup the deadband
	EPwm6Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE; 	// enable Dead-band module
	EPwm6Regs.DBCTL.bit.POLSEL 		= DB_ACTV_LOC; 		// Active Hi complementary

	EPwm6Regs.DBRED 				= 50;
	EPwm6Regs.DBFED 				= 50;

//PWM-chopping Disable
	EPwm6Regs.PCCTL.bit.CHPEN		= 0;

//PWM-Trip-Zone Submodule
	EALLOW;
	EPwm6Regs.TZSEL.bit.OSHT1		= 1;	//one-shot trip1
	EPwm6Regs.TZCTL.bit.TZA			= 1;
	EPwm6Regs.TZCTL.bit.TZB			= 1;
	EDIS;

// Interrupt where we will change the deadband
	EPwm6Regs.ETSEL.bit.INTEN 		= 0;                  // Disable INT
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void EnablePWM(char channel)
�������ƣ���PWM
�������ܣ�
��ڲ�����channel---ͨ����	EPWM_CH1	ͨ��1
						......
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
#pragma CODE_SECTION(EnablePWM, "ramcode")

void EnablePWM(char channel)  
{
    switch(channel)
	{
    	case EPWM_CH1:
		EPwm1Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
		EPwm1Regs.AQCSFRC.bit.CSFA      = 0;     			//����ǿ�������Ч��
		EPwm1Regs.AQCSFRC.bit.CSFB      = 0;
		EPwm1Regs.AQCTLA.bit.CAU 		= AQ_SET;          // Set PWM1A
		EPwm1Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;        // Set PWM1B
        break;

    	case EPWM_CH2:
	    EPwm2Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	    EPwm2Regs.AQCSFRC.bit.CSFA      = 0;     			//����ǿ�������Ч��                 
    	EPwm2Regs.AQCSFRC.bit.CSFB      = 0;   
	   	EPwm2Regs.AQCTLA.bit.CAU 		= AQ_SET;          	// Set PWM2A
   	    EPwm2Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;        	// Set PWM2B  
		break;

    	case EPWM_CH3:
	    EPwm3Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	    EPwm3Regs.AQCSFRC.bit.CSFA      = 0;     			//����ǿ�������Ч��                 
    	EPwm3Regs.AQCSFRC.bit.CSFB      = 0;   
	   	EPwm3Regs.AQCTLA.bit.CAU 		= AQ_SET;          	// Set PWM3A
   	    EPwm3Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;        	// Set PWM3B  
		break;

    	case EPWM_CH4:
//	    EPwm4Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	    EPwm4Regs.AQCSFRC.bit.CSFA      = 0;     			//����ǿ�������Ч��
	    EPwm4Regs.AQCSFRC.bit.CSFB      = 0;
		EPwm4Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;			// Set PWM4A
		EPwm4Regs.AQCTLA.bit.CAD 		= AQ_SET;
		EPwm4Regs.AQCTLB.bit.CBU 		= AQ_SET;			// Set PWM4B
		EPwm4Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;
		break;

    	case EPWM_CH5:
//    	EPwm5Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	    EPwm5Regs.AQCSFRC.bit.CSFA      = 0;     			//����ǿ�������Ч��
	    EPwm5Regs.AQCSFRC.bit.CSFB      = 0;
		EPwm5Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;			// Set PWM5A
		EPwm5Regs.AQCTLA.bit.CAD 		= AQ_SET;
		EPwm5Regs.AQCTLB.bit.CBU 		= AQ_SET;			// Set PWM5B
		EPwm5Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;
		break;

    	case EPWM_CH6:
//	    EPwm6Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	    EPwm6Regs.AQCSFRC.bit.CSFA      = 0;     			//����ǿ�������Ч��
	    EPwm6Regs.AQCSFRC.bit.CSFB      = 0;
		EPwm6Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;			// Set PWM6A
		EPwm6Regs.AQCTLA.bit.CAD 		= AQ_SET;
		EPwm6Regs.AQCTLB.bit.CBU 		= AQ_SET;			// Set PWM6B
		EPwm6Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;
		break;

    	case  EPWM_CH123:
	    EPwm1Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
        EPwm2Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
        EPwm3Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
		EPwm1Regs.AQCSFRC.bit.CSFA  	= 0;     			//����ǿ�������Ч��
    	EPwm1Regs.AQCSFRC.bit.CSFB  	= 0;
		EPwm2Regs.AQCSFRC.bit.CSFA  	= 0;     			//����ǿ�������Ч��
    	EPwm2Regs.AQCSFRC.bit.CSFB  	= 0;
		EPwm3Regs.AQCSFRC.bit.CSFA  	= 0;     			//����ǿ�������Ч��
    	EPwm3Regs.AQCSFRC.bit.CSFB  	= 0;
		EPwm1Regs.AQCTLA.bit.CAU 		= AQ_SET;          	// Set PWM1A
		EPwm1Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;        	// Set PWM1B
		EPwm2Regs.AQCTLA.bit.CAU 		= AQ_SET;          	// Set PWM2A
		EPwm2Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;        	// Set PWM2B
		EPwm3Regs.AQCTLA.bit.CAU 		= AQ_SET;          	// Set PWM3A
		EPwm3Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;        	// Set PWM3B
	    break; 

    	case EPWM_CH46:
//		EPwm4Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
//		EPwm6Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
		EPwm4Regs.AQCSFRC.bit.CSFA  	= 0;     			//����ǿ�������Ч��
		EPwm4Regs.AQCSFRC.bit.CSFB  	= 0;
		EPwm6Regs.AQCSFRC.bit.CSFA  	= 0;     			//����ǿ�������Ч��
		EPwm6Regs.AQCSFRC.bit.CSFB  	= 0;
		EPwm4Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;			// Set PWM4A
		EPwm4Regs.AQCTLA.bit.CAD 		= AQ_SET;
		EPwm4Regs.AQCTLB.bit.CBU 		= AQ_SET;			// Set PWM4B
		EPwm4Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;
		EPwm6Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;			// Set PWM6A
		EPwm6Regs.AQCTLA.bit.CAD 		= AQ_SET;
		EPwm6Regs.AQCTLB.bit.CBU 		= AQ_SET;			// Set PWM6B
		EPwm6Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;
		break;

    	case EPWM_CH56:
//		EPwm5Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
//		EPwm6Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
		EPwm5Regs.AQCSFRC.bit.CSFA  	= 0;     			//����ǿ�������Ч��
		EPwm5Regs.AQCSFRC.bit.CSFB  	= 0;
		EPwm6Regs.AQCSFRC.bit.CSFA  	= 0;     			//����ǿ�������Ч��
		EPwm6Regs.AQCSFRC.bit.CSFB  	= 0;
		EPwm5Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;			// Set PWM5A
		EPwm5Regs.AQCTLA.bit.CAD 		= AQ_SET;
		EPwm5Regs.AQCTLB.bit.CBU 		= AQ_SET;			// Set PWM5B
		EPwm5Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;
		EPwm6Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;			// Set PWM6A
		EPwm6Regs.AQCTLA.bit.CAD 		= AQ_SET;
		EPwm6Regs.AQCTLB.bit.CBU 		= AQ_SET;			// Set PWM6B
		EPwm6Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;
		break;

    	default:
    	NOP;
	    break; 
   	}   
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void DisablePWM(char channel)
�������ƣ��������Ϊ�ߵ�ƽ�����ǽ�ֹPWM�����
�������ܣ�
��ڲ�����channel---ͨ����	EPWM_CH1	ͨ��1
						......
���ڲ�������
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
#pragma CODE_SECTION(DisablePWM, "ramcode")

void DisablePWM(char channel)
{
	switch(channel)
	{
		case EPWM_CH1:
		EPwm1Regs.AQSFRC.bit.RLDCSF   =3;
		EPwm1Regs.AQCSFRC.bit.CSFA    =2;     //����ǿ������ߣ�
		EPwm1Regs.AQCSFRC.bit.CSFB    =2;
		EPwm1Regs.DBCTL.bit.OUT_MODE  =0;     //��ֹ����
		break;

	    case EPWM_CH2:
	    EPwm2Regs.AQSFRC.bit.RLDCSF   =3;     //˲ʱװ�أ�
   	    EPwm2Regs.AQCSFRC.bit.CSFA    =2;     //����ǿ������ߣ�                 
    	EPwm2Regs.AQCSFRC.bit.CSFB    =2;  
    	EPwm2Regs.DBCTL.bit.OUT_MODE  =0;     //��ֹ����         
		break;

	    case EPWM_CH3:
	    EPwm3Regs.AQSFRC.bit.RLDCSF   =3;     //˲ʱװ�أ�
   	    EPwm3Regs.AQCSFRC.bit.CSFA    =2;     //����ǿ������ߣ�                 
    	EPwm3Regs.AQCSFRC.bit.CSFB    =2;  
    	EPwm3Regs.DBCTL.bit.OUT_MODE  =0;     //��ֹ���� 
		break;

	    case EPWM_CH4:
		EPwm4Regs.AQSFRC.bit.RLDCSF   =3;     //˲ʱװ�أ�
		EPwm4Regs.AQCSFRC.bit.CSFA    =2;     //����ǿ������ߣ�
		EPwm4Regs.AQCSFRC.bit.CSFB    =2;
		EPwm4Regs.DBCTL.bit.OUT_MODE  =0;     //��ֹ����
		break;

	    case EPWM_CH5:
		EPwm5Regs.AQSFRC.bit.RLDCSF   =3;     //˲ʱװ�أ�
		EPwm5Regs.AQCSFRC.bit.CSFA    =2;     //����ǿ������ߣ�
		EPwm5Regs.AQCSFRC.bit.CSFB    =2;
		EPwm5Regs.DBCTL.bit.OUT_MODE  =0;     //��ֹ����
		break;

	    case EPWM_CH6:
		EPwm6Regs.AQSFRC.bit.RLDCSF   =3;     //˲ʱװ�أ�
		EPwm6Regs.AQCSFRC.bit.CSFA    =2;     //����ǿ������ߣ�
		EPwm6Regs.AQCSFRC.bit.CSFB    =2;
		EPwm6Regs.DBCTL.bit.OUT_MODE  =0;     //��ֹ����
		break;

	    case EPWM_CH123:
	    EPwm1Regs.AQSFRC.bit.RLDCSF   =3;  
  	    EPwm1Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�                 
    	EPwm1Regs.AQCSFRC.bit.CSFB    =2;       
	    EPwm1Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    EPwm2Regs.AQSFRC.bit.RLDCSF   =3;  
	    EPwm2Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�                 
    	EPwm2Regs.AQCSFRC.bit.CSFB    =2;      
        EPwm2Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    EPwm3Regs.AQSFRC.bit.RLDCSF   =3;  
        EPwm3Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�                 
    	EPwm3Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm3Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    break;

	    case EPWM_CH46:
	    EPwm4Regs.AQSFRC.bit.RLDCSF   =3;
  	    EPwm4Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�
    	EPwm4Regs.AQCSFRC.bit.CSFB    =2;
	    EPwm4Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    EPwm6Regs.AQSFRC.bit.RLDCSF   =3;
	    EPwm6Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�
    	EPwm6Regs.AQCSFRC.bit.CSFB    =2;
        EPwm6Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    break;

	    case EPWM_CH56:
	    EPwm5Regs.AQSFRC.bit.RLDCSF   =3;
  	    EPwm5Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�
    	EPwm5Regs.AQCSFRC.bit.CSFB    =2;
	    EPwm5Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    EPwm6Regs.AQSFRC.bit.RLDCSF   =3;
	    EPwm6Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�
    	EPwm6Regs.AQCSFRC.bit.CSFB    =2;
        EPwm6Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    break; 
	
	    case EPWM_ALL:
        EPwm1Regs.AQCSFRC.bit.CSFA    =2;      	//����ǿ������ߣ�                 
    	EPwm1Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm1Regs.DBCTL.bit.OUT_MODE  =0;		//��ֹ����
	    EPwm2Regs.AQCSFRC.bit.CSFA    =2;      //����ǿ������ߣ�                 
    	EPwm2Regs.AQCSFRC.bit.CSFB    =2;      
        EPwm2Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
        EPwm3Regs.AQCSFRC.bit.CSFA    =2;      //����ǿ������ߣ�                 
    	EPwm3Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm3Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    EPwm4Regs.AQSFRC.bit.RLDCSF   =3;
        EPwm4Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�
    	EPwm4Regs.AQCSFRC.bit.CSFB    =2;
	    EPwm4Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    EPwm5Regs.AQSFRC.bit.RLDCSF   =3;
        EPwm5Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�
    	EPwm5Regs.AQCSFRC.bit.CSFB    =2;
	    EPwm5Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
	    EPwm6Regs.AQSFRC.bit.RLDCSF   =3;  
        EPwm6Regs.AQCSFRC.bit.CSFA    =2;    //����ǿ������ߣ�                 
    	EPwm6Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm6Regs.DBCTL.bit.OUT_MODE  =0;    //��ֹ����
		break;

	    default:
	    NOP;
	    break;
   	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitTzGpio(void)
�������ƣ���ʼ��TZ�����������źţ�
�������ܣ�Ӳ�����������ΪGPIO12
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2014��06��26��
----------------------------------------------------------------------*/
void InitTzGpio(void)
{
	EALLOW;

// ʹ���ڲ���������TZ���ų�ʼ��Ϊ�ߣ�TZΪ�͵�������
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12 (TZ1)
//	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // Enable pull-up on GPIO13 (TZ2)
//	GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up on GPIO14 (TZ3)
//	GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up on GPIO15 (TZ4)
//	GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;    // Enable pull-up on GPIO16 (TZ5)
//	GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    // Enable pull-up on GPIO17 (TZ6)

// ���������������ƣ�0--��ϵͳʱ��ͬ����1--3�������������ƣ�2--6�������������ƣ�3--��ͬ��������������Ϊ���������
	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // Asynch input GPIO12 (TZ1)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // Asynch input GPIO13 (TZ2)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;  // Asynch input GPIO14 (TZ3)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (TZ4)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  // Asynch input GPIO16 (TZ5)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // Asynch input GPIO17 (TZ6)

// ѡ��TZ��������Դ��һ��ʱ�ӵĵ͵�ƽ���Դ������ϱ���
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;   // Configure GPIO12 as TZ1
	//GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;   // Configure GPIO13 as GPIO
	//GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 as GPIO
	//GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 as GPIO
	//GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;   // Configure GPIO16 as CANTXB
	//GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;   // Configure GPIO17 as CANRXB

   	EDIS;
}