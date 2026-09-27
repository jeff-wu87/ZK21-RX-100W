/*
 ******************************************************************************
 * @file    app_protect.c
 * @author  wu.jianming
 * @version V1.0.0
 * @date    2026-08-16
 * @brief   保护 功能，
 * 
 ******************************************************************************
*/


#include "app_protect.h"
#include "app_control.h"

 
/*变量 --门限 --恢复值--延时 --计数 -- 类型 -- 故障 */
ProtectItem protect_table[] =
{    
    
    //发射端保护
    {&system_info.sys_voltage.vbus,SYS_OVP_V,REC_SYS_OVP_V,100,1000,0,0,  PROTECT_OVER,0},   
    {&system_info.sys_voltage.vbus,SYS_UVP_V,REC_SYS_UVP_V ,500,2000,0,0,   PROTECT_UNDER,1},   //vbus欠压 
    {&system_info.sys_voltage.cur, SYS_OCP_V, REC_SYS_OCP_V, 20,8000,0, 0,   PROTECT_OVER,2},  
    {&system_info.sys_voltage.ntc_temp, SYS_OTP_V,REC_SYS_OTP_V,100,5000, 0,0, PROTECT_OVER,3},
    {&system_info.sys_voltage.vin, SYS_IN_OVP_V,REC_SYS_IN_OVP_V,500,5000,0,0, PROTECT_OVER,4},
    {&system_info.sys_voltage.vin, SYS_IN_UVP_V,REC_SYS_IN_UVP_V,800,2000,0,0, PROTECT_UNDER,5},  //输入欠压
  
};


#define PROTECT_AUTO_RECOVER  1

/*写入保护值，做保护。。。。。。。*/
#define PROTECT_NUM  (sizeof(protect_table)/sizeof(protect_table[0]))
    


/**
过压过流欠压保护， 带自动恢复功能   

**/
void Protect_Scan(uint16_t *pt)
{
    uint8_t i;

    for(i=0;i<PROTECT_NUM;i++)
    {
        bool trigger = false;

        if(protect_table[i].type == PROTECT_OVER)
        {
            trigger = (*protect_table[i].value > protect_table[i].threshold);
        }
        else
        {
            trigger = (*protect_table[i].value < protect_table[i].threshold);
        }

        if(trigger)
        {
            protect_table[i].recover_counter = 0;
            if(protect_table[i].counter < protect_table[i].delay)
            {
                protect_table[i].counter++;
            }
            else
            {
                *pt |= (1 << protect_table[i].fault_id);
            }
        }
        else
        {
             protect_table[i].counter = 0;
             /*------------恢复----------------*/
            if(PROTECT_AUTO_RECOVER)
            {
                bool recover=false;

                if(protect_table[i].type==PROTECT_OVER)
                    recover = (*protect_table[i].value < protect_table[i].recover);
                else
                    recover = (*protect_table[i].value > protect_table[i].recover);

                if(recover)
                {
                    if(protect_table[i].recover_counter < protect_table[i].recover_delay)
                    {
                        protect_table[i].recover_counter++;
                    }
                    else
                    {
                       // protect_table[i].sys_fault.err_dat&=~p->fault_mask;
                        *pt &= ~(1 << protect_table[i].fault_id);
                    }
                }
                else
                {
                    protect_table[i].recover_counter=0;
                }
            }
           
        }
    }
}





