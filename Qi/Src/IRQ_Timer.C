#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"


/***********************************
// Check the Catch Data Code
************************************/
void F_Cap2_Assembly_Catch_Code()
{
    uint8_t bi;
    if( ! fgCap2Capture)
    {

        if( fgCap2HalfClk )
            fgCap2EndHalfBYTE = 1;
        bCap2PacketStutureIndex = 0;
        fgCap2HalfClk = 0;
        fgCap2SignalStatus =0;
    }
//    bCap2BackupbBitBufferIndex = bCap2BitBufferIndex;
//    bCap2BackupbBitBuffer = bCap2BitBuffer;
//    bCap2BitBufferIndex = 0;
//    bCap2BitBuffer = 0;
//    fgCap2ModifyLastBit = 0;
    if( !fgCap2SignalStatusBefore && fgCap2SignalStatus)
    {
        fgCap2PacketStart = 1;
        fgPacketStart = 1;
        fgCap2EndHalfBYTE = 0;
    }
    if( fgCap2SignalStatusBefore && !fgCap2SignalStatus )
    {
        //	if( fgCap2EndHalfBYTE )
        //	{
//		    bCap2BackupbBitBufferIndex++;
//		    bCap2BackupbBitBuffer = bCap2BackupbBitBuffer<<1 |1;
        //	}
         Capturebit0_intdly=220;
        bCap2ReceiveCodeEnd = 2;
//        memcpy(bDecode2,bCaptureData2,bCaptureDataIndex2);
        for(bi=0; bi<=bCaptureDataIndex2; bi++)
            bDecode2[bi]=bCaptureData2[bi];
        bDecodeIndex2=bCaptureDataIndex2;
		printf("Cap2 0x%x, 0x%x, 0x%x, 0x%x, 0x%x , 0x%x\n\r", (int16_t) bDecode2[0], (int16_t)bDecode2[1], (int16_t)bDecode2[2],	(int16_t)bDecode2[3], (int16_t)bDecode2[4], (int16_t)bDecode2[5]);	 //" %i, %d, %d, %x, %d, %d"(int16_t) wPrintdata2[0] , wPrintdata2[1] , wPrintdata2[2] ,  wPrintdata2[3] , wPrintdata2[4], wPrintdata2[5]

   //     F_CaptureData2_Clear();
//        bBitoneCNT2=0;
//        for(bCaptureDataIndex2=0; bCaptureDataIndex2<D_CAP_BUFF; bCaptureDataIndex2++)
//            bCaptureData2[bCaptureDataIndex2]=0;
//        bCaptureDataIndex2=0;
//        bBitIndex2=0;
    }
    fgCap2SignalStatusBefore = fgCap2SignalStatus;
    fgCap2Capture = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
void F_Cap0_Assembly_Catch_Code()
{
    uint8_t bi;
    if( ! fgCap0Capture)
    {

        if( fgCap0HalfClk )
            fgCap0EndHalfBYTE = 1;
        bCap0PacketStutureIndex = 0;
        fgCap0HalfClk = 0;
        fgCap0SignalStatus =0;
    }
//    bCap0BackupbBitBufferIndex = bCap0BitBufferIndex;
//    bCap0BackupbBitBuffer = bCap0BitBuffer;
//    bCap0BitBufferIndex = 0;
//    bCap0BitBuffer = 0;
//    fgCap0ModifyLastBit = 0;
    if( !fgCap0SignalStatusBefore && fgCap0SignalStatus)
    {
        fgCap0PacketStart = 1;
        fgPacketStart = 1;
        fgCap0EndHalfBYTE = 0;
    }
    if( fgCap0SignalStatusBefore && !fgCap0SignalStatus )
    {
        //	if( fgCap0EndHalfBYTE )
        //	{
//		    bCap0BackupbBitBufferIndex++;
//		    bCap0BackupbBitBuffer = bCap0BackupbBitBuffer<<1 |1;
        //	}
         Capturebit0_intdly=220;
        bCap0ReceiveCodeEnd = 2;
//        memcpy(bDecode0,bCaptureData0,bCaptureDataIndex0);
        for(bi=0; bi<=bCaptureDataIndex0; bi++)
            bDecode0[bi]=bCaptureData0[bi];
        bDecodeIndex0=bCaptureDataIndex0;
		printf("Cap0 0x%x, 0x%x, 0x%x, 0x%x, 0x%x , 0x%x\n\r", (int16_t) bDecode0[0], (int16_t)bDecode0[1], (int16_t)bDecode0[2],	(int16_t)bDecode0[3], (int16_t)bDecode0[4], (int16_t)bDecode0[5]);	 //" %i, %d, %d, %x, %d, %d"(int16_t) wPrintdata2[0] , wPrintdata2[1] , wPrintdata2[2] ,  wPrintdata2[3] , wPrintdata2[4], wPrintdata2[5]

    //    F_CaptureData0_Clear();

//        bBitoneCNT0=0;
//        for(bCaptureDataIndex0=0; bCaptureDataIndex0<D_CAP_BUFF; bCaptureDataIndex0++)
//            bCaptureData0[bCaptureDataIndex0]=0;
//        bCaptureDataIndex0=0;
//        bBitIndex0=0;


    }
    fgCap0SignalStatusBefore = fgCap0SignalStatus;
    fgCap0Capture = 0;
}




uint8_t F_Packet_CheckSum(uint8_t  * bCapDecode, uint8_t  bCapRowCount)
{
    uint8_t bCheckSumError=D_CheckPacketContent_OK,bCalculateCheckSum,bi,bj;
    uint8_t fgPacketError=0;
    switch(bCapDecode[0])
    {
    case D_QI_SignalStrength:               //Signal Strength Packet;
    case D_QI_EdnPowerTransfer:               //End Power Transfer Packet;
    case D_QI_ControlError:               //Control Error Packet;
    case D_QI_RectifiedPower:               //Retified Power Packet
    case D_QI_ChargeStatus:               //Charge Status Packet
    case D_QI_PowerControlHold_off:               //Power Control Hold-off Packet
    case D_QI_Proprietary_0x18:
    case D_QI_Proprietary_0x19:
    case D_QI_GeneralRequest:
    case D_QI_Renegotiate:
        bj = 2;
        break;
    case D_QI_Configuration:              //Configuration Packet
    case D_QI_Proprietary_0x58:
        bj = 6;
        break;
    case D_QI_Identification:              //Identification Packet
    case D_QI_Proprietary_0x78:
        bj = 8;
        break;
    case D_QI_ExtendedIdentification:              //Extended Identification Packet
    case D_QI_Proprietary_0x84:
        bj = 9;
        break;
    case D_QI_Proprietary_0x28:
    case D_QI_Proprietary_0x29:
    case D_QI_SpecificRequest:
    case D_QI_FODStatus:
        bj = 3;
        break;
    case D_QI_Proprietary_0x38:
    case D_QI_24bitReceiverPower:
        bj = 4;
        break;
    case D_QI_Proprietary_0x48:
        bj = 5;
        break;
    case D_QI_Proprietary_0x68:
        bj = 7; //bj = 8;     //20131015 Joson
        break;
    case D_QI_Proprietary_0xA4:
        bj = 13;
        break;
    case D_QI_Proprietary_0xC4:
        bj = 17;
        break;
    case D_QI_Proprietary_0xE2:
        bj = 21;
        break;
    case D_QI_Proprietary_0xF2:
        bj = 25;
        break;
    default:
        fgPacketError = 1;
        break;
    }
    if( fgPacketError)
        bCheckSumError = D_CheckPacketContent_nothispacket;
    else
    {
        bCalculateCheckSum = bCapDecode[0]^ bCapDecode[1];
        if( bCapRowCount >= bj )
        {
            if(bj>=2)
            {
                if(bj == 2);
                else
                {
                    for(bi = 2; bi < bj ; bi++)
                        bCalculateCheckSum = bCalculateCheckSum^bCapDecode[bi];
                }

                if(bCapDecode[bj] == bCalculateCheckSum);
                else
                    bCheckSumError = D_CheckPacketContent_CheckSumError;      //Check Sum Fail
            }
            else
            {
                // while(1); //Code Error
            }
        }
        else
        {
            bCheckSumError = D_CheckPacketContent_ReceiveError;      //Packet byte count error
        }
    }
    return bCheckSumError;
}

/***********************************
// Timer5 Capture Interrupt
************************************/
void F_Cap2_Receiver_Code()
{
    uint8_t bj,bk;
    if( bCap2ReceiveCodeEnd == 2)   // Receive Code Finished
    {
        bCap2ReceiveCodeEnd--;
        bk = D_CheckPacketContent_Decode_OK;
        if( D_CheckPacketContent_Decode_OK == bk )
        {
            bj = F_Packet_CheckSum(bDecode2, bDecodeIndex2);
            if(bj==D_CheckPacketContent_OK  )
            {
                fgCap2PacketStop = 1;
            }
            else if( D_CheckPacketContent_CheckSumError == bj )
            {
                fgPacketCheckSumError = 1;
            }
            else
            {
                fgCap2PacketStop = 0;
				if(bDecode2[0]==0x3||bDecode2[0]==0x1||bDecode2[0]==0x51||bDecode2[0]==0x71)
				 {
				    fgCap0PacketStop=1;
					
				}
            }
        }
        wPOWERTRANSFER_COMPATIABLE_COUNT=0;

    }
    else if( bCap2ReceiveCodeEnd == 1)
    {
        bCap2ReceiveCodeEnd--;

  //      F_CaptureData2_Clear();
//        //************************************
//        bBitoneCNT2=0;
//        for(bCaptureDataIndex2=0; bCaptureDataIndex2<D_CAP_BUFF; bCaptureDataIndex2++)
//        {
//            bCaptureData2[bCaptureDataIndex2]=0;
//        }
//        bCaptureDataIndex2=0;
//        bBitIndex2=0;
//        //************************************
    }
    else;
}

/************************************/
void F_Cap0_Receiver_Code()
{
    uint8_t bj,bk;
    if( bCap0ReceiveCodeEnd == 2)   // Receive Code Finished
    {
        bCap0ReceiveCodeEnd--;
        bk = D_CheckPacketContent_Decode_OK;
        if( D_CheckPacketContent_Decode_OK == bk )
        {
            bj = F_Packet_CheckSum(bDecode0, bDecodeIndex0);

            if(bj==D_CheckPacketContent_OK  )
            {
                fgCap0PacketStop = 1;
            }
            else if( D_CheckPacketContent_CheckSumError == bj )
            {
                fgPacketCheckSumError = 1;
            }
            else
				
            {

			   
                fgCap0PacketStop = 0;
				if(bDecode0[0]==0x3||bDecode0[0]==0x1||bDecode0[0]==0x51||bDecode0[0]==0x71)
				 {
				    fgCap0PacketStop=1;
					
				}
            }
        }
        wPOWERTRANSFER_COMPATIABLE_COUNT=0;

    }
    else if( bCap0ReceiveCodeEnd == 1)
    {
        bCap0ReceiveCodeEnd--;

    //    F_CaptureData0_Clear();
//        //************************************
//        bBitoneCNT0=0;
//        for(bCaptureDataIndex0=0; bCaptureDataIndex0<D_CAP_BUFF; bCaptureDataIndex0++)
//        {
//            bCaptureData0[bCaptureDataIndex0]=0;
//        }
//        bCaptureDataIndex0=0;
//        bBitIndex0=0;
//        //************************************
    }
    else;
}

void F_PacketDecode_Handler(void)
{   uint8_t  bm;
    F_Cap2_Receiver_Code();
    F_Cap0_Receiver_Code();
    if(bPacketReceiveDebounce==0)
    {
        if( fgCap2PacketStop )
        {
            bPacketReceiveDebounce = D_PacketReceiveDebounceTime;
            for(bm=0; bm<=bDecodeIndex2; bm++)
                bDecode[bm]=bDecode2[bm];
            bDecodeIndex=bDecodeIndex2;

            fgPacketStop = 1;
            fgCap2PacketStop = 0;
            wCap2CNT=0;
        }
        if( fgCap0PacketStop )
        {
            bPacketReceiveDebounce = D_PacketReceiveDebounceTime;
            for(bm=0; bm<=bDecodeIndex0; bm++)
                bDecode[bm]=bDecode0[bm];
            bDecodeIndex=bDecodeIndex0;
            fgPacketStop = 1;
            fgCap0PacketStop = 0;
            wCap0CNT=0;
        }


    }

    else
    {
        bPacketReceiveDebounce --;
        fgCap2PacketStop = 0;
        fgCap0PacketStop = 0;
    }
}


/***********************************
// Timer0 Interrupt
************************************/

//void Timer0_ISR(void)    interrupt 1  //interrupt address is 0x000B
//{
//
//
//	   if( fsendstart==1 || fsendstart1==1 )
//     {
//			 TH0 = D_T_50uS_H;
//			 TL0 = D_T_50uS_L;
//		   wsendTimecount++;
//		 }
//    else
//		{
//  	    TH0 = D_T_1MS_H;
//	 	    TL0 = D_T_1MS_L;
//				F_Cap2_Assembly_Catch_Code();
//	      F_Cap0_Assembly_Catch_Code();
//					 if( bPhase == SELECTION )
//						F_QI_Selection_Phase_Manage();
//				else if( bPhase == PING )
//						F_QI_Ping_Phase_Manage();
//				else if( bPhase == ID_CONF )
//						F_QI_ID_CONF_Phase_Manage();
//				else
//						F_POWERTRANSFER_Phase_Manage();
//
//				F_QI_Change_Phase_Judgement();
//				F_PacketDecode_Handler();
//				F_LED_Handler();
//				F_Measure_Real_Current();
//         wCap2CNT++;
//         wCap0CNT++;
//			}
//
//}


