#ifndef __SELFBUILD_ELECTRON_CALCU_H_
#define __SELFBUILD_ELECTRON_CALCU_H_

#include "headfile.h"

#define Circle_LEFT  -1
#define Circle_RIGHT 1

#define ADC_Max 3088	//五电感最大值

#define ADC_R_R_Min ADC_R_R_Min_8bit*(ADC_Max/193)	//五电感最小值
#define ADC_R_Min   ADC_R_Min_8bit*(ADC_Max/193)
#define ADC_M_Min   ADC_M_Min_8bit*(ADC_Max/193)
#define ADC_L_Min   ADC_L_Min_8bit*(ADC_Max/193)
#define ADC_L_L_Min ADC_L_L_Min_8bit*(ADC_Max/193)

extern float g_fHuandaoTurn;
extern float g_fHuandaoOut;                            ;

typedef enum
{
  L__ = 0,
  L_1,
  M_1,
  R_1,
  R__,
  ADC_NUM,
}ADC_POSITION;

typedef struct 	//电磁传感器数据相关结构体
{
	uint8 AD_L_L;	//最左侧电感值
	uint8 AD_L_L_Last;	//最左侧电感值上一次
	
	uint8 AD_L;	//左侧电感值
	uint8 AD_L_Last;	//左侧电感值上一次
	
	uint8 AD_M;	//中间电感值
	uint8 AD_M_Last;	//中间电感值上一次
	
	uint8 AD_R;	//右侧电感值
	uint8 AD_R_Last;	//右侧电感值上一次
	
	uint8 AD_R_R;	//最右侧电感值
	uint8 AD_R_R_Last;	//最右侧电感值上一次
	
	float Error_Out;		//最终输出偏差值
	int32  Control_Out;		//偏差值经PID后输出的控制量
}AD_Data_Typedef;

extern AD_Data_Typedef AD_Data; //电磁相关变量

void ADC_Calcu(void);


#endif
