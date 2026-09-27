#ifndef _APP_CONTROL_H
#define _APP_CONTROL_H
#include "stdint.h"

typedef struct
{    
    
    uint16_t vin;
    uint16_t vbus;
    uint16_t vbus1; 
    uint16_t cur;
    uint16_t ntc_temp;
    
    
}SYS_voltage;


 


typedef union
{
    uint16_t err_dat;
    struct
    {

        uint16_t vbus_OVP:1;
        uint16_t vbus_UVP:1;
        uint16_t vbus_OCP:1;
        uint16_t ntc_OTP:1;
        uint16_t vin_OVP:1;
        uint16_t vin_UVP:1;
        uint16_t over_temp:1;
        uint16_t over_ntc1:1;
        
        uint16_t nc2_over:8;
   
    }fault_flag;



}_SYS_FAULT_t;

typedef struct
{
   SYS_voltage  sys_voltage;
   _SYS_FAULT_t sys_fault;
   uint8_t buck_enable_state;
   uint8_t vg_mos_state;
   uint8_t protect_begin;
   uint16_t uvp_dly_time;
}SYS_inform;



 




void app_control(void);
void app_sys_voltage(SYS_voltage *pt);
void app_send_sys_dat(uint8_t CMD);

uint16_t app_sys_vbus(void);
uint16_t app_sys_vin(void);
uint16_t app_sys_cur(void);
uint16_t app_sys_temp(void);
uint16_t app_sys_fault(void);
void app_sys_init(void);
void app_set_uvptime(uint16_t dat);
extern SYS_inform system_info;

#endif

