#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"
extern void PWM_SetFreq(unsigned int freq);

extern void PWM_SetDami(unsigned int dami0,unsigned int dami1);

/***********************************
// ID and CONF Phase Register Bank 1
************************************/
void F_QI_ID_CONF_Phase_Manage()
{
    if(bError)  //Error Handler
    {
        bErrorTerminateCounter++;
        switch( bError )
        {
        case D_QI_ID_CONF_ERROR_A:
        case D_QI_ID_CONF_ERROR_B:
            if( bErrorTerminateCounter >= D_QI_tterminate)
            {
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                fgChangePhase = 1;
                D_PWM_Clear;
                bTargetStatus = D_Target_Status_None;
            }
            break;
        case D_QI_ID_CONF_ERROR_E:
            if( bErrorTerminateCounter >= D_QI_texpire)
            {
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                fgChangePhase = 1;
                D_PWM_Clear;
                bTargetStatus = D_Target_Status_None;
            }
            break;
        case D_QI_ID_CONF_ERROR_C:
        case D_QI_ID_CONF_ERROR_F:
            if( bErrorTerminateCounter >= D_QI_tterminate)
            {
                bPhaseBefore = bPhase;
                bPhase = SELECTION;
                fgChangePhase = 1;
                D_PWM_Clear;
                bTargetStatus = D_Target_Status_None;
            }
            break;
        default:
            bPhaseBefore = bPhase;
            bPhase = SELECTION;
            fgChangePhase = 1;
            bTargetStatus = D_Target_Status_None;
            D_PWM_Clear;
            break;
        }
    }
    else    //Normal Handler
    {
        bPacketErrorCounter++;
        if( D_GeneralTime_No_Signal == b_GeneralTime_Status )
        {
            if(  D_QI_ID_CONF_tnext == bPacketErrorCounter )
            {
                bErrorTerminateCounter = 0;
                bError = D_QI_ID_CONF_ERROR_A; //ID CONF Error
            }
        }
        else if( D_GeneralTime_Signal == b_GeneralTime_Status )
        {
            if( D_QI_ID_CONF_tmax == bPacketErrorCounter )
            {
                bErrorTerminateCounter= 0;
                bError = D_QI_ID_CONF_ERROR_B; //ID CONF Error
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
        }
        if( fgPacketStop )
        {
            switch(b_ID_CONF_Packet_Index)
            {
            case D_ID_CONF_Identification_Packet:

                if( D_QI_Identification == bDecode[0] )   //Should Receive first
                {
                    fgPrintf = 3;
                    bx_QI_Identification[0] = bDecode[1];
                    bx_QI_Identification[1] = bDecode[2];
                    bx_QI_Identification[2] = bDecode[3];
                    bx_QI_Identification[3] = bDecode[4];
                    bx_QI_Identification[4] = bDecode[5];
                    bx_QI_Identification[5] = bDecode[6];
                    bx_QI_Identification[6] = bDecode[7];
                    if(bx_QI_Identification[3]& 0x80)
                        b_ID_CONF_Packet_Index = D_ID_CONF_Ext_Identification_Packet;
                    else
                        b_ID_CONF_Packet_Index = D_ID_CONF_Optional_Configuration_Packet1;

                    if( 0== bx_QI_Identification[1] &&0x5A ==bx_QI_Identification[2] )
                    {
                        bManufactor=D_Manufactor_apple;

                       #if 0
                        if(bAppleIsFind==0)
                        {
                            bPhase  = PING_APPLE;
                            fgChangePhase = 1;
                            D_PWM_Clear;

                            bAppleIsFind = 1;
                        }
						#endif
                    }
                   if( 0== bx_QI_Identification[1] &&0x6e ==bx_QI_Identification[2] )
                    {
                         bManufactor= D_Manufactor_HuaWei;  //D_Manufactor_XiaoMi;
					//	 PWM_SetFreq_(64000/146);  
                 //  PWM_SetFreq(64000/160);    
		                //  PWM_SetDami(64000/146/10,0);
                          puts("D_Manufactor_XiaoMi\n\r");
                    }
					
				  if(bx_QI_Identification[2]==0xf2&&bx_QI_Identification[3]==0xe4&&bx_QI_Identification[4]==0xc2)
				  	{
                       //D_PWM_OpenAll;
					   fgChangePhase = 1;
                        bPhaseBefore = bPhase;
                        bPhase = POWERTRANSFER;
						find36wflg=1;
						
					                	  bx_QI_Configuration[0] = 0x42;
										   bx_QI_Configuration[1] = 0;
										   bx_QI_Configuration[2] = 0;
										   bx_QI_Configuration[3] = 0;
										   bx_QI_Configuration[4] = 0;
						
                     
				  	}
                }
                else
                {
                    bErrorTerminateCounter = 0;
                    bError = D_QI_ID_CONF_ERROR_C;
                }
                break;
            case D_ID_CONF_Ext_Identification_Packet:
                if( D_QI_ExtendedIdentification == bDecode[0] )   //Should Re
                {
                    bx_QI_Extended_Identification[0] = bDecode[1];
                    bx_QI_Extended_Identification[1] = bDecode[2];
                    bx_QI_Extended_Identification[2] = bDecode[3];
                    bx_QI_Extended_Identification[3] = bDecode[4];
                    bx_QI_Extended_Identification[4] = bDecode[5];
                    bx_QI_Extended_Identification[5] = bDecode[6];
                    bx_QI_Extended_Identification[6] = bDecode[7];
                    bx_QI_Extended_Identification[7] = bDecode[8];
                    b_ID_CONF_Packet_Index = D_ID_CONF_Optional_Configuration_Packet1;
                }
                else
                {
                    bErrorTerminateCounter = 0;
                    bError = D_QI_ID_CONF_ERROR_C;
                }
                break;
            case D_ID_CONF_Optional_Configuration_Packet1:
            case D_ID_CONF_Optional_Configuration_Packet2:
            case D_ID_CONF_Optional_Configuration_Packet3:
            case D_ID_CONF_Optional_Configuration_Packet4:
            case D_ID_CONF_Optional_Configuration_Packet5:
            case D_ID_CONF_Optional_Configuration_Packet6:
            case D_ID_CONF_Optional_Configuration_Packet7:
            case D_ID_CONF_Configuration_Packet:
                switch(bDecode[0])
                {
                case D_QI_PowerControlHold_off:
                    if( bDecode[1] < D_QI_Control_Hold_off_Time_min  ||
                            bDecode[1] > D_QI_Control_Hold_off_Time_max )
                    {
                        bErrorTerminateCounter = 0;
                        bError = D_QI_ID_CONF_ERROR_F;
                    }
                    else
                    {
                        b_QI_PowerHoldOff_Time = bDecode[1];
                        b_ID_CONF_private_setting_counter++;
                    }
                    break;
                case D_QI_Proprietary_0x18:
                case D_QI_Proprietary_0x19:
                case D_QI_Proprietary_0x28:
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
                    b_ID_CONF_private_setting_counter++;
                    break;
                case D_QI_Configuration:
                    bx_QI_Configuration[0] = bDecode[1];
                    bx_QI_Configuration[1] = bDecode[2];
                    bx_QI_Configuration[2] = bDecode[3];
                    bx_QI_Configuration[3] = bDecode[4];
                    bx_QI_Configuration[4] = bDecode[5];

                    if( b_ID_CONF_private_setting_counter - (bx_QI_Configuration[2] & 0x07 ) ==0)
                    {
                        //Finally here.
                        fgChangePhase = 1;
                        bPhaseBefore = bPhase;
                        bPhase = POWERTRANSFER;
                    }
                    else
                    {
                        bErrorTerminateCounter = 0;
                        bError = D_QI_ID_CONF_ERROR_C;
                    }
                    fgPrintf = 5;
                    break;
                default:
                    bErrorTerminateCounter = 0;
                    bError = D_QI_ID_CONF_ERROR_C;
                    break;
                }
                b_ID_CONF_Packet_Index++;
                break;
            default:
                bErrorTerminateCounter = 0;
                bError = D_QI_ID_CONF_ERROR_C;
                break;
            }
            fgPacketStop = 0;
            b_GeneralTime_Status = D_GeneralTime_No_Signal;
            bPacketErrorCounter = 0;
        }
        if ( fgPacketCheckSumError )
        {
            bErrorTerminateCounter = 0;
            bError = D_QI_ID_CONF_ERROR_C;
            fgPacketCheckSumError = 0;
        }
    }
}
