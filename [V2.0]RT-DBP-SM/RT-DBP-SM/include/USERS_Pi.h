#ifndef  _USERS_PI_H_
#define  _USERS_PI_H_




//pi�����ṹ�嶨��
struct PIRegulator
{
	int16 Input;		//����ֵ����
	int16 Output;		//�������
	int16 PiRef;		//�ο�ֵ
	int16 Kp;			//pi����
	int16 Ki;
	int16 Kc;			//У��ϵ��
	int16 PIMax;		//������ֵ
	int16 PIMin;
	int32 PiUi;			//����ֵ
//	void (*PiFuncQn)(struct PIRegulator *PiSet);	//pi�����ص�����
};



void PiFuncQ12(struct PIRegulator *PiSet);



#endif
