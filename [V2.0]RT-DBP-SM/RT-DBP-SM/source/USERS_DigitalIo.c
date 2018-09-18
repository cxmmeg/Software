// ���س�ʼ���ļ�
// ���£�2018-08 -- RT-DBP-MB-V4.0
//###########################################################################
//
// �ļ�:	DSP2833x_Gpio.c
//
// ����:	���ڳ�ʼ��������IO��
//
// ����:	����  1. XINTF��ʼ��
//		    2. ����IO�ӿڳ�ʼ��
//			3. IO����/�������

// �ӿ�:	IN1��GPIO33
//		IN2��GPIO61
//		IN3��GPIO63
//		IN4��GPIO62
//		IN5��GPIO22
//		IN6��GPIO21
//		IN7��GPIO20
//		IN8��GPIO59

//		OUT1��GPIO25
//		OUT2��GPIO52
//		OUT3��GPIO26
//		OUT4��GPIO24
//		OUT5��GPIO60
//		OUT6��GPIO27
//		OUT7��GPIO23
//		OUT8��GPIO32

//		DRIVER_EN��GPIO48
//		SNB_EN��GPIO49
//		SCI_CON:GPIO58

//###########################################################################
#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File

struct  COUNT_GPIOIN_BUF_UP  	CountGpioInUp; 		// GPIO�ɿ���ʱ�����ṹ�����
struct	COUNT_GPIOIN_BUF_DOWN	CountGpioInDown;	// GPIO������ʱ�����ṹ�����

extern union  	FLAG_GPIOIN   	FlagGpioIn;			// GPIO��־λ�ṹ��λ����

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
	//	OUT1��GPIO25
	GpioDataRegs.GPADAT.bit.GPIO25		= 1;   //Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX2.bit.GPIO25		= 0;   //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO25		= 1;   //Setup the direction is outport;

	//	OUT2��GPIO52
	GpioDataRegs.GPBDAT.bit.GPIO52	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO52    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO52     	= 1;    //Setup the direction is outport;

	//	OUT3��GPIO26
	GpioDataRegs.GPADAT.bit.GPIO26	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX2.bit.GPIO26    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO26     	= 1;    //Setup the direction is outport;

	//	OUT4��GPIO24
	GpioDataRegs.GPADAT.bit.GPIO24	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX2.bit.GPIO24    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO24     	= 1;    //Setup the direction is outport;

	//	OUT5��GPIO60
	GpioDataRegs.GPBDAT.bit.GPIO60	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO60    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO60     	= 1;    //Setup the direction is outport;

	//	OUT6��GPIO27
	GpioDataRegs.GPADAT.bit.GPIO27	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX2.bit.GPIO27    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO27     	= 1;    //Setup the direction is outport;

	//	OUT7��GPIO23
	GpioDataRegs.GPADAT.bit.GPIO23	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPAMUX2.bit.GPIO23    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPADIR.bit.GPIO23     	= 1;    //Setup the direction is outport;

	//	OUT8��GPIO32
	GpioDataRegs.GPBDAT.bit.GPIO32	   	= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX1.bit.GPIO32    	= 0;    //setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO32     	= 1;    //Setup the direction is outport;

	//	DRIVER_EN��GPIO48
	GpioDataRegs.GPBDAT.bit.GPIO48		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO48 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO48		= 1;	//Setup the direction is outport;

	//	SNB_EN��GPIO49
	GpioDataRegs.GPBDAT.bit.GPIO49		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO49 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO49		= 1;	//Setup the direction is outport;

	//	SCI_CON:GPIO58
	GpioDataRegs.GPBDAT.bit.GPIO58		= 1;	//Setup the output DATA=1; Then default output of the connenction is low;
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 	= 0;	//setup the 	function is GPIO;
	GpioCtrlRegs.GPBDIR.bit.GPIO58		= 1;	//Setup the direction is outport;

/*---------------------------��������˿�-----------------------------*/
   //	IN1��GPIO33
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;  		//setup the function is GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO33	= 0;  		//setup the direction is input;

	//	IN2��GPIO61
	GpioCtrlRegs.GPBMUX2.bit.GPIO61	= 0;  		//setup the function is GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO61	= 0;  		//setup the direction is input;

	//	IN3��GPIO63
	GpioCtrlRegs.GPBMUX2.bit.GPIO63	= 0;  //setup the function is GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO63	= 0;  //setup the direction is input;

	//	IN4��GPIO62
	GpioCtrlRegs.GPBMUX2.bit.GPIO62	= 0;  //setup the function is GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO62	= 0;  //setup the direction is input;

	//	IN5��GPIO22
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;  //setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO22	= 0;  //setup the direction is input;

	//	IN6��GPIO21
	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;  //setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO21	= 0;  //setup the direction is input;

	//	IN7��GPIO20
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;  //setup the function is GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO20	= 0;  //setup the direction is input;

	//	IN8��GPIO59
	GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;  //setup the function is GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO59  = 0;  //setup the direction is input;

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
	if(!GpioDataRegs.GPBDAT.bit.GPIO33)
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
	if(!GpioDataRegs.GPBDAT.bit.GPIO61)
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
	if(!GpioDataRegs.GPBDAT.bit.GPIO63)
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
	if(!GpioDataRegs.GPBDAT.bit.GPIO62)
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


