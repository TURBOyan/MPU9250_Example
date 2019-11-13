/****************************************************************************
 * @文件       SelfBuild_OLEDMenu.c
 * @作者       刘力铭
 * @功能       菜单，用于调整参数
 * @注意       该程序使用基于OLED，五个按键（对应上下左右确认），FLASH读写
               和一个蜂鸣器（可关闭），移植时请确保初始化
 * @完成时间   2018-4-25
****************************************************************************/
#include "SelfBuild_OLEDMenu.h"

Menu_TypeDef Menu[MENU_MAX]={0};

/******************修改部分从这行开始，其他程序请不要随意改动******************/

//下面菜单使用的外部全局变量声明


__STATIC_INLINE void Menu_Data_Init(void)
{       
//菜单设置
//例如
//
//Menu[3].Title = "PID_SPEED";                  //一级菜单显示的是PID_SPEED
//Menu[3].Sub_Value[0] = &PID.Proportion;       //二级菜单第一个可调整的参数是PID.Proportion
//Menu[3].Sub_Title[0] = "SPEED_P";             //该变量在菜单上显示的是SPEED_P
//Menu[3].Sub_Type[0] = -4;                     //该变量的类型为float 注意：-4代表float -2代表int16 2代表uint16
//注意，Menu数组选择范围为1~(MENU_MAX-1)，Menu.Sub等数组的选择范围是0~(SUBMENU_MAX-1)！！！
//使用Menu[0]无效！！！
//Sub_Type类型选择错误会死机！！！
//最多的变量数为16(16个还不够你用?)
	Menu[1].Title = "PID_Dis";
	Menu[1].Sub_Value[0] = &PID_Dis.Param_Kp;
	Menu[1].Sub_Title[0] = "P";
	Menu[1].Sub_Type[0] =-4;
	Menu[1].Sub_Value[1] = &PID_Dis.Param_Kd;
	Menu[1].Sub_Title[1] = "D";
	Menu[1].Sub_Type[1] =-4;
	
	Menu[2].Title = "PID_Speed";
	Menu[2].Sub_Value[0] = &PID_Speed.Param_Kd;
	Menu[2].Sub_Title[0] = "D";
	Menu[2].Sub_Type[0] =-4;
	Menu[2].Sub_Value[1] = &PID_Speed.Param_Ki;
	Menu[2].Sub_Title[1] = "I";
	Menu[2].Sub_Type[1] =-4;
	
	Menu[3].Title = "PID_Dir";
	Menu[3].Sub_Value[0] = &PID_Dir.Param_Kp;
	Menu[3].Sub_Title[0] = "P";
	Menu[3].Sub_Type[0] =-4;
	Menu[3].Sub_Value[1] = &PID_Dir.Param_Kd;
	Menu[3].Sub_Title[1] = "D";
	Menu[3].Sub_Type[1] =-4;

}

/******************修改部分从这行结束，其他程序请不要随意改动******************/

static void Menu_HomePage_Show(void);
static void Menu_SubPage_Show(Menu_TypeDef* Menu_Type);
static int8 Button_Get(void);
//显示二级菜单界面
static void Menu_HomePage_Show(void)
{
	uint8 temp;
	LED_Fill(0x00);//清屏
	LED_P6x8Str((OLED_X_MAX - 1 - 6 * (sizeof("Menu") - 1)) / 2, 0, "Menu");//显示界面
	
	for(temp = 0; temp < MENU_MAX; ++temp)
	{
		if(Menu[temp].Title != 0)
		{
			LED_P6x8Str(MENU_SHOW((temp % 2)) + 8, temp / 2 + 1, (char*)Menu[temp].Title);
		}
	}
}

