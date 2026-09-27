#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"
extern uint16_t wCompleteCounter;
extern uint16_t w10SCounter;
extern uint8_t  b15WMOD;
extern uint8_t  bsumsung10WMOD;
extern uint16_t FODcounter;
extern uint8_t  FODflag;

extern uint16_t  wPingCurrent;

extern uint16_t Get_PWM_Frequency_Value(void);
extern void PWM_SetFreq(unsigned int freq);
extern void PWM_SetFreq_(unsigned int freq);
extern void PWM_Duty_Set(uint16_t Value);

extern void PWM_SetDami(unsigned int dami0,unsigned int dami1);
extern void F_Breath_LED_Set(uint8_t flag);

void F_CaptureData2_Clear(void)
{
    bBitoneCNT2=0;
    for(bCaptureDataIndex2=0; bCaptureDataIndex2<D_CAP_BUFF; bCaptureDataIndex2++)
    {
        bCaptureData2[bCaptureDataIndex2]=0;
    }
    //memset(bCaptureData2,D_CAP_BUFF,0);
    bCaptureDataIndex2=0;
    bBitIndex2=0;
}
void F_CaptureData0_Clear(void)
{
    bBitoneCNT0=0;
    for(bCaptureDataIndex0=0; bCaptureDataIndex0<D_CAP_BUFF; bCaptureDataIndex0++)
    {
        bCaptureData0[bCaptureDataIndex0]=0;
    }
    //memset(bCaptureData0,D_CAP_BUFF,0);
    bCaptureDataIndex0=0;
    bBitIndex0=0;
}

