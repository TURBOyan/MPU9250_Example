#ifndef _SELFBUILD_MECANUM_CTRL_H_
#define _SELFBUILD_MECANUM_CTRL_H_

#include "common.h"

#define a_PARAMETER_cm 18
#define b_PARAMETER_cm 18

typedef enum
{
//	Right_Back =0,
//	Right_Front    ,
//	Left_Back  ,
//	Left_Front   ,
	
	
	Left_Front =0,
	Left_Back    ,
	Right_Front  ,
	Right_Back   ,
	
	Wheel_Sum,
}WheelNum_Typedef;

extern struct MECANUM_Motor_Data_Typedef
{
	double SPEED_Set_cm_s[Wheel_Sum+1];
	double SPEED_Get_cm_s[Wheel_Sum+1];
	double SPEED_Get_cm_s_Last[Wheel_Sum+1];
	double PWM_Set[Wheel_Sum+1];
	
	double Speed_All;
	double Speed_X;
	double Speed_Y;
	
	double Speed_X_Real;
	double Speed_Y_Real;
	
	int8 X_Dir;
	int8 Y_Dir;
	
	double Speed_GyroZ_Out;
	double Speed_GyroZ_Set;
	
	double Distance_Sum_cm[Wheel_Sum+1];
	double Distance_Aid[Wheel_Sum+1];
}MECANUM_Motor_Data;

extern uint8 Encoder_Pulse_TIMER[Wheel_Sum+1];
extern uint8 Encoder_Dir_Pin[Wheel_Sum+1];
extern uint8 Encoder_Dir_Set[Wheel_Sum+1];
extern uint8 Motor_PWM[2][Wheel_Sum+1];

void Wheel_Analysis(void);
void Wheel_Speed_Get_cm_s(void);
void Wheel_Speed_Real_Get(void);
void Wheel_Speed_PID(void);
void Motor_PWM_Set(int16 PWM_MAX);

#endif
