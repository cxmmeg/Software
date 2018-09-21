#ifndef _USERS_CPLD_H_
#define _USERS_CPLD_H_

#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h"

#define CS0AND1_ADDR (volatile unsigned int *)0x4000


/*---------------------------------------------------------------------
函数原型：unsigned int ReadSts(void)
函数名称：读取电路状态
函数功能：
入口参数：无
出口参数：返回状态值
作    者：
日    期：
----------------------------------------------------------------------*/
unsigned int ReadCPLD_STS(void);

/*---------------------------------------------------------------------
函数原型：void ClrSts(unsigned int)
函数名称：清除故障
函数功能：向端口3写入任意数据清除故障
入口参数：任意数
出口参数：无
作    者：
日    期：
----------------------------------------------------------------------*/
void ClrCPLD_STS(void);


#endif



