#ifndef _SELFBUILD_BALANCE_H_
#define _SELFBUILD_BALANCE_H_

#include "headfile.h"

extern int32
						 Speed_Min,	// 左右最小速度
						 Theory_Duty,// 理论直立占空比
						 Vel_Set,	// 目标转向角速度
						 Direct_Parameter,// 转向系数
						 Direct_Last,
						 Radius;		// 目标转向半径倒数
extern uint8 System_OK;


int Balance(void);

#endif