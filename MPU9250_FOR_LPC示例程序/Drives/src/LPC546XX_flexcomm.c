/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		FLEXCOMM（多功能串行通信接口）
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/

#include "common.h"
#include "LPC546XX_flexcomm.h" 

FLEXCOMM_Type * FLEXCOMMN[] = FLEXCOMM_BASE_PTRS;

void flexcomm_clk_enable(FLEXCOMMN_enum flexcommn, FLEXCOMM_TYPE_enum type)
{
    vuint32  now_type;
    //校验当前的FLEXCOMM是否被其他外设占用
    if(FLEXCOMM_PSELID_LOCK_MASK & FLEXCOMMN[flexcommn]->PSELID)    
    {
        now_type = FLEXCOMMN[flexcommn]->PSELID & FLEXCOMM_PSELID_PERSEL_MASK;//1:USART 2:SPI 3:IIC 4:IIS
        ASSERT(0);//进入断言失败
    }

    if(1 != (SYSCON_FROHFCLKDIV_DIV_MASK & SYSCON->FROHFCLKDIV))
    {
        SYSCON->FROCTRL &= ~SYSCON_FROCTRL_HSPDCLK_MASK; //打开FRO_HF 设置为96MHz
        SYSCON->FROHFCLKDIV = SYSCON_FROHFCLKDIV_HALT_MASK;
        SYSCON->FROHFCLKDIV = SYSCON_FROHFCLKDIV_DIV(0x01)| SYSCON_FROHFCLKDIV_HALT_MASK | SYSCON_FROHFCLKDIV_RESET_MASK;
        while(!(SYSCON->FROHFCLKDIV & SYSCON_FROHFCLKDIV_REQFLAG_MASK));
        SYSCON->FROHFCLKDIV = SYSCON_FROHFCLKDIV_DIV(0x01);
    }
    SYSCON->FROCTRL |= SYSCON_FROCTRL_SEL(0x01) | SYSCON_FROCTRL_HSPDCLK(0x01); //打开FRO_HF 设置为96MHz
    
    SYSCON->FCLKSEL[flexcommn] = SYSCON_FCLKSEL_SEL(0x01);                      //选择FRO_HF_DIV作为FLEXCOMM时钟输入源
    
    if(FLEXCOMM_8 == flexcommn)
    {
        SYSCON->AHBCLKCTRLSET[2] = SYSCON_AHBCLKCTRL_FLEXCOMM8_MASK;            //打开FLEXCOMM时钟
        SYSCON->PRESETCTRLCLR[2] = SYSCON_PRESETCTRL_FC8_RST_MASK;              //清除复位FLEXCOMM时钟
    }
    else
    {
        SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_FLEXCOMM0_MASK<<flexcommn; //打开FLEXCOMM时钟
        SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_FC0_RST_MASK<<flexcommn;   //清除复位FLEXCOMM时钟
    }
    
    FLEXCOMMN[flexcommn]->PSELID |= (FLEXCOMM_PSELID_PERSEL(type) | FLEXCOMM_PSELID_LOCK_MASK);     
}

uint32 flexcomm_get_clk(FLEXCOMMN_enum flexcommn)
{
    return 48000000U;
}

void flexcomm_irq(FLEXCOMMN_enum flexcommn, uint8 status)
{
    if(status)
    {
        if(FLEXCOMM_8 == flexcommn)     enable_irq(FLEXCOMM8_IRQn);
        else                            enable_irq((IRQn_Type)(FLEXCOMM0_IRQn+(IRQn_Type)flexcommn));
    }
    else
    {
        if(FLEXCOMM_8 == flexcommn)     disable_irq(FLEXCOMM8_IRQn);
        else                            disable_irq((IRQn_Type)(FLEXCOMM0_IRQn+(IRQn_Type)flexcommn));
    }
}
