/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		misc
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/
 


#ifndef _misc_h
#define _misc_h



void write_vtor (uint32);                                           //设置中断向量表的地址

/*
  WDT_BOD_IRQn                 = 0,                < Windowed watchdog timer, Brownout detect 
  DMA0_IRQn                    = 1,                < DMA controller 
  GINT0_IRQn                   = 2,                < GPIO group 0 
  GINT1_IRQn                   = 3,                < GPIO group 1 
  PIN_INT0_IRQn                = 4,                < Pin interrupt 0 or pattern match engine slice 0 
  PIN_INT1_IRQn                = 5,                < Pin interrupt 1or pattern match engine slice 1 
  PIN_INT2_IRQn                = 6,                < Pin interrupt 2 or pattern match engine slice 2 
  PIN_INT3_IRQn                = 7,                < Pin interrupt 3 or pattern match engine slice 3 
  UTICK0_IRQn                  = 8,                < Micro-tick Timer 
  MRT0_IRQn                    = 9,                < Multi-rate timer 
  CTIMER0_IRQn                 = 10,               < Standard counter/timer CTIMER0 
  CTIMER1_IRQn                 = 11,               < Standard counter/timer CTIMER1 
  SCT0_IRQn                    = 12,               < SCTimer/PWM 
  CTIMER3_IRQn                 = 13,               < Standard counter/timer CTIMER3 
  FLEXCOMM0_IRQn               = 14,               < Flexcomm Interface 0 (USART, SPI, I2C, FLEXCOMM) 
  FLEXCOMM1_IRQn               = 15,               < Flexcomm Interface 1 (USART, SPI, I2C, FLEXCOMM) 
  FLEXCOMM2_IRQn               = 16,               < Flexcomm Interface 2 (USART, SPI, I2C, FLEXCOMM) 
  FLEXCOMM3_IRQn               = 17,               < Flexcomm Interface 3 (USART, SPI, I2C, FLEXCOMM) 
  FLEXCOMM4_IRQn               = 18,               < Flexcomm Interface 4 (USART, SPI, I2C, FLEXCOMM) 
  FLEXCOMM5_IRQn               = 19,               < Flexcomm Interface 5 (USART, SPI, I2C,, FLEXCOMM) 
  FLEXCOMM6_IRQn               = 20,               < Flexcomm Interface 6 (USART, SPI, I2C, I2S,, FLEXCOMM) 
  FLEXCOMM7_IRQn               = 21,               < Flexcomm Interface 7 (USART, SPI, I2C, I2S,, FLEXCOMM) 
  ADC0_SEQA_IRQn               = 22,               < ADC0 sequence A completion. 
  ADC0_SEQB_IRQn               = 23,               < ADC0 sequence B completion. 
  ADC0_THCMP_IRQn              = 24,               < ADC0 threshold compare and error. 
  DMIC0_IRQn                   = 25,               < Digital microphone and DMIC subsystem 
  HWVAD0_IRQn                  = 26,               < Hardware Voice Activity Detector 
  USB0_NEEDCLK_IRQn            = 27,               < USB Activity Wake-up Interrupt 
  USB0_IRQn                    = 28,               < USB device 
  RTC_IRQn                     = 29,               < RTC alarm and wake-up interrupts 
  Reserved46_IRQn              = 30,               < Reserved interrupt 
  Reserved47_IRQn              = 31,               < Reserved interrupt 
  PIN_INT4_IRQn                = 32,               < Pin interrupt 4 or pattern match engine slice 4 int 
  PIN_INT5_IRQn                = 33,               < Pin interrupt 5 or pattern match engine slice 5 int 
  PIN_INT6_IRQn                = 34,               < Pin interrupt 6 or pattern match engine slice 6 int 
  PIN_INT7_IRQn                = 35,               < Pin interrupt 7 or pattern match engine slice 7 int 
  CTIMER2_IRQn                 = 36,               < Standard counter/timer CTIMER2 
  CTIMER4_IRQn                 = 37,               < Standard counter/timer CTIMER4 
  RIT_IRQn                     = 38,               < Repetitive Interrupt Timer 
  SPIFI0_IRQn                  = 39,               < SPI flash interface 
  FLEXCOMM8_IRQn               = 40,               < Flexcomm Interface 8 (USART, SPI, I2C, FLEXCOMM) 
  FLEXCOMM9_IRQn               = 41,               < Flexcomm Interface 9 (USART, SPI, I2C, FLEXCOMM) 
  SDIO_IRQn                    = 42,               < SD/MMC  
  CAN0_IRQ0_IRQn               = 43,               < CAN0 interrupt0 
  CAN0_IRQ1_IRQn               = 44,               < CAN0 interrupt1 
  CAN1_IRQ0_IRQn               = 45,               < CAN1 interrupt0 
  CAN1_IRQ1_IRQn               = 46,               < CAN1 interrupt1 
  USB1_IRQn                    = 47,               < USB1 interrupt 
  USB1_NEEDCLK_IRQn            = 48,               < USB1 activity 
  ETHERNET_IRQn                = 49,               < Ethernet 
  ETHERNET_PMT_IRQn            = 50,               < Ethernet power management interrupt 
  ETHERNET_MACLP_IRQn          = 51,               < Ethernet MAC interrupt 
  EEPROM_IRQn                  = 52,               < EEPROM interrupt 
  LCD_IRQn                     = 53,               < LCD interrupt 
  SHA_IRQn                     = 54,               < SHA interrupt 
  SMARTCARD0_IRQn              = 55,               < Smart card 0 interrupt 
  SMARTCARD1_IRQn              = 56                < Smart card 1 interrupt 

*/
                                                                    
#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //使能IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //禁止IRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //设置优先级，优先级范围0-7
                                                                    
                                                                    
#define EnableInterrupts                __enable_irq()              //使能全部中断
#define DisableInterrupts               __disable_irq()             //禁止全部中断




#endif
