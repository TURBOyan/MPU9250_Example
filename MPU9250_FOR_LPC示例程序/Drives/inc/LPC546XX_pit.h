/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		周期定时器(RIT)
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/



#ifndef _LPC546XX_pit_h
#define _LPC546XX_pit_h




#define PIT_FLAG_CLEAR  (RIT->CTRL |= RIT_CTRL_RITINT_MASK)



//------------------------------------以下代码用于PIT中断------------------------------------
#define pit_init_ms(x)  pit_init(x * main_clk_mhz * 1000)   //初始化PIT，并设置定时时间(单位为 毫秒)
#define pit_init_us(x)  pit_init(x * main_clk_mhz)          //初始化PIT，并设置定时时间(单位为 微秒)
#define pit_init_ns(x)  pit_init(x * main_clk_mhz / 1000)   //初始化PIT，并设置定时时间(单位为 纳秒)(180M主时钟频率时最小单位为5.55纳秒)


//------------------------------------以下代码用于PIT延时------------------------------------
#define pit_delay_ms(x) pit_delay(x * main_clk_mhz * 1000)  //PIT延时 毫秒
#define pit_delay_us(x) pit_delay(x * main_clk_mhz)         //PIT延时 微秒
#define pit_delay_ns(x) pit_delay(x * main_clk_mhz / 1000)  //PIT延时 纳秒(180M主时钟频率时最小单位为5.55纳秒)


//------------------------------------以下代码用于获取PIT计时时间------------------------------------
#define pit_get_ms()    (pit_get() / 1000 / main_clk_mhz)   //获取PIT计时时间  单位毫秒
#define pit_get_us()    (pit_get() / main_clk_mhz)          //获取PIT计时时间  单位微秒
#define pit_get_ns()    (pit_get() * 1000 /main_clk_mhz)    //获取PIT计时时间  单位纳秒(180M主时钟频率时最小单位为5.55纳秒)



void pit_init(long long time);
void pit_delay(long long time);
void pit_start(void);
long long pit_get(void);
void pit_clean(void);
void pit_deinit(void);




#endif
