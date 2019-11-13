#ifndef _SELFBUILD_SONIC_H_
#define _SELFBUILD_SONIC_H_

#include "headfile.h"

struct SONIC_Data_Typedef
{
	uint8 Measure_Flag,Finish_Flag;
	uint32 Time,
				 Distance;
};
extern struct SONIC_Data_Typedef SONIC_Data;

void SONIC_Init(CTIMER_CAPTURE_enum capturech,CAPTURE_ch_enum CAPTURE_ch);
void SONIC_gettime(CTIMER_CAPTURE_enum capturech,CAPTURE_ch_enum CAPTURE_ch);

#endif
