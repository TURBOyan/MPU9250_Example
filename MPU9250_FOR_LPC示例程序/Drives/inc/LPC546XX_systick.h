/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		滴答定时器
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/

#ifndef _LPC546XX_systick_h
#define _LPC546XX_systick_h


void systick_delay(uint32 time);
uint32 systick_getval(void);


//------------------------------------以下宏定义用于SYSTICK延时------------------------------------
void systick_delay_ms(uint32 ms);                                           //毫秒级systick延时函数 由于滴答定时器最大只有24位，因此用函数实现，这样延时时间范围较宽
#define systick_delay_us(x)         systick_delay(x*main_clk_mhz)           //设置SYSTICK延时时间  单位us   范围0  -  16777215(0xffffff)/(main_clk_mhz*1000)ms   主频180M 最大延时时间大约为93ms
#define systick_delay_ns(x)         systick_delay(x*main_clk_mhz/1000)      //设置SYSTICK延时时间  单位ns   范围0  -  16777215(0xffffff)*1000/(main_clk_mhz)ns   主频180M 最大延时时间大约为93ms


//------------------------------------以下宏定义用于SYSTICK定时------------------------------------
#define systick_timing_ms(x)        systick_timing(x*main_clk_mhz*1000)     //设置SYSTICK定时时间  单位ms   范围0  -  16777215(0xffffff)/(main_clk_mhz*1000)ms   主频180M 最大定时时间大约为93ms
#define systick_timing_us(x)        systick_timing(x*main_clk_mhz)          //设置SYSTICK定时时间  单位us   范围0  -  16777215(0xffffff)/(main_clk_mhz)us        主频180M 最大定时时间大约为93ms
#define systick_timing_ns(x)        systick_timing(x*main_clk_mhz/1000)     //设置SYSTICK定时时间  单位ns   范围0  -  16777215(0xffffff)*1000/(main_clk_mhz)ns   主频180M 最大定时时间大约为93ms


//------------------------------------以下宏定义用于获取当前SYSTICK时间------------------------------------
#define systick_getval_ms()         (systick_getval()/1000)/main_clk_mhz    //获取SYSTICK当前计时时间  单位ms
#define systick_getval_us()         (systick_getval())/main_clk_mhz         //获取SYSTICK当前计时时间  单位us
#define systick_getval_ns()         (systick_getval()*1000)/main_clk_mhz    //获取SYSTICK当前计时时间  单位ns
    

#endif
