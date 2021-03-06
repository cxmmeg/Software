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
函数原型：void ReadCPLDSTS(void)
函数名称：读取电路状态
函数功能：选通使能PORT2
入口参数：无                  
出口参数：返回状态值
作    者：
日    期：
----------------------------------------------------------------------*/
unsigned int ReadCPLDSTS(void)
{   
	static Uint16 count = 0;
	Uint16 temp, temp1, temp2;

	do
	{
		temp1 = ((*(CS0AND1_ADDR + 2)) & 0xff);
		DelayUs(50);

		temp2 = ((*(CS0AND1_ADDR + 2)) & 0xff);
		DelayUs(50);

		count++;
		if (count >= 3)
		{
			count = 3;
			break;
		}
	}
	while (temp1 != temp2);

	if (count == 3)
	{
		temp = 0x5555;
	}
	else
	{
		temp = temp1;
	}

	count = 0;

    return (temp);
}

/*---------------------------------------------------------------------
函数原型：void ClrCPLDSTS(void)
函数名称：清除故障
函数功能：向端口3写入任意数据清除故障
入口参数：无                  
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void ClrCPLDSTS(void)
{
	*(CS0AND1_ADDR + 3) = 0;
    DelayUs(50);
	*(CS0AND1_ADDR + 3) = 0;
	DelayUs(50);
	*(CS0AND1_ADDR + 3) = 0;
	DelayUs(50);
	*(CS0AND1_ADDR + 3) = 0;
	DelayUs(50);
	*(CS0AND1_ADDR + 3) = 0;
	DelayUs(50);
	*(CS0AND1_ADDR + 3) = 0;
	DelayUs(50);
	*(CS0AND1_ADDR + 3) = 0;
	DelayUs(50);
	*(CS0AND1_ADDR + 3) = 0;
	DelayUs(50);
	*(CS0AND1_ADDR + 3) = 0;
	DelayUs(50);
	*(CS0AND1_ADDR + 3) = 0;
}


