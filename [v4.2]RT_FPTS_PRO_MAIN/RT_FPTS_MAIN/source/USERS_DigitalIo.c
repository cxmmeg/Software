// ���س�ʼ���ļ�
// ���£�2017-09 -- RT-FPTS-MAIN-V4.2
//###########################################################################
//
// �ļ�:	DSP2833x_Gpio.c
//
// ����:	���ڳ�ʼ��������IO��
//
// ����:	����  1. XINTF��ʼ��
//		    2. ����IO�ӿڳ�ʼ��
//			3. IO����/�������

// �ӿ�:	IN1_DSP��GPIO27
//		IN2_DSP��GPIO26
//		IN3_DSP��GPIO24
//		IN4_DSP��GPIO10
//		IN5_DSP��GPIO8
//		IN6_DSP��GPIO6
//		IN7_DSP��GPIO52
//		IN8_DSP��GPIO49

//		OUT1_DSP��GPIO53
//		OUT2_DSP��GPIO50
//		OUT3_DSP��GPIO4
//		OUT4_DSP��GPIO2
//		IO_RESET��GPIO9

//		LED1: GPIO1
//		LED2: GPIO3
// 		LED3: GPIO5
//		LED4: GPIO51
//		LED5: GPIO48
//		LED6: GPIO7

//		NO1_ACS: GPIO32
//		NO1_BCS: GPIO33
//		NO1_CCS: GPIO21
//		NO2_ACS: GPIO59
//		NO2_BCS: GPIO54
//		NO2_CCS: GPIO56

//		D00: GPIO20
//		D01: GPIO23
//		D02: GPIO55
//		D03: GPIO13

//		SCI_CONB: GPIO37
//		SCI_CONC: GPIO58
//###########################################################################
#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File

struct  COUNT_GPIOIN_BUF_UP  	CountGpioInUp; 		// GPIO�ɿ���ʱ�����ṹ�����
struct	COUNT_GPIOIN_BUF_DOWN	CountGpioInDown;		// GPIO������ʱ�����ṹ�����

extern union  	FLAG_GPIOIN   	FlagGpioIn;				// GPIO��־λ�ṹ��λ����
extern union	FLAG_SYSTEM		FlagSystem;
extern struct 	COUNT_SYSTEM	CountSystem;
extern struct   PWMISR_REGS     PwmIsrReg;

