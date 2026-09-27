#include "bsp_io.h"
#include "../inc/main.h"

#include "app_led.h"

#define R_LED_ON()     PA03_SETLOW()
#define R_LED_OFF()    PA03_SETHIGH()  
#define G_LED_ON()     PA04_SETLOW()
#define G_LED_OFF()    PA04_SETHIGH()
 
#define  G_LED_TOG()   PA04_TOG()
#define  R_LED_TOG()   PA03_TOG()


 #define MOS_VG_ON()     PB02_SETLOW()
#define MOS_VG_OFF()    PB02_SETHIGH()

#define BUCK_EN_ON()     PB05_SETLOW()
#define BUCK_EN_OFF()     PB06_SETHIGH() 
 
 


void bsp_LedInit(void)
{
    // 填充底层硬件操作函数地址
        LedHwInterface_t ledDrv = {
        .RedOn    =      HW_RedLed_On,
        .RedOff   =      HW_RedLed_Off,
        .RedTog   =      HW_RedLed_Toggle,
        .GreenOn  =      HW_GreenLed_On,
        .GreenOff =      HW_GreenLed_Off,
        .GreenTog =      HW_GreenLed_Toggle
    };
    // 注册到LED业务层，完成解耦绑定
    Led_RegisterHwDrv(&ledDrv);

    // 上电默认正常状态
    app_Led_SetSysStatus(SYS_NORMAL);
}

void HW_RedLed_On()
{
    R_LED_ON();

}
void HW_RedLed_Off()
{
    R_LED_OFF();
}
void HW_RedLed_Toggle()
{
    R_LED_TOG();
}
void HW_GreenLed_On()
{
    PA04_SETLOW();
}
void HW_GreenLed_Off()
{
    PA04_SETHIGH();
}
void HW_GreenLed_Toggle()
{
    G_LED_TOG();
}
void bsp_HW_VG_ctrlon(uint8_t en)
{   
    if(en == 1)
    {
        MOS_VG_ON();
    }
    else
    {
        MOS_VG_OFF();
    }
  
}

void bsp_buck_enable(uint8_t en)
{

    if(en == 1)
    {
        BUCK_EN_ON();
    }
    else
       BUCK_EN_OFF(); 
}

 


