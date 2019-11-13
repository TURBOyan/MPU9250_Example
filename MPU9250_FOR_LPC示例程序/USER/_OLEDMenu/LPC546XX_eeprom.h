/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		EEPROM
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#ifndef _LPC546XX_eeprom_h
#define _LPC546XX_eeprom_h


#define EEPROM_BASE_ADDRESS         (0x40108000)                    //EEPROM 基地址
#define EEPROM_SIZE                 (0x00004000)                    //EEPROM 大小
#define EEPROM_PAGE_COUNT           (128)                           //EEPROM 页数
#define EEPROM_PAGE_SIZE            (EEPROM_SIZE/EEPROM_PAGE_COUNT) //EEPROM 页大小
#define EEPROM_OFFSET_MAX           (EEPROM_SIZE/4)            		//EEPROM 偏差最大值
#define EEPROM_PROGRAM_CMD          (6)                             //EEPROM 命令
#define EEPROM_INTERNAL_FREQ        (1500000)                       //EEPROM 频率



//-------------------------------------------------------------------------------------------------------------------
//  @brief      EEPROM读取一个字(4个字节)
//  @param      type        写入EEPROM的页 一页可写32个uint32类型数据
//  @param      offset      需要写入的数据的地址 
//  @return     读取的内容
//  Sample usage:            uint32 test = EEPROM_READ_WORD(uint32,0);  读取偏移0  类型为uint32
//  @note                    
//-------------------------------------------------------------------------------------------------------------------
#define EEPROM_READ_WORD(type,offset)    (*(type *)(EEPROM_BASE_ADDRESS + ((offset<EEPROM_OFFSET_MAX?offset:0)*4)))
//#define my_EEPROM_READ_WORD(Page_Num,offset,type)    (*(type *)(EEPROM_BASE_ADDRESS + ((offset<EEPROM_OFFSET_MAX?offset:0)*4)+Page_Num*EEPROM_PAGE_SIZE))
#define my_EEPROM_READ_WORD(Page_Num,offset,type)     (*(type *)((uint32)((EEPROM_BASE_ADDRESS + ((offset<EEPROM_OFFSET_MAX?offset:0)*4)+Page_Num*EEPROM_PAGE_SIZE))))
void eeprom_init(void);
void my_eeprom_write_word(uint16 Page_num,int16 offset, uint32 data);
void eeprom_write_page(uint16 pagenum, uint32 *data);
void my_eeprom_write_word(uint16 Page_num,int16 offset, uint32 data);


#endif

