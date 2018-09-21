#ifndef USERS_PROTECT_H
#define USERS_PROTECT_H

// ������������
#define VRMS_90PERCENT              3686            //  ����10%�ȴ�����ӳ��л�
#define VRMS_80PERCENT              3276            //  ����20%����Ͷ���л�
#define VRMS_70PERCENT              2867            //  ����30%ֱ���л�

#define AC_CUR_RUSUM5               204             //  ��������ֵ�ز�                   Q12 5%Ie
#define AC_VOL_RESUM5               204             //  ��ѹ�ز�5%              Q12 19v
#define AC_VOL_RESUM10              409             //  ��ѹ�ز�10%             Q12 28v
#define TEMP_RESUM5                 50              //  �¶Ȼز�                             5��

#define AC_VOLLOW_DRC               3399            //  �����ѹ��ѹֱ��ת��ֵ     Q12 315V-- 83%380V
#define AC_VOLLOW_DLC               3481            //  �����ѹ��ѹ�ӳ�ת��ֵ     Q12 323V-- 85%380V
#define AC_VOLLOW_NORMAL            3686            //  �����ѹ���������ѹֵ     Q12 342V-- 90%380V
#define AC_VOLHIGH_NORMAL           4505            //  �����ѹ���������ѹֵ     Q12 418V-- 110%380V

#define DELAY_DIRECT_CHANGE         1               //  ǿ���л���ʱ
#define DELAY_PROTECT               1000            //  Ͷ����ɺ�ȴ�����ȶ�ʱ��     3ms * 1000  = 3s
#define DELAY_NORMAL_DETC           2000            //  �����������ӳ�ֵ                   3ms * 2000  = 6s
#define DELAY_ACVLOW_CHANGE         1000            //  �����ѹ�л��ӳ�ֵ                   3ms * 1000  = 3s
#define DELAY_TEMP_PROTECT          600             //  ϵͳ���±����ӳ�ֵ                   3ms * 600   = 1.8s
#define DELAY_SCR_SHORT_DETC        10000           //  �ɿع��·����ӳ�ֵ                3ms * 10000  = 30s
#define DELAY_SCR_BROKEN_DETC       1000            //  �ɿع��·����ӳ�ֵ                3ms * 1000  = 3s


#ifdef __cplusplus
extern "C" {
#endif

void ForceModeChosen(void);
void DelayModeChosen(void);
void InVolProtect(void);
void InCurProtect(void);
void SCRProtect(void);
void TempProtect(void);
void PDPClearFunc(Uint16 ClearTime);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_XINTF_H definitio 





