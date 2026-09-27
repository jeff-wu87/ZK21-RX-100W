#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"

uint8_t bPowerLimit;
uint16_t FODcounter=0;
uint8_t FODcounter2=0;
extern uint8_t FODcounter3;
uint8_t  b15WMOD;
extern uint8_t  bsumsung10WMOD;
extern  int16_t   iMDPRDNegMax,iMDPRDPosMax;

uint16_t timerms_current_ticks=0;


extern uint16_t Get_PWM_Frequency_Value(void);
extern void PWM_SetFreq(unsigned int freq);
extern void PWM_SetDami(unsigned int dami0,unsigned int dami1);

//extern uint8_t F_GetNowTemp(uint16_t adValue);

void F_CtrlErrorAdj(void)
{
    bCRCLbackup = Get_PWM_Frequency_Value();//(((uint16_t)PWMPH)<<8) + PWMPL;

    bCRCLbackup += 2;

    if(bCRCLbackup>=D_QI_AI_Sv_Lv0_Code) //PWM frequency minimum limit
    {
        bCRCLbackup=D_QI_AI_Sv_Lv0_Code;
    }

    if(bCRCLbackup<=D_QI_AI_Sv_Lv4_Code) //PWM frequency maxmum limit
    {
        bCRCLbackup=D_QI_AI_Sv_Lv4_Code;
    }
    PWM_SetFreq(bCRCLbackup);
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
}


void F_QI_POWERTRANSFER_Phase_EndPowertransfer_Handler()
{

    if( bDecode[1] == 0x07)     //when end pwer transfer parameter was 0x07 , reconfiguration ==P42 button==
    {
        fgChangePhase = 1;
        bPhaseBefore = bPhase;
        bPhase = ID_CONF;
    }
    //else                        //any other end power transfer parameter means to return SELECTION
    else if(( bDecode[1] < 0x0c)&&(bDecode[1]>0))                      //any other end power transfer parameter means to return SELECTION
    {
        fgChangePhase = 1;
        bPhaseBefore = bPhase;
        bPhase = SELECTION;
        bErrorFrom = bDecode[1]+1;
        if( D_QI_EndPowerTransfer_ChargeComplete == bDecode[1] )
        {
            bTargetStatus = D_Target_Status_Complete;
            fgChangePhase = 0;
            bPhaseBefore = ID_CONF;
            bPhase = POWERTRANSFER;
            bErrorFrom = 0;
            fgHeat = 1;
            wColdTimeCounter = D_OverTemperature_WakeupTime_Code;
            if( D_Manufactor_TI == bManufactor )
            {
                wColdTimeCounter = D_OverTemperature_WakeupTime_Code;
                bTargetStatus =  D_Target_Status_TI_Complete;
                fgChangePhase = 0;
                bPhaseBefore = ID_CONF;
                bPhase = POWERTRANSFER;
                bErrorFrom = 0;
            }
            bReconfigCounter++;
        }
        else if( D_QI_EndPowerTransfer_InternalFault == bDecode[1] ||
                 D_QI_EndPowerTransfer_OverVoltage == bDecode[1] ||
                 D_QI_EndPowerTransfer_OverCurrent == bDecode[1] ||
                 D_QI_EndPowerTransfer_BatteryFailure == bDecode[1] ||
                 D_QI_EndPowerTransfer_Unkown == bDecode[1] ||
                 bDecode[1] >  D_QI_EndPowerTransfer_NoResponse  )
        {
            bTargetStatus =  D_Target_Status_Device_Error;
            b_ErrorStayCounter++;
        }
        else if( D_QI_EndPowerTransfer_NoResponse == bDecode[1] )
        {
            bReconfigCounter++;
            bTargetStatus = D_Target_Status_NoResponse;

            // if Qi then stop
            if(bManufactor==D_Manufactor_QI)
            {
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                fgChangePhase = 1;
                D_PWM_Clear;

                bTargetStatus = D_Target_Status_None;
                FODflag=1;
                FODcounter2=0;
                FODcounter3	=0;
            }
            //
        }
        else if(D_QI_EndPowerTransfer_Reconfigure == bDecode[1])
        {
            bReconfigCounter++;
            bPhase = ID_CONF;
            //bTargetStatus = D_Target_Reconfigure;
            bTargetStatus = D_Target_Status_Complete;
            fgChangePhase = 0;
            bPhaseBefore = ID_CONF;
            bPhase = POWERTRANSFER;
            bErrorFrom = 0;
        }
        else if( D_QI_EndPowerTransfer_OverTemerature == bDecode[1] )
        {
            bTargetStatus = D_Target_Status_OverTemperature;
            fgHeat = 1;
            wColdTimeCounter = D_OverTemperature_WakeupTime_Code;
        }
        else
        {
            bTargetStatus = D_Target_Status_None;
        }
    }
}
void F_QI_POWERTRANSFER_Phase_Stop_handler()
{
    if( wPOWERTRANSFER_Error_Packet_Timeout_Counter > D_QI_POWERTRANSFER_ttimeout)
    {
        if( lPOWERTRANSFER_COMPATIABLE_DELAY < 12000 )
        {
            if( wPOWERTRANSFER_COMPATIABLE_COUNT > D_QI_POWERTRANSFER_ttimeout)
            {
                bErrorTerminateCounter = 0;
                bError = D_QI_POWERTRANSFER_ERROR_A;
            }
            else
            {
                if( !fgCompatiableMode )
                    wCompatiableLEDDelay = 0 ;
                fgCompatiableMode = 1;
            }
        }
        else
        {
            bErrorTerminateCounter = 0;
            bError = D_QI_POWERTRANSFER_ERROR_A;
        }
    }
    if( wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter > D_QI_POWERTRANSFER_tpower)
    {
        if( !fgCompatiableMode )
        {
            bErrorTerminateCounter = 0;
            bError = D_QI_POWERTRANSFER_ERROR_F;
        }
    }

}

void F_QI_POWERTRANSFER_Phase_Thermal_Stop_handler()
{
    fgChangePhase = 1;
    bPhaseBefore = bPhase;
    bPhase = SELECTION;
    bTargetStatus = D_Target_Status_Error;
    bErrorFrom = D_ErrorFrom_PowerTransfer_TX_OverTemperature;
    FODflag=1;
    fgHeat = 1;
    wColdTimeCounter = D_OverTemperature_WakeupTime_Code;
}

