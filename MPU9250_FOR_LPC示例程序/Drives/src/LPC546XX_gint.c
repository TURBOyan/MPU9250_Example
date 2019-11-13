/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		GPIO0与GPIO1 组中断
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
#include "LPC546XX_gint.h"


GINT_Type * GINTN[] = GINT_BASE_PTRS;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO0与GPIO1 组中断初始化
//  @param      gint        组中断模块号（GROUP0,GROUP1）,一个模块对应一个中断函数
//  @param      pin         引脚 A0 - A31  B0 - B31
//  @param      trigger     触发方式 查看common.h文件TRIGGER_enum枚举内容
//  @return     void
//  Sample usage:           gint_init(GROUP0,A0,RISING,PULLUP);       //使用组中断0    A0  上升沿触发中断
//  @note                   建议使用方式一个组中断仅使用一个引脚中断，如果需要使用多个，每个信号需要为一样的触发方式
//-------------------------------------------------------------------------------------------------------------------
void gint_init(GINTN_enum gint, PIN_enum pin, TRIGGER_enum trigger)
{
    uint16 port;
    uint16 pinn;
    
    port = pin>>5;
    pinn = pin&0x1f;
    
    if((RISING == trigger) || (HIGH == trigger))    gpio_init(pin,GPI,0,PULLDOWN);
    else                                            gpio_init(pin,GPI,0,PULLUP);

    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_GINT_MASK;     //开启GINT时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_GINT_RST_MASK; //清除复位GINT时钟

    GINTN[gint]->PORT_POL[port] &= ~(uint32)(1<<pinn);
    GINTN[gint]->PORT_POL[port] |= ((RISING == trigger) || (HIGH == trigger))<<pinn;

    GINTN[gint]->PORT_ENA[port] |= 1<<pinn;
    
    GINTN[gint]->CTRL = ( 0
                        //| GINT_CTRL_COMB_MASK                                       //所有引脚使用或连接 屏蔽为或  否则为与
                        | GINT_CTRL_TRIG((LOW == trigger) || (HIGH == trigger))       //触发类型 0：边沿触发      1：电平触发
                        );
    
    //enable_irq(GINT0_IRQn);//开中断 GINT0_IRQn GINT1_IRQn
    //set_irq_priority(GINT0_IRQn,0);//设置优先级 越低优先级越高
    
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      启用引脚触发组合中断
//  @param      gint        组合中断模块号（GROUP0,GROUP1）
//  @param      pin         引脚 A0 - A31  B0 - B31
//  @return     void
//  Sample usage:           gint_disable(GROUP0,A0);       //启用A0触发组合中断0
//-------------------------------------------------------------------------------------------------------------------
void gint_enable(GINTN_enum gint, PIN_enum pin)
{
    uint16 port;
    uint16 pinn;
    
    port = pin>>5;
    pinn = pin&0x1f;
    
    GINTN[gint]->PORT_ENA[port] |= 1<<pinn;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁用引脚触发组合中断
//  @param      gint        组合中断模块号（GROUP0,GROUP1）
//  @param      pin         引脚 A0 - A31  B0 - B31
//  @return     void
//  Sample usage:           gint_disable(GROUP0,A0);       //禁用A0触发组合中断0
//-------------------------------------------------------------------------------------------------------------------
void gint_disable(GINTN_enum gint, PIN_enum pin)
{
    uint16 port;
    uint16 pinn;
    
    port = pin>>5;
    pinn = pin&0x1f;
    
    GINTN[gint]->PORT_ENA[port] &= ~(((uint32)1)<<pinn);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO0与GPIO1 组中断反初始化
//  @param      gint        组合中断模块号（GROUP0,GROUP1）
//  @return     void
//  Sample usage:           gint_deinit(GROUP0);       //反初始化组中断0
//-------------------------------------------------------------------------------------------------------------------
void gint_deinit(GINTN_enum gint)
{
    SYSCON->PRESETCTRLSET[0] = SYSCON_PRESETCTRL_GINT_RST_MASK; //复位GINT时钟
    SYSCON->AHBCLKCTRLCLR[0] = SYSCON_AHBCLKCTRL_GINT_MASK;     //关闭GINT时钟
    disable_irq((IRQn_Type)(GINT0_IRQn + (IRQn_Type)gint));
}


