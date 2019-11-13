#ifndef _SELFBUILD_CONTROL_H_
#define _SELFBUILD_CONTROL_H_

#include "headfile.h"

typedef enum
{
	Switch_1_Data		   =0x08,
	Switch_2_Data		   =0x04,
	Switch_3_Data		 	 =0x02,
	Switch_4_Data		 	 =0x01,
	
	Button_Up_Data		 =0x10,
	Button_Down_Data	 =0x08,
	Button_Left_Data	 =0x04,
	Button_Right_Data  =0x02,
	Button_Mid_Data	 	 =0x01,
}ButtSwit_Data_enum;

extern struct Base_Data_Typedef	//»ù´¡Êý¾Ý
{
	uint8 Switch_Data,Button_Data;
	
	uint8 Gray_Data[6][6];
	uint8 Gray_Data_Last[6][6];
	uint8 Gray_Data_Rise[6][6];
	uint8 Gray_Data_Fall[6][6];
}Base_Data;
 
extern PIN_enum Gray[6][6];

#define Query_ButtSwitData(ButtOrSwit,Data) ((Base_Data.ButtOrSwit & Data)==Data)

void Init_ALL(void);
void Read_ButtSwitData(void);
void Read_GrayData(uint8 show);
void Save_GrayData(uint8 data_new[6][6],uint8 data_save[6][6]);
void Judge_GrayData(void);
uint8 View_MPUddata(void);
#endif