void F_QI_POWERTRANSFER_Phase_Stop()
{
    bPhaseBefore = bPhase;
    bPhase = SELECTION;
    fgChangePhase = 1;
    D_PWM_Clear;
    bTargetStatus = D_Target_Status_None;
    LEDshiftCNT = 0;
}

void F_QI_POWERTRANSFER_Phase_Error_Handler()
{

    switch( bError )
    {
    case D_QI_POWERTRANSFER_ERROR_A:
    case D_QI_POWERTRANSFER_ERROR_F:
    case D_QI_POWERTRANSFER_ERROR_B:
        if( bErrorTerminateCounter >= D_QI_tterminate)
        {
//            bPhaseBefore = bPhase;
//            bPhase = SELECTION;
//            fgChangePhase = 1;
//            D_PWM_Clear;
//            bTargetStatus = D_Target_Status_None;
            F_QI_POWERTRANSFER_Phase_Stop();
        }
        break;
    case D_QI_POWERTRANSFER_ERROR_E:
        if( bErrorTerminateCounter >= D_QI_texpire)
        {
//            bPhaseBefore = bPhase;
//            bPhase = SELECTION;
//            fgChangePhase = 1;
//            D_PWM_Clear;
//            bTargetStatus = D_Target_Status_None;
            F_QI_POWERTRANSFER_Phase_Stop();
        }

        break;
    case D_QI_POWERTRANSFER_ERROR_C:
        if( bErrorTerminateCounter >= D_QI_tterminate)
        {
//            bPhaseBefore = bPhase;
//            bPhase = SELECTION;
//            fgChangePhase = 1;
//            D_PWM_Clear;
//            bTargetStatus = D_Target_Status_None;
            F_QI_POWERTRANSFER_Phase_Stop();
        }
        break;
    default:
//        bPhaseBefore = bPhase;
//        bPhase = SELECTION;
//        fgChangePhase = 1;
//        D_PWM_Clear;
//        bTargetStatus = D_Target_Status_None;
        F_QI_POWERTRANSFER_Phase_Stop();
        break;
    }
    if( D_Target_Status_FOD == bTargetStatus )
    {
        fgChangePhase = 1;
        bPhaseBefore = bPhase;
        bPhase = SELECTION;
        bTargetStatus = D_Target_Status_Device_Error;
        bErrorFrom = D_ErrorFrom_PowerTransfer_TX_VirtualWork;
        b_ErrorStayCounter++;
    }
}




void F_Limit_Power(uint8_t bCheck)
{
    if( bCheck )
    {
        //
        //过温之后不调节
        if(bCheck>=100)
        {
            fgEndThisControl = 1;
            return;
        }
        //
        if( bUSB_LED_Debounce <0xfe)
            bUSB_LED_Debounce++;

		#if 0

        if(bManufactor == D_Manufactor_apple)
        {
            //bCRCLbackup = (((uint16_t)PWMD0H)<<8) + PWMD0L   +  (((uint16_t)PWMD1H)<<8) + PWMD1L ;

            bCRCLbackup-=2;

            if(bCRCLbackup<=D_QI_AI_Sv_apple_Code/10) //PWM frequency maxmum limit
                bCRCLbackup=D_QI_AI_Sv_apple_Code/10;

            if(bCRCLbackup<=D_QI_AI_Sv_apple_Code/2)
            {
                D_PWM_OpenHalf;
                PWM_SetDami(bCRCLbackup,0);
            }
            else
            {
              //  D_PWM_OpenAll;
                PWM_SetDami(D_QI_AI_Sv_apple_Code/2,bCRCLbackup-D_QI_AI_Sv_apple_Code/2);
            }
            //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
            //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
            //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//            PWMCNTCLR = 0x03;
        }
        else
		#endif
		
        {
        
             bCRCLbackup = Get_PWM_Frequency_Value();//(((uint16_t)PWMPH)<<8) + PWMPL;
            if( bCRCLbackup  > D_QI_AI_Sv_Lv4_Code   )
            {
                bCRCLbackup-=1;
                PWM_SetFreq(bCRCLbackup);

                //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
                //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
                //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                PWMCNTCLR = 0x03;
//					  while(LOAD);
//          	PWMPH = (uint8_t)(bCRCLbackup >> 8);
//          	PWMPL = (uint8_t)bCRCLbackup;
//
//        		bCRCLbackup=bCRCLbackup/2;
//          	while(LOAD);
//          	PWM0H = (uint8_t)(bCRCLbackup >> 8);
//          	PWM0L = (uint8_t)bCRCLbackup ;
//        		while(LOAD);
//        		PWM2H = (uint8_t)(bCRCLbackup >> 8);
//          	PWM2L = (uint8_t)bCRCLbackup ;
//            set_LOAD;


#ifdef DEBUG_PRINT
                puts("\r\n<><><>");
                my_itoa(bPowerLimit,0);
				  puts("\r\n<><><>");
			    my_itoa(wRC_Low_Pass_Current,0);
				
                puts("<><><>\r\n");
#endif

            }
            else
            {
                /*if( PWM->CMPDAT1 >D_QI_AI_Sv_Below_Lv4_min_duty_Code)
                {
                    PWM->CTL |= PWM_CTL_CNTCLR_Msk;
                    PWM->CMPDAT0--;
                				  PWM->CMPDAT1++;
                    //CMP1--;
                }*/
            }
        }

        fgEndThisControl = 1;
        if( bUSB_LED_Debounce > D_USB_LED_Debounce_Time || wUSBPowerCounter >0)
        {
            wUSBPowerCounter  = 2000;
            //bTargetStatus = D_Target_Status_USB_Power;
        }
    }
    else
    {
        if( wUSBPowerCounter )
        {
            wUSBPowerCounter--;
            bUSB_LED_Debounce =0;
        }
        else
        {
            if(bTargetStatus != D_Target_Status_Complete && bTargetStatus != D_Target_Status_TI_Complete)
                bTargetStatus = D_Target_Status_Pinged;
        }
    }
}


