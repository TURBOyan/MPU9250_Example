/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SPI
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#ifndef _LPC546XX_spi_h
#define _LPC546XX_spi_h

//此枚举定义不允许用户修改
typedef enum
{
    SPI0_SCK_A28  = 0*21+0*3, SPI0_SCK_B4,
    SPI0_MOSI_A24 = 0*21+1*3, SPI0_MOSI_A29, SPI0_MOSI_B5,
    SPI0_MISO_A25 = 0*21+2*3, SPI0_MISO_A30, SPI0_MISO_B6,
    SPI0_CS0_B8   = 0*21+3*3, SPI0_CS0_A31,
    SPI0_CS1_B0   = 0*21+4*3, SPI0_CS1_B7,
    SPI0_CS2_NC   = 0*21+5*3, 
    SPI0_CS3_NC   = 0*21+6*3, 
                           
    SPI1_SCK_A7   = 1*21+0*3, SPI1_SCK_B9,
    SPI1_MOSI_B10 = 1*21+1*3,
    SPI1_MISO_A10 = 1*21+2*3, SPI1_MISO_B11,
    SPI1_CS0_A13  = 1*21+3*3,
    SPI1_CS1_A14  = 1*21+4*3, 
    SPI1_CS2_NC   = 1*21+5*3, 
    SPI1_CS3_NC   = 1*21+6*3, 
                           
    SPI2_SCK_B23  = 2*21+0*3, 
    SPI2_MOSI_A26 = 2*21+1*3, SPI2_MOSI_B24,
    SPI2_MISO_A27 = 2*21+2*3, SPI2_MISO_B25,
    SPI2_CS0_B26  = 2*21+3*3, 
    SPI2_CS1_B27  = 2*21+4*3,
    SPI2_CS2_NC   = 2*21+5*3, 
    SPI2_CS3_NC   = 2*21+6*3, 
                           
    SPI3_SCK_A0   = 3*21+0*3, SPI3_SCK_A6,
    SPI3_MOSI_A3  = 3*21+1*3, SPI3_MOSI_B1,
    SPI3_MISO_A2  = 3*21+2*3, SPI3_MISO_A12,
    SPI3_CS0_A1   = 3*21+3*3, SPI3_CS0_A20,
    SPI3_CS1_A7   = 3*21+4*3, SPI3_CS1_A21,
    SPI3_CS2_A9   = 3*21+5*3, SPI3_CS2_B23,
    SPI3_CS3_A8   = 3*21+6*3, SPI3_CS3_B24,
                           
    SPI4_SCK_A4   = 4*21+0*3, SPI4_SCK_B19,
    SPI4_MOSI_A5  = 4*21+1*3, SPI4_MOSI_B21,
    SPI4_MISO_A16 = 4*21+2*3, SPI4_MISO_B20,
    SPI4_CS0_A18  = 4*21+3*3, SPI4_CS0_B9,
    SPI4_CS1_A19  = 4*21+4*3, SPI4_CS1_B15,
    SPI4_CS2_A17  = 4*21+5*3, SPI4_CS2_B8,
    SPI4_CS3_B22  = 4*21+6*3, 
                           
    SPI5_SCK_A7   = 5*21+0*3, 
    SPI5_MOSI_A8  = 5*21+1*3, 
    SPI5_MISO_A9  = 5*21+2*3, 
    SPI5_CS0_B14  = 5*21+3*3, 
    SPI5_CS1_B15  = 5*21+4*3, 
    SPI5_CS2_NC   = 5*21+5*3, 
    SPI5_CS3_NC   = 5*21+6*3, 
                           
    SPI6_SCK_A10  = 6*21+0*3, SPI6_SCK_B12,
    SPI6_MOSI_A11 = 6*21+1*3, SPI6_MOSI_B13,
    SPI6_MISO_A22 = 6*21+2*3, SPI6_MISO_B16,
    SPI6_CS0_A15  = 6*21+3*3, 
    SPI6_CS1_NC   = 6*21+4*3, 
    SPI6_CS2_NC   = 6*21+5*3, 
    SPI6_CS3_NC   = 6*21+6*3, 
                           
    SPI7_SCK_A21  = 7*21+0*3, SPI7_SCK_B28,
    SPI7_MOSI_A20 = 7*21+1*3, SPI7_MOSI_B29,
    SPI7_MISO_A19 = 7*21+2*3, SPI7_MISO_B30,
    SPI7_CS0_B21  = 7*21+3*3, 
    SPI7_CS1_B20  = 7*21+4*3,
    SPI7_CS2_NC   = 7*21+5*3, 
    SPI7_CS3_NC   = 7*21+6*3, 
                           
    SPI8_SCK_B19  = 8*21+0*3, 
    SPI8_MOSI_B17 = 8*21+1*3, 
    SPI8_MISO_B18 = 8*21+2*3, 
    SPI8_CS0_B31  = 8*21+3*3, 
    SPI8_CS1_B22  = 8*21+4*3,
    SPI8_CS2_NC   = 8*21+5*3, 
    SPI8_CS3_NC   = 8*21+6*3, 
    
    SPI_CS_NOT_USE,

}SPI_PIN_enum;

//此枚举定义不允许用户修改
typedef enum //枚举串口号
{
    SPI_0,
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_4,
    SPI_5,
    SPI_6,
    SPI_7,
    SPI_8,
    SPI_MAX,
}SPIN_enum;



uint32  spi_init(SPIN_enum spi_n, SPI_PIN_enum cs_pin, SPI_PIN_enum sck_pin, SPI_PIN_enum mosi_pin, SPI_PIN_enum miso_pin, uint8 mode, uint32 baud);
void spi_mosi(SPIN_enum spi_n, SPI_PIN_enum cs_pin, uint8 *modata, uint8 *midata, uint32 len, uint8 eot);
void spi_mosi_cmd(SPIN_enum spi_n, SPI_PIN_enum cs_pin, uint8 *mocmd , uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len);


#endif
