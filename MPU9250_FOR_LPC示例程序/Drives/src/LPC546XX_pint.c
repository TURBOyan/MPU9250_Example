/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		引脚中断
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
#include "LPC546XX_pint.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PINT中断初始化
//  @param      pint        PINT通道(PINT_CH0-PINT_CH7) 注意默认的情况下摄像头会使用PINT_CH7中断
//  @param      pin         选择引脚（A0-A31  B0-B31）
//  @param      trigger     触发方式 查看common.h文件TRIGGER_enum枚举内容
//  @return     void
//  Sample usage:           pint_init(PINT_CH0,A5,RISING);     //初始化PINT通道0为上升沿触发    PINT通道0使用A5引脚
//-------------------------------------------------------------------------------------------------------------------
void pint_init(PINTNUM_enum pint, PIN_enum pin, TRIGGER_enum trigger)
{
    uint16 port;
    uint16 pinn;
    port = pin>>5;
    pinn = pin&0x1f;
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_INPUTMUX_MASK; //打开多路复用时钟
    INPUTMUX->PINTSEL[pint] = INPUTMUX_PINTSEL_INTPIN((uint8)port*32 + pinn);
    SYSCON->AHBCLKCTRLCLR[0] = SYSCON_AHBCLKCTRL_INPUTMUX_MASK; //关闭多路复用时钟
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_PINT_MASK;     //打开PINT时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_PINT_RST_MASK; //清除PINT复位时钟
    
    gpio_init(pin,GPI,0,PULLUP);
    
    PINT_RISE_FLAG_CLEAR(pint);                                 //清除标志位
    PINT_FALL_FLAG_CLEAR(pint);                                 //清除标志位
    
    if(LOW == trigger || HIGH == trigger)
    {
        PINT->ISEL |= (uint32)(1)<<pint;                        //电平触发模式
    }
    else
    {
        PINT->ISEL &= (~((1UL)<<pint) & PINT_ISEL_PMODE_MASK);  //边沿触发模式
    }
    
    if(RISING == trigger || BOTH == trigger)
    {
        PINT->SIENR |= (uint32)(1)<<pint;                       //上升沿或者电平触发中断
    }
    else
    {
        PINT->CIENR |= (uint32)(1)<<pint;                       //关闭上升沿或者电平触发中断
    }
    
    if(FALLING == trigger || HIGH == trigger || BOTH == trigger)
    {
        PINT->SIENF |= (uint32)(1)<<pint;                       //下降沿或者高电平触发中断  
    }
    else
    {
        PINT->CIENF |= (uint32)(1)<<pint;                       //低电平触发中断
    }
    
    PINT_IST_FLAG_CLEAR (pint);                                 //清除标志位
    
    //enable_irq(PIN_INT0_IRQn);//开中断 PIN_INT0_IRQn - PIN_INT7_IRQn
//    set_irq_priority(PIN_INT0_IRQn,0);//设置优先级 越低优先级越高
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      使能PINT中断开关
//  @param      pint        PINT通道(PINT_CH0-PINT_CH7)
//  @return     void
//  Sample usage:           pint_enable_irq(PINT_CH0);      //开启通道0中断开关
//-------------------------------------------------------------------------------------------------------------------
void pint_enable_irq(PINTNUM_enum pint)
{
    if(PINT_CH4 > pint) enable_irq((IRQn_Type)(PIN_INT0_IRQn+(IRQn_Type)pint));
    else                enable_irq((IRQn_Type)(PIN_INT4_IRQn+(IRQn_Type)(pint-4)));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁用PINT中断开关
//  @param      pint        PINT通道(PINT_CH0-PINT_CH7)
//  @return     void
//  Sample usage:           pint_enable_irq(PINT_CH0);      //开启通道0中断开关
//-------------------------------------------------------------------------------------------------------------------
void pint_disable_irq(PINTNUM_enum pint)
{
    if(PINT_CH4 > pint) disable_irq((IRQn_Type)(PIN_INT0_IRQn+(IRQn_Type)pint));
    else                disable_irq((IRQn_Type)(PIN_INT4_IRQn+(IRQn_Type)(pint-4)));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PINT中断关闭
//  @param      pint        PINT通道(PINT_CH0-PINT_CH7)
//  @return     void
//  Sample usage:           pint_close(PINT_CH0);           //关闭PINT通道0中断功能
//-------------------------------------------------------------------------------------------------------------------
void pint_close(PINTNUM_enum pint)
{
    PINT->ISEL &= (~((1UL)<<pint) & PINT_ISEL_PMODE_MASK);  //边沿触发模式
    PINT->CIENR |= (uint32)(1)<<pint;                       //关闭上升沿或者电平触发中断
    PINT->CIENF |= (uint32)(1)<<pint;                       //低电平触发中断
    pint_disable_irq(pint);
}



