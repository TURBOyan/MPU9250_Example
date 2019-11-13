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


#ifndef _LPC546XX_dma_h
#define _LPC546XX_dma_h

//此枚举定义不允许用户修改
typedef enum
{
    DMA_CH0,
    DMA_CH1,
    DMA_CH2,
    DMA_CH3,
    DMA_CH4,
    DMA_CH5,
    DMA_CH6,
    DMA_CH7,
    DMA_CH8,
    DMA_CH9,
    DMA_CH10,
    DMA_CH11,
    DMA_CH12,
    DMA_CH13,
    DMA_CH14,
    DMA_CH15,
    DMA_CH16,
    DMA_CH17,
    DMA_CH18,
    DMA_CH19,
    DMA_CH20,
    DMA_CH21,
    DMA_CH22,
    DMA_CH23,
    DMA_CH24,
    DMA_CH25,
    DMA_CH26,
    DMA_CH27,
    DMA_CH28,
    DMA_CH29,
    DMA_CHMAX,  //通道数量
}DMACH_enum;

//此枚举定义不允许用户修改
typedef struct _dma_descriptor
{
    uint32_t xfercfg;     //发送配置
    void *srcEndAddr;     //源地址终点
    void *dstEndAddr;     //目的地址终点
    void *linkToNextDesc; //DMA下一个描述符地址
} dma_descriptor_t;



ALIGN(512) extern dma_descriptor_t s_dma_descriptor_table[DMA_CHMAX];//DMA通道描述符




#define CLEAR_DMA_FLAG(n)   (DMA0->COMMON[0].INTA = 1<<n)       //DMA中断标志位清零 n为DMA通道编号
#define READ_DMA_FLAG(n)    ((DMA0->COMMON[0].INTA & 1<<n)>>n)  //读取DMA中断标志位 n为DMA通道编号


#define DMA_ENABLE(dmach)   (DMA0->CHANNEL[dmach].CFG |= DMA_CHANNEL_CFG_HWTRIGEN_MASK)            //启用硬件触发
#define DMA_DISABLE(dmach)  (DMA0->CHANNEL[dmach].CFG &= ~(uint32)DMA_CHANNEL_CFG_HWTRIGEN_MASK)   //禁用硬件触发
#define DMA_ABORT(dmach)    (DMA0->COMMON[0].ABORT = 1 << dmach)                                   //取消当前DMA传输
#define DMA_STATUS(dmach)   (DMA0->COMMON[0].ACTIVE & (1<<dmach))                                  //获取当前DMA激活状态

//读取DMA当前剩余传输次数
#define DMA_XFERCOUNT(dmach)  ((DMA_CHANNEL_XFERCFG_XFERCOUNT_MASK & DMA0->CHANNEL[dmach].XFERCFG)>>DMA_CHANNEL_XFERCFG_XFERCOUNT_SHIFT)
//读取当前配置有效性
#define DMA_CFGVALID(dmach)   (DMA_CHANNEL_XFERCFG_CFGVALID_MASK & DMA0->CHANNEL[dmach].XFERCFG)


//使用__STATIC_INLINE为了将这段函数内嵌到使用该函数的地方，这样可以减少函数调用的时间
__STATIC_INLINE void dma_reload(DMACH_enum dmach, void *SADDR, void *DADDR, uint16 count)//DMA参数重载  重新设置参数无需调用初始化
{
    DMA0->COMMON[0].ENABLECLR = 1<<dmach;
    DMA0->COMMON[0].ABORT = 1<<dmach;
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
    DMA0->COMMON[0].ENABLESET = 1<<dmach;
    DMA0->CHANNEL[dmach].XFERCFG = s_dma_descriptor_table[dmach].xfercfg;
}

//使用__STATIC_INLINE为了将这段函数内嵌到使用该函数的地方，这样可以减少函数调用的时间
__STATIC_INLINE void dma_reload_linked(DMACH_enum dmach, void *DADDR, uint32 count)//DMA参数重载  重新设置参数无需调用初始化
{
    DMA0->COMMON[0].ENABLECLR = 1<<dmach;
    DMA0->COMMON[0].ABORT = 1<<dmach;
    s_dma_descriptor_table[0].linkToNextDesc = (void *)(&s_dma_descriptor_table[1]);
    if(count>1024)  s_dma_descriptor_table[dmach].dstEndAddr = (void *)((uint32)DADDR + 1024 - 1);
    else            s_dma_descriptor_table[dmach].dstEndAddr = (void *)((uint32)DADDR + count - 1);
    DMA0->CHANNEL[dmach].XFERCFG = s_dma_descriptor_table[0].xfercfg;
    DMA0->COMMON[0].ENABLESET = 1<<dmach;
}



void dam_init(DMACH_enum dmach, void *SADDR, void *DADDR, uint16 count);
void dam_init_linked(DMACH_enum dmach, void *SADDR, void *DADDR, uint32 count);



#endif



