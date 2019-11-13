/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		总钻风
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看common.h内VERSION宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 * @note		
					接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SDA(51的RX)         A9
						SCL(51的TX)         A8
						场中断(VSY)         A0
						行中断(HREF)		未使用，因此不接
						像素中断(PCLK)      A1        
						数据口(D0-D7)		B24-B31
					------------------------------------ 
	
					默认分辨率是            188*120
					默认FPS                 50帧
 ********************************************************************************************************************/



#ifndef _SEEKFREE_MT9V032_h
#define _SEEKFREE_MT9V032_h




//配置摄像头参数
#define MT9V032_DMA_CH          DMA_CH0         //定义摄像头的DMA采集通道
#define MT9V032_W               188             //图像宽度   范围1-752     K60采集不允许超过188
#define MT9V032_H               120             //图像高度	范围1-480



//--------------------------------------------------------------------------------------------------
//引脚配置
//--------------------------------------------------------------------------------------------------
#define MT9V032_COF_UART        USART_5         //配置摄像头所使用到的串口     
#define MT9V032_COF_UART_TX     UART5_TX_A9
#define MT9V032_COF_UART_RX     UART5_RX_A8


#define MT9V032_VSYNC_PINT      PINT_CH7        //场中断所使用的PINT中断通道
#define MT9V032_VSYNC_PIN       A0              //场中断引脚
#define MT9V032_VSYNC_IRQN      PIN_INT7_IRQn   //中断号

#define MT9V032_PCLK_SCT        SCT_INPUT_MUX1  //SCT0_CAMERA_PCLK  此处修改后应同步将sct.h文件内的SCT0_CAMERA_PCLK宏定义修改与此处一样
#define MT9V032_PCLK            SCT0_GPI1_A1    //摄像头像素时钟



//--------------------------------------------------------------------------------------------------
//        摄像头数据接口
//        GPIO_PIN(B,0)
//        第一个参数B代表着使用B口，可以选择A、B分别对应GPIO_PIN(A,0)，GPIO_PIN(B,0)
//        第二个参数3代表着使用的是B24 - B31口,
//		  可以选择0、1、2、3，分别对应的是	该端口的X0-X7，X8-X15，X16-X23，X24-X31(这里x代表的是第一个字母，比如现在的就是B24 - B31)
//--------------------------------------------------------------------------------------------------
#define MT9V032_DATAPORT 		GPIO_PIN(B,3)	                //DMA数据口

                        
                        
                        
//摄像头命令枚举
typedef enum
{
    INIT = 0,               //摄像头初始化命令
    AUTO_EXP,               //自动曝光命令
    EXP_TIME,               //曝光时间命令
    FPS,                    //摄像头帧率命令
    SET_COL,                //图像列命令
    SET_ROW,                //图像行命令
    LR_OFFSET,              //图像左右偏移命令
    UD_OFFSET,              //图像上下偏移命令
    GAIN,                   //图像偏移命令
    CONFIG_FINISH,          //非命令位，主要用来占位计数
    
    SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
    GET_STATUS,             //获取摄像头配置命令
    GET_VERSION,            //固件版本号命令
	
    SET_ADDR = 0XFE,        //寄存器地址命令
    SET_DATA                //寄存器数据命令
}CMD;
     
     
     
                            
                            

extern uint8 mt9v032_finish_flag;  //一场图像采集完成标志位
extern uint8 image[MT9V032_H][MT9V032_W];      //内部使用
                            
                            
                        
void  mt9v032_cof_uart_interrupt(void);
void  mt9v032_vsync(void);
void  camera_init(void);
void  mt9v032_dma(void);
void  seekfree_sendimg_032(UARTN_enum uartn);




#endif