void F_QI_Change_Phase_Judgement()
{
    if( fgChangePhase )
    {
//        fgPrintf = 1;
        if( bPhase == SELECTION)
        {
            wObjectDetectionIntervalCounter = 0;
            D_PWM_Clear;
            b_QI_PowerHoldOff_Time = 0; //Clear Power Hold off Time setting
            bx_QI_Identification[0] =0;
            bx_QI_Identification[1] =0;
            bx_QI_Identification[2] =0;
            bx_QI_Identification[3] =0;
            bx_QI_Identification[4] =0;
            bx_QI_Identification[5] =0;
            bx_QI_Identification[6] =0;
            bx_QI_Configuration[0] = 0;
            bx_QI_Configuration[1] = 0;
            bx_QI_Configuration[2] = 0;
            bx_QI_Configuration[3] = 0;
            bx_QI_Configuration[4] = 0;
            bx_QI_Extended_Identification[0] = 0;
            bx_QI_Extended_Identification[1] = 0;
            bx_QI_Extended_Identification[2] = 0;
            bx_QI_Extended_Identification[3] = 0;
            bx_QI_Extended_Identification[4] = 0;
            bx_QI_Extended_Identification[5] = 0;
            bx_QI_Extended_Identification[6] = 0;
            bx_QI_Extended_Identification[7] = 0;
//            w_Object_Frequency_max = D_QI_AI_Sv_Lv4_Code ;
//            w_Object_Frequency_min = D_QI_AI_Sv_Lv0_Code;
            fgPacketStart = fgPacketStop = 0;
            fgCap2PacketStart = fgCap2PacketStop = 0;
            fgPacketCheckSumError = 0;
            bPacketReceiveDebounce = 0;
            fmidlepower=0;
            wStart9VCounter=0;
            wStart9VCounter1=0;
            fboostup=0;
            fSendNeg=0;
            fSendPolarity=0;
            fSendDepth=0;
            f15Wstartup=0;
            b15WMOD=0;
            wCompleteCounter=0;
//						w10SCounter=0;

        //    F_CaptureData2_Clear();
     //       F_CaptureData0_Clear();
//            //************************************
//            bBitoneCNT2=0;
//            for(bCaptureDataIndex2=0; bCaptureDataIndex2<D_CAP_BUFF; bCaptureDataIndex2++)
//            {
//                bCaptureData2[bCaptureDataIndex2]=0;
//            }
//            bCaptureDataIndex2=0;
//            bBitIndex2=0;
//            //************************************
//            bBitoneCNT0=0;
//            for(bCaptureDataIndex0=0; bCaptureDataIndex0<D_CAP_BUFF; bCaptureDataIndex0++)
//            {
//                bCaptureData0[bCaptureDataIndex0]=0;
//            }
//            bCaptureDataIndex0=0;
//            bBitIndex0=0;
//            //************************************

            //ACMP_ClearIntFlag(ACMP0);
//            CNIF &= ~(1<< ACMP0);
//            //ACMP_ClearIntFlag(ACMP1);
//            CNIF &= ~(1<< ACMP1);
//            //ACMP_DisableInt(ACMP0);
//            CNIE &=  ~(1<< ACMP0);
//            //ACMP_DisableInt(ACMP1);
//            CNIE &=  ~(1<< ACMP1);

            ACMP_ClearIntFlag(ACMP0);
            ACMP_ClearIntFlag(ACMP1);
            ACMP_DisableInt(ACMP0);
          //  ACMP_DisableInt(ACMP1);

            bAppleIsFind = 0;

            bSamsungIsFull = 0;

        }

		
        else if ( bPhase == PING_APPLE)
        {
        }
		
        else if ( bPhase == PING)
        {
            bError = 0;
            b_GeneralTime_Status = D_GeneralTime_No_Signal;
            bPacketErrorCounter = 0;
            bTargetStatus = D_Target_Status_Pinging;


//            clr_PWMRUN;
//           	PWMPL = (BYTE)Feq_QI_Fop_INI_Code;
//         		PWMPH = (BYTE)(Feq_QI_Fop_INI_Code>>8);
//
//         		PWM0L = (BYTE)Feq_QI_Fop_INI_Code/2;
//         		PWM0H = (BYTE)((Feq_QI_Fop_INI_Code/2)>>8);;
//         		PWM2L = (BYTE)Feq_QI_Fop_INI_Code/2;
//         		PWM2H = (BYTE)((Feq_QI_Fop_INI_Code/2)>>8);

//						set_PWMRUN;

//            EPWM_Stop(EPWM_CH_0_MSK|EPWM_CH_1_MSK);
            D_PWM_Start;
       //     fFastcharge=0;
         #if 0
            if(fFastcharge>=1)
            {
                if(bAppleIsFind==1)
                {
                    D_PWM_OpenHalf;
                    PWM_SetFreq(D_QI_AI_Sv_apple_Code);
                    PWM_SetDami(D_QI_AI_Sv_apple_Code/5,0);
                    //PWM_SetDami(D_QI_AI_Sv_apple_Code/9,0);
//                    if(fFastcharge==1)
//                    {
//                        PWM_SetDami(D_QI_AI_Sv_apple_Code/2,0);
//                    }
//                    if(fFastcharge==2)
//                    {
//                        PWM_SetDami(D_QI_AI_Sv_apple_Code/2,0);
//                    }
                }
                else
                {
                    D_PWM_OpenHalf;
                    if(fFastcharge==1)
                    {
                        PWM_SetFreq(64000/125);
                        //PWM_SetFreq(48000/128);
                        //PWM_SetFreq(48000/128);
                    }
                    if(fFastcharge==2)
                    {
                        PWM_SetFreq(64000/130);
                        //PWM_SetFreq(48000/133);
                        //PWM_SetFreq(48000/128);
                    }
                }
//                fgPrintf = 2;

//                CCAPM0 &= ~PWM0;//??
//                CCAPM1 &= ~PWM1;//??
//    CCAPM2 &= ~PWM2;//??
//    CCAPM3 &= ~PWM3;//??

//						 while(LOAD);
//						 set_PMEN0;
//             set_PMEN1;
//             clr_PMD0;
//             set_PMD1;
            }
		
			
            else
		   #endif 	
            {
                  D_PWM_OpenAll;
               // PWM_SetFreq(Feq_QI_Fop_INI_Code);
            //   PWM_SetFreq(64000/136);    //133
           //        PWM_SetFreq(64000/170);    
               //    D_PWM_OpenHalf;
			      //  PWM_SetFreq_(64000/149);  
                   PWM_SetFreq(64000/168);    
				//    PWM_SetDami(64000/149/10,0);
               //    PWM_SetFreq(64000/135);  
            }
            //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
            //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
            //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
            //EPWM_Start(EPWM_CH_2_MSK | EPWM_CH_3_MSK);

            wPingCurrent = 0;

            // while(LOAD);
            //set_PMEN0;
            //set_PMEN1;
            // clr_PMD0;
            // set_PMD1;


            ACMP_ClearIntFlag(ACMP0);
            ACMP_ClearIntFlag(ACMP1);
            ACMP_EnableInt(ACMP0);
            ACMP_EnableInt(ACMP1);

//            CNIF &= ~(1<< ACMP0);
//            CNIF &= ~(1<< ACMP1);
//            //ACMP_EnableInt(ACMP0);
//            CNIE |= (1<<ACMP0);
//            //ACMP_EnableInt(ACMP1);
//            CNIE |= (1<<ACMP1);
        }
        else if ( bPhase == ID_CONF )
        {
            if( bPhaseBefore == PING)
            {
                b_QI_PowerHoldOff_Time = 5;
                b_ID_CONF_Packet_Index = D_ID_CONF_Identification_Packet;
                bError = 0;
                b_GeneralTime_Status = D_GeneralTime_No_Signal;
                bPacketErrorCounter = 0;
                bTargetStatus = D_Target_Status_Pinging;
                b_ID_CONF_private_setting_counter = 0;
            }
            else if( bPhaseBefore == POWERTRANSFER )
            {
                b_QI_PowerHoldOff_Time = 5;
                b_ID_CONF_Packet_Index = D_ID_CONF_Optional_Configuration_Packet1;
                bError = 0;
                b_GeneralTime_Status = D_GeneralTime_No_Signal; //
                bPacketErrorCounter = 0;
                bTargetStatus = D_Target_Status_Pinging;
                b_ID_CONF_private_setting_counter = 0;
                if( bReconfigCounter <  D_QI_POWERTRANSFER_tPowerPhaseTime );
                else
                {
                    bPhaseBefore = bPhase;
                    bTargetStatus = D_Target_Reconfigure;
                    bPhase  = SELECTION;
                    fgChangePhase = 1;
                    D_PWM_Clear;
                }
            }
            else
            {
                bPhase  = SELECTION;
                fgChangePhase = 1;
                D_PWM_Clear;
            }
        }
        else if ( bPhase == POWERTRANSFER )
        {
            fgFODStart = 0;
            bReceivePowerCounter=0;
            w_QI_Retified_Power_Limit = 0xFFF0;
            //  bOverCurrentCounter = 0;
            bCurrentCounter = 254;
            F_Measure_Real_Current();
            wAC_Low_Pass_Voltage = 0;
            //  wVirtualWorkCounter = 0;
            wPowerTransferCounter = 0;
            F_QI_ChangePhase_Power_Transfer_Common();
//            wAC_Zero_Current_Code = F_Measure_Control_Current();
//            wBackupMDPRD = Get_PWM_Frequency_Value();//((uint16_t)PWMPH<<8) + PWMPL;
//            wBackupDuty  = wBackupMDPRD/2;//((uint16_t)PWM0H<<8) + PWM0L;
            bPacketErrorCounter = 0;
            bError = 0;
            fgErrorControl = 0;
//            bCounter = 0;
//            wFrequncyLimit = 0;
//            fgLearningFrequncy = 0;
            bControlErrorCNTsend=0;
            wStart9VCounter=0;
            wStart9VCounter1=0;
            wsendTimecount=0;
            fgPacketStartbitsend=0;
            fsendparity=0;
            bByteindexsendCNT=0;
            bBitindexsendCNT=0;
            bDatatosend[0]=0x00;
            bDatatosend[1]=0x00;
            bDatatosend[2]=0x00;
            bDatatosend[3]=0x00;
            bDatatosend[4]=0x00;
            bByteindexsend=0;
            bBitindexsend=0;
            bTimebase=0;
            fsendstart=0;
            fsendstart1=0;
            fsendbit=0;
            fhalfbitsend=0;
            fFrequencyshift=0;
            firstinchargeFrequency=0;
            fmidlepower=0;
            fboostup=0;
            f15Wstartup=0;
            wCompleteCounter=0;
            w10SCounter=0;
//			 w10SCounter=0;
            b15WMOD=0;
            //P23=1;
            //P24=1;
            bsumsung10WMOD=0;
            FODcounter=0;
            wChargeCompleteCNT=0;
            bChargeCompleteCNT=0;

            bCaptureDataIndex0=0;
            bBitIndex0=0;
            bBitoneCNT0=0;

            bCaptureDataIndex2=0;
            bBitIndex2=0;
            bBitoneCNT2=0;

            wQiTotalError = 0;
            bQiRPAdjFlag = 0;
			//if(bManufactor==D_Manufactor_XiaoMi)
				//{
			   //    PWM_SetFreq_(64000/149);  
                 //  PWM_SetFreq(64000/160);    
		        //   PWM_SetDami(64000/149/10,0);
				//}

            wPOWERTRANSFER_Error_Packet_Timeout_Counter=0;
            wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter=0;
            lPOWERTRANSFER_COMPATIABLE_DELAY = 0;
            wPOWERTRANSFER_COMPATIABLE_COUNT = 0;;
            b_QI_Retified_Power = 128;
            b_QI_Retified_Power_Before = 128;
            i_QI_Error = 0;
//            w_QI_Control_ta_j_1_Before = 0;
//            i_QI_Error_Before = 0;
            b_QI_Charge_Status = 0;
            //Measure the actural Primary Cell Current for j loop initial
//            w_QI_Control_ta_j_1 = F_Measure_Control_Current();
//            i_QI_Control_e_j_i_1 = 0;
//            i_QI_Control_I_j_i_1 = 0;
            bTargetStatus = D_Target_Status_Pinged;
            wPOWERTRANSFER_Temperature_Counter = 250;
            // wCommErrorCounter = 0;
            fgLimitFreBoundary = 0;
//            fgBelow205K = 0;
            b_PP_ControlStatus = D_PP_ConrolPhase_1; //Reset the Control Phase
//            fgCompatiableNokia = 0;
            wCompatiableLEDDelay = 1000;
            bLEDBreathingT1=0;
            bLEDBreathingT2=0;
            fgLEDBreathingC=0;
            bLEDBreathingC1=11040;
            LEDshiftCNT=0;

            wCheckOverTemp_Delay = 0;
            bNowTempValueBackup = 0;

            bInvertCounter = 0;
            fgMicroFunction = 0;
            bReceivedPowerStatus = D_QI_Received_Power_Status_Nothing;
            bReceivedPowerCounter = 0;
            if(0 == (bx_QI_Configuration[0]>>6))///0x40 )
                bPOWERtRANSFER_PowerClass = 1;
            else if( 1 == (bx_QI_Configuration[0]>>6))///0x40 )
                bPOWERtRANSFER_PowerClass = 10;
            else if( 2 == (bx_QI_Configuration[0]>>6))///0x40 )
                bPOWERtRANSFER_PowerClass = 100;
            else
                bPOWERtRANSFER_PowerClass = 100;
            bPOWERtRANSFER_MaxPower = bx_QI_Configuration[0]&0x3f;//%0x40;
            wUSBPowerCounter = 0;
            bUSB_LED_Debounce = 0;
            bIDRXCounter=0;
			
			#if 0
            if(bManufactor==D_Manufactor_apple)
            {
                D_PWM_OpenHalf;
                PWM_SetFreq(D_QI_AI_Sv_apple_Code);
                PWM_SetDami(D_QI_AI_Sv_apple_Code/6,0);
                //D_PWM_OpenAll;

                //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
                //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
                //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                PWMCNTCLR = 0x03;

//   						  while(LOAD);
//   	            PWMPL = (BYTE)(D_QI_AI_Sv_apple_Code);
//            		PWMPH = (BYTE)((D_QI_AI_Sv_apple_Code)>>8);
//            		while(LOAD);
//            		PWM0L = 0;
//            		PWM0H = 0;
//   		          while(LOAD);
//            		PWM2L = (BYTE)(D_QI_AI_Sv_apple_Code-D_QI_AI_Sv_apple_Code/3);
//            		PWM2H = (BYTE)((D_QI_AI_Sv_apple_Code-D_QI_AI_Sv_apple_Code/3)>>8);
//   						  set_LOAD;
            }
			#endif
			
            //while(LOAD);
            //set_PMEN0;
            //set_PMEN1;
            //clr_PMD0;
            // set_PMD1;

#ifdef BREATH_LED
            //start
            F_Breath_LED_Set(1);
#endif
        }
        fgChangePhase = 0;
    }
}
void F_QI_ChangePhase_Power_Transfer_Common()
{
//    if( D_ID_BYTE4_GP == bx_QI_Identification[4] &&
//            D_ID_BYTE5_GP == bx_QI_Identification[5] &&
//            D_ID_BYTE6_GP == bx_QI_Identification[6] )
//    {
//        w_Object_Frequency_max = D_GP_Fre_max_code;
//        w_Object_Frequency_min = D_GP_Fre_min_code;
//    }
//    else if(D_ID_BYTE4_Maxwell_1F == bx_QI_Identification[4] &&
//            D_ID_BYTE5_Maxwell_1F == bx_QI_Identification[5] &&
//            D_ID_BYTE6_Maxwell_1F == bx_QI_Identification[6] )
//    {
//        w_Object_Frequency_max = D_Maxwell_1F_Fre_max_code;
//        w_Object_Frequency_min = D_Maxwell_1F_Fre_min_code;
//    }
//    else if(D_ID_BYTE4_Maxwell_2F == bx_QI_Identification[4] &&
//            D_ID_BYTE5_Maxwell_2F == bx_QI_Identification[5] &&
//            D_ID_BYTE6_Maxwell_2F == bx_QI_Identification[6] )
//    {
//        w_Object_Frequency_max = D_Maxwell_2F_Fre_max_code;
//        w_Object_Frequency_min = D_Maxwell_2F_Fre_min_code;
//    }
//    else
//    {
//        w_Object_Frequency_max = D_Others_Fre_max_code ;
//        w_Object_Frequency_min = D_Others_Fre_min_code;
//    }
    bMP_ID = D_MPID_None;
    fgFOD = 0;
    bManufactor = D_Manufactor_none;
    if( D_QI_WPC_Version_11 == bx_QI_Identification[0] || D_QI_WPC_Version_12 == bx_QI_Identification[0])
    {
        fgFOD = 1;
        if( D_MOSFET_Hardware_FOXCON == bMOSFETDriver_Setting )
        {
            bFOD_EffTable[0] = bFOD_FOXCON_Table[0];
            bFOD_EffTable[1] = bFOD_FOXCON_Table[1];
            bFOD_EffTable[2] = bFOD_FOXCON_Table[2];
            bFOD_EffTable[3] = bFOD_FOXCON_Table[3];
            bFOD_EffTable[4] = bFOD_FOXCON_Table[4];
            bFOD_EffTable[5] = bFOD_FOXCON_Table[5];
        }
        else
        {
            bFOD_EffTable[0] = bFOD_Comm_Table[0];
            bFOD_EffTable[1] = bFOD_Comm_Table[1];
            bFOD_EffTable[2] = bFOD_Comm_Table[2];
            bFOD_EffTable[3] = bFOD_Comm_Table[3];
            bFOD_EffTable[4] = bFOD_Comm_Table[4];
            bFOD_EffTable[5] = bFOD_Comm_Table[5];
        }
    }
    if( 0== bx_QI_Identification[1] &&0x10 ==bx_QI_Identification[2] )
    {
        if( D_QI_WPC_Version_11 == bx_QI_Identification[0] )
        {
            if( D_ID_BYTE4_N4 == bx_QI_Identification[4] &&
                    D_ID_BYTE5_N4 == bx_QI_Identification[5] &&
                    D_ID_BYTE6_N4 == bx_QI_Identification[6] )
            {
                fgFOD = 0;  //if N4, give up FOD
                bMP_ID = D_MPID_N4;
            }
            else
            {
                bFOD_EffTable[0] = bFOD_Ti_Table[0];
                bFOD_EffTable[1] = bFOD_Ti_Table[1];
                bFOD_EffTable[2] = bFOD_Ti_Table[2];
                bFOD_EffTable[3] = bFOD_Ti_Table[3];
                bFOD_EffTable[4] = bFOD_Ti_Table[4];
                bFOD_EffTable[5] = bFOD_Ti_Table[5];
                fgFOD = 1;  //if TI , than set them be FOD Function.
            }
        }
        bManufactor = D_Manufactor_TI;
    }
    if( 0== bx_QI_Identification[1] &&0x24 ==bx_QI_Identification[2] )
    {
        bManufactor=D_Manufactor_QI;
    }

    if( 0== bx_QI_Identification[1] &&0x42 ==bx_QI_Identification[2] )
    {
        bManufactor=D_Manufactor_sumsung;
    }

    if( 0== bx_QI_Identification[1] &&0x60 ==bx_QI_Identification[2] )
    {
        bManufactor=D_Manufactor_HuaWei;
    }

    if( 0== bx_QI_Identification[1] &&0x5A ==bx_QI_Identification[2] )
    {
        bManufactor=D_Manufactor_apple;
        dwAppleLowCurrentCnt = 0;

        //PWM->CTL |= PWM_CTL_CNTCLR_Msk;
        //PWM->PERIOD1 = D_QI_AI_Sv_Lv1_128_Code;
        //PWM->CMPDAT1 = D_QI_AI_Sv_Below_Lv1_128_min_duty_Code;
        //PWM->PERIOD0 = D_QI_AI_Sv_Lv1_128_Code;
        // PWM->CMPDAT0 = 0;
    }
}
