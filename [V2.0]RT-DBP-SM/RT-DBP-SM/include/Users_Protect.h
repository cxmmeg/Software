#ifndef USERS_PROTECT_H
#define USERS_PROTECT_H

// 保护参数设置
#define VRMS_90PERCENT              3686            //  掉电10%
#define VRMS_85PERCENT              3481          	//  掉电15%
#define VRMS_80PERCENT              3276            //  掉电20%
#define VRMS_70PERCENT              2867            //  掉电30%

#define AC_CUR_RUSUM5               204             //  电流保护值回差                   Q12 5%Ie
#define AC_VOL_RESUM5               204             //  电压回差5%          Q12 19v
#define AC_VOL_RESUM10              409             //  电压回差10%         Q12 28v
#define TEMP_RESUM5                 50              //  温度回差                             5℃

#define AC_VOLLOW_DRC               3605            //  输入电压低压直接转换值     Q12 315V-- 88%380V
#define AC_VOLLOW_NORMAL            3686            //  输入电压正常区间低压值     Q12 342V-- 90%380V
#define AC_VOLHIGH_NORMAL           4505            //  输入电压正常区间高压值     Q12 418V-- 110%380V

#define DELAY_DIRECT_CHANGE         1               //  强制切换延时
#define DELAY_PROTECT               1000            //  投切完成后等待输出稳定时间     3ms * 1000  = 3s
#define DELAY_NORMAL_DETC           2000            //  正常输入检测延迟值                   3ms * 2000  = 6s
#define DELAY_ACVLOW_CHANGE         1000            //  输入低压切换延迟值                   3ms * 1000  = 3s
#define DELAY_TEMP_PROTECT          600             //  系统高温保护延迟值                   3ms * 600   = 1.8s
#define DELAY_SCR_SHORT_DETC        2000            //  可控硅短路检测延迟值                3ms * 2000  = 6s
#define DELAY_SCR_BROKEN_DETC       1000            //  可控硅断路检测延迟值                3ms * 1000  = 3s


#ifdef __cplusplus
extern "C" {
#endif

void PhaseLackProtect(void);
void InVolProtect(void);
void InCurProtect(void);
void SCRProtect(void);
void TempProtect(void);
void PDPClearFunc(Uint16 ClearTime);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_XINTF_H definitio 






