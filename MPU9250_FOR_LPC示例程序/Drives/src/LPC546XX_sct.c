/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SCTimer/PWM
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
#include "LPC546XX_pll.h"
#include "LPC546XX_sct.h"



void sct_input_mux(SCT_INPUT_MUX_enum mux_ch, SCT_IUPUT_PIN_enum input_pin)
{
    uint8 sct_gpin;

    sct_gpin = input_pin>>2;                                                //计算输入的SCT_GPIN模块号

    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_INPUTMUX_MASK;             //打开多路复用时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_MUX_RST_MASK;              //清除多路复用复位时钟
    INPUTMUX->SCT0_INMUX[mux_ch] = INPUTMUX_SCT0_INMUX_INP_N(sct_gpin);     //设置SCT输入复用通道的SCT_GPIN模块
    SYSCON->AHBCLKCTRLCLR[0] = SYSCON_AHBCLKCTRL_INPUTMUX_MASK;             //关闭多路复用时钟
    
    //设置SCT_GPIN  外部引脚
    switch(sct_gpin)
    {
        case 0:
        {
            if(SCT0_GPI0_A0       == input_pin)     iocon_init(A0,  ALT4 | PULLUP | DIGITAL | FILTEROFF);// | FILTEROFF
            else if(SCT0_GPI0_A13 == input_pin)     iocon_init(A13, ALT4 | PULLUP | DIGITAL | FILTEROFF);// | FILTEROFF
            else if(SCT0_GPI0_A24 == input_pin)     iocon_init(A24, ALT4 | PULLUP | DIGITAL | FILTEROFF);// | FILTEROFF
            else if(SCT0_GPI0_B5  == input_pin)     iocon_init(B5,  ALT4 | PULLUP | DIGITAL | FILTEROFF);// | FILTEROFF
        }break;                                                                            
                                                                                           
        case 1:                                                                            
        {                                                                                  
            if(SCT0_GPI1_A1       == input_pin)     iocon_init(A1,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI1_A14 == input_pin)     iocon_init(A14, ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI1_A25 == input_pin)     iocon_init(A25, ALT4 | PULLUP | DIGITAL | FILTEROFF);
        }break;                                                                            
                                                                                           
        case 2:                                                                            
        {                                                                                  
            if(SCT0_GPI2_A2       == input_pin)     iocon_init(A2,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI2_A20 == input_pin)     iocon_init(A20, ALT4 | PULLUP | DIGITAL | FILTEROFF);
        }break;                                                                            
                                                                                           
        case 3:                                                                            
        {                                                                                  
            if(SCT0_GPI3_A3       == input_pin)     iocon_init(A3,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI3_A21 == input_pin)     iocon_init(A21, ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI3_B6  == input_pin)     iocon_init(B6,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
        }break;                                                                            
                                                                                           
        case 4:                                                                            
        {                                                                                  
            if(SCT0_GPI4_A4       == input_pin)     iocon_init(A4,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI4_B0  == input_pin)     iocon_init(B0,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI4_B7  == input_pin)     iocon_init(B7,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
        }break;                                                                            
                                                                                           
        case 5:                                                                            
        {                                                                                  
            if(SCT0_GPI5_A5       == input_pin)     iocon_init(A5,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI5_B1  == input_pin)     iocon_init(B1,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI5_B22 == input_pin)     iocon_init(B22, ALT4 | PULLUP | DIGITAL | FILTEROFF);
        }break;                                                                            
                                                                                           
        case 6:                                                                            
        {                                                                                  
            if(SCT0_GPI6_A6       == input_pin)     iocon_init(A6,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI6_B2  == input_pin)     iocon_init(B2,  ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI6_B29 == input_pin)     iocon_init(B29, ALT3 | PULLUP | DIGITAL | FILTEROFF);
        }break;                                                                            
                                                                                           
        case 7:                                                                            
        {                                                                                  
            if(SCT0_GPI7_A12      == input_pin)     iocon_init(A12, ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI7_A17 == input_pin)     iocon_init(A17, ALT3 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI7_B19 == input_pin)     iocon_init(B19, ALT4 | PULLUP | DIGITAL | FILTEROFF);
            else if(SCT0_GPI7_B30 == input_pin)     iocon_init(B30, ALT3 | PULLUP | DIGITAL | FILTEROFF);
        }break;
    }

}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCT摄像头采集初始化
//  @param      pclk_mux_ch         像素时钟信号使用的SCT复用通道
//  @param      pclk_input_pin      像素时钟连接的引脚
//  @param      pclk_trigger        像素时钟触发 触发边沿选择    RISING和FALLING
//  @return     void
//  Sample usage:           sct_camera_dma(SCT_INPUT_MUX1, SCT0_GPI1_A14, RISING);     //初始化SCT  像素时钟使用SCT通道1 引脚使用A14  上升沿触发 
//-------------------------------------------------------------------------------------------------------------------
void sct_camera_dma(SCT_INPUT_MUX_enum pclk_mux_ch, SCT_IUPUT_PIN_enum pclk_input_pin, TRIGGER_enum pclk_trigger)
{
    uint8  pclk_trig;
    uint32 temp_ctrl;
    
    SYSCON->FROCTRL |= SYSCON_FROCTRL_SEL(0x01) | SYSCON_FROCTRL_HSPDCLK(0x01); //打开FRO_HF 设置为96MHz
    
    //开启SCT时钟及设置频率
    SYSCON->SCTCLKSEL = SYSCON_SCTCLKSEL_SEL(0x2);  //选择FRO_HF作为时钟源
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_SCT0_RST_MASK;
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_SCT0_MASK;
    
    sct_input_mux(pclk_mux_ch,  pclk_input_pin);    //设置SCT输入复用通道及引脚
    
    if(RISING == pclk_trigger)          pclk_trig = 1;
    else if(FALLING == pclk_trigger)    pclk_trig = 2;

    temp_ctrl = SCT0->CTRL;
    SCT0->CTRL |= SCT_CTRL_HALT_H_MASK | SCT_CTRL_HALT_L_MASK;  //暂停SCT
    
    SCT0->CONFIG = ( 0
                   | SCT_CONFIG_UNIFY(0)                        //双定时器模式
                   | SCT_CONFIG_CLKMODE(0)                      //使用System Clock驱动整个SCT 
                   | SCT_CONFIG_INSYNC(1<<SCT0_CAMERA_PCLK)     //启用同步模式 
                   );
    
    SCT0->STATE = (SCT0->STATE & SCT_STATE_STATE_H_MASK) | SCT_STATE_STATE_L(0U);   //状态清零
    SCT0->EVENT[pclk_mux_ch].STATE = 0xffff;                    //捕获到PCLK信号切换到SCT_STATE_WAIT_PCLK状态
    SCT0->EVENT[pclk_mux_ch].CTRL = ( 0 
                                    | SCT_EVENT_CTRL_HEVENT(0)                      //使用State_L状态机
                                    | SCT_EVENT_CTRL_OUTSEL(0)                      //设置为输入 接受输入引脚的触发
                                    | SCT_EVENT_CTRL_IOSEL(pclk_mux_ch)             //选择pclk_mux_ch为输入引脚
                                    | SCT_EVENT_CTRL_IOCOND(pclk_trig)              //上升沿触发  1：上升沿    2：下降沿
                                    | SCT_EVENT_CTRL_COMBMODE(2)                    //仅使用IO引脚产生的触发信号
                                    | SCT_EVENT_CTRL_STATELD(1)                     //修改方式：直接载入新值
                                    | SCT_EVENT_CTRL_STATEV(1)                      //事件发生后载入新的状态值
                                    );
    SCT0->DMA0REQUEST = SCT_DMA0REQUEST_DEV_0(1U<<pclk_mux_ch);                     //使能PCLK事件触发DMA
    SCT0->CTRL = temp_ctrl;//设置SCT控制寄存器
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      sct启动（开始采集摄像头数据）
//  @param      void
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void sct_start(void)
{
    SCT0->EVFLAG = (1<<SCT_INPUT_MUX1); //清标志位 
    SCT0->CTRL &= ~SCT_CTRL_HALT_L_MASK;//启动SCT
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      sct停止（停止采集摄像头数据）
//  @param      void
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void sct_stop(void)
{
    SCT0->CTRL |= SCT_CTRL_HALT_L_MASK; //停止SCT
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      sct停止后 重新启动
//  @param      void
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void sct_restart(void)
{
    SCT0->CTRL |= SCT_CTRL_HALT_L_MASK; //暂停SCT
    SCT0->EVFLAG = (1<<SCT_INPUT_MUX1); //清标志位 
    SCT0->CTRL &= ~SCT_CTRL_HALT_L_MASK;//启动SCT
}


//----------------------------------------------SCT  PWM输出函数-----------------------------------------------------

const uint32 sct_duty_max[]={SCT0_OUTPUT_CH0_DUTY_MAX,SCT0_OUTPUT_CH1_DUTY_MAX,SCT0_OUTPUT_CH2_DUTY_MAX,SCT0_OUTPUT_CH3_DUTY_MAX,SCT0_OUTPUT_CH4_DUTY_MAX,
                             SCT0_OUTPUT_CH5_DUTY_MAX,SCT0_OUTPUT_CH6_DUTY_MAX,SCT0_OUTPUT_CH7_DUTY_MAX,SCT0_OUTPUT_CH8_DUTY_MAX,SCT0_OUTPUT_CH9_DUTY_MAX};

void sct_pwm_mux(SCT_OUTPUT_PIN_enum pwm_pin)
{
    uint8 sct_out_ch;
    sct_out_ch = pwm_pin>>2;

    switch(sct_out_ch)
    {
        case 0:
        {
            if     (SCT0_OUT0_A2  == pwm_pin)       iocon_init(A2,  ALT3 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT0_A17 == pwm_pin)       iocon_init(A17, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT0_B4  == pwm_pin)       iocon_init(B4,  ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT0_B23 == pwm_pin)       iocon_init(B23, ALT2 | DIGITAL | FILTEROFF);
        }break;
        
        case 1:
        {
            if     (SCT0_OUT1_A3  == pwm_pin)       iocon_init(A3,  ALT3 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT1_A18 == pwm_pin)       iocon_init(A18, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT1_B8  == pwm_pin)       iocon_init(B8,  ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT1_B24 == pwm_pin)       iocon_init(B24, ALT2 | DIGITAL | FILTEROFF);
        }break;
        
        case 2:
        {
            if     (SCT0_OUT2_A15 == pwm_pin)       iocon_init(A15, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT2_A19 == pwm_pin)       iocon_init(A19, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT2_B9  == pwm_pin)       iocon_init(B9,  ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT2_B25 == pwm_pin)       iocon_init(B25, ALT2 | DIGITAL | FILTEROFF);
        }break;
        
        case 3:
        {
            if     (SCT0_OUT3_A22 == pwm_pin)       iocon_init(A22, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT3_A31 == pwm_pin)       iocon_init(A31, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT3_B10 == pwm_pin)       iocon_init(B10, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT3_B26 == pwm_pin)       iocon_init(B26, ALT2 | DIGITAL | FILTEROFF);
        }break;
        
        case 4:
        {
            if     (SCT0_OUT4_A23 == pwm_pin)       iocon_init(A23, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT4_B3  == pwm_pin)       iocon_init(B3,  ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT4_B17 == pwm_pin)       iocon_init(B17, ALT4 | DIGITAL | FILTEROFF);
        }break;
        
        case 5:
        {
            if     (SCT0_OUT5_A26 == pwm_pin)       iocon_init(A26, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT5_B18 == pwm_pin)       iocon_init(B18, ALT4 | DIGITAL | FILTEROFF);
        }break;
        
        case 6:
        {
            if     (SCT0_OUT6_A27 == pwm_pin)       iocon_init(A27, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT6_B31 == pwm_pin)       iocon_init(B31, ALT4 | DIGITAL | FILTEROFF);
        }break;
        
        case 7:
        {
            if     (SCT0_OUT7_A28 == pwm_pin)       iocon_init(A28, ALT4 | DIGITAL | FILTEROFF);
            else if(SCT0_OUT7_B19 == pwm_pin)       iocon_init(B19, ALT2 | DIGITAL | FILTEROFF);
        }break;
        
        case 8:
        {
            if     (SCT0_OUT8_A29 == pwm_pin)       iocon_init(A29, ALT4 | DIGITAL | FILTEROFF);
        }break;
        
        case 9:
        {
            if     (SCT0_OUT9_A30 == pwm_pin)       iocon_init(A30, ALT4 | DIGITAL | FILTEROFF);
        }break;
    }
    
    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCT PWM模式初始化
//  @param      pwm_pin     PWM通道号及引脚
//  @param      freq        PWM频率
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           sct_pwm_init(SCT0_OUT2_A15,50,5000);     //初始化SCT0 2通道 使用引脚A15为PWM模式  频率50HZ   占空比为百分之 5000/TIMER0_PWM_DUTY_MAX*100
//  @note                   由于摄像头的PCLK和PWM周期需要各占用一个通道，因此SCT一共十个通道最多只能输出8路PWM
//  @note                   需要特别注意，sct模块能够输出最多8路PWM，但是8路PWM频率都必须一致。所以适合用于控制多个电机
//-------------------------------------------------------------------------------------------------------------------
void sct_pwm_init(SCT_OUTPUT_PIN_enum pwm_pin, uint32 freq, uint32 duty)
{
    uint8  sct_out_ch;
    uint16 temp_div,prescale;
    uint32 temp,temp_res;
    uint32 temp_ctrl;
    uint32 match_temp;
    uint32 period_temp;
    uint32 sct_clk = main_clk_mhz*1000*1000;

    for(temp_div=1;temp_div<=256;temp_div++)
    {
        temp = sct_clk/freq/temp_div;
        if(temp<=(0xffff)) break;             //找到最佳参数
    }

    if(257 == temp_div)   ASSERT(0);        //断言失败 频率过低或者主频过高
    
    sct_out_ch = pwm_pin>>2;                //计算通道
    ASSERT(duty<=sct_duty_max[sct_out_ch]); //最大占空比断言
    
    
    prescale = temp_div - 1;                //计算分频系数
    period_temp = sct_clk/freq/temp_div;    //周期计数次数
    match_temp = duty*period_temp/sct_duty_max[sct_out_ch];//计算占空比匹配数

    SYSCON->FROCTRL |= SYSCON_FROCTRL_SEL(0x01) | SYSCON_FROCTRL_HSPDCLK(0x01); 
    SYSCON->SCTCLKSEL = SYSCON_SCTCLKSEL_SEL(0x2);
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_SCT0_RST_MASK;
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_SCT0_MASK; //开启SCT时钟

    sct_pwm_mux(pwm_pin);
    
    temp_ctrl = SCT0->CTRL;
    SCT0->CTRL |= SCT_CTRL_HALT_H_MASK | SCT_CTRL_HALT_L_MASK;  //暂停SCT
    
    temp_ctrl &= ~SCT_CTRL_PRE_H_MASK;             //清除分频系数
    temp_ctrl |= SCT_CTRL_PRE_H(prescale);         //设置分频系数
        
    SCT0->CONFIG = ( 0
                   | SCT_CONFIG_UNIFY(0)                    //双定时器模式
                   | SCT_CONFIG_CLKMODE(0)                  //使用System Clock驱动整个SCT 
                   | SCT_CONFIG_INSYNC(1<<SCT0_CAMERA_PCLK) //启用同步模式
                   );
    
    SCT0->LIMIT |= SCT_LIMIT_LIMMSK_H(1U << SCT0_PWM_PERIOD_CH);
    SCT0->REGMODE = 0;//匹配模式
    
    SCT0->STATE = (SCT0->STATE & SCT_STATE_STATE_L_MASK) | SCT_STATE_STATE_H(0U);   //状态清零
    
    //设置周期事件
    SCT0->SCTMATCH[SCT0_PWM_PERIOD_CH] = period_temp<<SCT_SCTMATCH_MATCHn_H_SHIFT;
    SCT0->SCTMATCHREL[SCT0_PWM_PERIOD_CH] = period_temp<<SCT_SCTMATCH_MATCHn_H_SHIFT;
    SCT0->EVENT[SCT0_PWM_PERIOD_CH].STATE = 1;
    SCT0->EVENT[SCT0_PWM_PERIOD_CH].CTRL = ( 0
                                           | SCT_EVENT_CTRL_MATCHSEL(SCT0_PWM_PERIOD_CH)
                                           | SCT_EVENT_CTRL_HEVENT(1)                   //使用State_H状态机
                                           | SCT_EVENT_CTRL_OUTSEL(1)                   //设置为输出 
                                           | SCT_EVENT_CTRL_IOSEL(SCT0_PWM_PERIOD_CH)   //选择输出的通道
                                           | SCT_EVENT_CTRL_COMBMODE(1)                 //仅使用指定的匹配项
                                           );
    //设置匹配事件
    SCT0->SCTMATCH[sct_out_ch] = match_temp<<SCT_SCTMATCH_MATCHn_H_SHIFT;               //匹配值
    SCT0->SCTMATCHREL[sct_out_ch] = match_temp<<SCT_SCTMATCH_MATCHn_H_SHIFT;            //匹配值
    SCT0->EVENT[sct_out_ch].STATE = 1;  
    SCT0->EVENT[sct_out_ch].CTRL = ( 0  
                                     | SCT_EVENT_CTRL_MATCHSEL(sct_out_ch)  
                                     | SCT_EVENT_CTRL_HEVENT(1)                         //使用State_H状态机
                                     | SCT_EVENT_CTRL_OUTSEL(1)                         //设置为输出 
                                     | SCT_EVENT_CTRL_IOSEL(sct_out_ch)                 //选择输出的通道
                                     | SCT_EVENT_CTRL_COMBMODE(1)                       //仅使用指定的匹配项
                                     );
    
    SCT0->OUT[sct_out_ch].CLR = 1<<sct_out_ch;             //设置清零 触发事件

    temp_res = SCT0->RES;                   //读取寄存器
    temp_res &= ~((uint32)SCT_RES_O0RES_MASK<<(2*sct_out_ch));
    if(0 == match_temp)
    {
        temp_res |= 2<<(2*sct_out_ch);  
        SCT0->RES = temp_res;               //同时触发
        SCT0->OUT[sct_out_ch].SET = 1<<sct_out_ch;             //设置置一 触发事件
    }
    else
    {
        temp_res |= 1<<(2*sct_out_ch);  
        SCT0->RES = temp_res;               //同时触发
        SCT0->OUT[sct_out_ch].SET = 1<<SCT0_PWM_PERIOD_CH;     //设置置一 触发事件
    }

    temp_ctrl &= ~SCT_CTRL_HALT_H_MASK;     //启动SCT
    SCT0->CTRL = temp_ctrl;                 //设置SCT控制寄存器
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCT PWM占空比设置
//  @param      pwm_pin     PWM通道号及引脚
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           sct_pwm_duty(SCT0_OUT2_A15,5000);     //SCT0 2通道 占空比设置为百分之 5000/TIMER0_PWM_DUTY_MAX*100
//-------------------------------------------------------------------------------------------------------------------
void sct_pwm_duty(SCT_OUTPUT_PIN_enum pwm_pin, uint32 duty)
{
    uint8  sct_out_ch;
    uint32 period_temp;
    uint32 match_temp;
    uint32 temp_res;
        
    sct_out_ch = pwm_pin>>2;                //计算通道
    ASSERT(duty<=sct_duty_max[sct_out_ch]); //最大占空比断言
    
    period_temp = SCT0->SCTMATCHREL[SCT0_PWM_PERIOD_CH]>>SCT_SCTMATCHREL_RELOADn_H_SHIFT;
    match_temp = duty*period_temp/sct_duty_max[sct_out_ch];     //计算占空比匹配数
    
    temp_res = SCT0->RES;                   //读取寄存器
    temp_res &= ~((uint32)SCT_RES_O0RES_MASK<<(2*sct_out_ch));
    if(0 == match_temp)
    {
        temp_res |= 2<<(2*sct_out_ch);  
        SCT0->RES = temp_res;               //同时触发
        SCT0->OUT[sct_out_ch].SET = 1<<sct_out_ch;             //设置置一 触发事件
    }
    else
    {
        temp_res |= 1<<(2*sct_out_ch);  
        SCT0->RES = temp_res;               //同时触发
        SCT0->OUT[sct_out_ch].SET = 1<<SCT0_PWM_PERIOD_CH;     //设置置一 触发事件
    }

    SCT0->SCTMATCHREL[sct_out_ch] = match_temp<<SCT_SCTMATCH_MATCHn_H_SHIFT;    //匹配值
}

