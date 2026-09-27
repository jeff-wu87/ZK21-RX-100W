
/*
 ******************************************************************************
 * @file    app_control.c
 * @author  wu.jianming
 * @version V1.0.0
 * @date    2026-08-16
 * @brief   zk21-100w 接收板子 
 * 
 ******************************************************************************
*/


#include "app_control.h"
#include "bsp_adc.h"
#include "bsp_flash.h"
#include "uart_handler.h"

#include "app_led.h"
#include "bsp_io.h"
#include "qi_tx_hw.h"

#include "app_protect.h"
#include "stdio.h"

 _ADC_sample adc_voltage;  
 
SYS_inform system_info;
 


/*  阀值设定 */
#define  VG_ON_THRE    2280u
#define  VG_OFF_THRE   2130u 

#define DLY_CNTMAX       200
#define DLY_CNTMAX_ON    50
#define STANDBY_CUR 300

#define  VG_ENABLE 1
#define  VG_DISABLE 0

#define BEG_DLYTIME 5000




enum 
{
   e_vg_off = 0,
   e_vg_on = 1,
   e_buck_on = 2,
   e_buck_off = 3,
    
}e_vg_mos;
 
/**************************/
void app_buck_protect(SYS_voltage  *bptr);

static SYS_STATUS_e e_ledstate = SYS_ALL_OFF;
#define CUR_TEST_LEN 20
uint16_t  cur_avg_test[CUR_TEST_LEN];
uint16_t  vbus_avg_test[CUR_TEST_LEN];
uint16_t  vin_avg_test[CUR_TEST_LEN];
#define FLASH_BUF_LEN 10
uint8_t flash_buf[FLASH_BUF_LEN];
//小端模式
#define UVP_ADR  8
#define VBUS_ADR 0

void app_sys_init(void)
{
    
    
    system_info.sys_fault.err_dat = 0;
    system_info.buck_enable_state =  e_buck_off;
    system_info.vg_mos_state = e_vg_off;
    bsp_HW_VG_ctrlon(0);
    bsp_buck_enable(0);
    qi_tx_hw_setdat(2400, 0, 40, 2400);
    system_info.sys_voltage.vbus =0;
    system_info.sys_voltage.cur = 0;
    
    system_info.protect_begin = 0;
    //system_info.uvp_dly_time = 200;
    e_ledstate = SYS_ALL_OFF;
    
    bsp_flash_read(flash_buf,FLASH_BUF_LEN);
    system_info.uvp_dly_time = flash_buf[UVP_ADR]|(flash_buf[UVP_ADR + 1] << 8);
    
    if(system_info.uvp_dly_time == 0 || system_info.uvp_dly_time == 0xffff)
    {
        system_info.uvp_dly_time = 200;
    }
    if(system_info.uvp_dly_time > 8000)
    {
        system_info.uvp_dly_time = 8000;
    }
    /* printf("uvp_dly_time %d\r\n",system_info.uvp_dly_time);*/
}

/**
 主控逻辑
**/



