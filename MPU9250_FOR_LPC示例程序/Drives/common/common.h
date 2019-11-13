/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		common
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/
 
 
 
#ifndef _common_h
#define _common_h



//数据类型声明
typedef unsigned char       uint8;   //  8 bits 
typedef unsigned short int  uint16;  // 16 bits 
typedef unsigned long int   uint32;  // 32 bits 
typedef unsigned long long  uint64;  // 64 bits 

typedef char                int8;    //  8 bits 
typedef short int           int16;   // 16 bits 
typedef long  int           int32;   // 32 bits 
typedef long  long          int64;   // 64 bits 

typedef volatile int8       vint8;   //  8 bits 
typedef volatile int16      vint16;  // 16 bits 
typedef volatile int32      vint32;  // 32 bits 
typedef volatile int64      vint64;  // 64 bits 

typedef volatile uint8      vuint8;  //  8 bits 
typedef volatile uint16     vuint16; // 16 bits 
typedef volatile uint32     vuint32; // 32 bits 
typedef volatile uint64     vuint64; // 64 bits 

typedef enum // 枚举端口编号 
{
    A  = 0,
    B  = 1,
    
    //按照数据手册 命名方式
    P0 = 0,
    P1 = 1,
    //A与P0等价  B与P1等价
}PORTN_enum;



typedef enum // 枚举端口编号 
{
    //由于数据手册的命名方式并不友好，因此系统板将命名方式更改为与K60那样
    //便于使用核心板的时候查找引脚
    //核心板A0引脚与数据手册的P0端口0号引脚（即P00引脚）是对应的，A表示P0端口，A后面的数据表示引脚编号
    A0=0,   A1,  A2,  A3,  A4,  A5,  A6,  A7,
    A8,     A9,  A10, A11, A12, A13, A14, A15,
    A16,    A17, A18, A19, A20, A21, A22, A23,
    A24,    A25, A26, A27, A28, A29, A30, A31,
            
    B0,     B1,  B2,  B3,  B4,  B5,  B6,  B7,
    B8,     B9,  B10, B11, B12, B13, B14, B15,
    B16,    B17, B18, B19, B20, B21, B22, B23,
    B24,    B25, B26, B27, B28, B29, B30, B31,
    
    
    //按照数据手册 命名方式
    P00=0,  P01,  P02,  P03,  P04,  P05,  P06,  P07,
    P08,    P09,  P010, P011, P012, P013, P014, P015,
    P016,   P017, P018, P019, P020, P021, P022, P023,
    P024,   P025, P026, P027, P028, P029, P030, P031,
            
    P10,    P11,  P12,  P13,  P14,  P15,  P16,  P17,
    P18,    P19,  P110, P111, P112, P113, P114, P115,
    P116,   P117, P118, P119, P120, P121, P122, P123,
    P124,   P125, P126, P127, P128, P129, P130, P131,

    //A0与P00代表同一个引脚     后面依次类推
}PIN_enum;



typedef enum //枚举端口方向
{
    GPI = 0, //定义管脚输入方向      
    GPO = 1, //定义管脚输出方向
}GPIODIR_enum;

typedef enum //枚举端口电平
{
    GPIO_LOW = 0,  //定义管脚输入方向      
    GPIO_HIGH = 1, //定义管脚输出方向
}GPIOLEVEL_enum;

typedef enum    // 枚举触发方式
{
    LOW,
    HIGH,
    RISING,
    FALLING,
    BOTH,       //仅PINT模块 可用此触发方式
}TRIGGER_enum;




// Compiler Related Definitions 
#ifdef __CC_ARM                         // ARM Compiler 
    #define ALIGN(n)                    __attribute__((aligned(n)))
#elif defined (__IAR_SYSTEMS_ICC__)     // for IAR Compiler 
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
#elif defined (__GNUC__)                // GNU GCC Compiler 
    #define ALIGN(n)                    __attribute__((aligned(n)))
#endif // Compiler Related Definitions 



/*
 * 确保x的范围为 min~max
 */
#define RANGE(x,max,min)        (((x)<(min) ? (min) : ( (x)>(max) ? (max):(x) )))



#include <math.h>
#include <string.h>

#include "SEEKFREE_PRINTF.h"
#include "LPC546XX_config.h"
#include "LPC54606.h"

#include "misc.h"

#include "assert.h"




#endif
