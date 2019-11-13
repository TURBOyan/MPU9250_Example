/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SYSTEM_LPC54606
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
#include "LPC546XX_config.h"
#include "LPC546XX_uart.h"
#include "SYSTEM_LPC54606.h"


void clk_out(void)
{
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_IOCON_MASK;        //打开IOCON时钟
    IOCON->PIO[0][26] = (0
                       | IOCON_PIO_FUNC(2)          //设置引脚复用为第二功能 CLK_OUT
                       | IOCON_PIO_MODE(2)          //上拉
                       | IOCON_PIO_DIGIMODE(1)      //数字模式
                       | IOCON_PIO_SLEW(1));        //高速率
    
    SYSCON->CLKOUTSELA = SYSCON_CLKOUTSELA_SEL(0);  //选择CLK_OUT时钟源 0:main_clk  1:clk_in 3:fro_hf 4:pll_clk
    
    SYSCON->CLKOUTDIV = SYSCON_CLKOUTDIV_HALT(1);   //暂停CLK_OUT输出
    SYSCON->CLKOUTDIV = (0
                       | SYSCON_CLKOUTDIV_HALT(1)   //暂停CLK_OUT输出
                       | SYSCON_CLKOUTDIV_DIV(200-1)//设置分频系数
                       | SYSCON_CLKOUTDIV_RESET(1));//复位CLK_OUT分频器以加载新的分频系数
    while(!(SYSCON->CLKOUTDIV & SYSCON_CLKOUTDIV_REQFLAG_MASK));//等待分频系数加载完毕
    SYSCON->CLKOUTDIV = (0
                       | SYSCON_CLKOUTDIV_HALT(0)   //恢复CLK_OUT输出
                       | SYSCON_CLKOUTDIV_DIV(200-1)//设置分频系数
                       | SYSCON_CLKOUTDIV_RESET(0)  //清零复位
                       | SYSCON_CLKOUTDIV_REQFLAG(0));//清零加载完成标志
}

void SystemInit (void) 
{
    extern void *__Vectors;

#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))         
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));      //开启FPU
#endif 
    
    if(LPC546XX_CLOSE_ISP)
    {
        SYSCON->AHBCLKCTRLSET[2] = SYSCON_AHBCLKCTRL_OTP_MASK;      //打开OTP时钟
        SYSCON->PRESETCTRLCLR[2] = SYSCON_PRESETCTRL_OTP_RST_SHIFT; //清除复位时钟
        (void)OTP_API->otpInit();   
        (void)OTP_API->otpEnableBankReadLock(3,1,0,0);          //允许读取
        if(!(OTPC->ECRP & OTPC_ECRP_CRP_ISP_DISABLE_PIN_MASK))  //如果没有禁用ISP，则禁用ISP通过引脚启动
        {
            (void)OTP_API->otpInit();                           //初始化
            (void)OTP_API->otpEnableBankWriteMask(0x8);         //启用3区域
            (void)OTP_API->otpEnableBankWriteLock(3,1,0,0);     //允许写入
            (void)OTP_API->otpProgramReg(3,0,0x40);
        }
        (void)OTP_API->otpDisableBankWriteMask(0x8);            //禁用3区域
        SYSCON->AHBCLKCTRLCLR[2] = SYSCON_AHBCLKCTRL_OTP_MASK;  //关闭OTP时钟
    }
    
    
    SCB->VTOR = (uint32_t) &__Vectors;

    SYSCON->ARMTRACECLKDIV = 0; //打开trace时钟

    SYSCON->AHBCLKCTRLSET[0] = (0                       
                              | SYSCON_AHBCLKCTRL_SRAM1_MASK    //打开SRAM1时钟
                              | SYSCON_AHBCLKCTRL_SRAM2_MASK    //打开SRAM2时钟
                              | SYSCON_AHBCLKCTRL_SRAM3_MASK);  //打开SRAM3时钟

    pll_init(); //PLL初始化并选择PLL输出作为main_clk输入
  
#if (1 == CLK_OUT)
    clk_out();  //通过P026 输出main_clk/200时钟   P026输出的频率乘200等于main_clk频率
#endif
    

}

void NMI_Handler(void)
{
    ;
}

void HardFault_Handler(void)
{
#if defined(PRINTF) && defined(DEBUG_ASSERT)
    printf("硬件上访，可能访问了未初始化的外设，或者数组越界访问");
#endif
    while(1);
}