uint8_t F_Check_Below_Voltage()
{
    uint8_t bResult;//,adci;
    //****************************************************************************
//			    Enable_ADC_AIN4;  //P05
//			    clr_ADCF;
//			    set_ADCS;					// ADC start trig signal
//          while(ADCF == 0);
//				  wVPPvoltage = wVPPvoltage /2 + ((((uint16_t)ADCRH)<<4)+ADCRL)/2;
//


//    adcInit(CON_ADC_CH_VPP);              //ADC???

//    adcStart();

//    while((ADCON0&ADCI)==0);
//    ADCON0 &= (~ADCI);
//    wVPPvoltage = wVPPvoltage /2 + getADCResult()/2;

//    wVPPvoltage = 0;

    //for(adci=0; adci<20; adci++);

//	        Enable_ADC_BandGap;  //1.22V
//	       for(adci=0;adci<20;adci++)
//	      {
//			    clr_ADCF;
//			    set_ADCS;					// ADC start trig signal
//          while(ADCF == 0);
//				  wVDD = wVDD/2 + ((((uint16_t)ADCRH)<<4)+ADCRL)/2;
//	      }

    //adcInit(CON_ADC_CH_Vref12);              //ADC???
    adcInit(CON_ADC_CH_VDD);              //ADC???
    //for(adci=0; adci<20; adci++)
    {
        adcWait();
        adcStart();

        adcWait();
        wVDD = ADC_GetADCResult(CON_ADC_CH_VDD);
    }
    wVPPvoltage = wVDD;

    if(fFastcharge)
    {
        if(wVDD < D_VDD_Limit_Code)
            bResult = 1;
        else
            bResult = 0;
    }
    else
    {
        if(wVDD < D_5V_Limit_Code)
            bResult = 1;
        else
            bResult = 0;
    }

    return bResult;
}