void InitXint(void);
interrupt void XINT1ISR(void);

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitGpio(void)
�������ƣ�����IO�������
�������ܣ�
��ڲ�������                  
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
void InitGpio(void)
{
	Uint16 i;
	Uint16 *pi = (void *)&CountGpioInUp;
	Uint16 *pj = (void *)&CountGpioInDown;
	EALLOW;

/*---------------------------��������˿�-----------------------------*/
	//	OUT1_DSP��GPIO53
	GpioDataRegs.GPBDAT.bit.GPIO53		= 1;   //Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO53		= 0;   //setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO53		= 1;   //Setup the direction is outport;

	//	OUT2_DSP��GPIO50
	GpioDataRegs.GPBDAT.bit.GPIO50	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO50    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO50     	= 1;    //Setup the direction is outport;

	//	OUT3_DSP��GPIO4
	GpioDataRegs.GPADAT.bit.GPIO4	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO4    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO4     	= 1;    //Setup the direction is outport;

	//	OUT4_DSP��GPIO2
	GpioDataRegs.GPADAT.bit.GPIO2	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO2    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO2     	= 1;    //Setup the direction is outport;

	//	IO_RESET��GPIO9
	GpioDataRegs.GPADAT.bit.GPIO9	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO9    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO9    	= 1;    //Setup the direction is outport;

	//	LED1: GPIO1
	GpioDataRegs.GPADAT.bit.GPIO1		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 		= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO1		= 1;	//Setup the direction is outport;

	//	LED2: GPIO3
	GpioDataRegs.GPADAT.bit.GPIO3		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 		= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO3		= 1;	//Setup the direction is outport;

	// 	LED3: GPIO5
	GpioDataRegs.GPADAT.bit.GPIO5		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 		= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO5		= 1;	//Setup the direction is outport;

	//	LED4: GPIO51
	GpioDataRegs.GPBDAT.bit.GPIO51		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO51		= 1;	//Setup the direction is outport;

	//	LED5: GPIO48
	GpioDataRegs.GPBDAT.bit.GPIO48		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO48 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO48		= 1;	//Setup the direction is outport;

	//	LED6: GPIO7
	GpioDataRegs.GPADAT.bit.GPIO7		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO7 		= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO7		= 1;	//Setup the direction is outport;

	//	D00: GPIO20
	GpioDataRegs.GPADAT.bit.GPIO20		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO20		= 1;	//Setup the direction is outport;

//	//	D01: GPIO23
//	GpioDataRegs.GPADAT.bit.GPIO23		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
//	GpioCtrlRegs.GPAMUX2.bit.GPIO23 	= 0;	//setup the 	function is GPIO;
//	GpioCtrlRegs.GPADIR.bit.GPIO23		= 1;	//Setup the direction is outport;

	//	D02: GPIO55
	GpioDataRegs.GPBDAT.bit.GPIO55		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO55		= 1;	//Setup the direction is outport;

	//	D03: GPIO13
	GpioDataRegs.GPADAT.bit.GPIO13		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO13		= 1;	//Setup the direction is outport;

	//	SCI_CONB:  GPIO37
	GpioDataRegs.GPBDAT.bit.GPIO37		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX1.bit.GPIO37 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO37		= 1;	//Setup the direction is outport;

	//	SCI_CONC:  GPIO58
	GpioDataRegs.GPBDAT.bit.GPIO58		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO58		= 1;	//Setup the direction is outport;

/*---------------------------��������˿�-----------------------------*/
   //	IN1_DSP��GPIO27
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;  		//setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO27	= 0;  		//setup the direction is input;

	//	IN2_DSP��GPIO26
	GpioCtrlRegs.GPAMUX2.bit.GPIO26	= 0;  		//setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO26	= 0;  		//setup the direction is input;

	//	IN3_DSP��GPIO24
	GpioCtrlRegs.GPAMUX2.bit.GPIO24	= 0;  //setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO24	= 0;  //setup the direction is input;

	//	IN4_DSP��GPIO10
	GpioCtrlRegs.GPAMUX1.bit.GPIO10	= 0;  //setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO10	= 0;  //setup the direction is input;

	//	IN5_DSP��GPIO8
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  //setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO8	= 0;  //setup the direction is input;

	//	IN6_DSP��GPIO6
	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;  //setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO6	= 0;  //setup the direction is input;

	//	IN7_DSP��GPIO52
	GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;  //setup the function is GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO52	= 0;  //setup the direction is input;

	//	IN8_DSP��GPIO49
	GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;  //setup the function is GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO49  = 0;  //setup the direction is input;

	EDIS;

	for(i = 0; i < 8; i++)
	{
		*pi = 0;
		*pj = 0;

		pi += 1;
		pj += 1;
	}
	FlagGpioIn.all = 0;
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitXint(void)
�������ƣ��ⲿ�жϳ�ʼ��
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
void InitXint(void)
{
   	EALLOW;

	// Make D01: GPIO23 the input source for Xint3
	GpioCtrlRegs.GPAQSEL2.bit.GPIO23= 0;   	// 6 samples
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;  	//setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO23	= 0;  	//setup the direction is input;
	GpioIntRegs.GPIOXINT1SEL.all 	= 23;   // Xint1 connected to GPIO23
	EDIS;

	XIntruptRegs.XINT1CR.bit.POLARITY = 1;	// �����ز����ж�
 	XIntruptRegs.XINT1CR.bit.ENABLE = 1;

	EALLOW;
	PieVectTable.XINT1 = &XINT1ISR;
	EDIS;

    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;		// ʹ��PIE�ڵ� XINT1_INT�ж�
    IER |= M_INT1;							// ʹ�� CPU INT1
//  EINT;   								// ȫ���ж�ʹ��INTM
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void InitSCRGpio(void)
�������ƣ���ʼ���ɿع��������
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
void InitSCRGpio(void)
{
	EALLOW;

	//	NO1_A: GPIO32
	GpioDataRegs.GPBDAT.bit.GPIO32		= 1;   	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX1.bit.GPIO32		= 0;   	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO32		= 1;   	//Setup the direction is outport;

	//	NO1_B: GPIO33
	GpioDataRegs.GPBDAT.bit.GPIO33		= 1;		//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX1.bit.GPIO33		= 0;    	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO33		= 1;    	//Setup the direction is outport;

	//	NO1_C: GPIO21
	GpioDataRegs.GPADAT.bit.GPIO21		= 1;		//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX2.bit.GPIO21		= 0;    	//setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO21		= 1;    	//Setup the direction is outport;

	//	NO2_A: GPIO59
	GpioDataRegs.GPBDAT.bit.GPIO59		= 1;		//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO59		= 0;    	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO59		= 1;    	//Setup the direction is outport;

	//	NO2_B: GPIO50
	GpioDataRegs.GPBDAT.bit.GPIO54		= 1;		//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO54		= 0;    	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO54		= 1;    	//Setup the direction is outport;

	//	NO2_C: GPIO5
	GpioDataRegs.GPBDAT.bit.GPIO56		= 1;		//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO56		= 0;    	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO56		= 1;    	//Setup the direction is outport;

	EDIS;
	CtrlSCRDisable(All);
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void ReadGPIOIn1~8(void)
�������ƣ����GPIOIN1~8����
�������ܣ�
��ڲ�������                  
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
void ReadGPIOIn1(void)
{
	if(!GpioDataRegs.GPADAT.bit.GPIO27)
	{
		CountGpioInUp.GpioIn1 = 0;
		CountGpioInDown.GpioIn1++;
		if(CountGpioInDown.GpioIn1 > ERROR_GPIOIN)
		{
			CountGpioInDown.GpioIn1 = ERROR_GPIOIN;
			FlagGpioIn.bit.GpioIn1 = 1;
		}
	}
	else
	{
		CountGpioInDown.GpioIn1 = 0;
		CountGpioInUp.GpioIn1++;
		if(CountGpioInUp.GpioIn1 > ERROR_GPIOIN)
		{
			CountGpioInUp.GpioIn1 = 0;
			FlagGpioIn.bit.GpioIn1 = 0;
		}
	}
}

Uint16 ReadGPIOIn2(void)
{
	if(!GpioDataRegs.GPADAT.bit.GPIO26)
	{
		CountGpioInUp.GpioIn2 = 0;
		CountGpioInDown.GpioIn2++;
		if(CountGpioInDown.GpioIn2 > ERROR_GPIOIN)
		{
			CountGpioInDown.GpioIn2 = ERROR_GPIOIN;
			FlagGpioIn.bit.GpioIn2 = 1;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		CountGpioInDown.GpioIn2 = 0;
		CountGpioInUp.GpioIn2++;
		if(CountGpioInUp.GpioIn2 > ERROR_GPIOIN)
		{
			CountGpioInUp.GpioIn2 = 0;
			FlagGpioIn.bit.GpioIn2 = 0;
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void ReadGPIOIn3(void)
{
	if(!GpioDataRegs.GPADAT.bit.GPIO24)
	{
		CountGpioInUp.GpioIn3 = 0;
		CountGpioInDown.GpioIn3++;
		if(CountGpioInDown.GpioIn3 > ERROR_GPIOIN)
		{
			CountGpioInDown.GpioIn3 = ERROR_GPIOIN;
			FlagGpioIn.bit.GpioIn3 = 1;
		}
	}
	else
	{
		CountGpioInDown.GpioIn3 = 0;
		CountGpioInUp.GpioIn3++;
		if(CountGpioInUp.GpioIn3 > ERROR_GPIOIN)
		{
			CountGpioInUp.GpioIn3 = 0;
			FlagGpioIn.bit.GpioIn3 = 0;
		}
	}
}

void ReadGPIOIn4(void)
{
	if(!GpioDataRegs.GPADAT.bit.GPIO10)
	{
		CountGpioInUp.GpioIn4 = 0;
		CountGpioInDown.GpioIn4++;
		if(CountGpioInDown.GpioIn4 > ERROR_GPIOIN)
		{
			CountGpioInDown.GpioIn4 = ERROR_GPIOIN;
			FlagGpioIn.bit.GpioIn4 = 1;
		}
	}
	else
	{
		CountGpioInDown.GpioIn4 = 0;
		CountGpioInUp.GpioIn4++;
		if(CountGpioInUp.GpioIn4 > ERROR_GPIOIN)
		{
			CountGpioInUp.GpioIn4 = 0;
			FlagGpioIn.bit.GpioIn4 = 0;
		}
	}
}

Uint16 ReadGPIOIn5(void)
{
	if(!GpioDataRegs.GPADAT.bit.GPIO8)
	{
		CountGpioInUp.GpioIn5 = 0;
		CountGpioInDown.GpioIn5++;
		if(CountGpioInDown.GpioIn5 > ERROR_GPIOIN)
		{
			CountGpioInDown.GpioIn5 = ERROR_GPIOIN;
			FlagGpioIn.bit.GpioIn5 = 1;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		CountGpioInDown.GpioIn5 = 0;
		CountGpioInUp.GpioIn5++;
		if(CountGpioInUp.GpioIn5 > ERROR_GPIOIN)
		{
			CountGpioInUp.GpioIn5 = 0;
			FlagGpioIn.bit.GpioIn5 = 0;
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void ReadGPIOIn6(void)
{
	if(!GpioDataRegs.GPADAT.bit.GPIO6)
	{
		CountGpioInUp.GpioIn6 = 0;
		CountGpioInDown.GpioIn6++;
		if(CountGpioInDown.GpioIn6 > ERROR_GPIOIN)
		{
			CountGpioInDown.GpioIn6 = ERROR_GPIOIN;
			FlagGpioIn.bit.GpioIn6 = 1;
		}
	}
	else
	{
		CountGpioInDown.GpioIn6 = 0;
		CountGpioInUp.GpioIn6++;
		if(CountGpioInUp.GpioIn6 > ERROR_GPIOIN)
		{
			CountGpioInUp.GpioIn6 = 0;
			FlagGpioIn.bit.GpioIn6 = 0;
		}
	}
}

void ReadGPIOIn7(void)
{
	if(!GpioDataRegs.GPBDAT.bit.GPIO52)
	{
		CountGpioInUp.GpioIn7 = 0;
		CountGpioInDown.GpioIn7++;
		if(CountGpioInDown.GpioIn7 > ERROR_GPIOIN)
		{
			CountGpioInDown.GpioIn7 = ERROR_GPIOIN;
			FlagGpioIn.bit.GpioIn7 = 1;
		}
	}
	else
	{
		CountGpioInDown.GpioIn7 = 0;
		CountGpioInUp.GpioIn7++;
		if(CountGpioInUp.GpioIn7 > ERROR_GPIOIN)
		{
			CountGpioInUp.GpioIn7 = 0;
			FlagGpioIn.bit.GpioIn7 = 0;
		}
	}
}

void ReadGPIOIn8(void)
{
	if(!GpioDataRegs.GPBDAT.bit.GPIO49)
	{
		CountGpioInUp.GpioIn8 = 0;
		CountGpioInDown.GpioIn8++;
		if(CountGpioInDown.GpioIn8 > ERROR_GPIOIN)
		{
			CountGpioInDown.GpioIn8 = ERROR_GPIOIN;
			FlagGpioIn.bit.GpioIn8 = 1;
		}
	}
	else
	{
		CountGpioInDown.GpioIn8 = 0;
		CountGpioInUp.GpioIn8++;
		if(CountGpioInUp.GpioIn8 > ERROR_GPIOIN)
		{
			CountGpioInUp.GpioIn8 = 0;
			FlagGpioIn.bit.GpioIn8 = 0;
		}
	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void ControlGPIOOut(Uint16 device,Uint16 state)
�������ƣ�����GPIO���
�������ܣ�
��ں�����device -- �˿ںţ�state -- ״̬
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
#pragma CODE_SECTION(ControlGPIOOut, "ramcode")

void ControlGPIOOut(Uint16 device,Uint16 state)
{
	switch(device)
    {
        case GPIO_OUT1:
            if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO53   = 1;
            else             GpioDataRegs.GPBSET.bit.GPIO53     = 1;
            break;
		case GPIO_OUT2:
            if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO50   = 1;
            else             GpioDataRegs.GPBSET.bit.GPIO50     = 1;
            break;
		case GPIO_OUT3:
            if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO4    = 1;
            else             GpioDataRegs.GPASET.bit.GPIO4      = 1;
            break;
		case GPIO_OUT4:
            if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO2    = 1;
            else             GpioDataRegs.GPASET.bit.GPIO2      = 1;
            break;
		case GPIO_LED1:
            if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO1    = 1;
            else             GpioDataRegs.GPASET.bit.GPIO1      = 1;
            break;
		case GPIO_LED2:
            if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO3    = 1;
            else             GpioDataRegs.GPASET.bit.GPIO3      = 1;
            break;
		case GPIO_LED3:
            if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO5    = 1;
            else             GpioDataRegs.GPASET.bit.GPIO5      = 1;
            break;
		case GPIO_LED4:
            if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO51   = 1;
            else             GpioDataRegs.GPBSET.bit.GPIO51     = 1;
            break;
		case GPIO_LED5:
            if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO48   = 1;
            else             GpioDataRegs.GPBSET.bit.GPIO48     = 1;
            break;
		case GPIO_LED6:
            if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO7    = 1;
            else             GpioDataRegs.GPASET.bit.GPIO7      = 1;
            break;
		case SCI_CONB:
			if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO37   = 1;
			else             GpioDataRegs.GPBSET.bit.GPIO37     = 1;
			break;
		case SCI_CONC:
			if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO58   = 1;
			else             GpioDataRegs.GPBSET.bit.GPIO58     = 1;
			break;
		case D00:
			if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO20   = 1;
			else             GpioDataRegs.GPASET.bit.GPIO20     = 1;
			break;
		case D02:
			if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO55   = 1;
			else             GpioDataRegs.GPBSET.bit.GPIO55     = 1;
			break;
		case D03:
			if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO13   = 1;
			else             GpioDataRegs.GPASET.bit.GPIO13     = 1;
			break;
		case IO_RESET:
			if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO9	= 1;
			else             GpioDataRegs.GPASET.bit.GPIO9	    = 1;
			break;
		default:
            break;
	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void ControlSCROut(Uint16 channel,Uint16 state)
�������ƣ�����SCR_CS���
�������ܣ�
��ں�����channel -- �˿ںţ�state -- ״̬
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
#pragma CODE_SECTION(ControlSCROut, "ramcode")

void ControlSCROut(Uint16 channel,Uint16 state)
{
	switch(channel)
    {
        case NO1_A_CS:
            if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO32   = 1;
            else             GpioDataRegs.GPBSET.bit.GPIO32     = 1;
            break;
		case NO1_B_CS:
            if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO33   = 1;
            else             GpioDataRegs.GPBSET.bit.GPIO33     = 1;
            break;
		case NO1_C_CS:
            if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO21   = 1;
            else             GpioDataRegs.GPASET.bit.GPIO21     = 1;
            break;
		case NO2_A_CS:
            if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO59   = 1;
            else             GpioDataRegs.GPBSET.bit.GPIO59     = 1;
            break;
		case NO2_B_CS:
            if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO54   = 1;
            else             GpioDataRegs.GPBSET.bit.GPIO54     = 1;
            break;
		case NO2_C_CS:
            if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO56   = 1;
            else             GpioDataRegs.GPBSET.bit.GPIO56     = 1;
            break;
		default:
            break;
	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void GpioRead(void)
�������ƣ���ȡIO������״̬
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
void GpioRead(void)
{
	ReadGPIOIn1();
	ReadGPIOIn2();
	ReadGPIOIn3();
	ReadGPIOIn4();
	ReadGPIOIn5();
	ReadGPIOIn6();
	ReadGPIOIn7();
	ReadGPIOIn8();
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void CtrlSCREnable(enum SCR_CHANNEL Channel)
�������ƣ�����SCR����
�������ܣ�
��ں�������
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
#pragma CODE_SECTION(CtrlSCREnable, "ramcode")

void CtrlSCREnable(enum SCR_CHANNEL Channel)
{
	switch(Channel)
	{
        case  No1All:
        {
            ControlSCROut(NO1_A_CS, ON);
            ControlSCROut(NO1_B_CS, ON);
            ControlSCROut(NO1_C_CS, ON);
            ControlSCROut(NO2_A_CS, OFF);
            ControlSCROut(NO2_B_CS, OFF);
            ControlSCROut(NO2_C_CS, OFF);
            break;
        }
        case No1AB:
        {
            ControlSCROut(NO1_A_CS, ON);
            ControlSCROut(NO1_B_CS, ON);
            break;
        }
        case No1BC:
        {
            ControlSCROut(NO1_B_CS, ON);
            ControlSCROut(NO1_C_CS, ON);
            break;
        }
        case No1CA:
        {
            ControlSCROut(NO1_A_CS, ON);
            ControlSCROut(NO1_C_CS, ON);
            break;
        }
        case No1A:
        {
            ControlSCROut(NO1_A_CS, ON);
            break;
        }
        case No1B:
        {
            ControlSCROut(NO1_B_CS, ON);
            break;
        }
        case No1C:
        {
            ControlSCROut(NO1_C_CS, ON);
            break;
        }

        case No2All:
        {
            ControlSCROut(NO1_A_CS, OFF);
            ControlSCROut(NO1_B_CS, OFF);
            ControlSCROut(NO1_C_CS, OFF);
            ControlSCROut(NO2_A_CS, ON);
            ControlSCROut(NO2_B_CS, ON);
            ControlSCROut(NO2_C_CS, ON);
            break;
        }
        case No2AB:
        {
            ControlSCROut(NO2_A_CS, ON);
            ControlSCROut(NO2_B_CS, ON);
            break;
        }
        case No2BC:
        {
            ControlSCROut(NO2_B_CS, ON);
            ControlSCROut(NO2_C_CS, ON);
            break;
        }
        case No2CA:
        {
            ControlSCROut(NO2_A_CS, ON);
            ControlSCROut(NO2_C_CS, ON);
            break;
        }
        case No2A:
        {
            ControlSCROut(NO2_A_CS, ON);
            break;
        }
        case No2B:
        {
            ControlSCROut(NO2_B_CS, ON);
            break;
        }
        case No2C:
        {
            ControlSCROut(NO2_C_CS, ON);
            break;
        }

        default:
        {
		    break;
        }
   	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void CtrlSCRDisable(enum SCR_CHANNEL Channel)
�������ƣ�����SCR�ض�
�������ܣ�
��ں�������
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
#pragma CODE_SECTION(CtrlSCRDisable, "ramcode")

void CtrlSCRDisable(enum SCR_CHANNEL Channel)
{
	switch(Channel)
	{
        case  No1All:
        {
            ControlSCROut(NO1_A_CS, OFF);
            ControlSCROut(NO1_B_CS, OFF);
            ControlSCROut(NO1_C_CS, OFF);
            break;
        }
        case No2All:
        {
            ControlSCROut(NO2_A_CS, OFF);
            ControlSCROut(NO2_B_CS, OFF);
            ControlSCROut(NO2_C_CS, OFF);
            break;
        }
        case All:
        {
            ControlSCROut(NO1_A_CS, OFF);
            ControlSCROut(NO1_B_CS, OFF);
            ControlSCROut(NO1_C_CS, OFF);
            ControlSCROut(NO2_A_CS, OFF);
            ControlSCROut(NO2_B_CS, OFF);
            ControlSCROut(NO2_C_CS, OFF);
            break;
        }
        default:
        {
		    break;
        }
   	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�Uint16 Motor_PhaseCtrl(int16 cur_alpha, int16 init_alpha)
�������ƣ�ѡ��SCR��ͨ��
�������ܣ�
��ں�����cur_alpha--��ǰ�Ƕ�ֵ��init_alpha--��ʼ�Ƕ�ֵ
���ڲ�����state -- CS������
��    �ߣ�
��    �ڣ�2016��03��
----------------------------------------------------------------------*/
Uint16 MotorPhaseCtrl(int16 cur_alpha, int16 init_alpha)
{
	Uint16 state;
	int16  negative_alpha, positive_alpha;

	positive_alpha = init_alpha;
	negative_alpha = init_alpha + 180;
	if((cur_alpha >= 0) && (cur_alpha < positive_alpha))
	{
		state = OFF;
	}
	else if((cur_alpha >= positive_alpha) && (cur_alpha < 180))
	{
		state = ON;
	}
	else if((cur_alpha >= 180) && (cur_alpha < negative_alpha))
	{
		state = OFF;
	}
	else if((cur_alpha >= negative_alpha) && (cur_alpha < 360))
	{
		state = ON;
	}
	
	return state;
}

/*---------------------------------------------------------------------
����ԭ�ͣ�Uint16 MotorSoftStart(enum SCR_CHANNEL Channel, struct SPLL_REGS *spllreg, Uint16 init_alpha, int16 step_time)
�������ƣ�����SCR������ͨ
�������ܣ�
��ں�����Channel--ѡ�񹩵�ͨ����*spllreg--�������໷�Ƕȣ�init_alpha--��ʼ�Ƕ�ֵ�� step_time--�����ӳ�ʱ��
���ڲ�����
��    �ߣ�
��    �ڣ�2016��03��
----------------------------------------------------------------------*/
#pragma CODE_SECTION(MotorSoftStart, "ramcode")

Uint16 MotorSoftStart(enum SCR_CHANNEL Channel, struct SPLL_REGS *spllreg, Uint16 init_alpha, int16 step_time)
{
	int16  temp_alpha;
	Uint16 cs_state, flag_success;

	temp_alpha = init_alpha - CountSystem.SoftStartAlpha * 1;		// ÿ������1��
	if(temp_alpha < 0)
	{
		temp_alpha = 0;
	}
	switch(Channel)
	{
		case  No1All:
        {
            if(++CountSystem.SoftStartDelay < step_time)
            {
                cs_state = MotorPhaseCtrl(spllreg->ThetaA, temp_alpha);
                ControlSCROut(NO1_A_CS, cs_state);						// A�������źſ���

                cs_state = MotorPhaseCtrl(spllreg->ThetaB, temp_alpha);
                ControlSCROut(NO1_B_CS, cs_state);						// B�������źſ���

                cs_state = MotorPhaseCtrl(spllreg->ThetaC, temp_alpha);
                ControlSCROut(NO1_C_CS, cs_state);						// C�������źſ���

                flag_success = 0;
            }
            else
            {
                CountSystem.SoftStartAlpha++;
                CountSystem.SoftStartDelay = 0;
                flag_success = 0;
            }

            if(temp_alpha == 60)		                                // ���﹦�������Ǽ�ȫ��
            {
                flag_success = 1;
            }
            break;
        }

		case  No2All:
        {
            if(++CountSystem.SoftStartDelay < step_time)
            {
                cs_state = MotorPhaseCtrl(spllreg->ThetaA, temp_alpha);
                ControlSCROut(NO2_A_CS, cs_state);						// A�������źſ���

                cs_state = MotorPhaseCtrl(spllreg->ThetaB, temp_alpha);
                ControlSCROut(NO2_B_CS, cs_state);						// B�������źſ���

                cs_state = MotorPhaseCtrl(spllreg->ThetaC, temp_alpha);
                ControlSCROut(NO2_C_CS, cs_state);						// C�������źſ���

                flag_success = 0;
            }
            else
            {
                CountSystem.SoftStartAlpha++;
                CountSystem.SoftStartDelay = 0;
                flag_success = 0;
            }

            if(temp_alpha == 60)		// ���﹦�������Ǽ�ȫ��
            {
                flag_success = 1;
            }
            break;
        }

		default:
		{
			flag_success = 0;
			break;
		}
	}

	return flag_success;
}

/*---------------------------------------------------------------------
����ԭ�ͣ�interrupt void XINT1ISR(void)
�������ƣ��ⲿ�ж�3������
�������ܣ�
��ں�����
���ڲ�����
��    �ߣ�
��    �ڣ�2016��03��
----------------------------------------------------------------------*/
interrupt void XINT1ISR(void)
{
	Uint32 TempCnt, TempFout;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   						// ��Ӧ�ж�

	TempCnt = CpuTimer1Regs.PRD.all - CpuTimer1Regs.TIM.all;
	TempFout = 150000000 / TempCnt;								// ���㱸��DSP������Ƶ���źţ�����DSP������������״̬ʱ�ᷢ�������ź�

	if ((TempFout >= 1500) && (TempFout <= 2100))
	{
		CountSystem.XintIOCatch++;
		if (CountSystem.XintIOCatch >= 10)
		{
			CountSystem.XintIOCatch = 10;
			CountSystem.XintIORelease = 0;
			FlagSystem.bit.XintIOCatch = 1;
		}
	}
	else
	{
		CountSystem.XintIOCatch = 0;
	}

	CpuTimer1Regs.TIM.all = CpuTimer1Regs.PRD.all;
	PwmIsrReg.SpOrMainDSPFre = TempFout;
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void XintIORelease(void)
�������ƣ�����DSP���ֺ���
�������ܣ�����DSP���ڷ�����״̬�������������źţ���DSP����������
��ں�����
���ڲ�����
��    �ߣ�
��    �ڣ�2016��03��
----------------------------------------------------------------------*/
void XintIORelease(void)
{
	CountSystem.XintIORelease++;
	if (CountSystem.XintIORelease >= 1000)					// �ӳ�3��δ�յ������źţ�����Ϊ����DSP����������״̬
	{
		CountSystem.XintIORelease = 1000;
		FlagSystem.bit.XintIOCatch = 0;
	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void GPIOTEST(void)
�������ƣ�����IO�Ĳ���
�������ܣ�
��ڲ�������
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
void GpioTest(void)
{
	static Uint16 flag_start = 0, flag_channel = 0;
	static Uint16 count = 0, num = 0, flag = 0;

// ������IO��״̬
	ReadGPIOIn1();
	ReadGPIOIn2();
	ReadGPIOIn3();
	ReadGPIOIn4();
	ReadGPIOIn5();
	ReadGPIOIn6();
	ReadGPIOIn7();
	ReadGPIOIn8();

// ��������˿�״̬��������˿�
	if((1 == FlagGpioIn.bit.GpioIn1) || (1 == FlagGpioIn.bit.GpioIn5))		ControlGPIOOut(GPIO_OUT1, ON);
	else																	ControlGPIOOut(GPIO_OUT1, OFF);
	if((1 == FlagGpioIn.bit.GpioIn2) || (1 == FlagGpioIn.bit.GpioIn6))		ControlGPIOOut(GPIO_OUT2, ON);
	else																	ControlGPIOOut(GPIO_OUT2, OFF);
	if((1 == FlagGpioIn.bit.GpioIn3) || (1 == FlagGpioIn.bit.GpioIn7))		ControlGPIOOut(GPIO_OUT3, ON);
	else																	ControlGPIOOut(GPIO_OUT3, OFF);
	if((1 == FlagGpioIn.bit.GpioIn4) || (1 == FlagGpioIn.bit.GpioIn8))		ControlGPIOOut(GPIO_OUT4, ON);
	else																	ControlGPIOOut(GPIO_OUT4, OFF);

// ״̬LED��ʾ��ˮ��
	count++;
	if(count >= 50)
	{
		count = 0;
		num++;
		if(num >= 6)
		{
			num = 0;
			flag = !flag;
		}
		ControlGPIOOut(num + 4, flag);
	}

// ����SCR��������� IN1-������IN2-ֹͣ��IN3-NO1��IN4-NO2
	if(1 == FlagGpioIn.bit.GpioIn1)		flag_start = 1;
	if(1 == FlagGpioIn.bit.GpioIn4)		flag_start = 0;

	if(1 == FlagGpioIn.bit.GpioIn2)		flag_channel = 1;
	if(1 == FlagGpioIn.bit.GpioIn3)		flag_channel = 0;

	if(1 == flag_start)
	{
		if(1 == flag_channel)
		{
			CtrlSCREnable(No1All);
			CtrlSCRDisable(No2All);
		}
		else
		{
			CtrlSCRDisable(No1All);
			CtrlSCREnable(No2All);
		}
	}
	else
	{
		CtrlSCRDisable(All);
	}
}

