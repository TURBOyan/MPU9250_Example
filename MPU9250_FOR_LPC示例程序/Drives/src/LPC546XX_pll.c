/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		pll
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#include "common.h"
#include "LPC546XX_uart.h"
#include "LPC546XX_pll.h"
     
     
#define EX_REF_CLK  12000U //(定义外部参考时钟为12000KHZ)




#define NVALMAX (0x100U)
#define PVALMAX (0x20U)
#define MVALMAX (0x8000U)

#define PLL_MDEC_VAL_P (0U)                                      /*!<  MDEC is in bits  16 downto 0 */
#define PLL_MDEC_VAL_M (0x1FFFFUL << PLL_MDEC_VAL_P)             /*!<  NDEC is in bits  9 downto 0 */
#define PLL_NDEC_VAL_P (0U)                                      /*!<  NDEC is in bits  9:0 */
#define PLL_NDEC_VAL_M (0x3FFUL << PLL_NDEC_VAL_P)
#define PLL_PDEC_VAL_P (0U)                                      /*!<  PDEC is in bits 6:0 */
#define PLL_PDEC_VAL_M (0x7FUL << PLL_PDEC_VAL_P)



//根据宏定义设置main_clk_mhz默认值
//以下代码不得进行更改
#if (0 == LPC546XX_MAIN_CLOCK)
    uint16 main_clk_mhz = 180; 
#elif (1 == LPC546XX_MAIN_CLOCK)
    uint16 main_clk_mhz = 200; 
#elif (2 == LPC546XX_MAIN_CLOCK)
    uint16 main_clk_mhz = 220; 
#else
    uint16 main_clk_mhz = 180; 
#endif
//以上代码不得进行更改
    



static uint32 pllDecodeN(uint32 NDEC)
{
    uint32 n, x, i;

    switch (NDEC)
    {
        case 0x3FF:
            n = 0;
            break;
        case 0x302:
            n = 1;
            break;
        case 0x202:
            n = 2;
            break;
        default:
            x = 0x080;
            n = 0xFFFFFFFFU;
            for (i = NVALMAX; ((i >= 3) && (n == 0xFFFFFFFFU)); i--)
            {
                x = (((x ^ (x >> 2) ^ (x >> 3) ^ (x >> 4)) & 1) << 7) | ((x >> 1) & 0x7F);
                if ((x & (PLL_NDEC_VAL_M >> PLL_NDEC_VAL_P)) == NDEC)   n = i;
            }
            break;
    }
    return n;
}


static uint32 pllDecodeM(uint32 MDEC)
{
    uint32 m, i, x;

    switch (MDEC)
    {
        case 0x1FFFF:
            m = 0;
            break;
        case 0x18003:
            m = 1;
            break;
        case 0x10003:
            m = 2;
            break;
        default:
            x = 0x04000;
            m = 0xFFFFFFFFU;
            for (i = MVALMAX; ((i >= 3) && (m == 0xFFFFFFFFU)); i--)
            {
                x = (((x ^ (x >> 1)) & 1) << 14) | ((x >> 1) & 0x3FFFU);
                if ((x & (PLL_MDEC_VAL_M >> PLL_MDEC_VAL_P)) == MDEC)   m = i;
            }
            break;
    }
    return m;
}

