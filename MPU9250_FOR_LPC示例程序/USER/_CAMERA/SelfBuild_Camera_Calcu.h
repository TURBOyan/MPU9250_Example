#ifndef _SELFBUILD_CAMERA_CALCU_H_
#define _SELFBUILD_CAMERA_CALCU_H_

#include "headfile.h"

typedef struct
{
	float Error_Out;		//最终输出偏差值
	int32 Control_Out;		//偏差值经PID后输出的控制量
	float Error_Aid;
	
}Camera_Data_Typedef;
extern Camera_Data_Typedef Camera_Data;

void Camera_Calcu(void);

#endif