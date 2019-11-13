/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MRT(多速率定时器)
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-11-21
 ********************************************************************************************************************/


#ifndef _LPC546XX_mrt_h
#define _LPC546XX_mrt_h


//此枚举定义不允许用户修改
typedef enum
{
    MRT_CH0,
    MRT_CH1,
    MRT_CH2,
    MRT_CH3,
}MRTNUM_enum;



#define MRT_FLAG_READ(MRTNUM)  (MRT0->CHANNEL[MRTNUM].STAT&MRT_CHANNEL_STAT_INTFLAG_MASK)
#define MRT_FLAG_CLR(MRTNUM)   MRT0->CHANNEL[MRTNUM].STAT = 1; MRT0->IRQ_FLAG = (1<<MRTNUM)


                 
//------------------------------------以下代码用于PIT中断------------------------------------
#define mrt_pit_init_ms(MRTNUM,time)    mrt_pit_init(MRTNUM,time*main_clk_mhz*1000) //初始化MRT定时器为PIT模式并设置周期时间 (单位 毫秒)    最大周期为180M为93.2毫秒
#define mrt_pit_init_us(MRTNUM,time)    mrt_pit_init(MRTNUM,time*main_clk_mhz)      //初始化MRT定时器为PIT模式并设置周期时间 (单位 微秒)    最大周期为180M为93.2毫秒
#define mrt_pit_init_ns(MRTNUM,time)    mrt_pit_init(MRTNUM,time*main_clk_mhz/1000) //初始化MRT定时器为PIT模式并设置周期时间 (单位 纳秒)    最大周期为180M为93.2毫秒，最小单位为5.55纳秒


//------------------------------------以下代码用于PIT延时------------------------------------
#define mrt_delay_ms(MRTNUM,time)       mrt_delay(MRTNUM,time*main_clk_mhz*1000)    //初始化MRT定时器为延时模式并设置周期时间 (单位 毫秒)   最大延时180M为93.2毫秒
#define mrt_delay_us(MRTNUM,time)       mrt_delay(MRTNUM,time*main_clk_mhz)         //初始化MRT定时器为延时模式并设置周期时间 (单位 微秒)   最大延时180M为93.2毫秒
#define mrt_delay_ns(MRTNUM,time)       mrt_delay(MRTNUM,time*main_clk_mhz/1000)    //初始化MRT定时器为延时模式并设置周期时间 (单位 纳秒)   最大延时180M为93.2毫秒，最小单位为5.55纳秒

//------------------------------------以下代码用于获取PIT计时时间------------------------------------
#define mrt_get_ms(MRTNUM)              (mrt_get(MRTNUM)/main_clk_mhz/1000)         //获取MRT计时时间  (单位 毫秒) 
#define mrt_get_us(MRTNUM)              (mrt_get(MRTNUM)/main_clk_mhz)              //获取MRT计时时间  (单位 微秒) 
#define mrt_get_ns(MRTNUM)              (mrt_get(MRTNUM)*1000/main_clk_mhz)         //获取MRT计时时间  (单位 纳秒) 



void mrt_pit_init(MRTNUM_enum mrtchx, uint32 time);
void mrt_delay(MRTNUM_enum mrtchx, uint32 time);
void mrt_start(MRTNUM_enum mrtchx);
uint32 mrt_get(MRTNUM_enum mrtchx);
void mrt_clean(MRTNUM_enum mrtchx);


#endif