//显示二级菜单界面
static void Menu_SubPage_Show(Menu_TypeDef* Menu_Type)
{
	uint8 temp;
		LED_Fill(0x00);//清屏
	
	for(temp = 0; Menu_Type->Title[temp] != 0; ++temp);//显示界面
	LED_P6x8Str((OLED_X_MAX - 1 - 6 * temp) / 2, 0, (char*)Menu_Type->Title);
	
	for(temp = 0; temp < Menu_Type->Sub_Value_Num; ++temp)
	{
		LED_P6x8Str(8, temp + 1, (char*)Menu_Type->Sub_Title[temp]);
		if(Menu_Type->Sub_Type[temp] == -2)
			OLED_P6x8Int(80, temp + 1, *((int16*)Menu_Type->Sub_Value[temp]), -3);
		else if(Menu_Type->Sub_Type[temp] == 2)
			OLED_P6x8Int(80, temp + 1, *((uint16*)Menu_Type->Sub_Value[temp]), -3);
		else if(Menu_Type->Sub_Type[temp] == -4)
		{
			if(*((float*)Menu_Type->Sub_Value[temp]) >= 0)
				OLED_P6x8Flo(80, temp + 1, *((float*)Menu_Type->Sub_Value[temp]) + 0.0001f, -2);
			else
				OLED_P6x8Flo(80, temp + 1, *((float*)Menu_Type->Sub_Value[temp]) - 0.0001f, -2);
		}
	}
	
	LED_P6x8Str(8, 6, "SAVE");
	LED_P6x8Str(8, 7, "BACK");
}

static int8 Button_Get(void)
{
	int8 return_flag = 0;
	if(gpio_get(BUTTON_UP) == 0)
		return_flag |= 0x01;
	if(gpio_get(BUTTON_DOWN) == 0)
		return_flag |= 0x02;
	if(gpio_get(BUTTON_LEFT) == 0)
		return_flag |= 0x04;
	if(gpio_get(BUTTON_RIGHT) == 0)
		return_flag |= 0x08;
	if(gpio_get(BUTTON_CONFIRM) == 0)
		return_flag |= 0x10;
	
	return return_flag;
}

