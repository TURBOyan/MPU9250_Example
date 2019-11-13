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

#ifndef _LPC546XX_iocon_h
#define _LPC546XX_iocon_h







//此枚举定义不允许用户修改
typedef enum //枚举端口状态
{
    //所有IO都可以设置复用功能
    ALT0            = IOCON_PIO_FUNC(0),    //通用IO
    ALT1            = IOCON_PIO_FUNC(1),    //复用功能1
    ALT2            = IOCON_PIO_FUNC(2),    //复用功能2 
    ALT3            = IOCON_PIO_FUNC(3),    //复用功能3
    ALT4            = IOCON_PIO_FUNC(4),    //复用功能4
    ALT5            = IOCON_PIO_FUNC(5),    //复用功能5
    ALT6            = IOCON_PIO_FUNC(6),    //复用功能6
    ALT7            = IOCON_PIO_FUNC(7),    //复用功能7

    
    //PIO0_13, PIO0_14, PIO3_23, PIO3_24除外其余IO都可以设置上下拉电阻
    NOPULL          = IOCON_PIO_MODE(0),    //没有上下拉电阻
    PULLDOWN        = IOCON_PIO_MODE(1),    //下拉
    PULLUP          = IOCON_PIO_MODE(2),    //上拉
    REPEATER        = IOCON_PIO_MODE(3),    //中继模式 中继模式可查阅手册182页中的图16
            
    
    //所有IO都可以设置倒置功能
    INVERT          = IOCON_PIO_INVERT(1),  //信号倒置
    
    
    //所有IO都可以在模拟于数字模式之间切换
    ANALOG          = IOCON_PIO_DIGIMODE(0),//模拟模式
    DIGITAL         = IOCON_PIO_DIGIMODE(1),//数字模式
    
    
    //所有IO都可以关闭或者启用输入滤波
    FILTEROFF       = IOCON_PIO_FILTEROFF(1),//关闭输入滤波
    
    
    //PIO0_13, PIO0_14, PIO3_23, PIO3_24, P0_10 to P0_12, P0_15 to P0_16, P0_23, P0_31], P1_0,除外其余IO都可以设置IO转换速率
    STANDARD_SLEW   = IOCON_PIO_SLEW(0),    //标准转换率
    FAST_SLEW       = IOCON_PIO_SLEW(1),    //快速转换率
    
    
    //PIO0_13, PIO0_14, PIO3_23, PIO3_24除外其余IO都可以设置开漏
    OD              = IOCON_PIO_OD(1),      //开漏
    
    
    
     
    //以下配置仅限PIO0_13, PIO0_14, PIO3_23, PIO3_24使用
    //PIO0_13, PIO0_14, PIO3_23, PIO3_24为开漏输出IO，无法设置上下拉电阻
    I2CMODE         = IOCON_PIO_I2CSLEW(0), //i2c模式
    GPIOMODE        = IOCON_PIO_I2CSLEW(0), //gpio模式
    
    LOW_DRIVE       = IOCON_PIO_I2CDRIVE(0),//低驱动能力 低于标准和快速i2c速度来说低驱动能力
    HIGH_DRIVE      = IOCON_PIO_I2CDRIVE(1),//高驱动能力
    
    I2CFILTEROFF    = IOCON_PIO_I2CFILTER(1),//关闭50ns毛刺滤波器    快速i2c模式需要开启50ns毛刺滤波器
    
}PINCONF_enum;



void iocon_init(PIN_enum pin, uint32 cfg);

void iocon_init_noalt(PIN_enum pin, uint32 cfg);




#endif

