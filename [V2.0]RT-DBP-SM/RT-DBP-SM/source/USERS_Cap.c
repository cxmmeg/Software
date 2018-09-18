// 用户程序
// 更新：2015年02月
//###########################################################################
//
// 文件:	USERS_Cap.c
//
// 标题:	用于捕捉模块初始化等
//
// 描述:	
//				
//########################################################################### 
#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h"

void SetCapChannel(Uint16 cap1, Uint16 cap2, Uint16 cap3, Uint16 cap4, Uint16 cap5, Uint16 cap6);
void SetCap1Mode(void);
void SetCap2Mode(void);

interrupt void ISRCap1(void);
interrupt void ISRCap2(void);

Uint32 t1,t2,t3,t4,t5,t6,t7,t8;
Uint32 T1,T2,T3,T4;

Uint32 Cap_data[1000];
/*---------------------------------------------------------------------
函数原型：void InitCap(void)
函数名称：初始化CAP模块
函数功能：
入口参数：无            
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void InitCap(void)
{
	SetCapChannel(1,1,1,1,1,1);

	EALLOW;  
//	PieVectTable. ECAP1_INT = &ISRCap1;
	PieVectTable. ECAP2_INT = &ISRCap2; 
   	EDIS; 

   	EALLOW;
   	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   	EDIS;

 	SetCap1Mode();
 	SetCap2Mode(); 
	
	IER |= M_INT4;

//    PieCtrlRegs.PIEIER4.bit.INTx1 = 1;			// Enable TINT0 in the PIE: Group 4 interrupt1
    PieCtrlRegs.PIEIER4.bit.INTx2 = 1;

    EINT;   									// Enable Global interrupt INTM
    ERTM;   									// Enable Global realtime interrupt DBGM
}

/*---------------------------------------------------------------------
函数原型：void SetCapChannel(Uint16 cap1, Uint16 cap2, Uint16 cap3, Uint16 cap4, Uint16 cap5, Uint16 cap6)
函数名称：设置ECAP引脚
函数功能：
入口参数：cap1: 1--GPIO5;	2--GPIO24;	3--GPIO34;
		  cap2: 1--GPIO7;	2--GPIO25;	3--GPIO37;
		  cap3: 1--GPIO9;	2--GPIO26;
	  	  cap4:	1--GPIO11;	2--GPIO27;
	  	  cap5:	1--GPIO3;	2--GPIO48;
		  cap6: 1--GPIO1;   2--GPIO49;             
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void SetCapChannel(Uint16 cap1, Uint16 cap2, Uint16 cap3, Uint16 cap4, Uint16 cap5, Uint16 cap6)
{
	EALLOW;
	
//	Init CAP1
	switch(cap1)
	{
		case 1 :		// GPIO5
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK	=	1;		// Enable clock to Cap1
			GpioCtrlRegs.GPAPUD.bit.GPIO5 		= 	0;    	// Enable pull-up on GPIO5 (CAP1)
			GpioCtrlRegs.GPAQSEL1.bit.GPIO5 	= 	0;    	// Synch to SYSCLKOUT GPIO5 (CAP1)
			GpioCtrlRegs.GPAMUX1.bit.GPIO5 		= 	3;   	// Configure GPIO5 as CAP1	
			break;	
		}
		case 2 :		// GPIO24
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK	=	1;		// Enable clock to Cap1
			GpioCtrlRegs.GPAPUD.bit.GPIO24 		= 	0;     	// Enable pull-up on GPIO24 (CAP1)
			GpioCtrlRegs.GPAQSEL2.bit.GPIO24 	= 	0;    	// Synch to SYSCLKOUT GPIO24 (CAP1)
			GpioCtrlRegs.GPAMUX2.bit.GPIO24 	= 	1;   	// Configure GPIO24 as CAP1	
			break;		
		}
		case 3 :		// GPIO34
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK	=	1;		// Enable clock to Cap1
			GpioCtrlRegs.GPBPUD.bit.GPIO34 		= 	0;     	// Enable pull-up on GPIO34 (CAP1)
			GpioCtrlRegs.GPBQSEL1.bit.GPIO34 	= 	0;    	// Synch to SYSCLKOUT GPIO34 (CAP1)
			GpioCtrlRegs.GPBMUX1.bit.GPIO34 	= 	1;   	// Configure GPIO34 as CAP1	
			break;		
		}
	}
	
//	Init CAP2	
	switch(cap2)
	{
		case 1 :		// GPIO7
		{
		  	SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK	=	1;		// Enable clock to Cap2
		  	GpioCtrlRegs.GPAPUD.bit.GPIO7 		= 	0;    	// Enable pull-up on GPIO7 (CAP2)
		  	GpioCtrlRegs.GPAQSEL1.bit.GPIO7 	= 	0;    	// Synch to SYSCLKOUT GPIO7 (CAP2)
		    GpioCtrlRegs.GPAMUX1.bit.GPIO7 		= 	3;   	// Configure GPIO7 as CAP2 	
		    break;	
		}
		case 2 :		// GPIO25
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK	=	1;		// Enable clock to Cap2
			GpioCtrlRegs.GPAPUD.bit.GPIO25 		= 	0;     	// Enable pull-up on GPIO25 (CAP2)
			GpioCtrlRegs.GPAQSEL2.bit.GPIO25 	= 	0;    	// Synch to SYSCLKOUT GPIO25 (CAP2)
			GpioCtrlRegs.GPAMUX2.bit.GPIO25 	= 	1;   	// Configure GPIO25 as CAP2	
			break;		
		}	
		case 3 :		// GPIO37
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK	=	1;		// Enable clock to Cap2
			GpioCtrlRegs.GPBPUD.bit.GPIO37 		= 	0;     	// Enable pull-up on GPIO37 (CAP2)
			GpioCtrlRegs.GPBQSEL1.bit.GPIO37 	= 	0;    	// Synch to SYSCLKOUT GPIO37 (CAP2)
			GpioCtrlRegs.GPBMUX1.bit.GPIO37 	= 	3;   	// Configure GPIO37 as CAP2
			break;			
		}
	}
	
//	Init CAP3
	switch(cap3)
	{
		case 1 :		// GPIO9
		{
		  	SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK	=	1;		// Enable clock to Cap3
		    GpioCtrlRegs.GPAPUD.bit.GPIO9 		= 	0;    	// Enable pull-up on GPIO5 (CAP3)
			GpioCtrlRegs.GPAQSEL1.bit.GPIO9 	= 	0;    	// Synch to SYSCLKOUT GPIO9 (CAP3)
			GpioCtrlRegs.GPAMUX1.bit.GPIO9 		= 	3;   	// Configure GPIO9 as CAP3
			break;		
		}
		case 2 :		// GPIO26
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK	=	1;		// Enable clock to Cap3
		    GpioCtrlRegs.GPAPUD.bit.GPIO26 		= 	0;    	// Enable pull-up on GPIO26 (CAP3)
			GpioCtrlRegs.GPAQSEL2.bit.GPIO26 	= 	0;    	// Synch to SYSCLKOUT GPIO26 (CAP3)
			GpioCtrlRegs.GPAMUX2.bit.GPIO26 	= 	1;   	// Configure GPIO26 as CAP3	
			break;	
		}	
	}
  	
//	Init CAP4
	switch(cap4)
	{
		case 1 :		// GPIO11
		{
		  	SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK	=	1;		// Enable clock to Cap4
		    GpioCtrlRegs.GPAPUD.bit.GPIO11 		= 	0;   	// Enable pull-up on GPIO11 (CAP4)
		 	GpioCtrlRegs.GPAQSEL1.bit.GPIO11 	= 	0; 		// Synch to SYSCLKOUT GPIO11 (CAP4)
			GpioCtrlRegs.GPAMUX1.bit.GPIO11 	= 	3;  	// Configure GPIO11 as CAP4
			break;	
		}
		case 2 :		// GPIO27
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK	=	1;		// Enable clock to Cap4
		    GpioCtrlRegs.GPAPUD.bit.GPIO27 		= 	0;    	// Enable pull-up on GPIO27 (CAP4)
			GpioCtrlRegs.GPAQSEL2.bit.GPIO27 	= 	0;    	// Synch to SYSCLKOUT GPIO27 (CAP4)
			GpioCtrlRegs.GPAMUX2.bit.GPIO27 	= 	1;   	// Configure GPIO27 as CAP4
			break;	
		}	
	}
	
//	Init CAP5
	switch(cap5)
	{
		case 1 :		// GPIO3
		{
		  	SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK	=	1;		// Enable clock to Cap5
		    GpioCtrlRegs.GPAPUD.bit.GPIO3 		= 	0;    	// Enable pull-up on GPIO3 (CAP5)	
			GpioCtrlRegs.GPAQSEL1.bit.GPIO3 	= 	0;  	// Synch to SYSCLKOUT GPIO3 (CAP5)
			GpioCtrlRegs.GPAMUX1.bit.GPIO3 		= 	2;   	// Configure GPIO3 as CAP5
			break;	
		}
		case 2 :		// GPIO48
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK	=	1;		// Enable clock to Cap5
		    GpioCtrlRegs.GPBPUD.bit.GPIO48 		= 	0;    	// Enable pull-up on GPIO48 (CAP5)
			GpioCtrlRegs.GPBQSEL2.bit.GPIO48 	= 	0;    	// Synch to SYSCLKOUT GPIO48 (CAP5)
			GpioCtrlRegs.GPBMUX2.bit.GPIO48 	= 	1;   	// Configure GPIO48 as CAP5
			break;		
		}	
	}
	
//	Init CAP6
	switch(cap6)
	{
		case 1 :		// GPIO1
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK	=	1;		// Enable clock to Cap6
		    GpioCtrlRegs.GPAPUD.bit.GPIO1 		= 	0;    	// Enable pull-up on GPIO1 (CAP6) 	
			GpioCtrlRegs.GPAQSEL1.bit.GPIO1 	= 	0;  	// Synch to SYSCLKOUT GPIO1 (CAP6)
			GpioCtrlRegs.GPAMUX1.bit.GPIO1 		= 	2;   	// Configure GPIO1 as CAP6
			break;	
		}
		case 2 :		// GPIO49
		{
			SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK	=	1;		// Enable clock to Cap6
		    GpioCtrlRegs.GPBPUD.bit.GPIO49 		= 	0;    	// Enable pull-up on GPIO49 (CAP6)
			GpioCtrlRegs.GPBQSEL2.bit.GPIO49 	= 	0;    	// Synch to SYSCLKOUT GPIO49 (CAP6)
			GpioCtrlRegs.GPBMUX2.bit.GPIO49 	= 	1;   	// Configure GPIO49 as CAP6
			break;	
		}	
	}

	EDIS;
}

/*---------------------------------------------------------------------
函数原型：void SetCap1Mode(void)
函数名称：设置CAP1工作模式
函数功能：
入口参数：无                    
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void SetCap1Mode(void)
{
	ECap1Regs.ECCTL1.bit.CAP1POL 		= 	EC_RISING;				
	ECap1Regs.ECCTL1.bit.CAP2POL 		= 	EC_RISING;
	ECap1Regs.ECCTL1.bit.CAP3POL 		= 	EC_RISING;
	ECap1Regs.ECCTL1.bit.CAP4POL 		= 	EC_RISING;
	ECap1Regs.ECCTL1.bit.CTRRST1		= 	EC_ABS_MODE;			
	ECap1Regs.ECCTL1.bit.CTRRST2 		= 	EC_ABS_MODE;
	ECap1Regs.ECCTL1.bit.CTRRST3 		= 	EC_ABS_MODE;
	ECap1Regs.ECCTL1.bit.CTRRST4 		= 	EC_ABS_MODE;
	ECap1Regs.ECCTL1.bit.CAPLDEN 		= 	EC_ENABLE;					
	ECap1Regs.ECCTL1.bit.PRESCALE 		= 	EC_DIV1;				
	ECap1Regs.ECCTL2.bit.CAP_APWM 		= 	EC_CAP_MODE;
	ECap1Regs.ECCTL2.bit.CONT_ONESHT 	= 	EC_CONTINUOUS;
	ECap1Regs.ECCTL2.bit.SYNCO_SEL 		= 	EC_SYNCO_DIS;
	ECap1Regs.ECCTL2.bit.SYNCI_EN 		= 	EC_SYNCI_DIS;
	ECap1Regs.ECEINT.all				=	0x0000;					//stop all interrupt
	ECap1Regs.ECCLR.all					=	0xFFFF;					//clear all flag
	ECap1Regs.ECCTL2.bit.TSCTRSTOP 		= 	EC_RUN;					// 启动
	ECap1Regs.ECEINT.bit.CEVT4			=	1;						// Enable cevt4 interrupt
}

/*---------------------------------------------------------------------
函数原型：void SetCap2Mode(void)
函数名称：设置CAP2工作模式
函数功能：
入口参数：无                    
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void SetCap2Mode(void)
{
	ECap2Regs.ECCTL1.bit.CAP1POL = EC_FALLING;
	ECap2Regs.ECCTL1.bit.CAP2POL = EC_FALLING;
	ECap2Regs.ECCTL1.bit.CAP3POL = EC_FALLING;
	ECap2Regs.ECCTL1.bit.CAP4POL = EC_FALLING;
	ECap2Regs.ECCTL1.bit.CTRRST1 = EC_ABS_MODE;
	ECap2Regs.ECCTL1.bit.CTRRST2 = EC_ABS_MODE;
	ECap2Regs.ECCTL1.bit.CTRRST3 = EC_ABS_MODE;
	ECap2Regs.ECCTL1.bit.CTRRST4 = EC_ABS_MODE;
	ECap2Regs.ECCTL1.bit.CAPLDEN = EC_ENABLE;
	ECap2Regs.ECCTL1.bit.PRESCALE = EC_DIV1;
	ECap2Regs.ECCTL2.bit.CAP_APWM = EC_CAP_MODE;
	ECap2Regs.ECCTL2.bit.CONT_ONESHT = EC_CONTINUOUS;
	ECap2Regs.ECCTL2.bit.SYNCO_SEL = EC_SYNCO_DIS;
	ECap2Regs.ECCTL2.bit.SYNCI_EN = EC_DISABLE;
	ECap2Regs.ECEINT.all=0x0000;//stop all interrupt
	ECap2Regs.ECCLR.all=0xFFFF;//clare all flag
	ECap2Regs.ECCTL2.bit.TSCTRSTOP = EC_RUN;// 启动
	ECap2Regs.ECEINT.bit.CEVT4=1;// Enable cevt4 interrupt
}

/*---------------------------------------------------------------------
函数原型：interrupt void ISRCap1(void)
函数名称：CAP中断处理函数
函数功能：
入口参数：无                    
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
interrupt void ISRCap1(void)
{
   // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
    ECap1Regs.ECCLR.all=0xFFFF;//clare all flag
	t1= ECap1Regs.CAP1;
	t2= ECap1Regs.CAP2;
	t3= ECap1Regs.CAP3;
  	t4= ECap1Regs.CAP4;  
    T1=t2-t1;T2=t4-t3;
}

/*---------------------------------------------------------------------
函数原型：interrupt void ISRCap1(void)
函数名称：CAP中断处理函数
函数功能：
入口参数：无                    
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
interrupt void ISRCap2(void)
{
	static Uint16 i = 0;
   // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
    ECap2Regs.ECCLR.all=0xFFFF;//clare all flag
	t5= ECap2Regs.CAP1;
	t6= ECap2Regs.CAP2;
	t7= ECap2Regs.CAP3;
  	t8= ECap2Regs.CAP4;  
    T3=t6-t5;T4=t8-t7;

	Cap_data[i] = T3;
	i++;
	if(i>1000)	i=0;
}