/***********************************
// Power transfer Phase Register Bank 1
************************************/
void F_POWERTRANSFER_Phase_Manage()
{
    uint8_t i,j;


	 if(bError&&Capturebit0_intdly)
        {
            	   lPOWERTRANSFER_COMPATIABLE_DELAY=0;
			       wPOWERTRANSFER_COMPATIABLE_COUNT=0;
				   wPOWERTRANSFER_Error_Packet_Timeout_Counter=0;
				   wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter=0;
				  
										 
				//	   printf("bError %d, %d",(uint16_t)bError,Capturebit0_intdly);
					    printf("bError %d, %d, %d",(uint16_t)bError,Capturebit0_intdly, wRC_Low_Pass_Current);
				       printf("\n\r");
					
					   
				if(Capturebit0_intdly>100)
					 bError=0;
	 	}
    if(bError)
    {
        bErrorTerminateCounter++;
        F_QI_POWERTRANSFER_Phase_Error_Handler();
    }
    else
    {

            timerms_current_ticks++;
		  if(timerms_current_ticks % 10 == 0) 
		 	{
                 if(Capturebit0_intdly)
                 	{
			            Capturebit0_intdly--;

                 	}
		  	}
		  if(timerms_current_ticks>25000)
			  	timerms_current_ticks=0;

	
        wPOWERTRANSFER_Temperature_Counter++;
        if(fboostup>=2)
        {
            fboostup=2;
            wStart9VCounter++;
            if(wStart9VCounter>=60000)
                wStart9VCounter=60000;
        }
        if(fmidlepower>=1)
        {
            wStart9VCounter1++;
        }

		

        //**************************************************************

	#if 0

        if(fmidlepower>=1&&wStart9VCounter>=8000 && wStart9VCounter<=8170 && bsumsung10WMOD==0 && fFastcharge>=1)//2000ms
        {
//            wBackupMDPRD = Get_PWM_Frequency_Value();//((uint16_t)PWMPH<<8) + PWMPL;
//            if(wBackupMDPRD>D_QI_AI_Sv_Lv1_10W_Code)
//            {
//                wBackupMDPRD--;
//            }
//            else
            {
                fgPrintf = 2;
                D_PWM_OpenAll;

//                puts("xxxx\n");
//                CCAPM0 |= PWM0;//??
//                CCAPM1 |= PWM1;//??
//                CCAPM2 |= PWM2;//??
//                CCAPM3 |= PWM3;//??
                //                            while(LOAD);
                //                            clr_PMEN0;
                //                            clr_PMEN1;
                bsumsung10WMOD=1;
            }
            PWM_SetFreq(D_QI_AI_Sv_Lv1_10W_Code);
            //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
            //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
            //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//					  while(LOAD);
//					 	PWMPL = (BYTE)(D_QI_AI_Sv_Lv1_10W_Code);
//         		PWMPH = (BYTE)(D_QI_AI_Sv_Lv1_10W_Code>>8);
//         		while(LOAD);
//         		PWM0L = (BYTE)(D_QI_AI_Sv_Lv1_10W_Code/2);
//         		PWM0H = (BYTE)((D_QI_AI_Sv_Lv1_10W_Code/2)>>8);
//				   	while(LOAD);
//         		PWM2L = (BYTE)(D_QI_AI_Sv_Lv1_10W_Code/2);
//         		PWM2H = (BYTE)((D_QI_AI_Sv_Lv1_10W_Code/2)>>8);

        }
        if(wStart9VCounter>=10000 /*|| wStart9VCounter1>2010*/)
        {
            wStart9VCounter=10000;
//            //wStart9VCounter1=2010;
//            //if(fmidlepower>=1 && PWMPL > D_QI_AI_Sv_Lv1_10W_Code)
//            {
//                //PWM->PERIOD0 =  D_QI_AI_Sv_Lv1_10W_Code;
//                //PWM->CMPDAT0 = PWM->PERIOD0/2;
//                //PWM->PERIOD1 =  D_QI_AI_Sv_Lv1_10W_Code;
//                //PWM->CMPDAT1 = PWM->PERIOD1/2;

//            }
        }

        /*if(fmidlepower>=1&&wStart9VCounter1>=2000 &&wStart9VCounter1<=2005 && fboostup>=1 && fFastcharge==1)
        {

        	   PWM->PERIOD0 =  D_QI_AI_Sv_Lv1_Code;
        PWM->CMPDAT0 = PWM->PERIOD0/2;


        		 SYS->P2_MFP |= SYS_MFP_P22_PWM0_CH0;

        	   P30=0;
        	   P54=0;
        	   bsumsung10WMOD=1;

        }*/

   #endif
   

        if( wPowerTransferCounter < 0xFFF0)
            wPowerTransferCounter++;
        if(wPowerTransferCounter==1 && bx_QI_Configuration[4]&0x80 && fFastcharge>=1)
        {
            fsendstart=1;
            wsendTimecount=0;
            bByteindexsendCNT=1;
            bBitindexsendCNT=1;
            bDatatosend[0]=bTX_ACK_Data[0];
            fgPacketStartbitsend=0;
        }
        wPOWERTRANSFER_Error_Packet_Timeout_Counter++;
        if( wPOWERTRANSFER_Error_Packet_Timeout_Counter > 0xFFF0)
            wPOWERTRANSFER_Error_Packet_Timeout_Counter = 0xFFF0;
        wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter++;
        if( wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter > 0xFFF0)
            wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter = 0xFFF0;
        bPacketErrorCounter++;
        //wCommErrorCounter++;
        wPOWERTRANSFER_COMPATIABLE_COUNT++;
        lPOWERTRANSFER_COMPATIABLE_DELAY++;
        bPowerLimit =0;

	  #if 0	
        if(bTargetStatus == D_Target_Status_FOD)
        {
            FODcounter++;
            if(FODcounter>=12000)
            {
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                fgChangePhase = 1;
                D_PWM_Clear;

                bTargetStatus = D_Target_Status_None;
                FODflag=1;
                FODcounter2=0;
                FODcounter3	=0;
            }

            if(wSendPower < w_QI_Retified_Power_Limit)
            {
                if(FODcounter>2)
                {
                    FODcounter -= 2;
                }
                else
                {
                    if(FODcounter)
                    {
                        FODcounter = 0;
                        bTargetStatus = D_Target_Status_Pinged;
                    }
                }
            }
        }
        else
        {
            if(FODcounter>2)
                FODcounter--;
        }
        if(F_Check_Below_Voltage() && fgUSBPower && 0 == bCurrentCounter)
        {
            bPowerLimit = 3;
            if(bManufactor==D_Manufactor_QI)
            {
                //D_LED_G_SET;
                LEDshiftCNT = 100;
            }
        }
	  #endif

        //

     //   bNowTempValue = F_GetNowTemp(wADCRadio);

        if(wCheckOverTemp_Delay<20000)
        {
            wCheckOverTemp_Delay++;
        }
        else
        {
            if(( bNowTempValue>= 65 )&&(wRC_Current>450))
                //if(( bNowTempValue>= 55 )&&(wRC_Current>450))
                //if(( bNowTempValue>= 55 )&&(wRC_Current>350))
                //if(( bNowTempValue>= 105 )&&(wRC_Current>350))
            {
                if(bNowTempValueBackup<bNowTempValue)
                {
                    bNowTempValueBackup=bNowTempValue;
                    bPowerLimit = 3;
                }
                else
                {
                    bPowerLimit = 100;
                }

                wCheckOverTemp_Delay++;
                if(wCheckOverTemp_Delay>30000)
                {
                    wCheckOverTemp_Delay = 20000;
                    bPowerLimit = 3;
                }
            }
            else
            {
                bPowerLimit = 0;
            }
        }
        //

	#if 0	

        if(0 == bCurrentCounter)
        {
            if(fFastcharge==2)
            {
                if(  wRC_Current > D_PowerTransfer_Current_Mos_12V_Limit_Code)
                {
                    bPowerLimit = 2;
//
//
//#ifdef DEBUG_PRINT
//                puts("\r\n----------");
//                my_itoa(wRC_Current,0);
//                my_itoa(D_PowerTransfer_Current_Mos_12V_Limit_Code,0);
//                puts("----------\r\n");
//#endif
                }
            }
            else if(fFastcharge==1)
            {
                if(  wRC_Current > D_PowerTransfer_Current_Mos_Limit_Code)
                {
                    bPowerLimit = 2;
                }
            }
            else
            {
                if(  wRC_Current > D_PowerTransfer_Current_Mos_5V_Limit_Code)
                {
                    bPowerLimit = 2;
                }
            }
        }

	#endif
	
	
      //  if(  wRC_Current > 400 && 0 == bCurrentCounter && bManufactor==D_Manufactor_sumsung)
      //  {
            //wPOWERTRANSFER_COMPATIABLE_COUNT = 0;
       // }
        if(timerms_current_ticks % 12 == 0) 
        	{

				if((wRC_Low_Pass_Current>510&&!find36wflg)||(wRC_Low_Pass_Current>800&&find36wflg))
				{
		            bPowerLimit = 1;

				}
		       // if( wAC_Low_Pass_Voltage >D_AC_Voltage_MD_Less_Limit_Code &&  D_Current_Check_Period/2 == bCurrentCounter && bManufactor != D_Manufactor_apple)
		        if( wAC_Low_Pass_Voltage >D_AC_Voltage_MD_Less_Limit_Code &&  D_Current_Check_Period/2 == bCurrentCounter )
		        {
		            bPowerLimit = 2;
		        }

        	}

//        if(bManufactor == D_Manufactor_apple)
//        {
//            bPowerLimit = 0;
//        }
        //bPowerLimit = 0;
        F_Limit_Power(bPowerLimit);
        
        #if 0

        if((wPOWERTRANSFER_Error_Packet_Timeout_Counter>800)&& bManufactor==D_Manufactor_sumsung)
        {
            if(wPOWERTRANSFER_Error_Packet_Timeout_Counter==900)
            {
                //F_CtrlErrorAdj(48);
                F_CtrlErrorAdj();
            }
//            if(wPOWERTRANSFER_Error_Packet_Timeout_Counter==1200)
//            {
//                //F_CtrlErrorAdj(48);
//                F_CtrlErrorAdj();
//            }
            if(wPOWERTRANSFER_Error_Packet_Timeout_Counter==1500)
            {
                //F_CtrlErrorAdj(48);
                F_CtrlErrorAdj();
            }
        }
	   #endif
	   
       // else
        //{
          //  F_Limit_Power(bPowerLimit);
     //   }


        if(b_QI_Retified_Power_Before<=20 && bManufactor==D_Manufactor_sumsung && bTargetStatus != D_Target_Status_TI_Complete && bTargetStatus !=D_Target_Status_Complete) //|| wRC_Current<=400)//((wRC_Current<=500 && fMicro5V_IN==1) || (wRC_Current<=280 && fMicro9V_IN==1))
        {
            wChargeCompleteCNT++;
            if(wChargeCompleteCNT>=30000)
            {
                wChargeCompleteCNT=0;
                bChargeCompleteCNT++;
                /* if( PWM->PERIOD1  > D_QI_AI_Sv_Lv4_Code )
                 {
                		//PWM->CTL |= PWM_CTL_CNTCLR_Msk;
                			PWM->PERIOD0-=10;
                			PWM->CMPDAT0 =  PWM->PERIOD0/2;
                			PWM->PERIOD1-=10;
                			PWM->CMPDAT1 =  PWM->PERIOD1/2;
                 }

                 if(bChargeCompleteCNT>=3)
                 {
                    wChargeCompleteCNT=0;
                    bChargeCompleteCNT=0;
                	  PWM_DisableOutput(PWM, 0x03);
                    //fChargeComplete=1;
                 }*/
            }
        }
        if(fgErrorControl)
        {
            bPOWERTRANSFER_Error_Control_Time_Counter++;
            if( bPOWERTRANSFER_Error_Control_Time_Counter == (b_QI_PowerHoldOff_Time+1))
            {
                F_QI_PowerTransfer_Control();
            }
            else if( bPOWERTRANSFER_Error_Control_Time_Counter ==
                     (b_QI_PowerHoldOff_Time + D_QI_POWERTRANSFER_tactive-D_CalculateChecksum_Delay+2) )
            {
                fgErrorControl=0;
            }
            else if( bPOWERTRANSFER_Error_Control_Time_Counter > b_QI_PowerHoldOff_Time &&
                     bPOWERTRANSFER_Error_Control_Time_Counter < (b_QI_PowerHoldOff_Time + D_QI_POWERTRANSFER_tactive-D_CalculateChecksum_Delay) )
            {
                F_QI_PowerTransfer_Control();
            }
            else if (bPOWERTRANSFER_Error_Control_Time_Counter == (b_QI_PowerHoldOff_Time + D_QI_POWERTRANSFER_tactive-D_CalculateChecksum_Delay+1) )
            {

            }
        }
        if( fgPacketStop )
        {
#ifdef DEBUG_PRINT
            if(bDecode[0]!=D_QI_ControlError&&bDecode[0]!=0x51)
            {
                fgPrint = 1;
                bPrinti = 2;
                wPrintdata2[0] = (uint16_t)bDecode[0];
                wPrintdata2[1] = (uint16_t)bDecode[1];
                wPrintdata2[2] = (uint16_t)bDecode[2];
                wPrintdata2[3] = (uint16_t)bDecode[3];
                wPrintdata2[4] = (uint16_t)bDecode[4];
                wPrintdata2[5] = (uint16_t)bDecode[5];

//                my_itoa(bDecode[0],bPrintfBuf);
////                putchar(bDecode[0]/100+'0');
////                putchar((bDecode[0]/10)%10+'0');
//                puts(bPrintfBuf);
//                putchar('\n');
//                putchar('\r');
            }
            else
            {
//                my_itoa((int8_t)bDecode[1],1);
//                puts("\n\r\n");
            }
#endif
            switch(bDecode[0])
            {
            case D_QI_EdnPowerTransfer:
                F_QI_POWERTRANSFER_Phase_EndPowertransfer_Handler();
                break;
            case D_QI_ControlError:

                if( fgCompatiableMode )
                {
                    if(  wPOWERTRANSFER_Error_Packet_Timeout_Counter < D_QI_POWERTRANSFER_ttimeout)
                    {
                        wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter = 0;
                        fgCompatiableMode = 0;
                    }
                    wCompatiableLEDDelay = 0;
                }
                wPOWERTRANSFER_COMPATIABLE_COUNT=0;
                lPOWERTRANSFER_COMPATIABLE_DELAY=0;
                bControlErrorCNTsend++;
                if(bControlErrorCNTsend>=10)
                    bControlErrorCNTsend=10;

//                if(bManufactor==D_Manufactor_apple)
//                {
//                    if(bAppleCtlErrorCnt<4)
//                    {
//                        bAppleCtlErrorCnt++;
//                        if(bAppleCtlErrorCnt==4)
//                        {
//                            PWM_SetFreq(D_QI_AI_Sv_apple_Code);
//                            PWM_SetDami(D_QI_AI_Sv_apple_Code/3,5);
//                            D_PWM_OpenAll;
//                        }
//                    }
//                }

                if(((bControlErrorCNTsend>=1 && bControlErrorCNTsend<=2 && fmidlepower==0 ) || (bControlErrorCNTsend>=1 && fboostup==1)) \
                        && fFastcharge>=1 && (bManufactor==D_Manufactor_sumsung||bManufactor==D_Manufactor_HuaWei) && (bx_QI_Configuration[4]&0x80)==0)
                {
                    fsendstart=1;
                    wsendTimecount=0;
                    bByteindexsendCNT=2;
                    bBitindexsendCNT=1;
                    for(i=0; i<2; i++)
                        bDatatosend[i]=bSamsung10WData[i];
                    fgPacketStartbitsend=1;
                }
//                                if(bManufactor==D_Manufactor_sumsung && fFastcharge==1 && bControlErrorCNTsend>=5 && fmidlepower==0)
//                                {
//                                    bPhaseBefore = bPhase;
//                                    bPhase = SELECTION;
//                                    fgChangePhase = 1;
//                                    D_PWM_Clear;
//                                    bTargetStatus = D_Target_Status_None;

//                                    //fgPrintf = 6;
//                                }

                j=0;
                for(i=0; i<5; i++)
                {   if((f15Wstartup>>i)&0x01)
                        j++;
                }
                if(j>=4 && f15Wstartup !=0x55)
                {
                    f15Wstartup=0x55;
                    bControlErrorCNTsend=0;
                }
                if(f15Wstartup==0x55 && bControlErrorCNTsend>=10 ) //
                {
                    f15Wstartup=0;
                    D_PWM_OpenAll;

//                    CCAPM0 |= PWM0;//??
//                    CCAPM1 |= PWM1;//??
//                    CCAPM2 |= PWM2;//??
//                    CCAPM3 |= PWM3;//??

                    PWM_SetFreq(D_QI_AI_Sv_Lv3_Code);
                    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
                    //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
                    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

                    // PWM->PERIOD0 =  D_QI_AI_Sv_Lv3_Code;
                    // PWM->CMPDAT0 = PWM->PERIOD0/2;
                    // PWM->PERIOD1 =  D_QI_AI_Sv_Lv3_Code;
                    // PWM->CMPDAT1 = PWM->PERIOD1/2;
                    // SYS->P2_MFP |= SYS_MFP_P22_PWM0_CH0;
                    bDecode[1]=0;
                    b_PP_ControlStatus = D_PP_ConrolPhase_1 ;
                    //P30=0;
                    b15WMOD=1;
                }
                fgErrorControl = 1;
                fgCap2PacketStart = 0;
                fgPacketStart = 0;
                bPOWERTRANSFER_Error_Control_Time_Counter = D_CalculateChecksum_Delay;
                i_QI_Error = (signed char)bDecode[1];
//                if( fmidlepower==1 && bManufactor==D_Manufactor_sumsung && i_QI_Error<=-2)
//                {
//                    i_QI_Error=-2;
//                }

                //if(bsumsung10WMOD)
                if(bManufactor==D_Manufactor_sumsung && fFastcharge>=1)
                {
                    wQiTotalError+=i_QI_Error;
                    if(wQiTotalError>500)
                    {
                        wQiTotalError = 500;
                    }
                    else if(wQiTotalError<-300)
                    {
                        wQiTotalError = -300;
                    }
                }


                wPOWERTRANSFER_Error_Packet_Timeout_Counter = 0;


//                i_QI_Error_Before = i_QI_Error;
                fgEndThisControl = 0;


//                if(  D_PP_ConrolPhase_1 == b_PP_ControlStatus ) //Judege the Control Phase and store Control content
//                {
//                    wBackupMDPRD = Get_PWM_Frequency_Value();//((uint16_t)PWMPH<<8) + PWMPL;
////                    wBackupDuty  = ((uint16_t)PWM0H<<8) + PWM0L;
//                }
//                else if( D_PP_ConrolPhase_2 == b_PP_ControlStatus  )
//                {
//                    wBackupMDPRD = Get_PWM_Frequency_Value();//((uint16_t)PWMPH<<8) + PWMPL;
////                    wBackupDuty  = ((uint16_t)PWM0H<<8) + PWM0L;
//                }
//                else
//                {
//                    wBackupMDPRD = Get_PWM_Frequency_Value();//((uint16_t)PWMPH<<8) + PWMPL;
////                   wBackupDuty  = ((uint16_t)PWM0H<<8) + PWM0L;
//                }
                //  wLearning_Note[ wLearning_NoteIndex] = PWM->PERIOD0;
                wLearning_NoteIndex++;
                if( wLearning_NoteIndex >=10 )
                    wLearning_NoteIndex = 0;


//                if(wVPPvoltage>480)// && bsumsung10WMOD !=1)
//                {
//                    wCal = (uint16_t)(((uint32_t)3*D_Refer_Voltage*100*wVPPvoltage)/wVDD);//20k & 10K
//                    wCal = (uint16_t)(((uint32_t)wCal*wRC_Current*(2400/25)/D_Current_Sensor/wVDD));

////                    wCal = (uint16_t)(((uint32_t)3*50*100*wVPPvoltage)/0x3ff);//20k & 10K
////                    wCal = (uint16_t)(((uint32_t)(wCal/20)*(wRC_Current/41)*(5000/D_Current_Sensor))/100);//(uint16_t)((uint32_t)wCal*1000/(D_Current_Amp_k+1)/D_Current_Sensor*wRC_Current/0xfff*5000/1000);
//                }
//                else
//                {
//                    wCal = (uint16_t)(((uint32_t)D_Refer_Voltage*100*0x3ff)/wVDD);//20k & 10K
//                    //wCal = (uint16_t)(((uint32_t)wCal*wRC_Current*(2400/25)/D_Current_Sensor/wVDD));
//                    wCal = (uint16_t)(((uint32_t)wCal*wRC_Current*(2400/19)/D_Current_Sensor/wVDD));

////                    wCal = (uint16_t)((uint32_t)D_Refer_Voltage*100*0x3ff/wVDD);
////                    wCal = (uint16_t)(((uint32_t)(wCal/20)*(wRC_Current/41)*(wCal/D_Current_Sensor))/100);//(uint16_t)((uint32_t)wCal*1000/(D_Current_Amp_k+1)/D_Current_Sensor*wRC_Current/0xfff*wCal/1000);
//                }

                wCal = (uint16_t)(((uint32_t)60*D_ADC_V_MAX*wVPPvoltage)/0xfff);//100k & 20K
                wCal = (uint16_t)(((uint32_t)wCal*wRC_Current*D_ADC_V_MAX/0xfff));


                wSendPower = ((uint32_t)wSendPower*7)/8 + wCal/8;

//                my_itoa(wRC_Low_Pass_Current);
//                putchar(',');
//                my_itoa(wRC_Current);
//                putchar(',');
//                my_itoa(wVPPvoltage);
//                putchar(',');
//                my_itoa(wSendPower);
//                puts("\n\r\n");


#ifdef DEBUG_PRINT
                fgPrint = 1;
                wPrintdata2[0] = (uint16_t)i_QI_Error;
                wPrintdata2[1] = (uint16_t)w_QI_Retified_Power_Limit;
                wPrintdata2[2] = (uint16_t)wSendPower;//((uint32_t)wCal*1000/(D_Current_Amp_k+1)/D_Current_Sensor*wRC_Current/0xfff*wCal/1000);
                wPrintdata2[3] = (uint16_t)w_QI_Receive_Power;//PWM->PERIOD0;
                wPrintdata2[4] = (uint16_t)b_QI_effciency_table;
                wPrintdata2[5] = (uint16_t)b_QI_Retified_Power;//w_QI_Receive_Power;
#endif
//                fgCompatiableNokia = 0;
                if(bIDRXCounter <254)
                    bIDRXCounter++;
                break;
            case D_QI_24bitReceiverPower:
                if(bDecode[1]<=2)
                {
                    fsendstart=1;
                    wsendTimecount=0;
                    bByteindexsendCNT=1;
                    bBitindexsendCNT=1;
                    bDatatosend[0]=bTX_ACK_Data[0];
                    fgPacketStartbitsend=0;
                }
            case D_QI_RectifiedPower:
                //40,62,63,64,65,65
                //40,50,52,54,56,56
                wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter=0;
                b_QI_Retified_Power_Before = b_QI_Retified_Power;
                if(bDecode[0]==D_QI_RectifiedPower)
                    b_QI_Retified_Power = bDecode[1];
                else if(bDecode[0]==D_QI_24bitReceiverPower)
                    b_QI_Retified_Power = bDecode[2];
                else;

                if(bManufactor==D_Manufactor_apple)
                {
                    if(b_QI_Retified_Power<23)
                    {
                        if(dwAppleLowCurrentCnt>=180000)
                        {
                            dwAppleLowCurrentCnt = 180000;
                            bTargetStatus = D_Target_Status_Complete;
                        }
                    }
                    else
                    {
                        dwAppleLowCurrentCnt = 0;
                    }
                }

//                w_QI_Retified_Power_Limit = b_QI_Retified_Power*bPOWERtRANSFER_MaxPower*bPOWERtRANSFER_PowerClass;
                w_QI_Receive_Power = (uint16_t)( (uint32_t)b_QI_Retified_Power*bPOWERtRANSFER_MaxPower*bPOWERtRANSFER_PowerClass*1000/256);



                if(fFastcharge>=1 && bsumsung10WMOD==1)
                {
                    if(wQiTotalError>100)
                    {
                        if(bQiRPAdjFlag==0)
                        {
                            bQiRPAdjFlag = 1;
                        }
                    }
                    else if(wQiTotalError<-100)
                    {
                        if(bQiRPAdjFlag==1)
                        {
                            bQiRPAdjFlag = 0;
                        }
                    }
                    if(bQiRPAdjFlag==1)
                    {
//                        if(w_QI_Receive_Power>((wSendPower>>3)*7))
//                        {
//                            bQiRPAdjFlag = 0;
//                        }
//                        else
//                        {
                        w_QI_Receive_Power <<= 1;
//                        }

                        if(w_QI_Receive_Power>((wSendPower>>3)*7))
                        {
                            bQiRPAdjFlag = 0;
                        }
                    }
                }
                wQiTotalError = 0;


                if(w_QI_Receive_Power>10000)
                {
                    w_QI_Receive_Power = 10000;
                }
                if(fFastcharge>=1 && bsumsung10WMOD==1)
                {
                    bFOD_EffTable[0] = 20-2-8;//24-4;//20-8;//35-8-4;
                    bFOD_EffTable[1] = 23-2-8;//30-4;//20-4;//35-8-4;
                    bFOD_EffTable[2] = 26-2-8;//32-4;//20-4;//35-8-4;
                    bFOD_EffTable[3] = 30-2-8;//36-4;//22;//40-10-6;
                    bFOD_EffTable[4] = 34-2-8;//42-4-2;//24;//45-10-9;
                    bFOD_EffTable[5] = 36-2-8;//44-4-2;//26;//45-10-10;
                }
                else
                {
                    if(bManufactor==D_Manufactor_QI)
                    {
                        bFOD_EffTable[0] = 40;
                        bFOD_EffTable[1] = 70;
                        bFOD_EffTable[2] = 73;
                        bFOD_EffTable[3] = 74;
                        bFOD_EffTable[4] = 76;
                        bFOD_EffTable[5] = 78;


//#define D_QI_TEST_FOD_EffTable_S0	35
//#define D_QI_TEST_FOD_EffTable_S1	50
//#define D_QI_TEST_FOD_EffTable_S2	60
//#define D_QI_TEST_FOD_EffTable_S3	62
//#define D_QI_TEST_FOD_EffTable_S4	65
//#define D_QI_TEST_FOD_EffTable_S5	65
//#define D_QI_TEST_FOD_EffTable_S6	65
                    }
                    else if(bManufactor==D_Manufactor_apple)
                    {
                        bFOD_EffTable[0] = 25;//35-7-3;
                        bFOD_EffTable[1] = 34;//45-8-3;
                        bFOD_EffTable[2] = 37;//50-10-3;
                        bFOD_EffTable[3] = 37;//55-10-3;
                        bFOD_EffTable[4] = 45;//60-10-5;
                        bFOD_EffTable[5] = 47;//65-10-8;

//												bFOD_EffTable[0] = 35;
//												bFOD_EffTable[1] = 45;
//												bFOD_EffTable[2] = 50;
//												bFOD_EffTable[3] = 55;
//												bFOD_EffTable[4] = 60;
//												bFOD_EffTable[5] = 65;
                    }
                    else
                    {

                        bFOD_EffTable[0] = 35-10-3-5;
                        bFOD_EffTable[1] = 40-10-3-5;
                        bFOD_EffTable[2] = 45-10-3-5;
                        bFOD_EffTable[3] = 50-10-3-5;
                        bFOD_EffTable[4] = 55-10-3-5;
                        bFOD_EffTable[5] = 60-15-3-5;


//                        bFOD_EffTable[0] = 35;
//                        bFOD_EffTable[1] = 40;
//                        bFOD_EffTable[2] = 45;
//                        bFOD_EffTable[3] = 50;
//                        bFOD_EffTable[4] = 55;
//                        bFOD_EffTable[5] = 60;
                    }
                }
                if( w_QI_Receive_Power < D_FOD_Power_efficiency_L1 )
                {
                    b_QI_effciency_table = bFOD_EffTable[0] +
                                           (uint8_t)( (uint32_t) ( bFOD_EffTable[0]- bFOD_EffTable[0])* ( w_QI_Receive_Power )/(D_FOD_Power_efficiency_L1) );
                }
                else if( w_QI_Receive_Power < D_FOD_Power_efficiency_L2 )
                {
                    b_QI_effciency_table = bFOD_EffTable[0] +
                                           (uint8_t)( (uint32_t) ( bFOD_EffTable[1]- bFOD_EffTable[0])* ( w_QI_Receive_Power -D_FOD_Power_efficiency_L1)/(D_FOD_Power_efficiency_L2- D_FOD_Power_efficiency_L1) );
                }
                else if( w_QI_Receive_Power < D_FOD_Power_efficiency_L3 )
                {
                    b_QI_effciency_table = bFOD_EffTable[1] +
                                           (uint8_t)( (uint32_t) ( bFOD_EffTable[2]- bFOD_EffTable[1])* ( w_QI_Receive_Power -D_FOD_Power_efficiency_L2)/(D_FOD_Power_efficiency_L3- D_FOD_Power_efficiency_L2) );
                }
                else if( w_QI_Receive_Power < D_FOD_Power_efficiency_L4 )
                {
                    b_QI_effciency_table = bFOD_EffTable[2] +
                                           (uint8_t)( (uint32_t) ( bFOD_EffTable[3]- bFOD_EffTable[2])* ( w_QI_Receive_Power -D_FOD_Power_efficiency_L3)/(D_FOD_Power_efficiency_L4- D_FOD_Power_efficiency_L3) );
                }
                else if( w_QI_Receive_Power < D_FOD_Power_efficiency_L5 )
                {
                    b_QI_effciency_table = bFOD_EffTable[3] +
                                           (uint8_t)( (uint32_t) ( bFOD_EffTable[4]- bFOD_EffTable[3])* ( w_QI_Receive_Power -D_FOD_Power_efficiency_L4)/(D_FOD_Power_efficiency_L5- D_FOD_Power_efficiency_L4) );
                }
                else
                {
                    b_QI_effciency_table = bFOD_EffTable[5];
                }
//                w_QI_Retified_Power_Limit = (uint16_t)b_QI_Retified_Power*bPOWERtRANSFER_MaxPower*bPOWERtRANSFER_PowerClass;
//                w_QI_Retified_Power_Limit = (uint16_t)((uint32_t)w_QI_Retified_Power_Limit *100 *1000/b_QI_effciency_table/256);

                w_QI_Retified_Power_Limit = (uint16_t)( (uint32_t)w_QI_Receive_Power*100/b_QI_effciency_table);

                if( wPowerTransferCounter > D_FOD_Limit_Power_Time*1000 )
                {
                    if(bManufactor==D_Manufactor_QI)
                    {
                        wCal = 800;//500;
                    }
                    else
                    {
                        wCal = D_FOD_Limit_Power_Over_Time;
                    }
                }
                else
                {
                    wCal = D_FOD_Limit_Power_Under_Time;
                }
                if( w_QI_Retified_Power_Limit <wCal)
                    w_QI_Retified_Power_Limit = wCal;

                bReceivedPowerStatus = D_QI_Received_Power_Status_active;
                bReceivedPowerCounter = 0;
                bIDRXCounter=0;
                if( bReceivePowerCounter <0xf0)
                    bReceivePowerCounter++;
                if( bReceivePowerCounter > D_Received_Power_Debounce )
                    fgFODStart = 1;
                break;

            case D_QI_ChargeStatus:
                b_QI_Charge_Status = bDecode[1];
                if(b_QI_Charge_Status>=100)
                {
                    bTargetStatus = D_Target_Status_Complete;
                }
                break;
            case D_QI_Proprietary_0x18:
                if(fFastcharge>=1)
                    fmidlepower++;
                if(fmidlepower>=5)
                    fmidlepower=5;
                break;
            case D_QI_Proprietary_0x19:
                break;
            case D_QI_Proprietary_0x28:
                if(fFastcharge>=1 && bControlErrorCNTsend>=5)
                    fboostup++;
                if(fboostup==1)
                    bControlErrorCNTsend=0;
                break;
            case D_QI_Proprietary_0x29:
            case D_QI_Proprietary_0x38:
            case D_QI_Proprietary_0x48:
            case D_QI_Proprietary_0x58:
            case D_QI_Proprietary_0x68:
            case D_QI_Proprietary_0x78:
            case D_QI_Proprietary_0x84:
            case D_QI_Proprietary_0xA4:
            case D_QI_Proprietary_0xC4:
            case D_QI_Proprietary_0xE2:
            case D_QI_Proprietary_0xF2:
                break;
            case D_QI_GeneralRequest:
                if(bDecode[1]==0x30)
                {
                    fsendstart=1;
                    wsendTimecount=0;
                    bByteindexsendCNT=5;
                    bBitindexsendCNT=1;
                    for(i=0; i<5; i++)
                        bDatatosend[i]=bTX_Identification_Data[i];
                    fgPacketStartbitsend=1;
                }
                else if(bDecode[1]==0x31)
                {
                    fsendstart=1;
                    wsendTimecount=0;
                    bByteindexsendCNT=5;
                    bBitindexsendCNT=1;
                    for(i=0; i<5; i++)
                        bDatatosend[i]=bTX_Capability_Data[i];
                    fgPacketStartbitsend=1;
                }
                else
                {  }
                break;
            case D_QI_SpecificRequest:
                if(bDecode[1]==0x00)
                {
                    f15Wstartup|=0x01;
                }
                else if(bDecode[1]==0x01)
                {
                    f15Wstartup|=0x02;
                }
                else if(bDecode[1]==0x02)
                {
                    f15Wstartup|=0x04;
                }
                else if(bDecode[1]==0x03)
                {
                    f15Wstartup|=0x08;
                    if(bDecode[2]>3)
                        bx_QI_Configuration[4]|=0xC0;
                }
                else if(bDecode[1]==0x04)
                {
                    f15Wstartup|=0x10;
                }
                else;
                fsendstart=1;
                wsendTimecount=0;
                bByteindexsendCNT=1;
                bBitindexsendCNT=1;
                bDatatosend[0]=bTX_ACK_Data[0];
                fgPacketStartbitsend=0;
                break;
            case D_QI_FODStatus:
                fsendstart=1;
                wsendTimecount=0;
                bByteindexsendCNT=1;
                bBitindexsendCNT=1;
                bDatatosend[0]=bTX_ACK_Data[0];
                fgPacketStartbitsend=0;
                break;
            case D_QI_Renegotiate:
                break;
            case D_QI_SignalStrength:
		    case D_QI_Configuration:	
                break;
            default:
                bErrorTerminateCounter = 0;
              //  bError = D_QI_POWERTRANSFER_ERROR_C;
                break;
            }
            fgPacketStop = 0;
            fgCap2PacketStop = 0;
            fgCap0PacketStop = 0;
        }

        F_QI_POWERTRANSFER_Phase_Stop_handler();
        if( wPOWERTRANSFER_Temperature_Counter > 500 )
        {
//				  Enable_ADC_AIN5;  //P04
//			    clr_ADCF;
//			    set_ADCS;					// ADC start trig signal
//          while(ADCF == 0);
//				  wADCRadio = wADCRadio/2+((((uint16_t)ADCRH)<<2)+(ADCRL>>2))/2;

            adcInit(CON_ADC_CH_Temperature);              //ADC???

            adcStart();

            adcWait();
            wADCRadio = wADCRadio/2 + ADC_GetADCResult(CON_ADC_CH_Temperature)/2;

            if( wADCRadio > D_T_Protection_High_Code )
            {
                F_QI_POWERTRANSFER_Phase_Thermal_Stop_handler();
                //bTargetStatus = D_Target_Status_FOD;

//                fgChangePhase = 1;
//                bPhaseBefore = bPhase;
//                bPhase = SELECTION;
//                bErrorFrom = D_ErrorFrom_None;
//                bTargetStatus =	D_Target_Status_OverTemperature;
            }
            wPOWERTRANSFER_Temperature_Counter = 0;
        }
    }
}
