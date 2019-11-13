#include "SelfBuild_Electron_Calcu.h"

float g_fHuandaoTurn = 2.50;
int16 g_nHuandaoTimer = 300;
int16 g_nHuandaoAnger = 330; 
float g_fHuandaoOut = 0.08;

int16 g_nPodaoFlag = 0;
int16 g_nPodaoTimer = 150;
int16 g_nPodaoThres = 250;
int16 g_nPodaoSpeed = 300;

AD_Data_Typedef  AD_Data;

uint8 ADC_DataGet(void)	
{
  	uint16 ad_temp[5][5]={0};
	
	for(int8 i=0;i<5;i++)	//取五次电感值
	{
		ad_temp[0][i]=adc_once(AD_CH_R__, ADC_12bit);
		ad_temp[1][i]=adc_once(AD_CH_R_1, ADC_12bit);
		ad_temp[2][i]=adc_once(AD_CH_M_1, ADC_12bit);
		ad_temp[3][i]=adc_once(AD_CH_L_1, ADC_12bit);
		ad_temp[4][i]=adc_once(AD_CH_L__, ADC_12bit);
	}
	for(int8 i=0;i<5;i++)//冒泡排序升序五次电感值
	{
		for(int8 j=0;j<4;j++)
		{
			for(int8 k=0;k<4-j;k++)
			{
				if(ad_temp[i][k] > ad_temp[i][k+1])        //前面的比后面的大  则进行交换
				{
					uint16 temp;
					temp = ad_temp[i][k+1];
					ad_temp[i][k+1] = ad_temp[i][k];
					ad_temp[i][k] = temp;
				}
			}
		}
	}
	//电感值归一化，排序后中间三个值得平均值作为当前电感值
    AD_Data.AD_R_R = (uint8)RANGE(100-(100*(((ad_temp[0][1] + ad_temp[0][2] + ad_temp[0][3]) / 3.0-ADC_R_R_Min)/ ((ADC_Max-ADC_R_R_Min)*1.0))),155,0);
	AD_Data.AD_R   = (uint8)RANGE(100-(100*(((ad_temp[1][1] + ad_temp[1][2] + ad_temp[1][3]) / 3.0-ADC_R_Min)  / ((ADC_Max-ADC_R_Min)  *1.0))),155,0);
    AD_Data.AD_M   = (uint8)RANGE(100-(100*(((ad_temp[2][1] + ad_temp[2][2] + ad_temp[2][3]) / 3.0-ADC_M_Min)  / ((ADC_Max-ADC_M_Min)  *1.0))),155,0);
    AD_Data.AD_L   = (uint8)RANGE(100-(100*(((ad_temp[3][1] + ad_temp[3][2] + ad_temp[3][3]) / 3.0-ADC_L_Min)  / ((ADC_Max-ADC_L_Min)  *1.0))),155,0);
    AD_Data.AD_L_L = (uint8)RANGE(100-(100*(((ad_temp[4][1] + ad_temp[4][2] + ad_temp[4][3]) / 3.0-ADC_L_L_Min)/ ((ADC_Max-ADC_L_L_Min)*1.0))),155,0);
	
//	OLED_P6x8Int(OLED_SHOW(1), 3, adc_once(AD_CH_L__, ADC_8bit), 3);
//    OLED_P6x8Int(OLED_SHOW(2), 3, adc_once(AD_CH_L_1, ADC_8bit), 3);
//    OLED_P6x8Int(OLED_SHOW(3), 3, adc_once(AD_CH_M_1, ADC_8bit), 3);
//    OLED_P6x8Int(OLED_SHOW(4), 3, adc_once(AD_CH_R_1, ADC_8bit), 3);
//    OLED_P6x8Int(OLED_SHOW(5), 3, adc_once(AD_CH_R__, ADC_8bit), 3);//显示电感读数
//	
//	OLED_P6x8Int(OLED_SHOW(1), 4, AD_Data.AD_L_L, 3);
//    OLED_P6x8Int(OLED_SHOW(2), 4, AD_Data.AD_L, 3);
//    OLED_P6x8Int(OLED_SHOW(3), 4, AD_Data.AD_M, 3);
//    OLED_P6x8Int(OLED_SHOW(4), 4, AD_Data.AD_R, 3);
//    OLED_P6x8Int(OLED_SHOW(5), 4, AD_Data.AD_R_R, 3);//显示电感读数
    return SUCCESS;
}

uint8 ADC_LastDataSave(void)	//保存上次AD值函数
{
    AD_Data.AD_L_Last   =  AD_Data.AD_L;
    AD_Data.AD_R_Last   =  AD_Data.AD_R;
    AD_Data.AD_L_L_Last =  AD_Data.AD_L_L;
    AD_Data.AD_R_R_Last =  AD_Data.AD_R_R;
    AD_Data.AD_M_Last   =  AD_Data.AD_M;
    return SUCCESS;	
}

