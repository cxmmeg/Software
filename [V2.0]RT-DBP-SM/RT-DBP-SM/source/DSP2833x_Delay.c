
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

/*---------------------------------------------------------------------
函数原型：void DelayUs(volatile int Usec)
函数名称：延时函数
函数功能：
入口参数：延时时间---Usec                  
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void DelayUs(volatile int Usec)
{
	while(Usec--)
	{
		asm(" RPT#139||NOP");	
	}
}

//===========================================================================
// End of file.
//===========================================================================
