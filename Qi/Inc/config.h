
#ifndef _CONFIG_H_
#define _CONFIG_H_

//#include "cms32f033x.h"
//#include "demo_uart.h"
//#include "demo_timer.h"
//#include "demo_opa.h"
//#include "demo_acmp.h"
//#include "demo_adc.h"
//#include "demo_epwm.h"
//#include "demo_pga.h"
#include <stdio.h>
#include <string.h>
#include "cw32l010_gpio.h"
#include "cw32l010.h"

//typedef BIT                   FlagStatus;

typedef unsigned char   bit;





//typedef uint32_t  u32;
//typedef uint16_t  u16;
//typedef uint8_t   u8;
//typedef signed char   s8;

#define DEBUG_PRINT 1

#define CFG_TEMPERATURE      1
//#define BREATH_LED

#define SET     1
#define RESET     0



// #define D_F_Max   205
// #define D_F_Min   110
// #define D_F_FirstOn     175

// #define D_F_FirstOn_code   (48000/D_F_FirstOn)
// #define D_F_Max_Code (48000/D_F_Max)
// #define D_F_Min_Code (48000/D_F_Min)

// #define D_VDD 50  //unit: 0.1V
// #define D_VDD_Limit 40  //unit: 0.1V
// #define D_Temperature_Limit 37//38 //unit: 0.1V
// #define D_Amplitude_Limit 12 //unit: 0.1V
// #define D_Current_Limit 5//20 //unit: 0.1V
// #define D_Temperature_Resume 26//20 //unit: 0.1V

// #define D_Refer_Voltage 12   //25  //unit: 0.1 V
// #define D_VDD_Limit_Code (u16)((u16)D_Refer_Voltage*0x1000/D_VDD_Limit)

// #define D_Temperature_Limit_Code  (u16)((u32)D_Temperature_Limit*0x1000/D_VDD)
// #define D_Amplitude_Limit_Code 	 (u16)((u32)D_Amplitude_Limit*0x1000/D_VDD)
// #define D_Current_Limit_Code 	(u16)((u32)D_Current_Limit*0x1000/D_VDD)

// #define D_Temperature_Resume_Code 	(u16)((u32)D_Temperature_Resume*0x1000/D_VDD)



#define D_DECODE_TIME_MAX   (700*2)//(600*2)
#define D_DECODE_TIME_ONE   (350*2)
#define D_DECODE_TIME_ZERO  (150*2)



//#define LedRed_ON    {P01 = 0;}
//#define LedRed_OFF   {P01 = 1;}
//#define LedGreen_ON  {}
//#define LedGreen_OFF {}
    
    
#define D_LED_INIT  	  {} 
#define D_LED_R_SET  	  {} 
#define D_LED_R_CLR  	  {}  
#define D_LED_R_TOGGLE       {}
#define D_LED_G_SET  	   {GPIO1->DO_f.P5 = 0;}//P17=0;}
#define D_LED_G_CLR  	  {GPIO1->DO_f.P5 = 1;}//P17=1;}
#define D_LED_G_TOGGLE      {GPIO1->DO_f.P5 = ~GPIO1->DO_f.P5;}//P17=~P17;} 
#define D_LED_B_SET  	   {}
#define D_LED_B_CLR  	  {}
#define D_LED_B_TOGGLE      {} 


#define PIN_COM_0       {CW_GPIOA->BRR = bv5;} //GPIO3->DO_f.P1 = 0;}
#define PIN_COM_1       {CW_GPIOA->BRR = bv6;} //GPIO3->DO_f.P2 = 0;}
#define PIN_COM_flip    {CW_GPIOA->TOG = bv5;} //GPIO3->DO_f.P1 =~ GPIO3->DO_f.P1;}
#define PIN_COM1_flip   {CW_GPIOA->TOG = bv6;} //GPIO3->DO_f.P2 =~ GPIO3->DO_f.P2;}


#define D_vol_scal_factor   12//25//8//5//10  //unit: ..

#define D_vol_scal_Interval_min   (signed char)-1  //unit: ..
#define D_vol_scal_Interval_max   (signed char)1  //unit: ..

#define D_Control_Error_min     -40//-50
#define D_Control_Error_max     40


#define D_11bit_SignalStrength         0x402    // Header of Packet with 11bit
#define D_11bit_EdnPowerTransfer       0x404
#define D_11bit_ControlError           0x606
#define D_11bit_RectifiedPower         0x408


#define D_QI_tsilent   9  //unit: ms  ==P48==
#define D_QI_tnext     (float)17  //unit: ms  ==P48== , tnext time shou
#define D_Header       (uint8_t)( D_QI_tnext*2-(uint8_t)(D_QI_tsilent*2))

#define D_QI_tcontrol       23  //unit:ms
#define D_QI_tdelay         5   //unit:ms
#define D_Save_Control_error_Delay  ( 250+50 + D_QI_tcontrol + D_QI_tdelay )
#define D_Save_Control_error_Delay_Long  ( 250+190 + D_QI_tcontrol + D_QI_tdelay )

#define D_bOverFlowCode    240//240//195//250//400//250





typedef enum
{
  LED_TIMER_10MS    = (0x01),
  LED_TIMER_500MS    = (0x02),    
}LED_TIMER_TypeDef;


typedef enum
{
  LED_ALL_OFF    = 0,
  LED_MODE_IDLE=1,
  LED_MODE_CHARGE=2,
  LED_MODE_FOD=3,
  LED_MODE_FULL=4,
  LED_MODE_POWERON=5,
    
}LED_MODE_TypeDef;


typedef enum
{
  CON_ADC_CH_VPP    = 0,
  CON_ADC_CH_Current,
  CON_ADC_CH_PcbTemperature,
  CON_ADC_CH_CoilTemperature,
  CON_ADC_CH_Vref12,
  CON_ADC_CH_VDD,
  
    
}ADC_CH_MODE_TypeDef;




#endif /* _CMS8S6990_H_ */
