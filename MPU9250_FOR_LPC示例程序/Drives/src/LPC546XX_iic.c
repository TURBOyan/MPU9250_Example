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





#include "common.h"
#include "LPC546XX_pll.h"
#include "LPC546XX_systick.h"
#include "LPC546XX_iocon.h"
#include "LPC546XX_flexcomm.h" 
#include "LPC546XX_iic.h"



I2C_Type *IICN[] = I2C_BASE_PTRS;




#define IIC_STRAT(iic_n)            {IICN[iic_n]->MSTCTL = I2C_MSTCTL_MSTSTART_MASK;}    //发送启动信号

#define IIC_STOP(iic_n)             {IICN[iic_n]->MSTCTL = I2C_MSTCTL_MSTSTOP_MASK;}     //发送停止信号

#define IIC_CONTINUE(iic_n)         {IICN[iic_n]->MSTCTL = I2C_MSTCTL_MSTCONTINUE_MASK;} //继续发送信号

#define DELAY_MAX                   10000//等待最大时间    400K的时候3000可正常通讯  波特率越低应设置更大

#define WAIT_TIME(iic_n,TIME)       {   TIME++;\
                                        if(TIME>DELAY_MAX){\
                                            IICN[iic_n]->CFG &= ~I2C_CFG_MSTEN_MASK; \
                                            IICN[iic_n]->CFG = I2C_CFG_MSTEN_MASK;\
                                            IIC_STOP(iic_n);\
                                            break;\
                                        }\
                                    }//如果超时没有检测到标志位，则重启

#define IIC_WAIT_IDLE(iic_n,TIME)   {TIME = 0;while(0X0 != (I2C_STAT_MSTSTATE_MASK & IICN[iic_n]->STAT) >> I2C_STAT_MSTSTATE_SHIFT) WAIT_TIME(iic_n,TIME);}    //等待IIC为空闲

#define IIC_WAIT_RX(iic_n,TIME)     {TIME = 0;while(0X1 != (I2C_STAT_MSTSTATE_MASK & IICN[iic_n]->STAT) >> I2C_STAT_MSTSTATE_SHIFT) WAIT_TIME(iic_n,TIME);}    //等待接收完成

#define IIC_WAIT_TX(iic_n,TIME)     {TIME = 0;while(0X2 != (I2C_STAT_MSTSTATE_MASK & IICN[iic_n]->STAT) >> I2C_STAT_MSTSTATE_SHIFT) WAIT_TIME(iic_n,TIME);}    //等待发送完成

#define IIC_WAIT_PENDING(iic_n,TIME){TIME = 0;while(!(IICN[iic_n]->STAT & I2C_STAT_MSTPENDING_MASK)) WAIT_TIME(iic_n,TIME);}






