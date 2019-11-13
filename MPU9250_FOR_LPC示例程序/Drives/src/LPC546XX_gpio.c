/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		GPIO
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


//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO初始化
//  @param      pin         选择引脚（A0-A31  B0-B31）
//  @param      dir         引脚方向 GPI：输入   GPO：输出
//  @param      dat         引脚电平 0：低电平   1：高电平  设置为输入是该参数无效
//  @param      pull        设置上下拉电阻 NOPULL:不设置    PULLDOWN:下拉    PULLUP:上拉  NOPULL:没有上下拉电阻
//  @return     void
//  Sample usage:           gpio_init(A8,GPO,0,NOPULL);     // 初始化A8引脚为输出模式,输出低电平,没有上下拉电阻
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(PIN_enum pin, GPIODIR_enum dir, uint8 dat, uint32 pull)
{
    uint16 port;
    uint16 pinn;
    
    port = pin>>5;
    pinn = pin&0x1f;
    
    if(P0 == port)      
    {
        SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_GPIO0_MASK;        //打开GPIO0时钟
        SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_GPIO0_RST_MASK;    //清除复位GPIO0时钟
    }
    else if(P1 == port) 
    {
        SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_GPIO1_MASK;        //打开GPIO1时钟
        SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_GPIO1_RST_MASK;    //清除复位GPIO1时钟
    }
    
    iocon_init(pin,ALT0 | DIGITAL | pull);//设置复用功能为通用IO
    
    if(GPI == dir)
    {
        if(pin<29)  GPIO->DIRCLR[port] = 1<<pinn;
        else
        {
            DisableInterrupts;
            GPIO->DIR[port] &= ~((uint32)1<<pinn);
            EnableInterrupts;
        }
        
    }
    else if(GPO == dir)
    {
        if(pinn<29)  GPIO->DIRSET[port] = 1<<pinn;
        else
        {
            DisableInterrupts;
            GPIO->DIR[port] |= 1<<pinn;
            EnableInterrupts;
        }
        if(!dat)        GPIO->CLR[port]  = 1<<pinn;
        else            GPIO->SET[port]  = 1<<pinn;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO设置引脚上下拉电阻设置
//  @param      pin         选择引脚（A0-A31  B0-B31）
//  @param      pull        设置上下拉电阻 NOPULL:不设置    PULLDOWN:下拉    PULLUP:上拉  NOPULL:没有上下拉电阻
//  @return     void
//  Sample usage:           gpio_pull_set(A8,NOPULL);       // 设置A8引脚没有上下拉电阻
//-------------------------------------------------------------------------------------------------------------------
void gpio_pull_set(PIN_enum pin, uint8 pull)
{
    uint16 port;
    uint16 pinn;
    
    port = pin>>5;
    pinn = pin&0x1f;
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_IOCON_MASK;        //打开IOCON时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_IOCON_RST_MASK;    //清除复位IOCON时钟
    
    IOCON->PIO[port][pinn] = (IOCON->PIO[port][pinn] & ~((uint32)IOCON_PIO_MODE_MASK)) | pull;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO设置引脚电平
//  @param      pin         选择引脚（A0-A31  B0-B31）
//  @param      dat         引脚电平 0：低电平   1：高电平
//  @return     void
//  Sample usage:           gpio_set(A8,0);       // 设置A8引脚输出低电平
//-------------------------------------------------------------------------------------------------------------------
void gpio_set(PIN_enum pin, uint8 dat)
{
    if(!dat)        GPIO->CLR[pin>>5]  = 1<<(pin&0x1f);
    else            GPIO->SET[pin>>5]  = 1<<(pin&0x1f);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO设置引脚方向
//  @param      pin         选择引脚（A0-A31  B0-B31）
//  @param      dir         引脚方向 GPI：输入   GPO：输出
//  @return     void
//  Sample usage:           gpio_dir(A8,GPI);       // 设置A8引脚为输入模式
//-------------------------------------------------------------------------------------------------------------------
void gpio_dir(PIN_enum pin, GPIODIR_enum dir)
{
    uint16 port;
    uint16 pinn;
    
    port = pin>>5;
    pinn = pin&0x1f;
    
    if(GPI == dir)
    {
        if(pinn<29)  GPIO->DIRCLR[port] = 1<<pinn;
        else
        {
            DisableInterrupts;
            GPIO->DIR[port] &= ~((uint32)1<<pinn);
            EnableInterrupts;
        }
    }
    else if(GPO == dir)
    {
        if(pinn<29)  GPIO->DIRSET[port] = 1<<pinn;
        else
        {
            DisableInterrupts;
            GPIO->DIR[port] |= 1<<pinn;
            EnableInterrupts;
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO获取引脚电平
//  @param      pin         选择引脚（A0-A31  B0-B31）
//  @return     uint8       引脚电平 0：低电平   1：高电平
//  Sample usage:           uint8 dat gpio_get(A8);  // 获取A8引脚电平
//-------------------------------------------------------------------------------------------------------------------
uint8 gpio_get(PIN_enum pin)
{
    return (GPIO->B[pin>>5][pin&0x1f]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO设置引脚切换(翻转)
//  @param      pin         选择引脚（A0-A31  B0-B31）
//  @return     void
//  Sample usage:           gpio_toggle(A8);       // 切换A8引脚电平
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle(PIN_enum pin)
{
    GPIO->NOT[pin>>5]  = 1<<(pin&0x1f);
}
