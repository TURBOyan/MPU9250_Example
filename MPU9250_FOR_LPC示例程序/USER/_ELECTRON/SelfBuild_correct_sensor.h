#ifndef _SELFBUILD_CORRECT_SENSOR_H
#define _SELFBUILD_CORRECT_SENSOR_H

#include "headfile.h"

#define	FLASH_SAVE_MCP41				(FLASH_SECTOR_NUM - 23)

#define ADC_R_R_Min_8bit 50	//五电感最小值
#define ADC_R_Min_8bit   50
#define ADC_M_Min_8bit   50
#define ADC_L_Min_8bit   50
#define ADC_L_L_Min_8bit 50

extern void Correct_Sensor(void);
extern void FLASH_InitMCP41(void);
extern void Normalized_MCP41(void);


#endif