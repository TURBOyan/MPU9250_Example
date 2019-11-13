/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		串口
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/

#ifndef _LPC546XX_uart_h
#define _LPC546XX_uart_h

//此枚举定义不允许用户修改
typedef enum //枚举串口引脚
{
    UART0_TX_A25, UART0_TX_A30, UART0_TX_B6,  //串口0 发送引脚可选范围
    UART0_RX_A24, UART0_RX_A29, UART0_RX_B5,  //串口0 接收引脚可选范围

    UART1_TX_A10, UART1_TX_B11,               //串口1 发送引脚可选范围        
    UART1_RX_B10,                             //串口1 接收引脚可选范围
                                                  
    UART2_TX_A27, UART2_TX_B25,               //串口2 发送引脚可选范围
    UART2_RX_A26, UART2_RX_B24,               //串口2 接收引脚可选范围
                        
    UART3_TX_A2,  UART3_TX_A12,               //串口3 发送引脚可选范围 
    UART3_RX_A3,  UART3_RX_B1,                //串口3 接收引脚可选范围
                        
    UART4_TX_A16, UART4_TX_B20,               //串口4 发送引脚可选范围
    UART4_RX_A5,  UART4_RX_B21,               //串口4 接收引脚可选范围
    
    UART5_TX_A9,                              //串口5 发送引脚可选范围
    UART5_RX_A8,                              //串口5 接收引脚可选范围
    
    UART6_TX_A22, UART6_TX_B16,               //串口6 发送引脚可选范围
    UART6_RX_A11, UART6_RX_B13,               //串口6 接收引脚可选范围 （A11与下载接口冲突）
    
    UART7_TX_A19, UART7_TX_B30,               //串口7 发送引脚可选范围
    UART7_RX_A20, UART7_RX_B29,               //串口7 接收引脚可选范围
    
    UART8_TX_B18,                             //串口8 发送引脚可选范围
    UART8_RX_B17,                             //串口8 接收引脚可选范围

}UARTPIN_enum;

//此枚举定义不允许用户修改
typedef enum //枚举串口号
{
    USART_0,
    USART_1,
    USART_2,
    USART_3,
    USART_4,
    USART_5,
    USART_6,
    USART_7,
    USART_8,
}UARTN_enum;

//-------------------------------------------------------------------------------------------------------------------
//  @brief        串口FIFO中断标志位宏定义
//  @return       标志位寄存器值  
//  位0：         TX FIFO错误中断     1：发生中断  0：未发生中断
//  位1：         RX FIFO错误中断     1：发生中断  0：未发生中断
//  位2：         TX FIFO等级中断     1：发生中断  0：未发生中断
//  位3：         RX FIFO等级中断     1：发生中断  0：未发生中断
//  位4：         外设中断            1：发生中断  0：未发生中断
//-------------------------------------------------------------------------------------------------------------------
#define UART0_FIFO_FLAG  USART0->FIFOINTSTAT;
#define UART1_FIFO_FLAG  USART1->FIFOINTSTAT;
#define UART2_FIFO_FLAG  USART2->FIFOINTSTAT;
#define UART3_FIFO_FLAG  USART3->FIFOINTSTAT;
#define UART4_FIFO_FLAG  USART4->FIFOINTSTAT;
#define UART5_FIFO_FLAG  USART5->FIFOINTSTAT;
#define UART6_FIFO_FLAG  USART6->FIFOINTSTAT;
#define UART7_FIFO_FLAG  USART7->FIFOINTSTAT;
#define UART8_FIFO_FLAG  USART8->FIFOINTSTAT;

uint32 uart_init(UARTN_enum uartn, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin);
void   uart_putchar(UARTN_enum uartn, uint8 dat);
void   uart_putbuff (UARTN_enum uartn, uint8 *buff, uint32 len);
void   uart_putstr (UARTN_enum uartn, const uint8 *str);
void   uart_getchar(UARTN_enum uartn, uint8 *dat);
uint8  uart_query(UARTN_enum uartn, uint8 *dat);
void   uart_tx_irq(UARTN_enum uartn,uint8 status);
void   uart_rx_irq(UARTN_enum uartn,uint8 status);

#endif
