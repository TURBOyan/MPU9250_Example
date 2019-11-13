/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		周期定时器(RIT)
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#include "common.h"
#include "LPC546XX_pit.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      周期中断初始化
//  @param      time        周期中断时间
//  @return     void
//  Sample usage:           无需用户调用，用户只需要使用h文件中的宏定义即可
//-------------------------------------------------------------------------------------------------------------------
void pit_init(long long time)
{
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_RIT_MASK;     //打开PIT时钟
    
    ASSERT(time <= RIT_COMPVAL_RICOMP_MASK);
    RIT->CTRL = 0;
    RIT->COMPVAL = time & 0xFFFFFFFF;
    RIT->COMPVAL_H = (time>>32) & 0xFFFF;
    RIT->CTRL = RIT_CTRL_RITENCLR_MASK | RIT_CTRL_RITEN_MASK;
    
    //enable_irq(RIT_IRQn);     								   //开启RIT中断
    //set_irq_priority(RIT_IRQn,0);//设置优先级 越低优先级越高
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      延时函数
//  @param      time        延时时间
//  @return     void
//  Sample usage:           无需用户调用，用户只需要使用h文件中的宏定义即可
//-------------------------------------------------------------------------------------------------------------------
void pit_delay(long long time)
{
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_RIT_MASK;     //打开PIT时钟
    
    ASSERT(time <= RIT_COMPVAL_RICOMP_MASK);
    RIT->CTRL = 0;
    RIT->COMPVAL = time & 0xFFFFFFFF;
    RIT->COMPVAL_H = (time>>32) & 0xFFFF;
    RIT->CTRL = RIT_CTRL_RITENCLR_MASK | RIT_CTRL_RITEN_MASK;
    disable_irq(RIT_IRQn);
    
    while(!(RIT->CTRL & RIT_CTRL_RITINT_MASK));
    RIT->CTRL = 0;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      计时函数
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void pit_start(void)
{
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_RIT_MASK;     //打开PIT时钟

    RIT->CTRL = 0;
    RIT->COMPVAL = 0xFFFFFFFF;
    RIT->COMPVAL_H = 0xFFFF;
    RIT->COUNTER_H = 0;
    RIT->COUNTER = 0;
    RIT->CTRL = RIT_CTRL_RITENCLR_MASK | RIT_CTRL_RITEN_MASK;
    disable_irq(RIT_IRQn);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      计时函数
//  @return     long long   当前计时时间
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
long long pit_get(void)
{
    long long time;
    
    time = RIT->COUNTER_H;
    time = time<<32 | RIT->COUNTER;
    
    return (time);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      计时清除
//  @return     void
//  Sample usage:           清除计数并关闭计时器
//-------------------------------------------------------------------------------------------------------------------
void pit_clean(void)
{
    RIT->CTRL = 0;
    RIT->COUNTER_H = 0;
    RIT->COUNTER = 0;
   //RIT->CTRL = RIT_CTRL_RITENCLR_MASK | RIT_CTRL_RITEN_MASK;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      周期中断反初始化
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void pit_deinit(void)
{
    RIT->CTRL = 0;
    SYSCON->AHBCLKCTRLCLR[1] = SYSCON_AHBCLKCTRL_RIT_MASK;     //打开PIT时钟
    disable_irq(RIT_IRQn);
}
