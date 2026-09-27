#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"

/***********************************
// Measure the AC Current
************************************/
//uint16_t F_Measure_Control_Current()
//{
//    uint16_t  wADCResult=0, wADCTemp;
//    uint8_t  bi;

//    adcInit(CON_ADC_CH_Current);              //ADC???

//    for(bi=0; bi<4; bi++)
//    {

//        adcStart();

//        adcWait();

//        wADCTemp = ADC_GetADCResult();
//        wADCTemp = wADCTemp/8;
//        wADCResult += wADCTemp;
//    }
//    return wADCResult;//+0x400;
//}
/***********************************
// Change Phase Check Function
************************************/
void F_Measure_Real_Current()
{
    uint32_t tVddValue;
    bCurrentCounter++;
    if( bCurrentCounter > D_Current_Check_Period )
    {
        adcInit(CON_ADC_CH_Current);//Enable_ADC_AIN2;	//  P07
        adcStart();

        adcWait();

        wRC_Current = ADC_GetADCResult(CON_ADC_CH_Current);
        wRC_Low_Pass_Current = wRC_Low_Pass_Current*4/5 + wRC_Current/5;  //wRC_Current/6;//ADC_GetADCResult();//(((uint16_t)ADCRH)<<2)+(ADCRL>>2);

//        if(wRC_Current>768)
//        {
//            wRC_Current -= 768;//768=(0.6*4096/3.2)
//        }
//        else
//        {
//            wRC_Current = 0;
//        }
//        if(wRC_Low_Pass_Current>768)
//        {
//            wRC_Low_Pass_Current -= 768;
//        }
//        else
//        {
//            wRC_Low_Pass_Current = 0;
//        }
        bCurrentCounter = 0;

    } else if ( bCurrentCounter == D_Current_Check_Period/2 )
    {
//			  Enable_ADC_AIN6;  //P03
//			  clr_ADCF;
//			  set_ADCS;									// ADC start trig signal
//        while(ADCF == 0);
//				wAC_Low_Pass_Voltage = wAC_Low_Pass_Voltage/2+((((uint16_t)ADCRH)<<4)+ADCRL)/2;

        adcInit(CON_ADC_CH_VPP);//Enable_ADC_AIN2;	//  P07
        adcStart();

        adcWait();

        wAC_Low_Pass_Voltage = wAC_Low_Pass_Voltage/2+ADC_GetADCResult(CON_ADC_CH_VPP)/2;
    }

    // add
    
    if( bCurrentCounter == D_Current_Check_Period )
    {
        //adcInit(CON_ADC_CH_Vref12);              //ADC???
        adcInit(CON_ADC_CH_VDD);              //ADC???
        //for(adci=0;adci<20;adci++)
        {
            adcStart();

            adcWait();
            wVDD = wVDD/2 + ADC_GetADCResult(CON_ADC_CH_VDD)/2;
        }
        wVPPvoltage = wVDD;

        tVddValue = wVDD;

        tVddValue *= 500;
        tVddValue /= 4095;



//        wVDD_V = ((uint32_t)D_ADC_V_MAX*wVDD/2);
//        wVDD_V /= (4096/2);

        //if(wVDD/2>1300)
      #if 0
        if(tVddValue>350)
        {

            bVddOverCnt++;

            if(bVddOverCnt>20)
            {
                // over volt
                //F_QI_POWERTRANSFER_Phase_Thermal_Stop_handler();
//            bPhaseBefore = bPhase;
//            bPhase = SELECTION;
//            fgChangePhase = 1;
                D_PWM_Clear;


                fgChangePhase = 1;
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                bTargetStatus = D_Target_Status_Error;
                bErrorFrom = D_ErrorFrom_PowerTransfer_TX_OverTemperature;
//            FODflag=1;
                fgHeat = 1;
            }
        }
        else
        {
            bVddOverCnt = 0;
        }
	  #endif
//
////     	Enable_ADC_AIN4;	//  P05
////			clr_ADCF;
////			set_ADCS;									// ADC start trig signal
////      while(ADCF == 0);
////			wVDD_V = wVDD_V/2 +((((uint16_t)ADCRH)<<4)+ADCRL)/2;
//
//        adcInit(ADCChannel_P15);//Enable_ADC_AIN2;	//  P07
//        adcStart();
//
//        while((ADCON0&ADCI)==0);
//        ADCON0 &= (~ADCI);
//
//        wVDD_V = wVDD_V/2 + getADCResult()/2;
//
//        if(wVDD_V>D_VDDMAX_Voltage_Code)
//		{
//            // over volt
//            //F_QI_POWERTRANSFER_Phase_Thermal_Stop_handler();
////            bPhaseBefore = bPhase;
////            bPhase = SELECTION;
////            fgChangePhase = 1;
//            D_PWM_Clear;
//
//
//            fgChangePhase = 1;
//            bPhaseBefore = bPhase;
//            bPhase = SELECTION;
//            bTargetStatus = D_Target_Status_Error;
//            bErrorFrom = D_ErrorFrom_PowerTransfer_TX_OverTemperature;
////            FODflag=1;
//            fgHeat = 1;
//        }
    }
}
