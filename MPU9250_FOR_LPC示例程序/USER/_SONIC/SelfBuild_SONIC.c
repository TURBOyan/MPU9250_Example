#include "SelfBuild_SONIC.h"

struct SONIC_Data_Typedef SONIC_Data;

void SONIC_Init(CTIMER_CAPTURE_enum capturech,CAPTURE_ch_enum CAPTURE_ch)
{
	ctimer_capture_init(capturech,CAPTURE_ch,CAPTURE_BOTH,CAPTURE_PS_1);
	gpio_init(SONIC_Trig, GPO, 0,PULLDOWN);
}
void SONIC_gettime(CTIMER_CAPTURE_enum capturech,CAPTURE_ch_enum CAPTURE_ch)
{
	if(SONIC_Data.Measure_Flag == 0 && SONIC_Data.Finish_Flag==1)
	{
		SONIC_Data.Measure_Flag=1;
		SONIC_Data.Finish_Flag=0;
		gpio_set(SONIC_Trig, 1);
		systick_delay_us(15);
		gpio_set(SONIC_Trig, 0);
		capture_enable_irq(TIMER0_CAPTURECH3_B1);
	}
}

void CTIMER0_DriverIRQHandler(void)
{
	if(CTIMER0_IR_FLAG(3))
	{
		CTIMER0_IR_FLAG_CLEAR(3);	//清中断标志位
		if(SONIC_Data.Measure_Flag==0)
		{
			 CTIMER0->TCR  = CTIMER_TCR_CRST_MASK;
			 SONIC_Data.Time= CTIMER0->CR[3];
			 capture_disable_irq(TIMER0_CAPTURECH3_B1);
			 SONIC_Data.Finish_Flag=1;
		}
		if(SONIC_Data.Measure_Flag==1)
		{
			 CTIMER0->TCR  = CTIMER_TCR_CEN_MASK;
			 SONIC_Data.Measure_Flag=0;
		}
	}
}