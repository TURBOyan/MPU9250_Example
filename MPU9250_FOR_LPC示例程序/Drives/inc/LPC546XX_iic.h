/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		IIC
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/

#ifndef _LPC546XX_iic_h
#define _LPC546XX_iic_h

//此枚举定义不允许用户修改
typedef enum //枚举串口号
{
    IIC0_SDA_A24 = 0*10+0*5, IIC0_SDA_A29, IIC0_SDA_A31, IIC0_SDA_B5, IIC0_SDA_B8, 
    IIC0_SCL_A25 = 0*10+1*5, IIC0_SCL_A30, IIC0_SCL_B0,  IIC0_SCL_B6, IIC0_SCL_B7,  

    IIC1_SDA_A13 = 1*10+0*5, IIC1_SDA_B10,
    IIC1_SCL_A10 = 1*10+1*5, IIC1_SCL_A14, IIC1_SCL_B11, 
    
    IIC2_SDA_A26 = 2*10+0*5, IIC2_SDA_B24, IIC2_SDA_B26,
    IIC2_SCL_A27 = 2*10+1*5, IIC2_SCL_B25, IIC2_SCL_B27,
    
    IIC3_SDA_A1  = 3*10+0*5, IIC3_SDA_A3,  IIC3_SDA_A20, IIC3_SDA_B1,
    IIC3_SCL_A2  = 3*10+1*5, IIC3_SCL_A7,  IIC3_SCL_A12, IIC3_SCL_A21, 
    
    IIC4_SDA_A5  = 4*10+0*5, IIC4_SDA_A18, IIC4_SDA_B9,  IIC4_SDA_B21, 
    IIC4_SCL_A16 = 4*10+1*5, IIC4_SCL_A19, IIC4_SCL_B15, IIC4_SCL_B20, 
    
    IIC5_SDA_A8  = 5*10+0*5, IIC5_SDA_B14,
    IIC5_SCL_A9  = 5*10+1*5, IIC5_SCL_B15,
    
    IIC6_SDA_A11 = 6*10+0*5, IIC6_SDA_A15, IIC6_SDA_B13, 
    IIC6_SCL_A22 = 6*10+1*5, IIC6_SCL_B16,
    
    IIC7_SDA_A20 = 7*10+0*5, IIC7_SDA_B21, IIC7_SDA_B29,
    IIC7_SCL_A19 = 7*10+1*5, IIC7_SCL_B20, IIC7_SCL_B30,
    
    IIC8_SDA_B17 = 8*10+0*5, IIC8_SDA_B31,
    IIC8_SCL_B18 = 8*10+1*5, IIC8_SCL_B22,

}IIC_PIN_enum;

//此枚举定义不允许用户修改
typedef enum //枚举串口号
{
    IIC_0,
    IIC_1,
    IIC_2,
    IIC_3,
    IIC_4,
    IIC_5,
    IIC_6,
    IIC_7,
    IIC_8,
    IIC_MAX,
}IICN_enum;

//此枚举定义不允许用户修改
typedef enum
{
    MWSR =   0x00,  // 主机写模式  
    MRSW =   0x01   // 主机读模式  
} MSMODEN_enum;

uint32 iic_init(IICN_enum iic_n, IIC_PIN_enum sda_pin, IIC_PIN_enum scl_pin, uint32 baud);
void   iic_write_reg(IICN_enum iic_n, uint8 slaveid, uint8 reg, uint8 data);
uint8  iic_read_reg(IICN_enum iic_n, uint8 slaveid, uint8 reg);
uint8  iic_read_reg_bytes(IICN_enum iic_n, uint8 slaveid, uint8 reg, uint8 * addr, uint8 num);

#endif
