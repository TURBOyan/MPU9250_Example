/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		标准计数/定时器
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#include "common.h"
#include "LPC546XX_pll.h"
#include "LPC546XX_iocon.h"
#include "LPC546XX_ctimer.h"


CTIMER_Type     * TIMERN[] = CTIMER_BASE_PTRS;
const uint32 period_ch[]={TIMER0_PWM_PERIOD_CH,TIMER1_PWM_PERIOD_CH,TIMER2_PWM_PERIOD_CH,TIMER3_PWM_PERIOD_CH,TIMER4_PWM_PERIOD_CH};
const uint32 duty_max[]={TIMER0_PWM_DUTY_MAX,TIMER1_PWM_DUTY_MAX,TIMER2_PWM_DUTY_MAX,TIMER3_PWM_DUTY_MAX,TIMER4_PWM_DUTY_MAX};

void ctimer_clock(CTIMER_enum ctimern)
{
    //开启时钟
    switch(ctimern)
    {
        case TIMER0:
        {
            SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_CTIMER0_MASK;      //打开CTIMER0时钟
            SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_CTIMER0_RST_MASK;  //清除CTIMER0复位时钟
        }break;
        
        case TIMER1:
        {
            SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_CTIMER1_MASK;     //打开CTIMER1时钟
            SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_CTIMER1_RST_MASK; //清除CTIMER1复位时钟
        }break;
        
        case TIMER2:
        {
            SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_CTIMER2_MASK;     //打开CTIMER2时钟
            SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_CTIMER2_RST_MASK; //清除CTIMER2复位时钟
        }break;
        
        case TIMER3:
        {
            SYSCON->ASYNCAPBCTRL = SYSCON_ASYNCAPBCTRL_ENABLE_MASK;
            ASYNC_SYSCON->ASYNCAPBCLKSELA = ASYNC_SYSCON_ASYNCAPBCLKSELA_SEL(0);
            ASYNC_SYSCON->ASYNCAPBCLKCTRLSET = ASYNC_SYSCON_ASYNCAPBCLKCTRL_CTIMER3_MASK;     //打开CTIMER3时钟
            ASYNC_SYSCON->ASYNCPRESETCTRLCLR = ASYNC_SYSCON_ASYNCPRESETCTRL_CTIMER3_MASK;     //清除CTIMER3复位时钟
        }break;
        
        case TIMER4:
        {
            SYSCON->ASYNCAPBCTRL = SYSCON_ASYNCAPBCTRL_ENABLE_MASK;
            ASYNC_SYSCON->ASYNCAPBCLKSELA = ASYNC_SYSCON_ASYNCAPBCLKSELA_SEL(0);
            ASYNC_SYSCON->ASYNCAPBCLKCTRLSET = ASYNC_SYSCON_ASYNCAPBCLKCTRL_CTIMER4_MASK;     //打开CTIMER4时钟
            ASYNC_SYSCON->ASYNCPRESETCTRLCLR = ASYNC_SYSCON_ASYNCPRESETCTRL_CTIMER4_MASK;     //清除CTIMER4复位时钟
        }break;
    }
}

