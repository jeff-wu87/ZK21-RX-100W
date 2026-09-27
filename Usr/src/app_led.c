#include "app_led.h"
#include "stdint.h"
#include "stdio.h"
#include "bsp_io.h"

 



// 全局硬件接口句柄，保存注册进来的底层函数指针
static LedHwInterface_t m_LedDrvIf;
static uint8_t m_IsDrvReg = 0;      // 标记是否完成注册
static SYS_STATUS_e m_SysState = SYS_NORMAL;
static uint16_t m_LedTick = 0U;


void Led_RegisterHwDrv(const LedHwInterface_t *pDrv)
{
    if(NULL == pDrv)
    {
        return;
    }
    m_LedDrvIf = *pDrv;
    m_IsDrvReg = 1;
}

void app_Led_SetSysStatus(SYS_STATUS_e status)
{
    m_SysState = status;
    
}

void app_Display_4_num(void) 
{
    static  uint16_t CNT_Led_count;
    static uint16_t CNT_Led_Base_Time;
    
    if(CNT_Led_Base_Time++ > 1400)
    {
        CNT_Led_Base_Time = 0;
        CNT_Led_count=1;
    }
	m_LedDrvIf.GreenOff();
	if(CNT_Led_count <= 8)
	{   
		if(CNT_Led_Base_Time ==(80 * CNT_Led_count))
		{
			CNT_Led_count++;
			m_LedDrvIf.RedTog(); 
		}
	}

}
    

void app_Led_Task_1ms(void)
{
    // 未注册硬件驱动直接退出，防空指针崩溃
    if(0 == m_IsDrvReg || NULL == m_LedDrvIf.RedOn)
    {
        return;
    }

    m_LedTick++;

    switch(m_SysState)
    {
        case SYS_NORMAL:
            m_LedDrvIf.GreenOn();
            m_LedDrvIf.RedOff();
            m_LedTick = 0;
            break;

        case SYS_UNDER_VIN:    //输入欠压
            m_LedDrvIf.GreenOff();
            if(m_LedTick >= 180U)
            {
                m_LedDrvIf.RedTog();
                m_LedTick = 0U;
            }
            break;

        case SYS_UNDER_VOUT:   //输出欠压
            m_LedDrvIf.GreenOff();
            if(m_LedTick >= 180U)
            {
                m_LedDrvIf.RedTog();
                m_LedTick = 0U;
            }
            break;
        case SYS_STANDBY_OUT:   
             m_LedDrvIf.RedOff();
            if(m_LedTick >= 600U)
            {
                m_LedDrvIf.GreenTog();
                m_LedTick = 0U;
            }
            break;
        
                     
        case  SYS_VBUS_OCP:
            
            m_LedDrvIf.GreenOff();
            if(m_LedTick >= 70U)
            {
                m_LedDrvIf.RedTog();
                m_LedTick = 0U;
            }
            break;
        
         case  SYS_VBUS_OVP:
            
            m_LedDrvIf.GreenOff();
            if(m_LedTick >= 600U)
            {
                m_LedDrvIf.RedTog();
                m_LedTick = 0U;
            }
            break;    
             

        case SYS_OTHER_FAULT:
            m_LedDrvIf.GreenOff();
           // m_LedDrvIf.RedOn();
           app_Display_4_num();
            m_LedTick = 0;
            break;
        case SYS_ALL_OFF:
            m_LedDrvIf.GreenOff();
            m_LedDrvIf.RedOff();
            break;
        
        default:  //其他错误
            m_LedDrvIf.GreenOff();
            m_LedDrvIf.RedOn();
            m_LedTick = 0;
            break;
    }
}