static uint32 pllDecodeP(uint32 PDEC)
{
    uint32 p, x, i;

    switch (PDEC)
    {
        case 0x7F:
            p = 0;
            break;
        case 0x62:
            p = 1;
            break;
        case 0x42:
            p = 2;
            break;
        default:
            x = 0x10;
            p = 0xFFFFFFFFU;
            for (i = PVALMAX; ((i >= 3) && (p == 0xFFFFFFFFU)); i--)
            {
                x = (((x ^ (x >> 2)) & 1) << 4) | ((x >> 1) & 0xFU);
                if ((x & (PLL_PDEC_VAL_M >> PLL_PDEC_VAL_P)) == PDEC)   p = i;
            }
            break;
    }
    return p;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据寄存器值计算N值
//  @return     void
//  @Note       用户无需调用
//-------------------------------------------------------------------------------------------------------------------
static uint32 findPllPreDiv(uint32 ctrlReg, uint32 nDecReg)
{
    uint32 preDiv = 1;

    if ((ctrlReg & SYSCON_SYSPLLCTRL_DIRECTI_MASK) == 0)
    {
        preDiv = pllDecodeN(nDecReg & 0x3FF);
        if (preDiv == 0)    preDiv = 1;
    }
    return preDiv;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据寄存器值计算P值
//  @return     void
//  @Note       用户无需调用
//-------------------------------------------------------------------------------------------------------------------
static uint32 findPllPostDiv(uint32 ctrlReg, uint32 pDecReg)
{
    uint32 postDiv = 1;
    
    if ((ctrlReg & SYSCON_SYSPLLCTRL_DIRECTO_MASK) == 0)
    {
        postDiv = 2 * pllDecodeP(pDecReg & 0x7F);
        if (postDiv == 0)   postDiv = 2;
    }
    return postDiv;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据寄存器值计算M值
//  @return     void
//  @Note       用户无需调用
//-------------------------------------------------------------------------------------------------------------------
static uint32 findPllMMult(uint32 ctrlReg, uint32 mDecReg)
{
    uint32 mMult = 1;
    
    mMult = pllDecodeM(mDecReg & 0x1FFFF);
    if (mMult == 0) mMult = 1;
    return mMult;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      计算N寄存器值并写入
//  @return     void
//  @Note       用户无需调用
//-------------------------------------------------------------------------------------------------------------------
void N_divider(uint16 N)
{
    uint32 x=0x00000080;
    uint32 i;
    uint16 N_max = 256;
    
    if(N > N_max)   N = N_max;

    switch (N) 
    {
        case 0: x = 0xFFFFFFFF;break;
        case 1: x = 0x00000302;break;
        case 2: x = 0x00000202;break;
        default: 
            for (i = N; i <= N_max; i++)
                x = (((x ^ (x>>2) ^ (x>>3) ^ (x>>4)) & 1) << 7) | ((x>>1) & 0x7F); 
    }
    x &= SYSCON_SYSPLLNDEC_NDEC_MASK;
    SYSCON->SYSPLLNDEC = SYSCON_SYSPLLNDEC_NDEC(x);
    SYSCON->SYSPLLNDEC |= SYSCON_SYSPLLNDEC_NREQ(1);
    SYSCON->SYSPLLNDEC = SYSCON_SYSPLLNDEC_NDEC(x) | SYSCON_SYSPLLNDEC_NREQ(0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      计算P寄存器值并写入
//  @return     void
//  @Note       用户无需调用
//-------------------------------------------------------------------------------------------------------------------
void P_divider(uint16 P)
{
    uint32 x=0x10;
    uint32 i;
    uint16 P_max = 0x20;
    
    if(P > P_max)   P = P_max;
    
    switch (P) 
    {
        case 0: x = 0xFFFFFFFF;break;
        case 1: x = 0x00000062;break;
        case 2: x = 0x00000042;break;
        default: 
            for (i = P; i <= P_max; i++)
                x = (((x ^ (x>>2)) & 1) << 4) | ((x>>1) & 0xF); 
    }
    x &= SYSCON_SYSPLLPDEC_PDEC_MASK;
    SYSCON->SYSPLLPDEC = SYSCON_SYSPLLPDEC_PDEC(x);
    SYSCON->SYSPLLPDEC |= SYSCON_SYSPLLPDEC_PREQ(1);
    SYSCON->SYSPLLPDEC = SYSCON_SYSPLLPDEC_PDEC(x) | SYSCON_SYSPLLPDEC_PREQ(0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      计算M寄存器值并写入
//  @return     void
//  @Note       用户无需调用
//-------------------------------------------------------------------------------------------------------------------
void M_divider(uint16 M)
{
    uint32 x=0x00004000;
    uint32 i;
    uint16 M_max = 0x00008000;
    
    if(M > M_max)   M = M_max;
    
    switch (M) 
    {
        case 0: x = 0xFFFFFFFF;break;
        case 1: x = 0x00018003;break;
        case 2: x = 0x00010003;break;
        default: 
            for (i = M; i <= M_max; i++)
                x = (((x ^ (x>>1)) & 1) << 14) | ((x>>1) & 0x3FFF); 
    }
    x &= SYSCON_SYSPLLMDEC_MDEC_MASK;
    SYSCON->SYSPLLMDEC = SYSCON_SYSPLLMDEC_MDEC(x);
    SYSCON->SYSPLLMDEC |= SYSCON_SYSPLLMDEC_MREQ(1);
    SYSCON->SYSPLLMDEC = SYSCON_SYSPLLMDEC_MDEC(x) | SYSCON_SYSPLLMDEC_MREQ(0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      计算PLLCTRL寄存器值并写入
//  @return     void
//  @Note       用户无需调用
//-------------------------------------------------------------------------------------------------------------------
void write_syspllctrl(void)
{
    uint32 M;
    uint32 SELR,SELI,SELP;
    
    M = SYSCON->SYSPLLMDEC & SYSCON_SYSPLLMDEC_MDEC_MASK;
    
    
    if (M < 60)         SELP = (M>>1) + 1;
    else                SELP = 31;
    
    if (M > 16384)      SELI = 1;
    else if (M > 8192)  SELI = 2;
    else if (M > 2048)  SELI = 4;
    else if (M >= 501)  SELI = 8;
    else if (M >=60)    SELI = 4*(1024/(M+9));
    else                SELI = (M & 0x3C) + 4; /* & denotes bitwise AND */
    
    SELR = 0;
    SYSCON->SYSPLLCTRL = 0
                        //| SYSCON_SYSPLLCTRL_UPLIMOFF(1)
                        | SYSCON_SYSPLLCTRL_SELR(SELR)
                        | SYSCON_SYSPLLCTRL_SELI(SELI)
                        | SYSCON_SYSPLLCTRL_SELP(SELP);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PLL初始化
//  @return     void
//  @Note       用户无需调用
//-------------------------------------------------------------------------------------------------------------------
void pll_init(void)
{
    SYSCON->SYSOSCCTRL = SYSCON_SYSOSCCTRL_FREQRANGE(0);//设置外部时钟范围 1-25MHZ
    
    SYSCON->FLASHCFG |= SYSCON_FLASHCFG_FLASHTIM(0x07); //设置flash访问时间
  
    SYSCON->PDRUNCFGCLR[0] = (0
                            | SYSCON_PDRUNCFGCLR_PDEN_SYS_PLL_MASK  //开启系统PLL电源
                            | SYSCON_PDRUNCFGCLR_PDEN_VD2_ANA_MASK  //开启系统振荡器的模拟电源
                            | SYSCON_PDRUNCFGCLR_PDEN_VD3_MASK);    //开启所有PLL电源
    
    

    if(0 == LPC546XX_PLL_CLOCK_SOURCE)      
    {
        SYSCON->SYSPLLCLKSEL = SYSCON_SYSPLLCLKSEL_SEL(0x00);           //选择PLL输入时钟为内部晶振
    }
    else if(1 == LPC546XX_PLL_CLOCK_SOURCE) 
    {
        SYSCON->PDRUNCFGCLR[1] = SYSCON_PDRUNCFGCLR_PDEN_SYSOSC_MASK;   //开启外部晶振电源
        SYSCON->SYSPLLCLKSEL = SYSCON_SYSPLLCLKSEL_SEL(0x01);           //选择PLL输入时钟为外部晶振
    }
    else                                    
    {
        SYSCON->SYSPLLCLKSEL = SYSCON_SYSPLLCLKSEL_SEL(0x00);           //选择PLL输入时钟为内部晶振
    }
    
    main_clk_mhz = 0;   //清零
    P_divider(2);       //设置分频系数
    //根据宏定义设置倍频系数
    if(0 == LPC546XX_MAIN_CLOCK)        M_divider(90);      //设置倍频系数
    else if(1 == LPC546XX_MAIN_CLOCK)   M_divider(100);     //设置倍频系数
    else if(2 == LPC546XX_MAIN_CLOCK)   M_divider(105);     //设置倍频系数
    else                                M_divider(90);      //设置倍频系数
    N_divider(3);       //设置分频系数
    
    write_syspllctrl(); //设置PLL模式及带宽参数
    
    SYSCON->PDRUNCFGSET[0] = SYSCON_PDRUNCFGCLR_PDEN_SYS_PLL_MASK;  //关闭系统PLL电源
    SYSCON->PDRUNCFGCLR[0] = SYSCON_PDRUNCFGCLR_PDEN_SYS_PLL_MASK;  //开启系统PLL电源
    
    while(!SYSCON->SYSPLLSTAT);                     //等待PLL锁定
    
    SYSCON->AHBCLKDIV = 0;
    
    
    SYSCON->MAINCLKSELB = SYSCON_MAINCLKSELB_SEL(2);//切换main_clk的输入源为pll_clk
}



void get_clk(void)
{
    uint32 temp_reg;
    uint32 M_val,P_val,N_val;
    
    temp_reg = SYSCON->SYSPLLCTRL;
    M_val = findPllMMult(temp_reg,SYSCON->SYSPLLMDEC);
    P_val = findPllPostDiv(temp_reg,SYSCON->SYSPLLPDEC);
    N_val = findPllPreDiv(temp_reg,SYSCON->SYSPLLNDEC);
    
    main_clk_mhz = (uint32)EX_REF_CLK*(2*M_val) / N_val / P_val / 1000;//计算main_clk频率

    //如果PRINTF定义为1，则初始化printf所使用的串口
    //PRINTF宏定义值可以在LPC546XX_config.h文件内进行修改
#if (1 == PRINTF)
    //初始化printf所使用的串口 串口号、波特率、引脚可以在LPC546XX_config.h文件内进行修改
    uart_init(DEBUG_UART,DEBUG_UART_BAUD,DEBUG_UART_TX_PIN,DEBUG_UART_RX_PIN);  
#endif
    
    DisableInterrupts;//禁用所有中断
}

