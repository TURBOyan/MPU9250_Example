#include "headfile.h"

#include "MPU_ForUser.h"		//只需包括此头文件

int main(void)
{
	get_clk();
	OLED_Init();
	
	LED_P6x8Str(0,0,"Yaw=");
	LED_P6x8Str(0,1,"Pitch=");
	LED_P6x8Str(0,2,"Roll=");	
	
	MPU_Init_ForUser();		//		MPU9250初始化
	
	pint_init(PINT_CH0, A3, FALLING);		//MPU9250的INT引脚连接在A3上，设置为下降沿触发
	set_irq_priority(PIN_INT0_IRQn,0);//设置优先级 越低优先级越高
	enable_irq(PIN_INT0_IRQn);		//开启引脚中断
	
	EnableInterrupts;		//开启所有中断
	while(1)		
	{
	}
}

//当MPU9250的FIFO数据准备好时，将把INT引脚置低，触发A3引脚下降沿中断，此时可以在中断服务函数内读取数据，
//做直立车的话可以把控制代码放在这里面,控制周期将由MPU9250_Config.h文件内的DEFAULT_MPU_HZ决定
void PIN_INT0_DriverIRQHandler(void)															
{
		PINT_IST_FLAG_CLEAR(PINT_CH0);
		disable_irq(PIN_INT0_IRQn);		//暂时关闭中断

		Refresh_MPUTeam(DMP_MPL);//等待数据读取完成
		OLED_P6x8Flo(50, 0, MPU_Data.Yaw, -3);		//显示
		OLED_P6x8Flo(50, 1, MPU_Data.Pitch, -3);	
		OLED_P6x8Flo(50, 2, MPU_Data.Roll, -3);
	
	/*********************这里放自己的控制代码**********************
	
	**************************************************************/
		
		enable_irq(PIN_INT0_IRQn);	//开启中断
}