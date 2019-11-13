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


     

#include "common.h"
#include "LPC546XX_iocon.h"
#include "SEEKFREE_FUN.h"
#include "LPC546XX_flexcomm.h" 
#include "LPC546XX_uart.h"
     

USART_Type    * UARTN[] = USART_BASE_PTRS;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口引脚复用及开启对应时钟
//  @param      uartn       串口模块号(USART_0,USART_1,USART_2,USART_3,USART_4,USART_5,USART_6,USART_7,USART_8)
//  @param      tx_pin      串口发送引脚
//  @param      rx_pin      串口接收引脚
//  @return     void
//  Sample usage:           无需用户调用
//-------------------------------------------------------------------------------------------------------------------
void uart_mux(UARTN_enum uartn, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
    switch(uartn)
    {
        case USART_0:
        {
            if     (UART0_TX_A25 == tx_pin)   iocon_init(A25,ALT1 | DIGITAL);
            else if(UART0_TX_A30 == tx_pin)   iocon_init(A30,ALT1 | DIGITAL);
            else if(UART0_TX_B6  == tx_pin)   iocon_init(B6, ALT1 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
            
            if     (UART0_RX_A24 == rx_pin)   iocon_init(A24,ALT1 | DIGITAL);
            else if(UART0_RX_A29 == rx_pin)   iocon_init(A29,ALT1 | DIGITAL);
            else if(UART0_RX_B5  == rx_pin)   iocon_init(B5, ALT1 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
   
        }break;
        
        case USART_1:
        {
            if     (UART1_TX_A10 == tx_pin)   iocon_init(A10,ALT4 | DIGITAL);
            else if(UART1_TX_B11 == tx_pin)   iocon_init(B11,ALT2 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
            
            if     (UART1_RX_B10 == rx_pin)   iocon_init(B10,ALT2 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
        }break;
        
        case USART_2:
        {
            if     (UART2_TX_A27 == tx_pin)   iocon_init(A27,ALT1 | DIGITAL);
            else if(UART2_TX_B25 == tx_pin)   iocon_init(B25,ALT1 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
            
            if     (UART2_RX_A26 == rx_pin)   iocon_init(A26,ALT1 | DIGITAL);
            else if(UART2_RX_B24 == rx_pin)   iocon_init(B24,ALT1 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
        }break;
        
        case USART_3:
        {
            if     (UART3_TX_A2  == tx_pin)   iocon_init(A2, ALT1 | DIGITAL);
            else if(UART3_TX_A12 == tx_pin)   iocon_init(A12,ALT1 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
            
            if     (UART3_RX_A3 == rx_pin)    iocon_init(A3, ALT1 | DIGITAL);
            else if(UART3_RX_B1 == rx_pin)    iocon_init(B1, ALT1 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
        }break;
        
        case USART_4:
        {
            if     (UART4_TX_A16 == tx_pin)   iocon_init(A16,ALT1 | DIGITAL);
            else if(UART4_TX_B20 == tx_pin)   iocon_init(B20,ALT5 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
            
            if     (UART4_RX_A5  == rx_pin)   iocon_init(A5, ALT2 | DIGITAL);
            else if(UART4_RX_B21 == rx_pin)   iocon_init(B21,ALT5 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
        }break;
        
        case USART_5:
        {
            if     (UART5_TX_A9 == tx_pin)    iocon_init(A9, ALT3 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
            
            if     (UART5_RX_A8 == rx_pin)    iocon_init(A8, ALT3 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
        }break;
        
        case USART_6:
        {
            if     (UART6_TX_A22 == tx_pin)   iocon_init(A22,ALT1 | DIGITAL);
            else if(UART6_TX_B16 == tx_pin)   iocon_init(B16,ALT2 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
            
            if     (UART6_RX_A11 == rx_pin)   iocon_init(A11,ALT1 | DIGITAL);
            else if(UART6_RX_B13 == rx_pin)   iocon_init(B13,ALT2 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
        }break;
        
        case USART_7:
        {
            if     (UART7_TX_A19 == tx_pin)   iocon_init(A19,ALT7 | DIGITAL);
            else if(UART7_TX_B30 == tx_pin)   iocon_init(B30,ALT1 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
            
            if     (UART7_RX_A20 == rx_pin)   iocon_init(A20,ALT7 | DIGITAL);
            else if(UART7_RX_B29 == rx_pin)   iocon_init(B29,ALT1 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
        }break;
        
        case USART_8:
        {
            if     (UART8_TX_B18 == tx_pin)   iocon_init(B18,ALT2 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
            
            if     (UART8_RX_B17 == rx_pin)   iocon_init(B17,ALT2 | DIGITAL);
            else                              ASSERT(0);//引脚错误 进入断言失败
        }break;
        
        default:    ASSERT(0);//串口号错误 进入断言失败
    }
    
    flexcomm_clk_enable((FLEXCOMMN_enum)uartn,USART);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口初始化
//  @param      uartn           串口模块号(USART_0,USART_1,USART_2,USART_3,USART_4,USART_5,USART_6,USART_7,USART_8)
//  @param      baud            串口波特率
//  @param      tx_pin          串口发送引脚
//  @param      rx_pin          串口接收引脚
//  @return     uint32          实际波特率
//  Sample usage:               uart_init(USART_0,115200,UART0_TX_A25,UART0_RX_A24);       // 初始化串口0 波特率115200 发送引脚使用P025 接收引脚使用P024
//  @note                       在同时使用USART、IIC、SPI通讯端口的时候，需要特别注意，同一个模块号的通讯端口不能同时使用
//                              如USART_0、IIC_0和SPI_0模块号都为0则不能同时使用，如果使用了USART_0模块，则IIC_0和SPI_0模块都不能使用
//                              同理模块号为1、2、3、4、5、6、7、8都是如此     
//-------------------------------------------------------------------------------------------------------------------
uint32 uart_init(UARTN_enum uartn, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
    uint32  baud_div;
    int32   baud_temp;
    vuint32 osr_temp;
    uint32  osr_val;
    int32   baud_diff;
    uint32  baud_diff_min = (uint32)(-1);
    uint32  baud_div_temp;
    uint32  real_baud;

                  
    uart_mux(uartn, tx_pin, rx_pin);//开启对应时钟及引脚复用设置
    
    UARTN[uartn]->CFG = 0;    //关闭串口
    UARTN[uartn]->CTL = 0;
    
    
    for(osr_temp=0xf; osr_temp>=0x4; osr_temp--)
    {
        baud_div_temp = (flexcomm_get_clk((FLEXCOMMN_enum)uartn)*10/(osr_temp+1)/baud + 5)/10 - 1;
        if(USART_BRG_BRGVAL_MASK < baud_div_temp)
        {
            ASSERT(0);//断言失败   不支持此波特率
        }

        baud_temp = flexcomm_get_clk((FLEXCOMMN_enum)uartn)/(osr_temp+1)/(baud_div_temp+1);
        baud_diff = myabs(baud_temp - baud);
        if(!baud_diff)//提前找到合适的分频系数
        {
            baud_div = baud_div_temp;
            osr_val = osr_temp;
            break;
        }
        
        if(baud_diff_min > baud_diff)
        {
            baud_diff_min = baud_diff;
            baud_div = baud_div_temp;
            osr_val = osr_temp;
        }
    }
    
    //断言失败，期望波特率与实际波特率误差大于百分之2.5
    //也可以屏蔽然后使用返回的真实波特率，然后将对应的目标设备设置为相同的波特率
    if( ((long)baud_diff_min*1000/baud) > 25)   ASSERT(0);  
    
    
    real_baud = flexcomm_get_clk((FLEXCOMMN_enum)uartn)/(osr_val+1)/(baud_div+1);      //计算真实的波特率
    UARTN[uartn]->OSR = osr_val;                            //设置采样
    UARTN[uartn]->BRG = USART_BRG_BRGVAL(baud_div);         //设置波特率分频系数


    UARTN[uartn]->FIFOCFG  = (0
                           | USART_FIFOCFG_ENABLETX_MASK    //开启TX FIFO
                           | USART_FIFOCFG_ENABLERX_MASK    //开启RX FIFO
                           //| USART_FIFOCFG_DMATX_MASK     //
                           //| USART_FIFOCFG_DMARX_MASK     //
                           | USART_FIFOCFG_EMPTYTX_MASK     //清空TX FIFO
                           | USART_FIFOCFG_EMPTYRX_MASK     //清空RX FIFO
                            );

    UARTN[uartn]->FIFOSTAT |= (USART_FIFOSTAT_RXERR_MASK | USART_FIFOSTAT_TXERR_MASK);
    UARTN[uartn]->FIFOTRIG = USART_FIFOTRIG_RXLVLENA_MASK;
    UARTN[uartn]->CFG = (0                                  
                       | USART_CFG_DATALEN(1)               //数据长度   0: 7位  1：8位  2:9位
                       | USART_CFG_PARITYSEL(0)             //奇偶校验   0：无奇偶校验位   2：偶校验  3：奇校验
                       | USART_CFG_ENABLE_MASK              //开启串口
                       //| USART_CFG_LOOP_MASK              //自发自收   不用外部接线 内部连通
                       //| USART_CFG_STOPLEN_MASK           //停止位长度 0：1位  1：两位   屏蔽为0 
                         ); 
    return real_baud;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口字节输出
//  @param      uartn           串口模块号(USART_0,USART_1,USART_2,USART_3,USART_4,USART_5,USART_6,USART_7,USART_8)
//  @param      dat             需要发送的字节
//  @return     void        
//  Sample usage:               uart_putchar(USART_0,0xA5);       // 串口0发送0xA5
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(UARTN_enum uartn, uint8 dat)
{
    while(!(UARTN[uartn]->FIFOSTAT & USART_FIFOSTAT_TXNOTFULL_MASK));
    UARTN[uartn]->FIFOWR = dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送数组
//  @param      uartn           串口模块号(USART_0,USART_1,USART_2,USART_3,USART_4,USART_5,USART_6,USART_7,USART_8)
//  @param      *buff           要发送的数组地址
//  @param      len             发送长度
//  @return     void
//  Sample usage:               uart_putbuff(USART_0,&a[0],sizeof(a));  
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff (UARTN_enum uartn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uartn, *buff);
        buff++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送字符串
//  @param      uartn           串口模块号(USART_0,USART_1,USART_2,USART_3,USART_4,USART_5,USART_6,USART_7,USART_8)
//  @param      *str            要发送的字符串地址
//  @return     void
//  Sample usage:               uart_putstr(USART_0,"i lvoe you"); 
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr (UARTN_enum uartn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uartn, *str++);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（whlie等待）
//  @param      uartn           串口模块号(USART_0,USART_1,USART_2,USART_3,USART_4,USART_5,USART_6,USART_7,USART_8)
//  @param      *dat            接收数据的地址
//  @return     void        
//  Sample usage:               uint8 dat; uart_getchar(USART_0,&dat);       // 接收串口0数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
void uart_getchar(UARTN_enum uartn, uint8 *dat)
{
    while(!(UARTN[uartn]->FIFOSTAT & USART_FIFOSTAT_RXNOTEMPTY_MASK));
    *dat = UARTN[uartn]->FIFORD;        //使用fifo队列时  默认使用FIFO队列
    //*dat = UARTN[uartn]->FIFORDNOPOP;   //不使用fifo队列时
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（查询接收）
//  @param      uartn           串口模块号(USART_0,USART_1,USART_2,USART_3,USART_4,USART_5,USART_6,USART_7,USART_8)
//  @param      *dat            接收数据的地址
//  @return     uint8           1：接收成功   0：未接收到数据
//  Sample usage:               uint8 dat; uart_query(USART_0,&dat);       // 接收串口0数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query(UARTN_enum uartn, uint8 *dat)
{
    if((UARTN[uartn]->FIFOSTAT & USART_FIFOSTAT_RXNOTEMPTY_MASK))
    {
        *dat = UARTN[uartn]->FIFORD;        //使用fifo队列时  默认使用FIFO队列
        //*dat = UARTN[uartn]->FIFORDNOPOP;   //不使用fifo队列时
        return 1;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送空闲中断设置
//  @param      uartn           串口模块号(USART_0,USART_1,USART_2,USART_3,USART_4,USART_5,USART_6,USART_7,USART_8)
//  @param      status          1：打开中断   0：关闭中断
//  @return     void        
//  Sample usage:               uart_tx_irq(USART_0,1);       // 打开串口0发送空闲中断
//-------------------------------------------------------------------------------------------------------------------
void uart_tx_irq(UARTN_enum uartn,uint8 status)
{
    if(status)  
    {
        UARTN[uartn]->FIFOTRIG &= ~(uint32)(USART_FIFOTRIG_TXLVL_MASK);
        UARTN[uartn]->FIFOTRIG |= USART_FIFOTRIG_TXLVLENA_MASK | USART_FIFOTRIG_TXLVL(0);
        UARTN[uartn]->FIFOINTENSET = USART_FIFOINTENSET_TXLVL_MASK;
        flexcomm_irq((FLEXCOMMN_enum)uartn,status);
    }
    else
    {
        UARTN[uartn]->FIFOTRIG &= ~(uint32)(USART_FIFOTRIG_TXLVLENA_MASK);
        UARTN[uartn]->FIFOINTENCLR = USART_FIFOINTENSET_TXLVL_MASK;
    }
    flexcomm_irq((FLEXCOMMN_enum)uartn,status);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口接收中断设置
//  @param      uartn           串口模块号(USART_0,USART_1,USART_2,USART_3,USART_4,USART_5,USART_6,USART_7,USART_8)
//  @param      status          1：打开中断   0：关闭中断
//  @return     void        
//  Sample usage:               uart_rx_irq(USART_0,1);       // 打开串口0接收中断
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq(UARTN_enum uartn,uint8 status)
{
    if(status)  
    {
        UARTN[uartn]->FIFOTRIG &= ~(uint32)(USART_FIFOTRIG_RXLVL_MASK);
        UARTN[uartn]->FIFOTRIG |= USART_FIFOTRIG_RXLVLENA_MASK | USART_FIFOTRIG_RXLVL(0);
        UARTN[uartn]->FIFOINTENSET = USART_FIFOINTENSET_RXLVL_MASK;
    }
    else
    {
        UARTN[uartn]->FIFOTRIG &= ~(uint32)(USART_FIFOTRIG_RXLVLENA_MASK);
        UARTN[uartn]->FIFOINTENCLR = USART_FIFOINTENSET_RXLVL_MASK;
    }
    flexcomm_irq((FLEXCOMMN_enum)uartn,status);
}


//不需要重定义printf 到串口，库里已经单独写了printf的函数
//-------------------------------------------------------------------------------------------------------------------
//  @brief      重定义printf 到串口
//  @param      ch      需要打印的字节
//  @param      stream  数据流
//  @note       此函数由编译器自带库里的printf所调用
//-------------------------------------------------------------------------------------------------------------------
//int fputc(int ch, FILE *stream)
//{
//    uart_putchar(DEBUG_UART, (char)ch);
//    return(ch);
//}
