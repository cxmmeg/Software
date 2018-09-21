#ifndef _VVVF_H_
#define _VVVF_H_
#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h"

extern int We_VVVF;
extern int VSD_T4_VVVF,VSQ_T4_VVVF;
void Init_VVVFVar(void);
void ReadVVVFVarFromPara(void);

void VsdqVvvfCal(void);
void WE_VVVF_cal(void);


#endif


