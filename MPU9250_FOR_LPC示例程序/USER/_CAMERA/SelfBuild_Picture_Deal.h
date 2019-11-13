#ifndef _SELFBUILD_PICTURE_DEAL_H_
#define _SELFBUILD_PICTURE_DEAL_H_

#include "headfile.h"

//------宏定义-------
#define Row      120   //行
#define Col      188   //列

#define RowCut   120

#define Black 0X00
#define White 0XFF

#define MissLine 0
#define MidLine_Expect 94
#define MidPwmOut 1800

//--全局变量-------------------------------------
extern uint8 imageUse[Row][Col];
extern uint8 CameraBinary[Row][Col];//储存图像二值化数据的数组
extern uint8 Three_Line[Row][Col];
extern uint8 Left_Line[Row];                  //左线
extern uint8 Right_Line[Row];                 //右线
extern int Mid_Line[Row];                   //中线

extern uint8 Left_Jump_Value;                 //左跳变点
extern uint8 Right_Jump_Value;                //右跳变点
extern uint8 Mid_Jump_Value;                  //两边同时丢线                                          
extern int diatance;                     //两线间距
extern uint8 ValueNum;                        //最终有效行
extern uint8 Road_Type;                     //赛道类型
extern uint8 Mut_Jump_Value;                  //第一次非全白行
extern int  Mid_Value;                     //中线值

void Save_image(void);
void Road_Get(void);
void Three_Line_Hander(void);
void Mid_Line_Hander(void);
void Throw_The_Left_Line(uint8 y);
void Throw_The_Right_Line(uint8 y);
void Road_Juage(void);
void Mid_Value_Read(void);
void Mid_read(void);
float Sqrt2(int a);

#endif