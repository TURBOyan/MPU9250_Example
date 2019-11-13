/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		DMA
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
#include "LPC546XX_pint.h"
#include "LPC546XX_gpio.h"
#include "LPC546XX_sct.h"
#include "LPC546XX_dma.h"


ALIGN(512) dma_descriptor_t s_dma_descriptor_table[DMA_CHMAX] = {0};//DMA通道描述符


//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMA初始化
//  @param      dmach       DMA通道
//  @param      *SADDR      源地址
//  @param      *DADDR      目的地址
//  @param      count       DMA传输次数
//  @return     void
//  Sample usage:           dam_init(DMA_CH0, (void *)&GPIO_PIN(0,0), (void *)&image[0][0], 188);     //初始化DMA  通道0   源地址为A0-A7  目的地址为image数组的首地址   传输字节数为188次
//-------------------------------------------------------------------------------------------------------------------
void dam_init(DMACH_enum dmach, void *SADDR, void *DADDR, uint16 count)
{
    uint8  n;
    uint32 temp_pin;

    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_DMA_MASK;              //打开DMA时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_DMA0_RST_MASK;         //清除DMA复位时钟
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_INPUTMUX_MASK;         //打开多路复用时钟
    INPUTMUX->DMA_ITRIG_INMUX[dmach] = INPUTMUX_DMA_ITRIG_INMUX_INP(2); //设置DMA触发复用通道 为SCT request0
    SYSCON->AHBCLKCTRLCLR[0] = SYSCON_AHBCLKCTRL_INPUTMUX_MASK;         //关闭多路复用时钟
    
    temp_pin = ((uint32)SADDR - (uint32)&GPIO_PIN(0,0)) * 8;
    n = 8;
    while(n--)
    {
        gpio_init((PIN_enum)(temp_pin+n),GPI,0,PULLUP | FILTEROFF);
    }

    DMA0->SRAMBASE = (uint32_t)s_dma_descriptor_table;
    DMA0->CTRL = DMA_CTRL_ENABLE_MASK;
    DMA0->COMMON[0].ENABLESET = 1<<dmach;
    
    DMA0->CHANNEL[dmach].CFG = ( 0
                               | DMA_CHANNEL_CFG_HWTRIGEN_MASK
                               | DMA_CHANNEL_CFG_TRIGPOL_MASK       //1 上升沿
                                //| DMA_CHANNEL_CFG_TRIGTYPE_MASK   //0 :边沿触发
                               | DMA_CHANNEL_CFG_TRIGBURST_MASK     //启用burst传输
                               | DMA_CHANNEL_CFG_BURSTPOWER(0)      //burst传输为一个字节
                               | DMA_CHANNEL_CFG_CHPRIORITY(0)      //优先级设置   0为最高
                               );
    
    DMA0->COMMON[0].SETVALID = 1<<dmach;
    DMA0->COMMON[0].INTENSET = 1<<dmach;
    
    s_dma_descriptor_table[dmach].xfercfg = ( 0
                                   //| DMA_CHANNEL_XFERCFG_RELOAD_MASK        //参数自动重载
                                   | DMA_CHANNEL_XFERCFG_CFGVALID_MASK
                                   | DMA_CHANNEL_XFERCFG_SETINTA_MASK       //
                                   | DMA_CHANNEL_XFERCFG_WIDTH(0)           //宽度8位
                                   | DMA_CHANNEL_XFERCFG_SRCINC(0)          //源地址不自增
                                   | DMA_CHANNEL_XFERCFG_DSTINC(1)          //目的地址自增一个数据宽度
                                   | DMA_CHANNEL_XFERCFG_XFERCOUNT(count-1) //DMA次数
                                   );
    
    s_dma_descriptor_table[dmach].srcEndAddr = SADDR;
    s_dma_descriptor_table[dmach].dstEndAddr = (void *)((uint32)DADDR + count - 1);
    s_dma_descriptor_table[dmach].linkToNextDesc = 0;
    
    DMA0->CHANNEL[dmach].XFERCFG = s_dma_descriptor_table[dmach].xfercfg;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMA 链接传输 初始化
//  @param      dmach       DMA通道
//  @param      *SADDR      源地址
//  @param      *DADDR      目的地址
//  @param      count       DMA传输次数
//  @return     void
//  Sample usage:           dam_init(DMA_CH0, (void *)&GPIO_PIN(0,0), (void *)&image[0][0], 188);     //初始化DMA  通道0   源地址为A0-A7  目的地址为image数组的首地址   传输字节数为188次
//  @note                   使用链接传输可以实现 一次性传输超过1024 但会占用其他DMA通道的 通道描述符
//-------------------------------------------------------------------------------------------------------------------
void dam_init_linked(DMACH_enum dmach, void *SADDR, void *DADDR, uint32 count)
{
    uint8  n;
    uint32 temp_pin;

    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_DMA_MASK;              //打开DMA时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_DMA0_RST_MASK;         //清除DMA复位时钟
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_INPUTMUX_MASK;         //打开多路复用时钟
    INPUTMUX->DMA_ITRIG_INMUX[dmach] = INPUTMUX_DMA_ITRIG_INMUX_INP(2); //设置DMA触发复用通道 为SCT request0
    SYSCON->AHBCLKCTRLCLR[0] = SYSCON_AHBCLKCTRL_INPUTMUX_MASK;         //关闭多路复用时钟
    
    temp_pin = ((uint32)SADDR - (uint32)&GPIO_PIN(0,0)) * 8;
    n = 8;
    while(n--)
    {
        gpio_init((PIN_enum)(temp_pin+n),GPI,0,PULLUP | FILTEROFF);
    }

    DMA0->SRAMBASE = (uint32_t)s_dma_descriptor_table;
    DMA0->CTRL = DMA_CTRL_ENABLE_MASK;
    DMA0->COMMON[0].ENABLESET = 1<<dmach;
    
    DMA0->CHANNEL[dmach].CFG = ( 0
                               | DMA_CHANNEL_CFG_HWTRIGEN_MASK
                               | DMA_CHANNEL_CFG_TRIGPOL_MASK       //1 上升沿
                                //| DMA_CHANNEL_CFG_TRIGTYPE_MASK   //0 :边沿触发
                               | DMA_CHANNEL_CFG_TRIGBURST_MASK     //启用burst传输
                               | DMA_CHANNEL_CFG_BURSTPOWER(0)      //burst传输为一个字节
                               | DMA_CHANNEL_CFG_CHPRIORITY(0)      //优先级设置   0为最高
                               );
    
    DMA0->COMMON[0].SETVALID = 1<<dmach;
    DMA0->COMMON[0].INTENSET = 1<<dmach;
    
    n = 0;
    while((n+1)<<10 < count)//剩下数量大于1024
    {
        s_dma_descriptor_table[n].xfercfg = ( 0
                                            | DMA_CHANNEL_XFERCFG_RELOAD_MASK        //参数自动重载
                                            | DMA_CHANNEL_XFERCFG_CFGVALID_MASK
                                            | DMA_CHANNEL_XFERCFG_SETINTA_MASK       //
                                            | DMA_CHANNEL_XFERCFG_WIDTH(0)           //宽度8位
                                            | DMA_CHANNEL_XFERCFG_SRCINC(0)          //源地址不自增
                                            | DMA_CHANNEL_XFERCFG_DSTINC(1)          //目的地址自增一个数据宽度
                                            | DMA_CHANNEL_XFERCFG_XFERCOUNT(1024-1)  //DMA次数
                                            );
        s_dma_descriptor_table[n].srcEndAddr = SADDR;
        s_dma_descriptor_table[n].dstEndAddr = (void *)((uint32)DADDR + ((n+1)<<10) - 1);
        s_dma_descriptor_table[n].linkToNextDesc = (void *)(&s_dma_descriptor_table[n+1]);
        n++;
    }
    
    //剩下数量不足或者等于1024  
    s_dma_descriptor_table[n].xfercfg = ( 0
                                        //| DMA_CHANNEL_XFERCFG_RELOAD_MASK        //参数自动重载
                                        | DMA_CHANNEL_XFERCFG_CFGVALID_MASK
                                        | DMA_CHANNEL_XFERCFG_SETINTA_MASK       //
                                        | DMA_CHANNEL_XFERCFG_WIDTH(0)           //宽度8位
                                        | DMA_CHANNEL_XFERCFG_SRCINC(0)          //源地址不自增
                                        | DMA_CHANNEL_XFERCFG_DSTINC(1)          //目的地址自增一个数据宽度
                                        | DMA_CHANNEL_XFERCFG_XFERCOUNT(count - (n<<10) - 1)  //DMA次数
                                        );
    s_dma_descriptor_table[n].srcEndAddr = SADDR;
    s_dma_descriptor_table[n].dstEndAddr = (void *)((uint32)DADDR + count - 1);
    s_dma_descriptor_table[n].linkToNextDesc = 0;
        
    DMA0->CHANNEL[dmach].XFERCFG = s_dma_descriptor_table[dmach].xfercfg;
}





