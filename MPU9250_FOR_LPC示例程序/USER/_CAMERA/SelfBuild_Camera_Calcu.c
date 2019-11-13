#include "SelfBuild_Camera_Calcu.h"

Camera_Data_Typedef Camera_Data;

void Camera_Calcu(void)
{
	int16 i,k1=0,k2=0,k3=0;
 	int16 MidLine_Sum1=0,MidLine_Sum2=0,MidLine_Sum3=0;
	float MidLine_Aver=0;
	for(int8 i=116;i>15;i--)
    {
          if(Mid_Line[i] != MissLine)
          {
              if(i>90)
              {
                  MidLine_Sum1 += Mid_Line[i];
                  k1++;
              }
              if(i<=90&&i>=30)
              {
                  MidLine_Sum2 += Mid_Line[i];
                  k2++; 
              }
              if(i<30)
              {
                  MidLine_Sum3 += Mid_Line[i];
                  k3++;
              }
          }
    }
	
	if(MidLine_Sum1!=0&&MidLine_Sum2!=0&&MidLine_Sum3!=0)
       Camera_Data.Error_Out=0.3*MidLine_Sum1/k1+0.4*MidLine_Sum2/k2+0.3*MidLine_Sum3/k3;
    else 
       Camera_Data.Error_Out=(MidLine_Sum1+MidLine_Sum2+MidLine_Sum3)/(k1+k2+k3);
	Camera_Data.Control_Out=PID_Calcu(  94 ,  MidLine_Aver   ,  &PID_Camer  ,  Local);
}