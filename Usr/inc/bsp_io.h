#ifndef __BSP_IO_H
#define __BSP_IO_H
#include "stdint.h"
void bsp_LedInit(void);
void HW_RedLed_On(void);
void HW_RedLed_Off(void);
void HW_RedLed_Toggle(void);
void HW_GreenLed_On(void);
void HW_GreenLed_Off(void);
void HW_GreenLed_Toggle(void);

void bsp_HW_VG_ctrlon(uint8_t en);

void bsp_buck_enable(uint8_t en);
#endif


