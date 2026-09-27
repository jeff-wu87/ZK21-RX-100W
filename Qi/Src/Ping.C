#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"
/***********************************
// Ping Phase
************************************/
uint16_t  wPingCurrent=0;
extern uint16_t FODcounter;
extern uint8_t FODcounter2;
extern uint8_t  FODflag;
uint8_t  FODcounter3=0;
void F_QI_Ping_Phase_Manage()
{
    if(bError)
    {
        bErrorTerminateCounter++;
        switch(bError)
        {
        case D_QI_PING_ERROR_A:
            bErrorTerminateCounter++;
            if( bErrorTerminateCounter >= D_QI_tterminate )
            {
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                fgChangePhase = 1;
                D_PWM_Clear;
                bTargetStatus = D_Target_Status_None;
            }
            break;
        case D_QI_PING_ERROR_C:
            bErrorTerminateCounter++;
            if( bErrorTerminateCounter >= D_QI_texpire )
            {
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                fgChangePhase = 1;
                bTargetStatus = D_Target_Status_Pinging;
                D_PWM_Clear;
            }
            break;
        case D_QI_PING_ERROR_D:
            bErrorTerminateCounter++;
            if( bErrorTerminateCounter >= D_QI_tterminate )
            {
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                fgChangePhase = 1;
                bTargetStatus = D_Target_Status_None;
                D_PWM_Clear;
            }
            break;
        case D_QI_PING_ERROR_E:
            bErrorTerminateCounter++;
            if( bErrorTerminateCounter >= D_QI_tterminate )
            {
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                fgChangePhase = 1;
                bTargetStatus = D_Target_Status_None;
                D_PWM_Clear;
            }
            break;
        default:  //unkown fail , return SELECTION phase.
            fgChangePhase = 1;
            bPhaseBefore = bPhase;
            bPhase = SELECTION;
            bTargetStatus = D_Target_Status_None;
            break;
        }
    }
    else
    {
//        if(wRC_Low_Pass_Current<200)
//        {
        wPingCurrent = wPingCurrent*3/4 + wRC_Low_Pass_Current/4;
//        }
//        else
//        {
//            wPingCurrent = wPingCurrent*3/4 + 50;
//        }

        //wPingCurrent = 0;

//        my_itoa(wPingCurrent);
//        puts("\n\r\n");

        // if Qi Test then must disable
        if((wPingCurrent > D_Selection_Ping_ID_CONF_Current_Limit_Code )&&( D_GeneralTime_No_Signal == b_GeneralTime_Status ))
        {
            bIdleFodCnt++;
            if(bIdleFodCnt>100)
            {
                bIdleFodCnt = 0;
//                fgChangePhase = 1;
//                bPhaseBefore = bPhase;
//                bPhase = SELECTION;
//                bErrorFrom = D_ErrorFrom_None;
//                bTargetStatus =	D_Target_Status_Device_Error;

#ifdef DEBUG_PRINT
                my_itoa(wPingCurrent,0);
                puts("===\r\n");
#endif
            }
        }
        else
        {
            bIdleFodCnt = 0;
        }
        bPacketErrorCounter++;
        if( D_GeneralTime_No_Signal == b_GeneralTime_Status )
        {
            if(  D_QI_tping_withDelay == bPacketErrorCounter )
            {
                bErrorTerminateCounter = 0;
                bError = D_QI_PING_ERROR_A; //PING Error
                FODcounter3++;
                if(FODcounter3>=5)
                {
                    FODcounter2=0;
                    FODflag=0;
                    FODcounter3=0;
                }
#ifdef DEBUG_PRINT
                my_itoa(wPingCurrent,0);
                puts("\r\n");
#endif
            }
        }
        else if( D_GeneralTime_Signal == b_GeneralTime_Status )
        {
            if( D_QI_tfirst_withDelay == bPacketErrorCounter )
            {
                bErrorTerminateCounter= 0;
                bError = D_QI_PING_ERROR_D; //PING Error

                FODcounter3++;
                if(FODcounter3>=5)
                {
                    FODcounter2=0;
                    FODflag=0;
                    FODcounter3=0;
                }
            }
        }
        else
        {
            bError = 0xFF; //PING Error
        }
        if( fgPacketStart )
        {
            b_GeneralTime_Status = D_GeneralTime_Signal;
            fgPacketStart = 0;
            bPacketErrorCounter = 0;
			   puts("fgPacketStart\r\n");
            //*********************************************************
            if(FODflag==1)
            {
                FODcounter3=0;
                FODcounter2++;
                //QI fod后不要起来
                if(bManufactor==D_Manufactor_QI)
                {
                    FODcounter2 = 100;
                }
                if(FODcounter2<=200)
                {
                    bPhaseBefore = bPhase;
                    bPhase = SELECTION;
                    fgChangePhase = 1;
                    D_PWM_Clear;
                    bTargetStatus = D_Target_Status_None;
                }
                else
                {
                    FODcounter2=0;
                    FODflag=0;
                }
            }
            //*********************************************************
        }
        if( fgPacketStop )
        {
             puts("fgPacketStop\r\n");
            if( D_QI_SignalStrength == bDecode[0] ) //Check The Packe is Signal Strength
            {
                fgPrintf = 4;
                b_QI_Signal_Strength_Value = bDecode[1];
                //Success enter the ID_CONF Phase
                fgChangePhase = 1;
                bPhaseBefore = bPhase;
                bPhase = ID_CONF;
            }
            else
            {
                bErrorTerminateCounter= 0;
                bError = D_QI_PING_ERROR_E; //PING Error
            }
            fgPacketStop = 0;
            b_GeneralTime_Status = D_GeneralTime_No_Signal;
            bPacketErrorCounter = 0;
        }
    }
}