void iic_mux(IICN_enum iic_n, IIC_PIN_enum sda_pin, IIC_PIN_enum scl_pin)
{
    switch(iic_n)
    {
        case IIC_0:
        {
            if     (IIC0_SDA_A24 == sda_pin)    iocon_init(A24,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC0_SDA_A29 == sda_pin)    iocon_init(A29,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC0_SDA_A31 == sda_pin)    iocon_init(A31,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC0_SDA_B5  == sda_pin)    iocon_init(B5 ,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC0_SDA_B8  == sda_pin)    iocon_init(B8 ,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败                  
                                                                                                   
            if     (IIC0_SCL_A25 == scl_pin)    iocon_init(A25,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC0_SCL_A30 == scl_pin)    iocon_init(A30,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC0_SCL_B0  == scl_pin)    iocon_init(A0 ,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC0_SCL_B6  == scl_pin)    iocon_init(A6 ,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC0_SCL_B7  == scl_pin)    iocon_init(A7 ,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
        }break;
        
        case IIC_1:
        {
            if     (IIC1_SDA_A13 == sda_pin)    iocon_init(A13,ALT1 | DIGITAL | FILTEROFF | PULLUP);
            else if(IIC1_SDA_B10 == sda_pin)    iocon_init(B10,ALT2 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
            if     (IIC1_SCL_A10 == scl_pin)    iocon_init(A10,ALT4 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC1_SCL_A14 == scl_pin)    iocon_init(A14,ALT1 | DIGITAL | FILTEROFF | PULLUP);
            else if(IIC1_SCL_B11 == scl_pin)    iocon_init(B11,ALT2 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
        }break;
        
        case IIC_2:
        {
            if     (IIC2_SDA_A26 == sda_pin)    iocon_init(A26,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC2_SDA_B24 == sda_pin)    iocon_init(B24,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC2_SDA_B26 == sda_pin)    iocon_init(B26,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败                  
                                                                                                   
            if     (IIC2_SCL_A27 == scl_pin)    iocon_init(A27,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC2_SCL_B25 == scl_pin)    iocon_init(B25,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC2_SCL_B27 == scl_pin)    iocon_init(B27,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
        }break;
        
        case IIC_3:
        {
            if     (IIC3_SDA_A1  == sda_pin)    iocon_init(A1 ,ALT2 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC3_SDA_A3  == sda_pin)    iocon_init(A3 ,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC3_SDA_A20 == sda_pin)    iocon_init(A20,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC3_SDA_B1  == sda_pin)    iocon_init(B1 ,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败                  
                                                                                                   
            if     (IIC3_SCL_A2  == scl_pin)    iocon_init(A2 ,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC3_SCL_A7  == scl_pin)    iocon_init(A7 ,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC3_SCL_A12 == scl_pin)    iocon_init(A12,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC3_SCL_A21 == scl_pin)    iocon_init(A21,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
        }break;
        
        case IIC_4:
        {
            if     (IIC4_SDA_A5  == sda_pin)    iocon_init(A5 ,ALT2 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC4_SDA_A18 == sda_pin)    iocon_init(A18,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC4_SDA_B9  == sda_pin)    iocon_init(B9 ,ALT5 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC4_SDA_B21 == sda_pin)    iocon_init(B21,ALT5 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败                  
                                                                                                   
            if     (IIC4_SCL_A16 == scl_pin)    iocon_init(A16,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC4_SCL_A19 == scl_pin)    iocon_init(A19,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC4_SCL_B15 == scl_pin)    iocon_init(B15,ALT5 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC4_SCL_B20 == scl_pin)    iocon_init(B20,ALT5 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
        }break;
        
        case IIC_5:
        {
            if     (IIC5_SDA_A8  == sda_pin)    iocon_init(A8 ,ALT3 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC5_SDA_B14 == sda_pin)    iocon_init(B14,ALT4 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败                  
                                                                                                   
            if     (IIC5_SCL_A9  == scl_pin)    iocon_init(A9 ,ALT3 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC5_SCL_B15 == scl_pin)    iocon_init(B15,ALT4 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
        }break;
        
        case IIC_6:
        {
            if     (IIC6_SDA_A11 == sda_pin)    iocon_init(A11,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC6_SDA_A15 == sda_pin)    iocon_init(A15,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC6_SDA_B13 == sda_pin)    iocon_init(B13,ALT2 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败                  
                                                                                                   
            if     (IIC6_SCL_A22 == scl_pin)    iocon_init(A22,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC6_SCL_B16 == scl_pin)    iocon_init(B16,ALT2 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
        }break;
        
        case IIC_7:
        {
            if     (IIC7_SDA_A20 == sda_pin)    iocon_init(A20,ALT7 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC7_SDA_B21 == sda_pin)    iocon_init(B21,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC7_SDA_B29 == sda_pin)    iocon_init(B29,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败                  
                                                                                                   
            if     (IIC7_SCL_A19 == scl_pin)    iocon_init(A19,ALT7 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC7_SCL_B20 == scl_pin)    iocon_init(B20,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC7_SCL_B30 == scl_pin)    iocon_init(B30,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
        }break;
        
        case IIC_8:
        {
            if     (IIC8_SDA_B17 == sda_pin)    iocon_init(B17,ALT2 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC8_SDA_B31 == sda_pin)    iocon_init(B31,ALT5 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败                  
                                                                                                   
            if     (IIC8_SCL_B18 == scl_pin)    iocon_init(B18,ALT2 | DIGITAL | FILTEROFF | PULLUP | OD);
            else if(IIC8_SCL_B22 == scl_pin)    iocon_init(B22,ALT1 | DIGITAL | FILTEROFF | PULLUP | OD);
            else                                ASSERT(0);//引脚错误 进入断言失败
            
        }break;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      IIC初始化
//  @param      iic_n           选择IIC模块
//  @param      sda_pin         选择IIC数据引脚
//  @param      scl_pin         选择IIC时钟引脚
//  @param      baud            设置IIC的波特率
//  @return     void
//  Sample usage:               iic_init(IIC_2, IIC2_SDA_A26, IIC2_SCL_A27,400*1000);//硬件IIC初始化     
//  @note                       在同时使用USART、IIC、SPI通讯端口的时候，需要特别注意，同一个模块号的通讯端口不能同时使用
//                              如USART_0、IIC_0和SPI_0模块号都为0则不能同时使用，如果使用了USART_0模块，则IIC_0和SPI_0模块都不能使用
//                              同理模块号为1、2、3、4、5、6、7、8都是如此     
//-------------------------------------------------------------------------------------------------------------------
uint32 iic_init(IICN_enum iic_n, IIC_PIN_enum sda_pin, IIC_PIN_enum scl_pin, uint32 baud)
{
    uint32 divval;
    
    flexcomm_clk_enable((FLEXCOMMN_enum)iic_n,IIC);     //开启flexcomm时钟
    
    iic_mux(iic_n, sda_pin, scl_pin);
    

    divval = (flexcomm_get_clk((FLEXCOMMN_enum)iic_n)*10 / (baud*4) + 5) / 10 - 1;
    if(I2C_CLKDIV_DIVVAL_MASK < divval)         ASSERT(0);//频率过小 进入断言失败
    IICN[iic_n]->CLKDIV = divval;                       //设置分频值
    IICN[iic_n]->MSTTIME = 0X00;                        //设置SCL高低电平时间
    
    IICN[iic_n]->CFG = I2C_CFG_MSTEN_MASK;              //开启主机模式 标准速度和快速模式
    
    return (flexcomm_get_clk((FLEXCOMMN_enum)iic_n)/4/(divval+1));
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      写入一个字节数据到I2C设备指定寄存器地址
//  @param      iic_n       IIC模块
//  @param      slaveid     从机地址(7位地址)
//  @param      reg         从机寄存器地址
//  @param      data        数据
//  @return     void
//  @since      v2.0
//  Sample usage:       	iic_write_reg(IIC_2, 0x2D, 0x50,2);     //写入数据2到0x50地址，从机地址为0x2D
//-------------------------------------------------------------------------------------------------------------------
void iic_write_reg(IICN_enum iic_n, uint8 slaveid, uint8 reg, uint8 data)
{
    int time = 0;
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_IDLE(iic_n,time);
    IICN[iic_n]->MSTDAT = (slaveid << 1) | MWSR;
    IIC_STRAT(iic_n);
    
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_TX(iic_n,time);
    IICN[iic_n]->MSTDAT = reg;
    IIC_CONTINUE(iic_n);
    
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_TX(iic_n,time);
    IICN[iic_n]->MSTDAT = data;
    IIC_CONTINUE(iic_n);

    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_TX(iic_n,time);
    
    IIC_STOP(iic_n);
    
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_IDLE(iic_n,time);
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取I2C设备指定地址寄存器的数据
//  @param      iic_n       IIC模块
//  @param      slaveid     从机地址(7位地址)
//  @param      reg         从机寄存器地址
//  @return                 读取的寄存器值
//  @since      v2.0
//  Sample usage:       	uint8 value = iic_read_reg(IIC_2, 0x2D, 0x50);//读取0x50地址的数据，从机地址为0x2D
//-------------------------------------------------------------------------------------------------------------------
uint8 iic_read_reg(IICN_enum iic_n, uint8 slaveid, uint8 reg)
{
    uint8 result;
    int time = 0;
    //地址是低七位
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_IDLE(iic_n,time);
    IICN[iic_n]->MSTDAT = (slaveid << 1) | MWSR;    //发送从机地址和写位
    IIC_STRAT(iic_n);
    
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_TX(iic_n,time);
    IICN[iic_n]->MSTDAT = reg;                      //发送寄存器地址
    IIC_CONTINUE(iic_n);

    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_TX(iic_n,time);
    IIC_STOP(iic_n);

    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_IDLE(iic_n,time);
    IICN[iic_n]->MSTDAT = (slaveid << 1) | MRSW;    //发送从机地址和读位
    IIC_STRAT(iic_n);
    
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_RX(iic_n,time);                        //等待接收完成
    result = IICN[iic_n]->MSTDAT;                   //读取数据        
    IIC_STOP(iic_n);

    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_IDLE(iic_n,time);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取I2C设备指定地址寄存器的数据
//  @param      iic_n       IIC模块(iic0、iic1)
//  @param      slaveid     从机地址(7位地址)
//  @param      reg         从机寄存器地址
//  @param      addr        读取的数据存储的地址
//  @param      num         读取字节数
//  @return     void
//  @since      v2.0
//  Sample usage:       	uint8 value = iic_read_reg(IIC_2, 0x2D, 0x50, 10, buf);//读取0x50地址的数据，从机地址为0x2D开始的10个字节
//-------------------------------------------------------------------------------------------------------------------
uint8 iic_read_reg_bytes(IICN_enum iic_n, uint8 slaveid, uint8 reg, uint8 * addr, uint8 num)
{
    int time = 0;
    //地址是低七位
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_IDLE(iic_n,time);
    IICN[iic_n]->MSTDAT = (slaveid << 1) | MWSR;
    IIC_STRAT(iic_n);
    
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_TX(iic_n,time);
    IICN[iic_n]->MSTDAT = reg;
    IIC_CONTINUE(iic_n);

    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_TX(iic_n,time);
    IIC_STOP(iic_n);

    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_IDLE(iic_n,time);
    IICN[iic_n]->MSTDAT = (slaveid << 1) | MRSW;
    IIC_STRAT(iic_n);
    
    
    while(--num)
    {
        IIC_WAIT_PENDING(iic_n,time);
        IIC_WAIT_RX(iic_n,time);
        *addr = IICN[iic_n]->MSTDAT;                //读取数据     
        IIC_CONTINUE(iic_n);
        addr++;
    }
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_RX(iic_n,time);                             //等待接收完成
    *addr = IICN[iic_n]->MSTDAT;                    //读取数据        
    
    IIC_STOP(iic_n);
    
    
    IIC_WAIT_PENDING(iic_n,time);
    IIC_WAIT_IDLE(iic_n,time);
    return 0;
}