void app_control(void)
{
    static uint16_t dly1 = 0;
    static uint16_t dly2 = 0;
    static uint16_t prot_dly  = 0;
    static uint16_t prot2_dly  = 0;
    
    bsp_get_sample_val(&adc_voltage);   /*采样adc数据*/
    app_sys_voltage(&system_info.sys_voltage);  /*换整形数据*/
   
    
    
    if(system_info.protect_begin)
    {
        Protect_Scan(&system_info.sys_fault.err_dat);  /*报错保护*/
        system_info.sys_fault.err_dat = system_info.sys_fault.err_dat & 0x00ff; 
    
    }
    
    if(system_info.buck_enable_state == e_buck_on)
    {   
        if(prot2_dly > system_info.uvp_dly_time)  //buck打开延迟200ms打开保护
        {
            system_info.protect_begin = 1;
        }
        else prot2_dly ++;
    }
    else 
    {
        if(prot_dly > (BEG_DLYTIME + system_info.uvp_dly_time))    //5秒没开 buck 就打开保护功能
        {
            system_info.protect_begin = 1;
        }
        else prot_dly ++;
        
        
    }        
    if(system_info.sys_fault.err_dat == 0)
    {   
      
           if(system_info.sys_voltage.vbus > VG_ON_THRE)
           {     
                if(dly1 > DLY_CNTMAX_ON)
                {  
                    
                    
                    //if(system_info.vg_mos_state == e_vg_off)
                   // {
                        bsp_HW_VG_ctrlon(VG_ENABLE);
                        system_info.vg_mos_state  = e_vg_on;
                   // }
                    

                    if(system_info.sys_voltage.cur < STANDBY_CUR) //0.3A
                    {
                        e_ledstate = SYS_STANDBY_OUT; 
                    }
                    else {
                        e_ledstate = SYS_NORMAL;
                    }
                    
                 
                }
                else 
                {
                    dly1++;
                    
                    
                }
                
                 
          }
 #if 0
          else  if(system_info.sys_voltage.vbus < VG_OFF_THRE)
          {              
              dly1 = 0;
              if(dly2 > DLY_CNTMAX)
              {
                    if(system_info.vg_mos_state  == e_vg_on)
                     {
                          system_info.vg_mos_state  = e_vg_off;
                          bsp_HW_VG_ctrlon(VG_DISABLE);
                     }
              }
              else dly2 ++;
          }
#else
           else 
           {
               dly1 = 0;
           }
 #endif            
           
           
 
           
 
               
          if(system_info.sys_voltage.vin > BUCK_OPEN_VAL){    //25V
               
               if(++dly2 > DLY_CNTMAX_ON)  //延时50ms
               {
                   dly2 = 0;
                  
                   bsp_buck_enable(VG_ENABLE);
                   system_info.buck_enable_state = e_buck_on;
                 
               }
         }
           

      } 
      else   //有错误
      {
          
          if(system_info.buck_enable_state == e_buck_on)
          {
          
   
              if(system_info.sys_fault.fault_flag.vbus_UVP != 1)  //vbus欠压不关闭buck
              {
                    bsp_buck_enable(VG_DISABLE);
                    system_info.buck_enable_state = e_buck_off;
              }
              else {
                     
                    system_info.buck_enable_state = e_buck_on;
              
              }
              
               
              
          }
          else {
             if(system_info.sys_fault.fault_flag.vbus_UVP == 1)
             {
                       
                    if(system_info.sys_voltage.vin > BUCK_OPEN_VAL){

                        bsp_buck_enable(VG_ENABLE);
                        system_info.buck_enable_state = e_buck_on;
                    }
             }
                    
          
          }
          if(system_info.vg_mos_state  == e_vg_on)
          {
                 system_info.vg_mos_state  = e_vg_off;
                 bsp_HW_VG_ctrlon(VG_DISABLE);
          }
           
          
           ///////////////////////////////////////
          /*下面是记录状态，更新到LED显示*/
          if(system_info.sys_fault.fault_flag.vin_UVP == 1)
          {
                e_ledstate = SYS_UNDER_VIN;
          }
         
          else if(system_info.sys_fault.fault_flag.vbus_OCP == 1)   
          {
                e_ledstate =  SYS_VBUS_OCP;
          }              
           
         else if(system_info.sys_fault.fault_flag.vbus_OVP == 1 || system_info.sys_fault.fault_flag.vin_OVP == 1)   
          {
                e_ledstate =  SYS_VBUS_OVP;
          } 
          
         else if(system_info.sys_fault.fault_flag.vbus_UVP == 1)   
           {
               e_ledstate =   SYS_UNDER_VOUT;
           }
         
           else  if(system_info.sys_fault.fault_flag.ntc_OTP == 1)   
           {
              e_ledstate = SYS_OTHER_FAULT;
           }
           else e_ledstate = SYS_DEFAULT;
          
      }
      

    app_Led_SetSysStatus(e_ledstate); 

    qi_tx_hw_setdat(system_info.sys_voltage.vbus, system_info.sys_voltage.cur    ,\
                    system_info.sys_voltage.ntc_temp,system_info.sys_voltage.vin);  //system_info.sys_voltage.cur 
   
      
           
}

