#ifndef  _USERS_PI_H_
#define  _USERS_PI_H_




//pi操作结构体定义
struct PIRegulator
{
	int16 Input;		//采样值输入
	int16 Output;		//计算输出
	int16 PiRef;		//参考值
	int16 Kp;			//pi参数
	int16 Ki;
	int16 Kc;			//校正系数
	int16 PIMax;		//饱和限值
	int16 PIMin;
	int32 PiUi;			//积分值
//	void (*PiFuncQn)(struct PIRegulator *PiSet);	//pi操作回调函数
};



void PiFuncQ12(struct PIRegulator *PiSet);



#endif
