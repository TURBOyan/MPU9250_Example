/*!
 * @file       mcp41.h
 * @brief      mcp41数字电位器函数实现(带OLED版本)
 * @author     llm
 */
#ifndef __SELFBUILD_MCP41_H
#define __SELFBUILD_MCP41_H

#include "headfile.h"

#define MCP41_SCK		PTE2
#define MCP41_SDA		PTE1

#define SPI_MCP41_SCKL	gpio_set(MCP41_SCK, 0)
#define SPI_MCP41_SCKH	gpio_set(MCP41_SCK, 1)
#define SPI_MCP41_SDAL	gpio_set(MCP41_SDA, 0)
#define SPI_MCP41_SDAH	gpio_set(MCP41_SDA, 1)

#define CS0		PTC8
#define CS1		PTC10
#define CS2		PTE6
#define CS3		PTE4
#define CS4		PTE5
#define CS5		0xff
#define CS6		0xff
#define CS7		0xff

//#define P0		AD_CH_L_1
//#define P1		AD_CH_L__
//#define P2		AD_CH_M_1
//#define P3		AD_CH_R__
//#define P4		AD_CH_R_1
//#define P5		0xff
//#define P6		0xff
//#define P7		0xff

void MCP41_Init(void);
void MCP41_SetCmd(uint8 Set_Num ,uint8 data);
uint8 MCP41_SetValue(uint8 Set_Num, uint8 Ask_Value);

#endif