void* Copy_Data[16] = {0};
int8 Copy_Type[16] = {0};
uint32 ZERO[32]={0};
//-------------------------------------------------------------------------
//  @函数         菜单初始化函数
//  @作用         用于初始化Menu数组的数据
//-------------------------------------------------------------------------
void Menu_Init(void)
{
	uint8 temp, Sub_temp, Sub_Menu_Sum = 0, Sub_Sum_temp = 0;
	
	Menu_Data_Init();
	Menu[0].Title = "CloseMenu";
	Menu[0].Sub_Title[0] = 0;
	eeprom_init();
	
	//将Menu里面的所有变量拷贝到Copy_Data数组中，与存放在FLASH中的数进行比较，并求出Menu中有多少变量
	for(temp = 1; temp < MENU_MAX; ++temp)
	{
     if(Menu[temp].Title == 0) continue;
		for(Sub_temp = 0; (Sub_temp < SUBMENU_MAX) && (Menu[temp].Sub_Title[Sub_temp] != 0); ++Sub_temp)
		{
			Copy_Data[Sub_Menu_Sum & 0x0f] = Menu[temp].Sub_Value[Sub_temp];
			Copy_Type[Sub_Menu_Sum & 0x0f] = Menu[temp].Sub_Type[Sub_temp];
			if(Menu[temp].Sub_Type[Sub_temp] == -2)
			{
				if(*((int16*)Copy_Data[Sub_Menu_Sum & 0x0f]) !=  my_EEPROM_READ_WORD(FLASH_SAVE_RAW_MENU,Sub_Menu_Sum ,int16))
					Sub_Menu_Sum |= 0x10;
			}
			else if(Menu[temp].Sub_Type[Sub_temp] == 2)
			{
				if(*((uint16*)Copy_Data[Sub_Menu_Sum & 0x0f]) !=  my_EEPROM_READ_WORD(FLASH_SAVE_RAW_MENU,Sub_Menu_Sum ,uint16))
					Sub_Menu_Sum |= 0x10;
			}
			else if(Menu[temp].Sub_Type[Sub_temp] == -4)
			{

				if(*((float*)Copy_Data[Sub_Menu_Sum & 0x0f]) != my_EEPROM_READ_WORD(FLASH_SAVE_RAW_MENU,Sub_Menu_Sum ,float)) 
				 {
					
					Sub_Menu_Sum |= 0x10;
				 }
			}
			++Sub_Menu_Sum;
		}
		Menu[temp].Sub_Value_Num = Sub_temp;
	}
	
	if(Sub_Menu_Sum & 0x10)//如果上面的结果为不一样，则说明Menu里面的变量值存在修改，则将之前FLASH中的参数重新写入，以备下一次检查用
	{
		Sub_Menu_Sum &= ~0x10;
//		FLASH_EraseSector(FLASH_SAVE_RAW_MENU);
//		FLASH_EraseSector(FLASH_SAVE_PRO_MENU);
//		eeprom_write_page(FLASH_SAVE_RAW_MENU, ZERO);
//		eeprom_write_page(FLASH_SAVE_PRO_MENU, ZERO);
		for(temp = 0; temp < Sub_Menu_Sum; ++temp)
		{
			if(Copy_Type[temp] == -2)
			{
				 my_eeprom_write_word(FLASH_SAVE_RAW_MENU,temp,*((int16*)Copy_Data[temp]));
				 my_eeprom_write_word(FLASH_SAVE_PRO_MENU,temp,*((int16*)Copy_Data[temp]));
			}
			else if(Copy_Type[temp] == 2)
			{
				 my_eeprom_write_word(FLASH_SAVE_RAW_MENU,temp,*((uint16*)Copy_Data[temp]));
				 my_eeprom_write_word(FLASH_SAVE_PRO_MENU,temp,*((uint16*)Copy_Data[temp]));
			}
			else if(Copy_Type[temp] == -4)
			{
				 my_eeprom_write_word(FLASH_SAVE_RAW_MENU,temp,*((int32*)Copy_Data[temp]));
				 my_eeprom_write_word(FLASH_SAVE_PRO_MENU,temp,*((int32*)Copy_Data[temp]));
				
			}
		}
	}
	else//如果上面的结果为一样，则认为程序没有对变量值进行修改，则默认使用保存在FLASH中的值作为各变量的值，并将这些值复制到Copy_Data中
	{
		for(temp = 0, Sub_Sum_temp = 0; temp < MENU_MAX; ++temp)
		{
			if(Menu[temp].Title != 0)
			{
				for(Sub_temp = 0; Sub_temp < Menu[temp].Sub_Value_Num; ++Sub_temp)
				{
					if(Menu[temp].Sub_Type[Sub_temp] == -2)
					{
						  	*((int16*)Copy_Data[Sub_Sum_temp])= my_EEPROM_READ_WORD(FLASH_SAVE_PRO_MENU,Sub_Sum_temp,int16); 
					}
					else if(Menu[temp].Sub_Type[Sub_temp] == 2)
					{
						*((uint16*)Copy_Data[Sub_Sum_temp]) =  my_EEPROM_READ_WORD(FLASH_SAVE_PRO_MENU,Sub_Sum_temp,uint16); 
					}
					else if(Menu[temp].Sub_Type[Sub_temp] == -4)
					{
						*((float*)Copy_Data[Sub_Sum_temp]) =  my_EEPROM_READ_WORD(FLASH_SAVE_PRO_MENU,Sub_Sum_temp,float);
					}
					++Sub_Sum_temp;
				}
			}
		}
	}
	
	Menu_HomePage_Show();
}

