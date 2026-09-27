#ifndef __APP_LED_H
#define __APP_LED_H


// 故障状态枚举
typedef enum
{
    SYS_NORMAL = 0,        // 正常，绿灯常亮
    SYS_UNDER_VIN = 1,     // 输入欠压，红灯1000ms翻转
    SYS_UNDER_VOUT = 2,    // 输出欠压，红灯500ms翻转
    SYS_STANDBY_OUT = 3,     // 待机状态0电流
    SYS_VBUS_OCP = 4,    //  过流保护
    SYS_VBUS_OVP = 5,    //  过流保护
    SYS_OTHER_FAULT = 6,    // 其他故障，红灯常亮
    SYS_DEFAULT = 7,
    SYS_ALL_OFF = 8,  //全部关闭
} SYS_STATUS_e;



typedef void (*LedFunc)(void);

// 硬件操作接口结构体，统一注册底层函数
typedef struct
{
    LedFunc RedOn;
    LedFunc RedOff;
    LedFunc RedTog;
    LedFunc GreenOn;
    LedFunc GreenOff;
    LedFunc GreenTog;
} LedHwInterface_t;
 // 对外API
// 注册底层硬件驱动接口
void Led_RegisterHwDrv(const LedHwInterface_t *pDrv);
// 设置系统故障状态
 
// 1ms周期调用任务
void Led_Task_1ms(void);
 void app_Led_SetSysStatus(SYS_STATUS_e status);
 void app_Led_Task_1ms(void);

#endif

