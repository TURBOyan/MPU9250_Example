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

#include "common.h"
#include "LPC546XX_pll.h"
#include "LPC546XX_eeprom.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      EEPROM初始化
//  @param      void        
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void eeprom_init(void)
{
    uint32_t clockdiv = 0;
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_EEPROM_MASK;
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_EEPROM_RST_MASK;
    

    clockdiv = main_clk_mhz*1000000 / EEPROM_INTERNAL_FREQ;
    //四舍五入
    if ((main_clk_mhz*1000000 % EEPROM_INTERNAL_FREQ) > (EEPROM_INTERNAL_FREQ>>1)) clockdiv += 1U;

    EEPROM->CLKDIV = clockdiv - 1;
    
    EEPROM->AUTOPROG = 1;
    //设置延时参数
    EEPROM->RWSTATE = EEPROM_RWSTATE_RPHASE1(14) | EEPROM_RWSTATE_RPHASE2(7);
    
    EEPROM->WSTATE = ( 0 
                     | EEPROM_WSTATE_PHASE1(4)
                     | EEPROM_WSTATE_PHASE2(8)
                     | EEPROM_WSTATE_PHASE3(2)
                     //| EEPROM_WSTATE_LCK_PARWEP_MASK
                      );
    EEPROM->CMD = EEPROM_PROGRAM_CMD;
    while(!(EEPROM->INTSTAT & EEPROM_INTSTAT_END_OF_PROG_MASK));

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      EEPROM写入一个字(4个字节)
//  @param      offset       写入EEPROM的地址偏移量
//  @param      *data        需要写入数据地址
//  @return     void
//  Sample usage:            eeprom_write_word(0,12345); 数据12345写入到偏移量为0的EEPROM区域
//-------------------------------------------------------------------------------------------------------------------
void my_eeprom_write_word(uint16 Page_num,int16 offset, uint32 data)
{
    uint32 *addr;

    if (offset >= EEPROM_OFFSET_MAX)    ASSERT(0);
    if(EEPROM->AUTOPROG)    EEPROM->AUTOPROG = 1;           //设置自动编程
    EEPROM->INTSTATCLR = EEPROM_INTENSET_PROG_SET_EN_MASK;  //清除标志位
    addr = (uint32 *)(EEPROM_BASE_ADDRESS + Page_num * EEPROM_PAGE_SIZE+offset*4);//计算地址
    *addr = data;                                           //写入数据
    if(0x01 != EEPROM->AUTOPROG)                            //检验是否需要手动清除
    {
        EEPROM->CMD = EEPROM_PROGRAM_CMD;
    }
    while(!(EEPROM->INTSTAT & EEPROM_INTSTAT_END_OF_PROG_MASK));//等待完成操作
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      EEPROM写入一个字(4个字节)
//  @param      pagenum      写入EEPROM的页 一页可写32个uint32类型数据
//  @param      *data        需要写入的数据的地址 
//  @return     void
//  Sample usage:            uint32 dat[32];  eeprom_write_page(0,dat); 将dat数组的内容全部写入EEPROM的第0页
//  @note                    定义数组 数组元素务必为32个 避免出现数组越界访问
//-------------------------------------------------------------------------------------------------------------------
void eeprom_write_page(uint16 pagenum, uint32 *data)
{
    uint8 i;
    uint32 *addr;

    if (pagenum >= EEPROM_PAGE_COUNT || !data)    ASSERT(0);

    if(EEPROM->AUTOPROG)    EEPROM->AUTOPROG = 2;           //设置自动编程
    EEPROM->INTSTATCLR = EEPROM_INTENSET_PROG_SET_EN_MASK;  //清除标志位
    addr = (uint32 *)(EEPROM_BASE_ADDRESS + pagenum * EEPROM_PAGE_SIZE);//计算地址
    for(i=0;i<EEPROM_PAGE_SIZE/4;i++)
    {
        addr[i] = data[i];                                  //写入数据
    }

    if(0x00 == EEPROM->AUTOPROG)                            //检验是否需要手动清除
    {
        EEPROM->CMD = EEPROM_PROGRAM_CMD;   
    }
    while(!(EEPROM->INTSTAT & EEPROM_INTSTAT_END_OF_PROG_MASK));//等待完成操作
}