/*
浮点换成整形
*/
void app_sys_voltage(SYS_voltage *pt)
{   
    uint32_t  tmp;
    uint32_t  tmp1;
    uint32_t  tmp2;
    uint8_t i;
    static uint8_t cur_cnt;
    
    if(pt == NULL) return ;
    
    //pt->vin = adc_voltage.vin * 100;   /*协议 电压放大100.电流放大1000*/
   // pt->vbus = adc_voltage.vbus * 100;
    //pt->cur = adc_voltage.cur * 1000;
    pt->vbus1 = adc_voltage.vbus1 * 100;
    pt->ntc_temp = (uint16_t)adc_voltage.ntc_temp;
    
    
     if(cur_cnt < CUR_TEST_LEN)
     {
          cur_avg_test[cur_cnt] = adc_voltage.cur * 1000; 
          vbus_avg_test[cur_cnt] = adc_voltage.vbus * 100;
          vin_avg_test[cur_cnt] = adc_voltage.vin * 100;
          
          cur_cnt ++;
           
     } 
     else
     {   
         cur_cnt = 0;
         tmp = 0;
         tmp2 = 0;
         tmp1 = 0;
         for(i = 0;i < CUR_TEST_LEN; i++)
         {
            tmp += cur_avg_test[i];
            tmp1 += vbus_avg_test[i];
            tmp2 += vin_avg_test[i];
         }
         pt->cur =  tmp /  CUR_TEST_LEN;
         pt->vbus = tmp1 / CUR_TEST_LEN;
         pt->vin = tmp2 / CUR_TEST_LEN;
         
         
         
     }  
     
     
}

/*
给 UVP保护开启时间赋值，
*/
void app_set_uvptime(uint16_t dat)
{
    
    bsp_flash_read(flash_buf,FLASH_BUF_LEN);
    system_info.uvp_dly_time = dat;
    flash_buf[UVP_ADR] =  dat & 0X00FF;  
    flash_buf[UVP_ADR + 1] =  (dat &0xff00) >> 8;   
    if(bsp_flash_write(flash_buf,FLASH_BUF_LEN) == false)
    {
        
        bsp_flash_write(flash_buf,FLASH_BUF_LEN);
    }
}



uint16_t app_sys_vbus()
{
     return  system_info.sys_voltage.vbus;
}
uint16_t app_sys_vin()
{
     return  system_info.sys_voltage.vin;
}

uint16_t app_sys_cur()
{

    return  system_info.sys_voltage.cur;
}

uint16_t app_sys_temp()
{
     return  system_info.sys_voltage.ntc_temp;
}

uint16_t app_sys_fault()
{
    return  system_info.sys_fault.err_dat;
}


//A5 01帧头 15命令 BB和校验
//接收主机回复指令 
//A5 0E 15 05 45 01 2C 05 2A 00 00 03 EB 09 8D 08 00 60 3F 00 99
//解析
//A5 0E帧头
//15 命令
//05 45 保留
//01 2C 保留
//05 2A 保留
//00 00 保留
//03 EB 输出电流1.003A
//09 8D 输出电压24.45V
//08 00 60 保留
//3F  接收温度63度
//00 保留
//99 和校验

void app_send_sys_dat(uint8_t d_cmd)
{   
     /*发固定命令*/
    uint8_t data[21] = {0xA5,0x0E,0x15,0x05,0x45 ,0x01,\
    0x2C,0x05,0x2A,0x00,0x00,0x03,0xEB,0x09,0x8D,0x08,0x00,0x60,0x3F,0x00,0x99};  /* 2 bytes payload */
    uint16_t sum;
    uint8_t i;
    uint16_t tmpdat;
    if(d_cmd != 0x15) return ;
    sum = 0;
    
    tmpdat = app_sys_fault();
    data[7] =   (tmpdat & 0xff00) >> 8;
    data[8] =   tmpdat & 0x00ff;
    
    tmpdat = app_sys_vin();
    data[9] =   (tmpdat & 0xff00) >> 8;
    data[10] =   tmpdat & 0x00ff;
    
    tmpdat = app_sys_cur();
    data[11] =   (tmpdat & 0xff00) >> 8;
    data[12] =   tmpdat & 0x00ff;
    
    tmpdat = app_sys_vbus();
    
    data[13] =   (tmpdat & 0xff00) >> 8;
    data[14] =   tmpdat & 0x00ff;
    
    tmpdat = app_sys_temp();
    
    data[18] =   tmpdat & 0x00ff;
    for(i = 0;i < 20;i ++) 
    {
        sum += data[i];
    }
    data[20] = sum &0x00ff;    
    uart_handler_write_buf(data,21); 
}
