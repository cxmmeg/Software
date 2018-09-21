/*****************头文件********************/
#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h"

extern Uint16 PDPData;
extern interrupt void ePWM1ISR(void);

/*---------------------------------------------------------------------
函数原型：void InitEPWM(void)
函数名称：初始化EPWM模块
函数功能：
入口参数：无                    
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void InitEPWM(void)
{	
	InitTzGpio();            //设置TZ1;
	DisablePWM(EPWM_ALL);

	EALLOW;
   	PieVectTable.EPWM1_INT = &ePWM1ISR;  //设置PWM中断
	PieVectTable.EPWM1_TZINT = &TZ1_ISR;  //设置PDP的tz中断；
   	EDIS; 

   	EALLOW;
   	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   	EDIS;

   	InitEPwm1(6400);
//	InitEPwm2(6400);
//	InitEPwm3(6400);
//	InitPWM4(6400);
//	InitEPwm56(12800);

   	EALLOW;
   	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   //启动PWM脉冲PWM2，PWM3和PWM1同步；
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
函数原型：void EnableTZ(void)
函数名称：使能TZ1
函数功能：
入口参数：无                    
出口参数：无
作    者：
日    期：
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
函数原型：void Disable_TZ(void)
函数名称：禁止TZ1
函数功能：
入口参数：无                    
出口参数：无
作    者：
日    期：
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
函数原型：void InitEPwm1(int freq)
函数名称：epwm1初始化
函数功能：
入口参数：freq---开关频率                    
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void InitEPwm1(int freq)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO0	= 1; // PWM1
	GpioCtrlRegs.GPAMUX1.bit.GPIO1	= 0; // PWM2
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
   	EPwm1Regs.CMPA.half.CMPA 		= (Uint32)EPwm1Regs.TBPRD * 10 / 100;

   // Active Low PWMs - Setup Deadband
  	EPwm1Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
   	EPwm1Regs.DBCTL.bit.POLSEL 		= DB_ACTV_LOC;
   	EPwm1Regs.DBRED 				= 200;
   	EPwm1Regs.DBFED 				= 200;

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
函数原型：void InitEPwm2(int freq)
函数名称：epwm2初始化
函数功能：
入口参数：freq---开关频率                     
出口参数：无
作    者：
日    期：
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
    // 	EPwm2Regs.DBCTL.bit.IN_MODE 	= DBA_ALL;
   	EPwm2Regs.DBRED 				= 200;
   	EPwm2Regs.DBFED 				= 200;

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
函数原型：void InitEPwm3(int freq)
函名称：epwm3初始化
函数功能：
入口参int freq---开关频率                     
出口参数：无
作    者：
日    期：
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
   
   	EPwm3Regs.DBRED 				= 200;
   	EPwm3Regs.DBFED 				= 200;

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



void InitPWM4(void)
{   EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Enable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Enable pull-up on GPIO7 (EPWM4B)

    //上管制动IGBT；
 	GpioDataRegs.GPADAT.bit.GPIO6	    = 1;   //Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO6      = 0;   //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO6       = 1;   //Setup the direction is outport;

	// 下管制动IGBT；
	GpioDataRegs.GPADAT.bit.GPIO7	    = 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO7      = 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO7    	= 1;    //Setup the direction is outport;
    EDIS;
}

/*---------------------------------------------------------------------
函数原型：void InitEPwm1(int freq)
函数名称：epwm1初始化
函数功能：
入口参数：freq---开关频率                    
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void InitEPwm56(int freq)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO8	= 1; // PWM9 
	GpioCtrlRegs.GPAMUX1.bit.GPIO9	= 1; // PWM10
    GpioCtrlRegs.GPAMUX1.bit.GPIO10	= 1; // PWM11 
	GpioCtrlRegs.GPAMUX1.bit.GPIO11	= 1; // PWM12

    EDIS;
   	
    EPwm5Regs.AQCTLA.bit.CAU        = AQ_CLEAR;              
    EPwm5Regs.AQCTLA.bit.CAD        = AQ_SET ;
   
    EPwm6Regs.AQCTLA.bit.CAU        = AQ_CLEAR;              
    EPwm6Regs.AQCTLA.bit.CAD        = AQ_SET ;


   	EPwm5Regs.TBPRD 			    = 37500000/freq;        	// TBCLK counts
	EPwm5Regs.TBPHS.half.TBPHS	    = 0; 			        	// Set Phase register to zero
   	EPwm5Regs.TBCTR 			    = 0;			        	// Clear counter
  

   	EPwm6Regs.TBPRD 			    = 37500000/freq;        	// TBCLK counts
	EPwm6Regs.TBPHS.half.TBPHS	    = EPwm6Regs.TBPRD; 		       	// Set Phase register to 180
   	EPwm6Regs.TBCTR 			    = 0;  	   	// Clear counter

   // Setup TBCLK
   	EPwm5Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 		// Count up down
   	EPwm5Regs.TBCTL.bit.PHSEN 		= TB_DISABLE;        	// Master module
   	EPwm5Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;       		// Clock ratio to SYSCLKOUT
   	EPwm5Regs.TBCTL.bit.CLKDIV 		= TB_DIV2;
	EPwm5Regs.TBCTL.bit.PRDLD 		= TB_SHADOW;
	EPwm5Regs.TBCTL.bit.SYNCOSEL 	= TB_CTR_ZERO; 			// Sync down-stream module

 	EPwm6Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 		// Count up down
   	EPwm6Regs.TBCTL.bit.PHSEN 		= TB_ENABLE;        	// Master module
   	EPwm6Regs.TBCTL.bit.HSPCLKDIV 	= TB_DIV1;       		// Clock ratio to SYSCLKOUT
   	EPwm6Regs.TBCTL.bit.CLKDIV 		= TB_DIV2;
	EPwm6Regs.TBCTL.bit.PRDLD 		= TB_SHADOW;
	EPwm6Regs.TBCTL.bit.SYNCOSEL 	= TB_CTR_ZERO; 			// Sync down-stream module

   	EPwm5Regs.CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;    		// Load registers every ZERO
   	EPwm5Regs.CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;
   	EPwm5Regs.CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO;
   	EPwm5Regs.CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO;

	EPwm6Regs.CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;    		// Load registers every ZERO
   	EPwm6Regs.CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;
   	EPwm6Regs.CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO;
   	EPwm6Regs.CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO;

   // Setup compare
   	EPwm5Regs.CMPA.half.CMPA 		=0;

   // Active Low PWMs - Setup Deadband
  	EPwm5Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
   	EPwm5Regs.DBCTL.bit.POLSEL 		= DB_ACTV_LOC;   //DB_ACTV_HIC; 
   	EPwm5Regs.DBRED 				= 150;//160;
   	EPwm5Regs.DBFED 				= 150;//160;


	EPwm6Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
   	EPwm6Regs.DBCTL.bit.POLSEL 		= DB_ACTV_LOC;   //DB_ACTV_HIC; 
   	EPwm6Regs.DBRED 				= 150;//160;
   	EPwm6Regs.DBFED 				= 150;//160;


	//PWM-chopping Disable
	EPwm5Regs.PCCTL.bit.CHPEN		= 0;
    EPwm6Regs.PCCTL.bit.CHPEN		= 0;

	//PWM-Trip-Zone Submodule
	EALLOW;
	EPwm5Regs.TZSEL.bit.OSHT1		= 1;	//CBC Trip1
	EPwm5Regs.TZCTL.bit.TZA			= 1;	//Force High
	EPwm5Regs.TZCTL.bit.TZB			= 1;

    EPwm6Regs.TZSEL.bit.OSHT1		= 1;	//CBC Trip1
	EPwm6Regs.TZCTL.bit.TZA			= 1;	//Force High
	EPwm6Regs.TZCTL.bit.TZB			= 1;
	EDIS;	

   // Interrupt where we will change the Deadband
   	EPwm5Regs.ETSEL.bit.INTEN 		= 0;               // Disable INT
 	EPwm6Regs.ETSEL.bit.INTEN 		= 0;               // Disable INT
 }





/*---------------------------------------------------------------------
函数原型：void EnablePWM(char channel)
函数名称：打开PWM
函数功能：
入口参数：channel---通道号	1:	A相  
						2:	B相
						4: 	C相;
						7: 	A,B,C相；
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void EnablePWM(char channel)  
{
    switch(channel)
	{
       case  1:
        EPwm1Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	    EPwm1Regs.AQCSFRC.bit.CSFA      = 0;     			//软件强制输出无效；                 
    	EPwm1Regs.AQCSFRC.bit.CSFB      = 0;      
	 	EPwm1Regs.AQCTLA.bit.CAU 		= AQ_SET;          // Set PWM1A
   	    EPwm1Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;        // Set PWM1B  
        break;
	   case 2:
	    EPwm2Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	    EPwm2Regs.AQCSFRC.bit.CSFA      = 0;     			//软件强制输出无效；                 
    	EPwm2Regs.AQCSFRC.bit.CSFB      = 0;   
	   	EPwm2Regs.AQCTLA.bit.CAU 		= AQ_SET;          	// Set PWM2A
   	    EPwm2Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;        	// Set PWM2B  
		break;
	   case 4:
	    EPwm3Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	    EPwm3Regs.AQCSFRC.bit.CSFA      = 0;     			//软件强制输出无效；                 
    	EPwm3Regs.AQCSFRC.bit.CSFB      = 0;   
	   	EPwm3Regs.AQCTLA.bit.CAU 		= AQ_SET;          	// Set PWM3A
   	    EPwm3Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;        	// Set PWM3B  
		break;
	   case  EPWM_ALL: 
	    EPwm1Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
        EPwm2Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
        EPwm3Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	
        EPwm5Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
        EPwm6Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	
		EPwm1Regs.AQCSFRC.bit.CSFA  = 0;     				//软件强制输出无效；                 
    	EPwm1Regs.AQCSFRC.bit.CSFB  = 0;   
		EPwm2Regs.AQCSFRC.bit.CSFA  = 0;     				//软件强制输出无效；                 
    	EPwm2Regs.AQCSFRC.bit.CSFB  = 0;   
		EPwm3Regs.AQCSFRC.bit.CSFA  = 0;     				//软件强制输出无效；                 
    	EPwm3Regs.AQCSFRC.bit.CSFB  = 0;   
		
		EPwm5Regs.AQCSFRC.bit.CSFA  = 0;     				//软件强制输出无效；                 
    	EPwm5Regs.AQCSFRC.bit.CSFB  = 0;   
		EPwm6Regs.AQCSFRC.bit.CSFA  = 0;     				//软件强制输出无效；                 
    	EPwm6Regs.AQCSFRC.bit.CSFB  = 0;   
		   	  
	    break; 
	   case 15:
	    EPwm1Regs.AQSFRC.bit.RLDCSF   =3;  
  	    EPwm1Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm1Regs.AQCSFRC.bit.CSFB    =2;       
	    EPwm1Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区

	    EPwm2Regs.AQSFRC.bit.RLDCSF   =3;  
	    EPwm2Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm2Regs.AQCSFRC.bit.CSFB    =2;      
        EPwm2Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区

	    EPwm3Regs.AQSFRC.bit.RLDCSF   =3;  
        EPwm3Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm3Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm3Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区
	   
	
        EPwm5Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
        EPwm6Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
		  
		EPwm5Regs.AQCSFRC.bit.CSFA  = 0;     //软件强制输出无效；                 
    	EPwm5Regs.AQCSFRC.bit.CSFB  = 0;   

    	EPwm6Regs.AQCSFRC.bit.CSFA  = 0;     //软件强制输出无效；
    	EPwm6Regs.AQCSFRC.bit.CSFB  = 0; 
		break;
       default:
	    EPwm1Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
        EPwm2Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
        EPwm3Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	
        EPwm5Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
        EPwm6Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	
		EPwm1Regs.AQCSFRC.bit.CSFA  = 0;     //软件强制输出无效；                 
    	EPwm1Regs.AQCSFRC.bit.CSFB  = 0;   
		EPwm2Regs.AQCSFRC.bit.CSFA  = 0;     //软件强制输出无效；                 
    	EPwm2Regs.AQCSFRC.bit.CSFB  = 0;   
		EPwm3Regs.AQCSFRC.bit.CSFA  = 0;     //软件强制输出无效；                 
    	EPwm3Regs.AQCSFRC.bit.CSFB  = 0;   

    	EPwm5Regs.AQCSFRC.bit.CSFA  = 0;     //软件强制输出无效；
    	EPwm5Regs.AQCSFRC.bit.CSFB  = 0;   
		EPwm6Regs.AQCSFRC.bit.CSFA  = 0;     //软件强制输出无效；                 
    	EPwm6Regs.AQCSFRC.bit.CSFB  = 0;   	
	
	    break; 
   	}   
}

/*---------------------------------------------------------------------
函数原型：void DisablePWM(char channel)
函数名称：设置输出为高电平：就是禁止PWM输出；
函数功能：
入口参数：channel---通道号	1:	A相
						2:	B相
						4: 	C相;
						7: 	A,B,C相；
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void DisablePWM(char channel)
{
	switch(channel)
	{
       case  1:    //软件设置强制输出高；
   	    EPwm1Regs.AQSFRC.bit.RLDCSF   =3;     
   	    EPwm1Regs.AQCSFRC.bit.CSFA    =2;     //软件强制输出高；                 
    	EPwm1Regs.AQCSFRC.bit.CSFB    =2; 
    	EPwm1Regs.DBCTL.bit.OUT_MODE  =0;     //禁止死区
	    break;
	   case 2:
	    EPwm2Regs.AQSFRC.bit.RLDCSF   =3;     //瞬时装载；
   	    EPwm2Regs.AQCSFRC.bit.CSFA    =2;     //软件强制输出高；                 
    	EPwm2Regs.AQCSFRC.bit.CSFB    =2;  
    	EPwm2Regs.DBCTL.bit.OUT_MODE  =0;     //禁止死区         
		break;

	   case 4:
	    EPwm3Regs.AQSFRC.bit.RLDCSF   =3;     //瞬时装载；
   	    EPwm3Regs.AQCSFRC.bit.CSFA    =2;     //软件强制输出高；                 
    	EPwm3Regs.AQCSFRC.bit.CSFB    =2;  
    	EPwm3Regs.DBCTL.bit.OUT_MODE  =0;     //禁止死区 
		break;
	   case  EPWM_ALL:
	    EPwm1Regs.AQSFRC.bit.RLDCSF   =3;  
  	    EPwm1Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm1Regs.AQCSFRC.bit.CSFB    =2;       
	    EPwm1Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区

	    EPwm2Regs.AQSFRC.bit.RLDCSF   =3;  
	    EPwm2Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm2Regs.AQCSFRC.bit.CSFB    =2;      
        EPwm2Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区

	    EPwm3Regs.AQSFRC.bit.RLDCSF   =3;  
        EPwm3Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm3Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm3Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区
	 
	    EPwm5Regs.AQSFRC.bit.RLDCSF   =3;  
        EPwm5Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm5Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm5Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区

	    EPwm6Regs.AQSFRC.bit.RLDCSF   =3;  
        EPwm6Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm6Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm6Regs.DBCTL.bit.OUT_MODE  =0;    	//禁止死区
	    break; 
	
	  default:
	
        EPwm1Regs.AQCSFRC.bit.CSFA    =2;      	//软件强制输出高；                 
    	EPwm1Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm1Regs.DBCTL.bit.OUT_MODE  =0;		//禁止死区

	    EPwm2Regs.AQCSFRC.bit.CSFA    =2;      //软件强制输出高；                 
    	EPwm2Regs.AQCSFRC.bit.CSFB    =2;      
        EPwm2Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区

        EPwm3Regs.AQCSFRC.bit.CSFA    =2;      //软件强制输出高；                 
    	EPwm3Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm3Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区
	
	    EPwm5Regs.AQSFRC.bit.RLDCSF   =3;  
        EPwm5Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm5Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm5Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区

	    EPwm6Regs.AQSFRC.bit.RLDCSF   =3;  
        EPwm6Regs.AQCSFRC.bit.CSFA    =2;    //软件强制输出高；                 
    	EPwm6Regs.AQCSFRC.bit.CSFB    =2;      
	    EPwm6Regs.DBCTL.bit.OUT_MODE  =0;    //禁止死区
		break;
   	}
}

/*---------------------------------------------------------------------
函数原型：void InitTzGpio(void)
函数名称：初始化TZ（错误区域信号）
函数功能：硬件故障输入口为GPIO12
入口参数：无
出口参数：无
作    者：
日    期：2014年06月26日
----------------------------------------------------------------------*/
void InitTzGpio(void)
{
	EALLOW;

// 使能内部上拉，将TZ引脚初始化为高，TZ为低点评触发
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12 (TZ1)
//	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // Enable pull-up on GPIO13 (TZ2)
//	GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up on GPIO14 (TZ3)
//	GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up on GPIO15 (TZ4)
//	GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;    // Enable pull-up on GPIO16 (TZ5)
//	GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    // Enable pull-up on GPIO17 (TZ6)

// 配置引脚输入限制，0--与系统时钟同步；1--3个采样周期限制；2--6个采样周期限制；3--不同步，仅用于配置为外设的引脚
	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // Asynch input GPIO12 (TZ1)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // Asynch input GPIO13 (TZ2)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;  // Asynch input GPIO14 (TZ3)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (TZ4)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  // Asynch input GPIO16 (TZ5)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // Asynch input GPIO17 (TZ6)

// 选择TZ故障输入源，一个时钟的低电平足以触发故障保护
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;   // Configure GPIO12 as TZ1
	//GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;   // Configure GPIO13 as GPIO
	//GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 as GPIO
	//GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 as GPIO
	//GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;   // Configure GPIO16 as CANTXB
	//GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;   // Configure GPIO17 as CANRXB

   	EDIS;
}