//-------------------------------------------------------------------------
//  @函数         菜单工作函数
//  @作用         调用该函数使菜单工作
//  @返回值       0:菜单没有按下“CloseMenu”选项 1:菜单按下“CloseMenu”选项
//-------------------------------------------------------------------------
uint8 Menu_Work(void)
{
	uint16 temp, temp_Sum = 0;
	int8 Button_Flag = 0;
	static int8 Current_Count = 0, Current_Count_Last = 0, Confirm_Flag = 0, Sub_Current_Count = 0, Sub_Current_Count_Last = 0, Sub_Confirm_Flag = 0;
	//Current_Count:一级菜单的计数值，用于控制显示光标的位置，和知道光标所指的菜单
	//Current_Count_Last:一级菜单的上一次计数值，用于控制消除光标位置
	//Confirm_Flag:一级菜单的确认标志，当置一后进入二级菜单
	//Sub_Current_Count:二级菜单的计数值，用于控制显示光标的位置，和知道光标所指的变量
	//Sub_Current_Count_Last:二级菜单的上一次计数值，用于控制消除光标位置
	//Sub_Confirm_Flag:二级菜单的确认标志，当置一后锁定变量开始修改
	static uint16 Copy_Data_Cnt = 0, Return_Flag = 0;
	static float temp_Data[SUBMENU_MAX]={0};
	Current_Count_Last = Current_Count;
	Sub_Current_Count_Last = Sub_Current_Count;
	Button_Flag = Button_Get();
	
	if(Button_Flag & 0x0f)//如果有上下左右被按下
	{
		if(Confirm_Flag == 0)//当一级菜单的确认标志没有置一时，此时OLED显示为一级菜单界面，按下左键，光标移动
		{
			if(Button_Flag & 0x01) Current_Count -= 2;
			if(Button_Flag & 0x02) Current_Count += 2;
			if(Button_Flag & 0x04) Current_Count -= 1;
			if(Button_Flag & 0x08) Current_Count += 1;
		}
		else//一级菜单确认标志置一，此时OLED显示为二级菜单
		{
			if(Sub_Confirm_Flag == 0)
			{
				if(Button_Flag & 0x01) Sub_Current_Count -= 1;//当二级菜单确认标志没有置一，按下左键，光标上移
				if(Button_Flag & 0x02) Sub_Current_Count += 1;
				if(Button_Flag & 0x04) Sub_Current_Count -= 1;
				if(Button_Flag & 0x08) Sub_Current_Count += 1;
			}
			else//否则，按下左边则为调整变量的值
			{
				if(Menu[Current_Count].Sub_Type[Sub_Current_Count] == -2)
				{
					if(Button_Flag & 0x01) temp_Data[Sub_Current_Count] += 5;
					if(Button_Flag & 0x02) temp_Data[Sub_Current_Count] -= 5;
					if(Button_Flag & 0x04) temp_Data[Sub_Current_Count] -= 1;
					if(Button_Flag & 0x08) temp_Data[Sub_Current_Count] += 1;
					OLED_P6x8Int(80, Sub_Current_Count + 1, (int16)temp_Data[Sub_Current_Count], -3);
				}
				else if(Menu[Current_Count].Sub_Type[Sub_Current_Count] == 2)
				{
					if(Button_Flag & 0x01) temp_Data[Sub_Current_Count] += 5;
					if(Button_Flag & 0x02) temp_Data[Sub_Current_Count] -= 5;
					if(Button_Flag & 0x04) temp_Data[Sub_Current_Count] -= 1;
					if(Button_Flag & 0x08) temp_Data[Sub_Current_Count] += 1;
					if(temp_Data[Sub_Current_Count] < 0) temp_Data[Sub_Current_Count] = 0;
					OLED_P6x8Int(80, Sub_Current_Count + 1, (int16)temp_Data[Sub_Current_Count], -3);
				}
				else if(Menu[Current_Count].Sub_Type[Sub_Current_Count] == -4)
				{
					if(Button_Flag & 0x01) temp_Data[Sub_Current_Count] += 0.1f;
					if(Button_Flag & 0x02) temp_Data[Sub_Current_Count] -= 0.1f;
					if(Button_Flag & 0x04) temp_Data[Sub_Current_Count] -= 0.01f;
					if(Button_Flag & 0x08) temp_Data[Sub_Current_Count] += 0.01f;
					if(temp_Data[Sub_Current_Count] >= 0)
						OLED_P6x8Flo(80, Sub_Current_Count + 1, temp_Data[Sub_Current_Count] + 0.0001f, -2);
					else
						OLED_P6x8Flo(80, Sub_Current_Count + 1, temp_Data[Sub_Current_Count] - 0.0001f, -2);
				}
			}
		}
	}
	
	//控制Current_Count和Sub_Current_Count的大小
	if(Confirm_Flag == 0)
	{
		if(Current_Count < 0) Current_Count += 14;
		if(Current_Count > 13) Current_Count -= 14;
	}
	else if(Confirm_Flag == 1)
	{
		if(Sub_Current_Count < 0) 
			Sub_Current_Count = 6;
		//这里因为二级菜单的BACK和SAVE选项与变量选项不连续，故需要另外计算
		if(Sub_Current_Count > Menu[Current_Count].Sub_Value_Num - 1 && Sub_Current_Count < 5 && Sub_Current_Count > Sub_Current_Count_Last) 
			Sub_Current_Count = (Sub_Current_Count - Menu[Current_Count].Sub_Value_Num) + 5;
		if(Sub_Current_Count > Menu[Current_Count].Sub_Value_Num - 1 && Sub_Current_Count < 5 && Sub_Current_Count < Sub_Current_Count_Last) 
			Sub_Current_Count = Sub_Current_Count - 5 + Menu[Current_Count].Sub_Value_Num;
		else if(Sub_Current_Count > 6) 
			Sub_Current_Count = 0;
	}
	else if(Confirm_Flag == 2)
	{
		if(Sub_Current_Count < 5) 
			Sub_Current_Count = 6;
		else if(Sub_Current_Count > 6) 
			Sub_Current_Count = 5;
	}
	
	if(Confirm_Flag == 0)//显示光标，消除之前的光标
	{
		LED_P6x8Str(MENU_SHOW(Current_Count_Last % 2), (Current_Count_Last) / 2 + 1, " ");
		LED_P6x8Str(MENU_SHOW(Current_Count % 2), (Current_Count) / 2 + 1, ">");
	}
	else
	{
		LED_P6x8Str(0, Sub_Current_Count_Last + 1, " ");
		if(Sub_Current_Count < Menu[Current_Count].Sub_Value_Num && Sub_Confirm_Flag == 1)
			LED_P6x8Str(0, Sub_Current_Count + 1, "*");
		else LED_P6x8Str(0, Sub_Current_Count + 1, ">");
	}
	
	if(Button_Flag & 0x10)//确认按键
	{
		if(Current_Count != 0)
		{
			if(Confirm_Flag == 0)//当处于第一菜单的时候
			{
				if(Menu[Current_Count].Title != 0)
				{
					Confirm_Flag = 1;
					Sub_Current_Count_Last = 0, Sub_Current_Count = 0;//清零控制二级菜单的变量
					Menu_SubPage_Show(&(Menu[Current_Count]));//显示二级菜单
					for(temp = 0; temp < Menu[Current_Count].Sub_Value_Num; ++temp)//将菜单中的变量值先储存到temp_Data中
					{
						if(Menu[Current_Count].Sub_Type[temp] == -2)
							temp_Data[temp] = *((int16 *)Menu[Current_Count].Sub_Value[temp]);
						else if(Menu[Current_Count].Sub_Type[temp] == 2)
							temp_Data[temp] = *((uint16 *)Menu[Current_Count].Sub_Value[temp]);
						else if(Menu[Current_Count].Sub_Type[temp] == -4)
							temp_Data[temp] = *((float *)Menu[Current_Count].Sub_Value[temp]);
					}
					for(temp = 0, Copy_Data_Cnt = 0; temp < Current_Count; ++temp)//计算得到该值在Copy_Data数组中的位置
						Copy_Data_Cnt += Menu[temp].Sub_Value_Num;
				}
			}
			else if(Confirm_Flag == 1)//当处于第二带单的时候
			{
				if(Sub_Current_Count == 6 || Sub_Current_Count == 5)//如果按下的是SAVE或者BACK的时候
				{
					if(Sub_Current_Count == 5)//如果按下的是SAVE按键
					{
						for(temp = 0; temp < Menu[Current_Count].Sub_Value_Num; ++temp)//将temp_Data中的临时储存值保存到变量
							{
								if(Menu[Current_Count].Sub_Type[temp] == -2)
								{
									if(temp_Data[temp] >= 0)
										*((int16*)Copy_Data[temp + Copy_Data_Cnt]) = (int16)(temp_Data[temp] + 0.0001f);
									else
										*((int16*)Copy_Data[temp + Copy_Data_Cnt]) = (int16)(temp_Data[temp] - 0.0001f);
								}
								else if(Menu[Current_Count].Sub_Type[temp] == 2)
								{
									*((uint16*)Copy_Data[temp + Copy_Data_Cnt]) = (uint16)(temp_Data[temp] + 0.0001f);
								}
								else if(Menu[Current_Count].Sub_Type[temp] == -4)
								{
									if(temp_Data[temp] >= 0)
										*((float*)Copy_Data[temp + Copy_Data_Cnt]) = (int16)((temp_Data[temp] + 0.0001f) * 100)  / 100.0;//最大程度的保证float的精度
									else
										*((float*)Copy_Data[temp + Copy_Data_Cnt]) = (int16)((temp_Data[temp] - 0.0001f) * 100)  / 100.0;//最大程度的保证float的精度
								}
							}
						//FLASH_EraseSector(FLASH_SAVE_PRO_MENU);//将修改之后的值存入FLASH中
           //	eeprom_write_page(FLASH_SAVE_PRO_MENU, ZERO);
						for(temp = 0, temp_Sum = 0; temp < MENU_MAX; ++temp) temp_Sum += Menu[temp].Sub_Value_Num;
						for(temp = 0; temp < temp_Sum; ++ temp)
						{
							if(Copy_Type[temp] == -2)
							  my_eeprom_write_word(FLASH_SAVE_PRO_MENU,temp,*((int16*)Copy_Data[temp]));
							else if(Copy_Type[temp] == -4)
							 my_eeprom_write_word(FLASH_SAVE_PRO_MENU,temp,*((int32*)Copy_Data[temp]));
							else if(Copy_Type[temp] == 2)
							 my_eeprom_write_word(FLASH_SAVE_PRO_MENU,temp,*((uint16*)Copy_Data[temp]));
						}
					}
					Copy_Data_Cnt = 0;
					for(temp = 0; temp < Menu[Current_Count].Sub_Value_Num; ++temp) temp_Data[temp] = 0;
					Confirm_Flag = 0;
					Menu_HomePage_Show();//重新显示一级菜单，将标志位清零
					LED_P6x8Str(MENU_SHOW(Current_Count % 2), (Current_Count) / 2 + 1, ">");
				}
				else if(Sub_Current_Count < Menu[Current_Count].Sub_Value_Num)//如果按下的不是BACK和SAVE选项，则按下的确认视为锁定和解除锁定变量
				{
					Sub_Confirm_Flag ^= 1;
				}
			}
		}
		else
		{
			Return_Flag = 1;
			Button_Flag = 0;
			while(gpio_get(BUTTON_CONFIRM) == 0);
				LED_Fill(0x00);
		}
	}
        
#if BUZZER_EN
	if(Button_Flag != 0) gpio_set(BUZZER, 0);
	else gpio_set(BUZZER, 0);
#endif
	return Return_Flag;
}