void ReadGPIOIn5(void)
{
	if(!GpioDataRegs.GPADAT.bit.GPIO22)
	{
		CountGpioInUp.GpioIn5 = 0;
		CountGpioInDown.GpioIn5++;
		if(CountGpioInDown.GpioIn5 > ERROR_GPIOIN)
		{
			CountGpioInDown.GpioIn5 = ERROR_GPIOIN;
			FlagGpioIn.bit.GpioIn5 = 1;
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
		}
	}
}

void ReadGPIOIn6(void)
{
	if(!GpioDataRegs.GPADAT.bit.GPIO21)
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
	if(!GpioDataRegs.GPADAT.bit.GPIO20)
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
	if(!GpioDataRegs.GPBDAT.bit.GPIO59)
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
����ԭ�ͣ�void GPIOOutOfDSP(Uint16 device,Uint16 state)
�������ƣ�DSPֱ�ӿ���GPIO���
�������ܣ�
��ں�����device -- �˿ںţ�state -- ״̬
���ڲ�������
��    �ߣ�
��    �ڣ�2017��04��
----------------------------------------------------------------------*/
#pragma CODE_SECTION(GPIOOutOfDSP, "ramcode")

void GPIOOutOfDSP(Uint16 device,Uint16 state)
{
	switch(device)
	{
		case GPIO_OUT1:
			if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO25   = 1;
			else             GpioDataRegs.GPASET.bit.GPIO25     = 1;
			break;
		case GPIO_OUT2:
			if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO52   = 1;
			else             GpioDataRegs.GPBSET.bit.GPIO52     = 1;
			break;
		case GPIO_OUT3:
			if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO26   = 1;
			else             GpioDataRegs.GPASET.bit.GPIO26     = 1;
			break;
		case GPIO_OUT4:
			if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO24   = 1;
			else             GpioDataRegs.GPASET.bit.GPIO24     = 1;
			break;
		case GPIO_OUT5:
			if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO60   = 1;
			else             GpioDataRegs.GPBSET.bit.GPIO60     = 1;
			break;
		case GPIO_OUT6:
			if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO27   = 1;
			else             GpioDataRegs.GPASET.bit.GPIO27     = 1;
			break;
		case GPIO_OUT7:
			if(state == ON)  GpioDataRegs.GPACLEAR.bit.GPIO23   = 1;
			else             GpioDataRegs.GPASET.bit.GPIO23     = 1;
			break;
		case GPIO_OUT8:
			if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO32   = 1;
			else             GpioDataRegs.GPBSET.bit.GPIO32     = 1;
			break;
		case EN_DRIVER:
			if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO48   = 1;
			else             GpioDataRegs.GPBSET.bit.GPIO48     = 1;
			break;
		case EN_SNB:
			if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO49   = 1;
			else             GpioDataRegs.GPBSET.bit.GPIO49     = 1;
			break;
		case SCI_CONB:
			if(state == ON)  GpioDataRegs.GPBCLEAR.bit.GPIO58   = 1;
			else             GpioDataRegs.GPBSET.bit.GPIO58     = 1;
			break;
		default:
			break;
	}
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void GPIOOutOfCLPD(Uint16 device,Uint16 state)
�������ƣ�ͨ��CPLDѡַ����GPIO���
�������ܣ�
��ں�����device -- ��ַ�ţ�state -- ״̬
���ڲ�������
��    �ߣ�
��    �ڣ�2018��08��
----------------------------------------------------------------------*/
#pragma CODE_SECTION(GPIOOutOfCLPD, "ramcode")

void GPIOOutOfCLPD(Uint16 device,Uint16 state)
{
	Uint16 a0, a1, a2, a3;

	a0 = (device >> 0) % 2;
	a1 = (device >> 1) % 2;
	a2 = (device >> 2) % 2;
	a3 = (device >> 3) % 2;

	if (1 == a0)	GpioDataRegs.GPASET.bit.GPIO24     = 1;
	else			GpioDataRegs.GPACLEAR.bit.GPIO24   = 1;
	if (1 == a1)	GpioDataRegs.GPASET.bit.GPIO26     = 1;
	else			GpioDataRegs.GPACLEAR.bit.GPIO26   = 1;
	if (1 == a2)	GpioDataRegs.GPASET.bit.GPIO27     = 1;
	else			GpioDataRegs.GPACLEAR.bit.GPIO27   = 1;
	if (1 == a3)	GpioDataRegs.GPBSET.bit.GPIO63     = 1;
	else			GpioDataRegs.GPBCLEAR.bit.GPIO63   = 1;

	if(state == ON)
	{
		GpioDataRegs.GPASET.bit.GPIO25     = 1;		// ������λ����
		GpioDataRegs.GPBCLEAR.bit.GPIO61   = 1;		// ʹ�ܸ�λ����
	}
	else
	{
		GpioDataRegs.GPBSET.bit.GPIO61     = 1;		// ���ܸ�λ����
		GpioDataRegs.GPACLEAR.bit.GPIO25   = 1;		// ʹ����λ����
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

// ״̬LED��ʾ��ˮ��
	count++;
	if(count >= 50)
	{
		count = 0;
		num++;
		if(num >= 14)
		{
			num = 0;
			flag = !flag;
		}
		GPIOOutOfCLPD(num, flag);
	}
}


