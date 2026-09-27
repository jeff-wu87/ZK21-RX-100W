#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"


//sbit P16				= P1^6;
//***********************************
// LED Handler Register Bank 1
//************************************/
uint16_t wLEDCounter;
uint8_t fgLEDInver;
uint16_t w10SCounter;

uint16_t wDelayResetAH8322=0;

extern uint8_t b15WMOD;
//extern uint8_t fgLearning;
extern  uint16_t wCompleteCounter;
extern uint8_t  FODflag;


extern void PWM_SetFreq(unsigned int freq);

extern void F_Breath_LED_Do(void);
extern void F_Breath_LED_Set(uint8_t flag);


//void F_ResetAH8322(void)
//{
//    if(wDelayResetAH8322)
//    {
//        wDelayResetAH8322--;
//        if(wDelayResetAH8322==0)
//        {
//            P31=1;
//        }
//        else
//        {
//            P31=0;
//        }
//    }
//}

void F_LED_Flash(void)
{
    wLEDCounter++;
    if( wLEDCounter > 500 )
    {
        wLEDCounter = 0;
        D_LED_G_SET;

    }
    else if( 250 == wLEDCounter)
    {
        D_LED_G_CLR;
    }
#ifdef BREATH_LED
    F_Breath_LED_Set(0);
#endif
}

void F_LED_Handler()
{

    if(dwAppleLowCurrentCnt<200000)
        dwAppleLowCurrentCnt++;

//    F_ResetAH8322();

//==========================
    switch(bTargetStatus)
    {
    case D_Target_Status_Complete:
    case D_Target_Status_TI_Complete:
//            wCompleteCounter++;
//            if(wCompleteCounter>=3000)
//            {
//                D_LED_G_SET;
//                D_LED_R_CLR;
//                wCompleteCounter=3000;
//            }
//            else
//            {
//                D_LED_G_CLR;
//                D_LED_R_SET;
//            }
#ifdef BREATH_LED
        F_Breath_LED_Set(0);
#endif
       // D_LED_G_SET;
       // D_LED_R_SET;
   #if 0
        if((bSamsungIsFull==0)&&(bManufactor==D_Manufactor_sumsung)&&(fFastcharge>=1))
        {
            bSamsungIsFull = 1;

            D_PWM_OpenHalf;
            PWM_SetFreq(64000/135);

            //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
            //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
            //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        }
	  #endif

        LEDshiftCNT=4;
        break;
    case D_Target_Status_None:

        if(FODflag)
        {
//            if( wLEDCounter > 500 )
//            {
//                wLEDCounter = 0;
//                D_LED_G_SET;

//            }
//            else if( 250 == wLEDCounter)
//            {
//                D_LED_G_CLR;
//            }
            F_LED_Flash();
        }
        else
        {
           // D_LED_G_SET;
			D_LED_G_CLR;
          ///  D_LED_R_SET;
        }

        //D_LED_G_SET;

        if(LEDshiftCNT==0)
        {
            // 手机拿开
            wDelayResetAH8322 = 250;
#ifdef BREATH_LED
            F_Breath_LED_Set(0);
#endif
        }
//        P01 = 0;

        LEDshiftCNT=4;
        break;
    case D_Target_Status_Pinging:



        LEDshiftCNT=4;
        break;
    case D_Target_Status_Pinged:

//	      w10SCounter++;
//          if(w10SCounter>20000)
//          {
//              w10SCounter = 20000;
//              D_LED_G_SET;
//              LEDshiftCNT = 4;
//          }


//				     if(w10SCounter>=10000)
//						 {
//						   //D_LED_SET_1;
//							 w10SCounter=10000;
//						 }
//						 else
//						 {
//						   //D_LED_SET_0;
//						 }

        if(LEDshiftCNT==0)
        {
            D_LED_G_SET;
		//	D_LED_G_CLR
//            P01 = 1;
        }
        else
        {
            LEDshiftCNT--;
        }
      //  D_LED_R_SET;
		
        //

        break;
    case D_Target_Status_USB_Power:
    case D_Target_Status_FOD:
//            if( wLEDCounter > 1000 )
//            {
//                wLEDCounter = 0;
//                D_LED_G_CLR;
//                D_LED_R_SET;
//            }
//            else if( 250 == wLEDCounter )
//            {
//                D_LED_G_CLR;
//                D_LED_R_SET;
//            }
//            else if( 500 == wLEDCounter )
//            {
//                D_LED_G_SET;
//                D_LED_R_SET;
//            }
//        if( wLEDCounter > 500 )
//        {
//            wLEDCounter = 0;
//            D_LED_G_SET;

//        }
//        else if( 250 == wLEDCounter)
//        {
//            D_LED_G_CLR;

//        }
        // 充电过程中FOD闪灯
      F_LED_Flash();
    //    D_LED_R_SET;
        break;
    case D_Target_Status_Error:    //Charge Fail or Temperature high. show Flash
    case D_Target_Status_Device_Error:
    case D_Target_Status_NoResponse:
//        if( wLEDCounter > 500 )
//        {
//            wLEDCounter = 0;
//            D_LED_G_SET;

//        }
//        else if( 250 == wLEDCounter)
//        {
//            D_LED_G_CLR;

//        }
        F_LED_Flash();
        break;
    case D_Target_Status_OverTemperature:    //    case D_Target_Status_Soft_Error:
//        if( wLEDCounter > 500 )
//        {
//            wLEDCounter = 0;
//            D_LED_G_SET;

//        }
//        else if( 250 == wLEDCounter)
//        {
//            D_LED_G_CLR;

//        }
        F_LED_Flash();
//            if( wLEDCounter > 250 )
//            {
//                wLEDCounter = 0;
//                if( fgLEDInver )
//                {
//                    fgLEDInver = 0;
//                    D_LED_G_SET;
//                    D_LED_R_SET;
//                }
//                else
//                {
//                    fgLEDInver = 1;
//                    D_LED_G_CLR;
//                    D_LED_R_SET;
//                }
//            }
        break;
    case D_Target_Status_Learning:
        //P13=0;
        break;
    default:

        break;
    }
}
