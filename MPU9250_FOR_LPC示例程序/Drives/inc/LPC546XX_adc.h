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

#ifndef _LPC546XX_adc_h
#define _LPC546XX_adc_h

//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{
    ADC_CH0_A10,    //A10
    ADC_CH1_A11,    //A11   与下载口冲突
    ADC_CH2_A12,    //A12   与下载口冲突
    ADC_CH3_A15,    //A15
    ADC_CH4_A16,    //A16
    ADC_CH5_A31,    //A31
    ADC_CH6_B0 ,    //B0
    ADC_CH7,        //P2_0    LQFP100封装没有这个引脚
    ADC_CH8,        //P2_1    LQFP100封装没有这个引脚
    ADC_CH9,        //P3_21   LQFP100封装没有这个引脚
    ADC_CH10,       //P3_22   LQFP100封装没有这个引脚
    ADC_CH11_A23,   //A23   
}ADCCH_enum;

//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{
    ADC_6BIT,     //6位分辨率
    ADC_8BIT,     //8位分辨率
    ADC_10BIT,    //10位分辨率
    ADC_12BIT,    //12位分辨率

}ADCRES_enum;


void    adc_init(ADCCH_enum ch);
uint16  adc_convert(ADCCH_enum ch, ADCRES_enum resolution);


#endif
