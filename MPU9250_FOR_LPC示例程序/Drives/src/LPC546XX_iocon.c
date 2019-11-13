/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		引脚复用设置
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
     
//-------------------------------------------------------------------------------------------------------------------
//  @brief      引脚配置及复用功能初始化
//  @param      pin         选择引脚（A0-A31  B0-B31）
//  @param      cfg         配置参数 查看h文件pin_config枚举内容
//  @return     void
//  Sample usage:           iocon_init(A8,ALT0 | PULLUP | DIGITAL);       // A8 引脚设置为通用IO且上拉
//-------------------------------------------------------------------------------------------------------------------
void iocon_init(PIN_enum pin, uint32 cfg)
{
    uint16 port;
    uint16 pinn;
    
    if(A11 == pin || A12 == pin)    
    {
        ASSERT(0);
        while(1);   //不允许使用A11 A12引脚，避免SWD出现问题。
    }
    port = pin>>5;
    pinn = pin&0x1f;
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_IOCON_MASK;        //打开IOCON时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_IOCON_RST_MASK;    //清除复位IOCON时钟
        
        
    IOCON->PIO[port][pinn] = cfg;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      引脚配置初始化（不改变原有复用功能，其余配置均会被覆写）
//  @param      pin         选择引脚（A0-A31  B0-B31）
//  @param      cfg         配置参数 查看h文件pin_config枚举内容
//  @return     void
//  Sample usage:           iocon_init(A8,PULLUP | DIGITAL);       // A8 引脚设置为数字模式且上拉
//-------------------------------------------------------------------------------------------------------------------
void iocon_init_noalt(PIN_enum pin, uint32 cfg)
{
    uint16 port;
    uint16 pinn;
    
    if(A11 == pin || A12 == pin)    
    {
        ASSERT(0);
        while(1);   //不允许使用A11 A12引脚，避免SWD出现问题。
    }
    
    port = pin>>5;
    pinn = pin&0x1f;
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_IOCON_MASK;        //打开IOCON时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_IOCON_RST_MASK;    //清除复位IOCON时钟
    
    IOCON->PIO[port][pinn] = (IOCON->PIO[port][pinn] & IOCON_PIO_FUNC_MASK) | (~((uint32)IOCON_PIO_FUNC_MASK) & cfg);
}





