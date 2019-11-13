#ifndef _SELFBUILD_PID_H
#define _SELFBUILD_PID_H

#include "common.h"

typedef struct 
{
  float Param_Kp;
	float Param_Ki;
	float Param_Kd;
	
  float AidData_Set;
	float Error_Sum;
	float Param_measure;
	
	float Error;
	float Error_last;
	float Error_last_last;
	
	int32 PID_Dynam_Out;
	int32 PID_Incre_Return;
	int32 PID_Local_Out;
}PID_Param_Set;

typedef enum
{
	Dynam,
	Incre,
	Local,
}PID_Mode_Typedef;

extern PID_Param_Set PID_Speed;	//速度环PID参数
extern PID_Param_Set PID_Dir;	  //方向环PID参数
extern PID_Param_Set PID_Dis;	  //方向环PID参数
int32 PID_Calcu	(float Aid_Data,float Measure_Data,PID_Param_Set* P_I_D,PID_Mode_Typedef PID_Mode);

#endif