void ADC_Calcu(void)	//电感算法
{
  	static float chabihe=0,CHA=0;
    static int8 Flag_Huandao=0,					//环岛标志
			 	 Flag_Huandao_DIR=0,				//环岛方向
			 	 Huandao_Timer=0,					
				 Huandao_Rise=0,					//中间树电感下降沿计数
				 Flag_Podao=0;
	static int16 Flag_PodaoTimer = 0;		//坡道行驶计时
	static int16 ADC_Prodata_M_1[3] = {0};
	static float GYROZ_SUM=0;		//旋转角度
	float fHuandaoErrorOut = 0;	//环岛误差输出
	
    ADC_DataGet();	//电感数据读取并滤波
	
	if((AD_Data.AD_R_R > 20 && AD_Data.AD_L_L > 20) || fabs(AD_Data.AD_R_R - AD_Data.AD_L_L) > fabs(CHA))	//锁差
	{
		CHA = AD_Data.AD_R_R - AD_Data.AD_L_L;
	}
	chabihe = 100 * (float)(CHA) / (float)(AD_Data.AD_L_L + AD_Data.AD_R_R+1);
	
	/////////////////////////////////////////////////////坡道判断待修改/////////////////////////////////////////////////////////////
//	if(Flag_Podao == 0 && g_fGyroX < -g_nPodaoThres && fabs(AD_Data.AD_R_R - AD_Data.AD_L_L) < 15)//坡道判断
//	{
//		Flag_Podao = 1;
//		Flag_PodaoTimer = Timer;
//	}
//	if( Flag_Podao == 1 && Timer > (Flag_PodaoTimer + g_nPodaoTimer))
//	{
//		Flag_Podao = 3;
//	}
	
	
	if(Flag_Huandao == 0 && (AD_Data.AD_R_R> 80 || AD_Data.AD_L_L > 80) && AD_Data.AD_M> 35 && (AD_Data.AD_L> 35 || AD_Data.AD_R> 35) && Flag_Podao != 1)//准备进环岛
	{
		Flag_Huandao = 1;
	}
	
	if(Flag_Huandao == 1 || Flag_Huandao == 3 || Flag_Huandao == 5)		//若进入环岛，开始记录旋转角度
		GYROZ_SUM += (MPU6050.GYRO_Real.z* 0.9765 * 0.006);
	if(Flag_Huandao == 1)
	{
		if(Huandao_Rise == 0 && (((int)(AD_Data.AD_M * 2) < ADC_Prodata_M_1[0] && ADC_Prodata_M_1[0] < ADC_Prodata_M_1[1] && ADC_Prodata_M_1[1] < ADC_Prodata_M_1[2])))
		{
			Huandao_Rise = 3;	//第一个下降沿
			Flag_Huandao_DIR = (AD_Data.AD_R_R >  AD_Data.AD_L_L  ? Circle_LEFT : Circle_RIGHT);
		}
		if(Huandao_Rise == 3 && (int)(AD_Data.AD_M* 2) >= ADC_Prodata_M_1[0])
		{
			Huandao_Rise = 5;	//第二个上升沿
		}
		if(Huandao_Rise == 5 && (AD_Data.AD_R - AD_Data.AD_L) * Flag_Huandao_DIR > 0)
		{
			Huandao_Rise = 0;			//第二个下降沿，电感正好处于电磁线重叠位置
			Flag_Huandao = 3;
			ADC_Prodata_M_1[0] = 0, ADC_Prodata_M_1[1] = 0, ADC_Prodata_M_1[2] = 0;
		}
		
		ADC_Prodata_M_1[2] = ADC_Prodata_M_1[1], ADC_Prodata_M_1[1] = ADC_Prodata_M_1[0], ADC_Prodata_M_1[0] = (int)(AD_Data.AD_M  * 2);
	}
	if(Flag_Huandao == 3)
	{
		fHuandaoErrorOut = g_fHuandaoTurn * ((AD_Data.AD_R_R - AD_Data.AD_L_L) * 1 + (AD_Data.AD_R - AD_Data.AD_L) * 4) / 5.0;
		if(AD_Data.AD_L_L + AD_Data.AD_R_R< 150)
		{
			if(chabihe * Flag_Huandao_DIR > 0 && fabs(chabihe) > 1.0 * fabs(fHuandaoErrorOut))
			{
				Flag_Huandao = 5;
				Error_Flag.Text_Stop=1;
			}
		}
		if(fabs(GYROZ_SUM) > 30)
		{
			Flag_Huandao = 5;
//			Error_Flag.Text_Stop=1;
		}
		
		if(fHuandaoErrorOut * Flag_Huandao_DIR < 0) fHuandaoErrorOut = 0;
		if(fHuandaoErrorOut > 70) fHuandaoErrorOut = 70;
		if(fHuandaoErrorOut < -70) fHuandaoErrorOut = -70;
	}
	
	if(Flag_Huandao == 5 && fabs(GYROZ_SUM) > g_nHuandaoAnger)	//准备出环
	{
		Flag_Huandao = 7;
		Huandao_Timer = g_nHuandaoTimer;
	}
	if(Flag_Huandao == 7 && Huandao_Timer -- < 0)
	{
		Flag_Huandao = 9;
		Huandao_Rise = 0;
		ADC_Prodata_M_1[0] = 0, ADC_Prodata_M_1[1] = 0, ADC_Prodata_M_1[2] = 0;
		GYROZ_SUM = 0;
	}
	if(Flag_Huandao == 9 && AD_Data.AD_M  < 60 && AD_Data.AD_R_R  < 80 && AD_Data.AD_L_L < 80)//判断是否出环，清零环岛标志位以便下次判断
		Flag_Huandao = 0;
	

	//以下是误差赋值
	if(Flag_Huandao == 3)
	{
		AD_Data.Error_Out = fHuandaoErrorOut;
	}
	else if(Flag_Huandao == 7)
	{
		AD_Data.Error_Out = chabihe;
		if(AD_Data.Error_Out * Flag_Huandao_DIR > 0) AD_Data.Error_Out *= g_fHuandaoOut;
	}
	else
	{
		AD_Data.Error_Out = chabihe;
	}
	AD_Data.Control_Out  = PID_Calcu(  0 ,  AD_Data.Error_Out   ,  &PID_Elec  ,  Local);
}