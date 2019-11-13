#include "SelfBuild_PID.h"

//最佳 P0.57 I0.01	D0.0
PID_Param_Set PID_Speed=
							{0.0,	//速度环PID参数
							 0.0,
							 0.0,
							 0,0,0,0,0,0,0,0,0},
							PID_Dir=
							{0.0,	//方向环PID参数
							 0.0,
							 0.0,
							 0,0,0,0,0,0,0,0,0},
							PID_Dis=
							{0.0,	//方向环PID参数
							 0.0,
							 0.0,
							 0,0,0,0,0,0,0,0,0};
			  			

void PID_Dynam(PID_Param_Set* PID_Xxx)	//动态位置式PID输出
{
  	float Kpp;
  	PID_Xxx->Error=PID_Xxx->AidData_Set-PID_Xxx->Param_measure;
	Kpp=1.0*(PID_Xxx->Error*PID_Xxx->Error)/PID_Xxx->Param_Kp+PID_Xxx->Param_Ki;
	PID_Xxx->PID_Dynam_Out=(int32)(Kpp*PID_Xxx->Error
			       +PID_Xxx->Param_Kd*(0.8*(PID_Xxx->Error-PID_Xxx->Error_last)));
	PID_Xxx->Error_last=PID_Xxx->Error;
}

void PID_Incre (PID_Param_Set* PID_Xxx)  //增量式PID输出
{
	PID_Xxx->Error = PID_Xxx->AidData_Set -  PID_Xxx->Param_measure;       //当前误差计算
	PID_Xxx->PID_Incre_Return =(int32)(PID_Xxx->Param_Kp*(PID_Xxx-> Error  - PID_Xxx->Error_last)			//PID增量值计算
	  		 	   +PID_Xxx->Param_Ki*PID_Xxx->Error
				   +PID_Xxx->Param_Kd*(PID_Xxx-> Error  - 2*PID_Xxx->Error_last + PID_Xxx->Error_last_last));
	PID_Xxx->Error_last_last=PID_Xxx->Error_last;
	PID_Xxx->Error_last=PID_Xxx-> Error ;
}

void PID_Local  (PID_Param_Set* PID_Xxx)         //位置式PID输出
{
	PID_Xxx->Error = PID_Xxx->AidData_Set -  PID_Xxx->Param_measure;
	PID_Xxx->Error_Sum += PID_Xxx-> Error ;
	PID_Xxx->PID_Local_Out = (int32)(PID_Xxx->Param_Kp* PID_Xxx-> Error 
	  		         +PID_Xxx->Param_Ki* PID_Xxx->Error_Sum
				 +PID_Xxx->Param_Kd* (PID_Xxx-> Error  - PID_Xxx->Error_last));
	PID_Xxx->Error_last = PID_Xxx-> Error;
}

int32 PID_Calcu	(float Aid_Data,float Measure_Data,PID_Param_Set* P_I_D,PID_Mode_Typedef PID_Mode)
{
	P_I_D->AidData_Set	    = Aid_Data;
	P_I_D->Param_measure	= Measure_Data;
	switch(PID_Mode)
	{
	case Dynam:
	  	PID_Dynam (P_I_D);
		return P_I_D->PID_Dynam_Out;
//	break;
	case Incre: 
	  	PID_Incre (P_I_D);
		return P_I_D->PID_Incre_Return;
	//	break;
	case Local: 
	  	PID_Local (P_I_D);
		return P_I_D->PID_Local_Out;
	//	break;
	default: while(1);
	}
}