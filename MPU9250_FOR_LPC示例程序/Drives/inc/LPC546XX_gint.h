/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		GPIO0与GPIO1 组中断
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/

#ifndef _LPC546XX_gint_h
#define _LPC546XX_gint_h


//此枚举定义不允许用户修改
typedef enum    // 枚举组中断模块编号
{
    GROUP0,
    GROUP1,
}GINTN_enum;



#define GINT0_FLAG   (GINT0->CTRL & GINT_CTRL_INT_MASK)         //组中断0标志位
#define GINT1_FLAG   (GINT1->CTRL & GINT_CTRL_INT_MASK)         //组中断1标志位

#define CLEAR_GINT0_FLAG   (GINT0->CTRL |= GINT_CTRL_INT_MASK)  //清除组中断0标志位
#define CLEAR_GINT1_FLAG   (GINT1->CTRL |= GINT_CTRL_INT_MASK)  //清除组中断1标志位



void gint_init(GINTN_enum gint, PIN_enum pin, TRIGGER_enum trigger);
void gint_enable(GINTN_enum gint, PIN_enum pin);
void gint_disable(GINTN_enum gint, PIN_enum pin);


#endif
