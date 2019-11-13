/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		ADC
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#include "common.h"
#include "LPC546XX_iocon.h"
#include "LPC546XX_gpio.h"
#include "LPC546XX_pll.h"
#include "LPC546XX_systick.h"
#include "LPC546XX_adc.h"

void adc_mux(ADCCH_enum ch)
{
    switch(ch)
    {
        case ADC_CH0_A10:   iocon_init(A10,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH1_A11:   iocon_init(A11,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH2_A12:   iocon_init(A12,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH3_A15:   iocon_init(A15,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH4_A16:   iocon_init(A16,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH5_A31:   iocon_init(A31,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH6_B0 :   iocon_init(B0 ,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH11_A23:  iocon_init(A23,ALT0 | NOPULL | ANALOG | FILTEROFF); break;

        default:        ASSERT(0);//通道错误 进入断言失败
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC初始化
//  @param      ch          ADC通道号
//  @return     void
//  Sample usage:           adc_init(ADC_CH0_A10);     // 初始化ADC通道0  分辨率为12位   通道对应的引脚为A10
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCCH_enum ch)
{
    uint16 temp_div;
    
    
    SYSCON->PDRUNCFGCLR[0] = ( 0
                             | SYSCON_PDRUNCFGCLR_PDEN_ADC0_MASK 
                             | SYSCON_PDRUNCFGCLR_PDEN_VD2_ANA_MASK 
                             | SYSCON_PDRUNCFGCLR_PDEN_VDDA_MASK
                             | SYSCON_PDRUNCFGCLR_PDEN_VREFP_MASK
                             ); //打开ADC电源
    systick_delay_us(20);                                       //必要延时
    
    
    SYSCON->ADCCLKSEL = SYSCON_ADCCLKSEL_SEL(0x01);             //选择ADC时钟源
    SYSCON->ADCCLKDIV = 0;  
    
    SYSCON->ADCCLKDIV = SYSCON_ADCCLKDIV_HALT_MASK;  
    //SYSCON->ADCCLKDIV = SYSCON_ADCCLKDIV_HALT_MASK | SYSCON_ADCCLKDIV_DIV(0) | SYSCON_ADCCLKDIV_RESET_MASK;  
    //while(!(SYSCON_ADCCLKDIV_REQFLAG_MASK & SYSCON->ADCCLKDIV));
    SYSCON->ADCCLKDIV = SYSCON_ADCCLKDIV_DIV(0);
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_ADC0_MASK;     //打开ADC时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_ADC0_RST_MASK; //清除复位ADC时钟
    
    
    adc_mux(ch);

    temp_div = (main_clk_mhz*100/80 + 99)/100;
    ADC0->CTRL = ( 0
                 | ADC_CTRL_CLKDIV(temp_div-1)      //分频最大不超过80M
                 //| ADC_CTRL_ASYNMODE_MASK         //模式      0:同步模式    1：异步模式   屏蔽为0
                 | ADC_CTRL_RESOL(0x3)              //默认12位分辨率
                 //| ADC_CTRL_BYPASSCAL_MASK        //采样校准  0:启用校准功能    1：关闭校准   屏蔽为0
                 | ADC_CTRL_TSAMP(0)                //采样周期
                 );

    ADC0->STARTUP = ADC_STARTUP_ADC_ENA_MASK;           //开启ADC
    systick_delay_us(10);                               //必要延时
    if (!(ADC0->STARTUP & ADC_STARTUP_ADC_ENA_MASK))
    {
        ASSERT(0);//ADC没有上电 进入断言失败
    }
    
    
    ADC0->CALIB = ADC_CALIB_CALIB_MASK;                 //ADC校准
    while(ADC_CALIB_CALIB_MASK == (ADC0->CALIB & ADC_CALIB_CALIB_MASK));
    
    ADC0->STARTUP |= ADC_STARTUP_ADC_INIT_MASK;         //ADC初始化
    while(ADC_STARTUP_ADC_INIT_MASK == (ADC0->STARTUP & ADC_STARTUP_ADC_INIT_MASK));
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC转换
//  @param      ch          ADC通道号
//  @param      resolution  ADC分辨率
//  @return     void
//  Sample usage:           adc_test = adc_convert(ADC_CH0_A10,ADC_12BIT);     //采集ADC通道0电压值，分辨率12位     电压=adc_test*3300/2^n*(毫伏)   n为ADC的分辨率
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_convert(ADCCH_enum ch, ADCRES_enum resolution)
{
    ADC0->CTRL &= ~ADC_CTRL_RESOL_MASK;
    ADC0->CTRL |= ADC_CTRL_RESOL(resolution);   //分辨率
    
    ADC0->SEQ_CTRL[1] = 0;
    ADC0->SEQ_CTRL[1] =( 0
                        | ADC_SEQ_CTRL_CHANNELS(1<<ch)  //设置通道
                        | ADC_SEQ_CTRL_SEQ_ENA_MASK
                        | ADC_SEQ_CTRL_TRIGPOL_MASK
                        | ADC_SEQ_CTRL_SINGLESTEP_MASK
                       );

    ADC0->SEQ_CTRL[1] |= ADC_SEQ_CTRL_START_MASK;               //启动ADC转换

    while(!(ADC_SEQ_GDAT_DATAVALID_MASK & ADC0->SEQ_GDAT[1]));  //等待转换完成
    return ((ADC0->SEQ_GDAT[1]&ADC_SEQ_GDAT_RESULT_MASK)>>(ADC_SEQ_GDAT_RESULT_SHIFT+(3-resolution)*2));
}





