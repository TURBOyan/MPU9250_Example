/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MRT(多速率定时器)
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-11-21
 ********************************************************************************************************************/


#include "common.h"
#include "LPC546XX_mrt.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MRT周期中断模式初始化
//  @param      mrtchx      MRT通道号
//  @param      time        周期中断时间
//  @return     void
//  Sample usage:           mrt_pit_init(MRT_CH0,1000);         //无需用户调用 请使用H文件内的宏定义
//-------------------------------------------------------------------------------------------------------------------
void mrt_pit_init(MRTNUM_enum mrtchx, uint32 time)
{
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_MRT_MASK;      //打开MRT时钟
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_MRT_RST_MASK;  //清除MRT复位时钟
    
    ASSERT(time <= MRT_CHANNEL_TIMER_VALUE_MASK);//断言
    
    MRT0->CHANNEL[mrtchx].CTRL = MRT_CHANNEL_CTRL_INTEN_MASK | MRT_CHANNEL_CTRL_MODE(0);//PIT模式 打开中断
    MRT0->CHANNEL[mrtchx].INTVAL = time;        //设置间隔时间 定时器停止后载入时间 并启动定时器
    enable_irq(MRT0_IRQn);     				    //开启RIT中断
    //set_irq_priority(MRT0_IRQn,0);//设置优先级 越低优先级越高
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      MRT延时函数
//  @param      mrtchx      MRT通道号
//  @param      time        周期中断时间
//  @return     void
//  Sample usage:           mrt_delay(MRT_CH0,1000);            //无需用户调用 请使用H文件内的宏定义
//-------------------------------------------------------------------------------------------------------------------
void mrt_delay(MRTNUM_enum mrtchx, uint32 time)
{
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_MRT_MASK;      //打开MRT时钟
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_MRT_RST_MASK;  //清除MRT复位时钟
    
    ASSERT(time <= MRT_CHANNEL_TIMER_VALUE_MASK);//断言

    MRT0->CHANNEL[mrtchx].CTRL = MRT_CHANNEL_CTRL_MODE(1);      //一次中断模式模式

    MRT0->CHANNEL[mrtchx].INTVAL = time | MRT_CHANNEL_INTVAL_LOAD_MASK;        //设置间隔时间 立即载入时间 并启动定时器
    while(!MRT_FLAG_READ(mrtchx));              //等待时间到
    MRT_FLAG_CLR(mrtchx);                       //清除标志位
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      MRT开始计时
//  @param      mrtchx      MRT通道号
//  @param      time        周期中断时间
//  @return     void
//  Sample usage:           mrt_start(MRT_CH0);            
//-------------------------------------------------------------------------------------------------------------------
void mrt_start(MRTNUM_enum mrtchx)
{
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_MRT_MASK;      //打开MRT时钟
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_MRT_RST_MASK;  //清除MRT复位时钟
    
    MRT0->CHANNEL[mrtchx].CTRL = MRT_CHANNEL_CTRL_MODE(1);      //一次中断模式模式
    MRT0->CHANNEL[mrtchx].INTVAL = MRT_CHANNEL_INTVAL_IVALUE_MASK | MRT_CHANNEL_INTVAL_LOAD_MASK;//设置间隔时间 立即载入时间 并启动定时器
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MRT获取计时时间
//  @param      mrtchx      MRT通道号
//  @return     void
//  Sample usage:           uint32 time = mrt_get(MRT_CH0);     //无需用户调用 请使用H文件内的宏定义
//-------------------------------------------------------------------------------------------------------------------
uint32 mrt_get(MRTNUM_enum mrtchx)
{
    return (MRT_CHANNEL_INTVAL_IVALUE_MASK - MRT0->CHANNEL[mrtchx].TIMER); //如果返回的是0，则计时已经超出
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MRT计时清除
//  @param      mrtchx      MRT通道号
//  @return     void
//  Sample usage:           清除计时并重新启动
//-------------------------------------------------------------------------------------------------------------------
void mrt_clean(MRTNUM_enum mrtchx)
{
    MRT0->CHANNEL[mrtchx].INTVAL = MRT_CHANNEL_INTVAL_IVALUE_MASK | MRT_CHANNEL_INTVAL_LOAD_MASK;//设置间隔时间 立即载入时间 并启动定时器
}





