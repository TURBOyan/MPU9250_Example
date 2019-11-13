#ifndef _SELFBUILD_UI_H_
#define _SELFBUILD_UI_H_

#include "headfile.h"

//从机要做的事：
//1、OLED界面判断是否更新变量库
//2、如果是，依次发送变量组名，变量名，变量类型，并分配和发送变量编号
//3、中断接收（变量编号+命令+数据）
//
//4、实时发送---命令表
//
//遥控器要做的事：
//1、等待接收更新命令
//2、若为更新，则显示连接成功，并接收变量组名，变量名，变量类型，变量编号。并将历史数据保存在flash内。
//3、改变变量值时选择是否发送，若为发送则保存在EEPROM内并发送数据（变量编号+命令+数据）
//
//4、实时接收---命令表 

//typedef struct
//{
//	const char* Title;                                                      //该菜单的名字
//	void* Sub_Value[SUBMENU_MAX];                                           //该菜单储存待调整的变量数值
//	int8 Sub_Type[SUBMENU_MAX];                                             //该菜单储存待调整的变量数类型
//	const char* Sub_Title[SUBMENU_MAX];					//该菜单储存待调整的变量数名字
//}Debug_Typedef;

extern struct Wireless_Flag_Typedef Wireless_Flag;
extern int16 Speed_Set;

struct Wireless_Flag_Typedef
{
uint8 Wirelesson_Flag,
			FLAG_START,
			FLAG_GO,
			FLAG_BACK,
			FLAG_Speedzero,
			FLAG_ALLSTOP,
			FLAG_AdcCal;
};

struct Wireless_ReceiveData_Typedef
{
	uint8 Cmd;
};

typedef enum
{
	Start=0x4c,		//起始位
	Stop=0x55,		//终止位
	Data=0x49,		//数据位
	Cmd=0x48,		//命令位
}Wireless_Cmd_Typedef;

void wireless_CommunInit(void);
void wireless_init(void);
void wireless_CommunSend(uint8 *data);
void wireless_Control(void);
void UART4_IRQHandler(void);
void Computer_Send(void);

#endif
