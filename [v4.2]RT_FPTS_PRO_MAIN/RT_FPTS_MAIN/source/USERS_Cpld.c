// 用户程序
// 更新：2014-07-10
//###########################################################################
//
// 文件:	USERS_Cpld.c
//
// 标题:	用于读写CPLD
//
// 描述:	
//				
//########################################################################### 
#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h"
#include "USERS_Cpld.h"

extern union  FLAG_SYSTEM_RUNERR 		FlagSysRunErr;
/*---------------------------------------------------------------------
函数原型：void ReadCPLD_STS(void)
函数名称：读取电路状态
函数功能：选通使能PORT2
入口参数：无                  
出口参数：返回状态值
作    者：
日    期：
----------------------------------------------------------------------*/
unsigned int ReadCPLD_STS(void)
{   
    return ((*(CS0AND1_ADDR+2)) & 0xff);
}

/*---------------------------------------------------------------------
函数原型：void ClrCPLD_STS(void)
函数名称：清除故障
函数功能：向端口3写入任意数据清除故障
入口参数：无                  
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void ClrCPLD_STS(void)
{
/*	*(CS0AND1_ADDR+3) = 0;
    DelayUs(5);
	*(CS0AND1_ADDR+3) = 0;
*/
	ControlGPIOOut(IO_RESET, ON);
    DelayUs(10000);
    ControlGPIOOut(IO_RESET, OFF);
}