void ctimer_pwmmux(CTIMER_PWMCH_enum pwmch)
{
    switch(pwmch/8)
    {
        case TIMER0:
        {
            if      (TIMER0_PWMCH0_A0  == pwmch)  iocon_init(A0, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH0_A30 == pwmch)  iocon_init(A30,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH1_A3  == pwmch)  iocon_init(A3, ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH1_A31 == pwmch)  iocon_init(A31,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH2_A19 == pwmch)  iocon_init(A19,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH3_B2  == pwmch)  iocon_init(B2, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH3_B27 == pwmch)  iocon_init(B27,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER1:
        {
            if      (TIMER1_PWMCH0_A18 == pwmch)  iocon_init(A18,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH0_B10 == pwmch)  iocon_init(B10,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH1_A20 == pwmch)  iocon_init(A20,ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH1_B12 == pwmch)  iocon_init(B12,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH2_A23 == pwmch)  iocon_init(A23,ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH2_B14 == pwmch)  iocon_init(B14,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH3_B16 == pwmch)  iocon_init(B16,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER2:
        {
            if      (TIMER2_PWMCH0_A10 == pwmch)  iocon_init(A10,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH0_B5  == pwmch)  iocon_init(B5, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH1_B4  == pwmch)  iocon_init(B4, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH1_B6  == pwmch)  iocon_init(B6, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH2_A11 == pwmch)  iocon_init(A11,ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH2_B7  == pwmch)  iocon_init(B7, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH3_A29 == pwmch)  iocon_init(A29,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH3_B22 == pwmch)  iocon_init(B22,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER3:
        {
            if      (TIMER3_PWMCH0_A5  == pwmch)  iocon_init(A5, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH1_B19 == pwmch)  iocon_init(B19,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH2_A27 == pwmch)  iocon_init(A27,ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH2_B21 == pwmch)  iocon_init(B21,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH3_A21 == pwmch)  iocon_init(A21,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH3_A23 == pwmch)  iocon_init(A23,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER4:
        {
            if      (TIMER4_PWMCH0_A6  == pwmch)  iocon_init(A6, ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
    }
}

void ctimer_countmux(CTIMER_COUNTCH_enum pwmch)
{
    switch(pwmch/8)
    {
        case TIMER0:
        {
            if      (TIMER0_COUNT0_A1  == pwmch)  iocon_init(A1 ,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT0_A13 == pwmch)  iocon_init(A13,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT1_A2  == pwmch)  iocon_init(A2 ,ALT2 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT1_A14 == pwmch)  iocon_init(A14,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT2_A28 == pwmch)  iocon_init(A28,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT3_B1  == pwmch)  iocon_init(B1 ,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT3_B26 == pwmch)  iocon_init(B26,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
        
        case TIMER1:
        {
            if      (TIMER1_COUNT0_A16 == pwmch)  iocon_init(A16,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER1_COUNT0_B9  == pwmch)  iocon_init(B9 ,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER1_COUNT1_B11 == pwmch)  iocon_init(B11,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER1_COUNT2_B13 == pwmch)  iocon_init(B13,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER1_COUNT3_B15 == pwmch)  iocon_init(B15,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
        
        case TIMER2:
        {
            if      (TIMER2_COUNT0_A24 == pwmch)  iocon_init(A24,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER2_COUNT1_A25 == pwmch)  iocon_init(A25,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER2_COUNT2_A10 == pwmch)  iocon_init(A10,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER2_COUNT3_A28 == pwmch)  iocon_init(A28,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
        
        case TIMER3:
        {
            if      (TIMER3_COUNT0_A4  == pwmch)  iocon_init(A4 ,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT1_A6  == pwmch)  iocon_init(A6 ,ALT2 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT2_A26 == pwmch)  iocon_init(A26,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT2_B20 == pwmch)  iocon_init(B20,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT3_A20 == pwmch)  iocon_init(A20,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT3_A22 == pwmch)  iocon_init(A22,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
        
        case TIMER4:
        {
            if      (TIMER4_COUNT0_A15 == pwmch)  iocon_init(A15,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器PWM模式初始化
//  @param      pwmch       PWM通道号及引脚
//  @param      freq        PWM频率
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           ctimer_pwm_init(TIMER0_PWMCH0_A0, 50, 5000);     //初始化定时器0  0通道 使用引脚A0为PWM模式  频率50HZ   占空比为百分之 5000/TIMER0_PWM_DUTY_MAX*100
//-------------------------------------------------------------------------------------------------------------------
void ctimer_pwm_init(CTIMER_PWMCH_enum pwmch, uint32 freq, uint32 duty)
{   
    uint32 match_temp;
    uint32 period_temp; 
    uint8  ctimer_num;

    ctimer_num = pwmch/8;
    
    
    ASSERT(period_ch[ctimer_num] != pwmch);    //断言 初始化的通道不能为周期通道
    ASSERT(duty_max[ctimer_num] >= duty);      //断言 占空比不能超过最大占空比
    
    //开启时钟
    ctimer_clock((CTIMER_enum)(ctimer_num));
    //引脚复用设置
    ctimer_pwmmux(pwmch);

    //计算预分频值与匹配寄存器值
    
    period_temp = (uint32)main_clk_mhz*1000*1000/freq;
    match_temp = (long long)period_temp*(duty_max[ctimer_num]-duty)/duty_max[ctimer_num];
    
    //TIMER寄存器配置
    TIMERN[ctimer_num]->TCR  &= ~CTIMER_TCR_CEN_MASK;
    
    TIMERN[ctimer_num]->PR = 0;                                    //设置预分频寄存器
    TIMERN[ctimer_num]->MR [(pwmch%8)/2] = match_temp;             //设置占空比
    TIMERN[ctimer_num]->MSR[(pwmch%8)/2] = match_temp;             //设置占空比
    TIMERN[ctimer_num]->MR[(period_ch[ctimer_num]%8)/2] = period_temp;//设置周期
    TIMERN[ctimer_num]->PWMC |= 1<<((pwmch%8)/2);
    TIMERN[ctimer_num]->EMR |= (0x2<<CTIMER_EMR_EMC0_SHIFT)<<((pwmch%8)&0xfe);
    TIMERN[ctimer_num]->MCR |= 1<<((period_ch[ctimer_num]%8)/2*3+1);
    TIMERN[ctimer_num]->MCR |= 1<<((pwmch%8)/2+24);
    
    TIMERN[ctimer_num]->TCR  = CTIMER_TCR_CRST_MASK;
    TIMERN[ctimer_num]->TCR  = CTIMER_TCR_CEN_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器PWM占空比更改
//  @param      pwmch       PWM通道号及引脚
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           ctimer_pwm_duty(TIMER0_PWMCH0_A0, 5000);     //初始化定时器0  0通道 使用引脚A0 占空比为百分之 5000/TIMER0_PWM_DUTY_MAX*100
//-------------------------------------------------------------------------------------------------------------------
void ctimer_pwm_duty(CTIMER_PWMCH_enum pwmch, uint32 duty)
{
    uint32 match_temp;
    uint32 period_temp; 
    uint8  ctimer_num;

    ctimer_num = pwmch/8;
    if(duty_max[ctimer_num] < duty)    duty = duty_max[ctimer_num];
    switch(ctimer_num)
    {
        case 0: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
        case 1: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
        case 2: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
        case 3: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
        case 4: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
    }
    
    
    period_temp = TIMERN[ctimer_num]->MR[(period_ch[ctimer_num]%8)/2];  //读取周期
    match_temp = (long long)period_temp*(duty_max[ctimer_num]-duty)/duty_max[ctimer_num];

    TIMERN[ctimer_num]->MSR[(pwmch%8)/2] = match_temp;               //设置占空比
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器PWM频率更改
//  @param      pwmch       PWM通道号及引脚
//  @param      freq        PWM频率
//  @return     void
//  Sample usage:           ctimer_pwm_freq(TIMER0_PWMCH0_A0, 50);     //初始化定时器0  0通道 使用引脚A0  频率50HZ
//-------------------------------------------------------------------------------------------------------------------
void ctimer_pwm_freq(CTIMER_PWMCH_enum pwmch, uint32 freq)
{
    uint32 period_temp; 
    uint8  ctimer_num;

    ctimer_num = pwmch/8;
    
    period_temp = (uint32)main_clk_mhz*1000*1000/freq;
    TIMERN[ctimer_num]->TCR  &= ~CTIMER_TCR_CEN_MASK;
    TIMERN[ctimer_num]->MR[(period_ch[ctimer_num]%8)/2] = period_temp;  //设置周期
    TIMERN[ctimer_num]->TCR  = CTIMER_TCR_CRST_MASK;
    TIMERN[ctimer_num]->TCR  = CTIMER_TCR_CEN_MASK;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器外部引脚计数模式初始化
//  @param      countch     计数通道号及引脚
//  @return     void
//  Sample usage:           ctimer_count_init(TIMER0_COUNT0_A1);     //初始化定时器0  0通道 使用引脚A1  为计数模式
//-------------------------------------------------------------------------------------------------------------------
void ctimer_count_init(CTIMER_COUNTCH_enum countch)
{
    ctimer_clock((CTIMER_enum)(countch/8));
    ctimer_countmux(countch);
    
    TIMERN[countch/8]->TCR  &= ~CTIMER_TCR_CEN_MASK;
    
    TIMERN[countch/8]->CTCR = CTIMER_CTCR_CTMODE(0X01) | CTIMER_CTCR_CINSEL((countch%8)/2); 
    TIMERN[countch/8]->PR = 0;    
    
    TIMERN[countch/8]->TCR  = CTIMER_TCR_CRST_MASK;
    TIMERN[countch/8]->TCR  = CTIMER_TCR_CEN_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取计数数值
//  @param      countch     计数通道号及引脚
//  @return     uint32      返回计数值
//  Sample usage:           num = ctimer_count_read(TIMER0_COUNT0_A1);     
//-------------------------------------------------------------------------------------------------------------------
uint32 ctimer_count_read(CTIMER_COUNTCH_enum countch)
{
    return TIMERN[countch/8]->TC;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      清除计数数值
//  @param      countch     计数通道号及引脚
//  @return     void      
//  Sample usage:           timer_count_clean(TIMER0_COUNT0_A1);     
//-------------------------------------------------------------------------------------------------------------------
void ctimer_count_clean(CTIMER_COUNTCH_enum countch)
{
    TIMERN[countch/8]->TC = 0;
}



