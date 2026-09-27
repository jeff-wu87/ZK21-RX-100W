#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"

uint16_t wCapdata[100];
uint8_t  cdi=0;

//extern uint16_t wCapTimeCnt;

#if 1
/***********************************
// Check Packet Function using Register Bank 1
************************************/
void Capturebit2(void) //using RegBank_2
{
    uint16_t wCopy ;

//    //if(ACMP_GetIntFlag(ACMP1)==0)//CAPCON0&SET_BIT0)//
//    if((CNIF & (1<< ACMP1))==0)//CAPCON0&SET_BIT0)//
//        return;

//    //ACMP_ClearIntFlag(ACMP1);
//    CNIF &= ~(1<< ACMP1);
//    // return;


//    //TMR2_Stop();
//    T2CON &= ~(TMR_T2CON_T2In_Msk);
//    wCap2BufBackup = TL2+(u16)(TH2<<8);
//    //TMR2_Start();
//    if(0x0 == (T2CON & TMR_T2CON_T2In_Msk))
//    {
//        T2CON |= (0x1 <<TMR_T2CON_T2In_Pos);
//    }

//    TR1 = 0;
//    //wCap2BufBackup =  wCapTimeCnt;//(WORD)(C0L)+ (WORD)(C0H<<8);//CAP0;  //P01 电压解码 CAP0;
//    wCap2BufBackup =  (uint16_t)(TL1)+ (uint16_t)(TH1<<8);//CAP0;  //P01 电压解码 CAP0;
//    TR1 = 1;


    TMR_Stop(TMR1);
    wCap2BufBackup = TMR_GetTimingData(TMR1);
    TMR_Start(TMR1);

//    if( wCap2Before > wCap2BufBackup )
//        wCopy = (uint16_t)(((uint16_t)0xFFFF - wCap2Before) +wCap2BufBackup);
//    else
//        wCopy = wCap2BufBackup - wCap2Before;


    if( wCap2Before < wCap2BufBackup )
        wCopy = (uint16_t)(((uint16_t)0xFFFF + wCap2Before) - wCap2BufBackup);
    else
        wCopy = wCap2Before- wCap2BufBackup;

    wCap2Before = wCap2BufBackup;


//     wCapdata[cdi]=wCopy;
//      cdi++;
//     if(cdi==30)
//      cdi=0;

    if( wCopy > D_T_500us_Max)
    {
        fgClear = 1;
        bCap2PreambleCounter = 0;
    }
    else if( wCopy > D_T_250us_500us_Middle )
    {
        fgCap2Capture = 1;  //Capture flag
        if( bCap2PacketStutureIndex == 1)
        {
            if( bCap2PreambleCounter >=12 && bCap2PreambleCounter < 50)
            {
                bCap2PacketStutureIndex =2;     //First Start
//                bCap2BitBufferIndex++;          //Record first bit
                // bCap2BitBuffer = bCap2BitBuffer<<1;
                fgCap2SignalStatus = 1;
                fgClear = 0;
                // iCap2CaptureTail = (int_least32_t)wCopy - D_T_500us;
                //fgCap2Tail = 255;
            }
            else
            {
                fgClear = 1;
            }
        }
        else if( bCap2PacketStutureIndex )  //Catch Data here , Header , Message , Check sum
        {
            //Catch Zero
//            bCap2BitBufferIndex++;
            // bCap2BitBuffer = bCap2BitBuffer<<1;
            fgClear = 0;
            //****************************************************************************************************
            //if(bBitIndex==9 && (bBitoneCNT&0x01)==0) fgClear = 255;
            if(bBitIndex2==10)
            {
                bCaptureDataIndex2++;
                bBitIndex2=0;
                bBitoneCNT2=0;
            }
            else
                bBitIndex2++;

            //****************************************************************************************************
        }
        else
        {
            fgClear = 1;
        }
        if( fgCap2HalfClk )     //Zero should aligned with Last one. fgCap2HalfClk never be 1 at zero catch.
        {
            fgClear = 1;
        }
        else
            fgClear = 0;

        bCap2PreambleCounter = 0;
    }
    else if( wCopy > D_T_250us_Min )
    {
        fgCap2Capture = 1;  //Capture flag
        // if( fgCap2Tail )
        // iCap2CaptureTail = (int_least32_t)wCopy - (int_least32_t)wCap2Target250us;
        if( bCap2PacketStutureIndex==1)
        {
//            bCap2PeriodIndex+= 32;
            bCap2PreambleCounter++;
            if(bCap2PreambleCounter>=15)
            {
                wPOWERTRANSFER_COMPATIABLE_COUNT=0;
            }
        }
        else if( bCap2PacketStutureIndex >0)
        {
            if( fgCap2HalfClk )
            {
//                bCap2BitBufferIndex++;
                // bCap2BitBuffer = bCap2BitBuffer<<1 | 0x01;
                //Catch 1
                fgCap2HalfClk = 0;
                //iCap2CaptureTail = 0;
                //******************************************************
                bBitoneCNT2++;
                //if(bBitIndex==9 && (bBitoneCNT&0x01)==1)fgClear=255;
                bCaptureData2[bCaptureDataIndex2] |= (1<<bBitIndex2);
                bBitIndex2++;
                //******************************************************

            }
            else
            {
                fgCap2HalfClk = 1;
            }
        }
        if( bCap2PacketStutureIndex==0)
        {
            bCap2PacketStutureIndex = 1;
        }
        fgClear = 0;
    }
    else
    {
        fgClear = 1;
    }
    if( fgClear)
    {
        if( fgCap2HalfClk )
            fgCap2EndHalfBYTE = 1;

        bCap2PacketStutureIndex = 0;
        fgCap2HalfClk = 0;
        fgCap2SignalStatus = 0;
        // fgCap2Tail = 0;
        //************************************
        bBitoneCNT2=0;
        //************************************
    }
//--------------------------------------------------------------
}

void Capturebit0(void) //using RegBank_2
{
    uint16_t wCopy ;

//    //if(ACMP_GetIntFlag(ACMP0)==0)//CAPCON0&SET_BIT0)//
//    if((CNIF & (1<< ACMP0))==0)//CAPCON0&SET_BIT0)//
//        return;

//    //ACMP_ClearIntFlag(ACMP0);
//    CNIF &= ~(1<< ACMP0);
//     //return;

//    //TMR2_Stop();
//    T2CON &= ~(TMR_T2CON_T2In_Msk);
//    wCap0BufBackup = TL2+(u16)(TH2<<8);
//    //TMR2_Start();
//    if(0x0 == (T2CON & TMR_T2CON_T2In_Msk))
//    {
//        T2CON |= (0x1 <<TMR_T2CON_T2In_Pos);
//    }

//    //P06=~P06;
//    TR1 = 0;
//    //wCap0BufBackup =wCapTimeCnt;//(WORD)(C2L)+ (WORD)(C2H<<8);//CAP0;  //P01 电压解码 CAP0;
//    wCap0BufBackup =  (uint16_t)(TL1)+ (uint16_t)(TH1<<8);//CAP0;  //P01 电压解码 CAP0;
//    TR1 = 1;

    TMR_Stop(TMR1);
    wCap0BufBackup = TMR_GetTimingData(TMR1);
    TMR_Start(TMR1);

//    if( wCap0Before > wCap0BufBackup )
//        wCopy = (uint16_t)(((uint16_t)0xFFFF - wCap0Before) +wCap0BufBackup);
//    else
//        wCopy = wCap0BufBackup - wCap0Before;

    if( wCap0Before < wCap0BufBackup )
        wCopy = (uint16_t)(((uint16_t)0xFFFF + wCap0Before) - wCap0BufBackup);
    else
        wCopy = wCap0Before- wCap0BufBackup;

    wCap0Before = wCap0BufBackup;


    wCapdata[cdi]=wCopy;
    cdi++;
    if(cdi==60)
        cdi=0;

    if( wCopy > D_T_500us_Max)
    {
        fgClear = 1;
        bCap0PreambleCounter = 0;
    }
    else if( wCopy > D_T_250us_500us_Middle )
    {
        fgCap0Capture = 1;  //Capture flag
        if( bCap0PacketStutureIndex == 1)
        {
            if( bCap0PreambleCounter >=12 && bCap0PreambleCounter < 50)
            {
                bCap0PacketStutureIndex =2;     //First Start
//                bCap0BitBufferIndex++;          //Record first bit
                // bCap0BitBuffer = bCap0BitBuffer<<1;
                fgCap0SignalStatus = 1;
                fgClear = 0;
                // iCap0CaptureTail = (int_least32_t)wCopy - D_T_500us;
                //fgCap0Tail = 255;
            }
            else
            {
                fgClear = 1;
            }
        }
        else if( bCap0PacketStutureIndex )  //Catch Data here , Header , Message , Check sum
        {
            //Catch Zero
//            bCap0BitBufferIndex++;
            // bCap0BitBuffer = bCap0BitBuffer<<1;
            fgClear = 0;
            //****************************************************************************************************
            //if(bBitIndex==9 && (bBitoneCNT&0x01)==0) fgClear = 255;
            if(bBitIndex0==10)
            {
                bCaptureDataIndex0++;
                bBitIndex0=0;
                bBitoneCNT0=0;
            }
            else
                bBitIndex0++;
            //****************************************************************************************************
        }
        else
        {
            fgClear = 1;
        }
        if( fgCap0HalfClk )     //Zero should aligned with Last one. fgCap0HalfClk never be 1 at zero catch.
        {
            fgClear = 1;
        }
        else
            fgClear = 0;

        bCap0PreambleCounter = 0;
    }
    else if( wCopy > D_T_250us_Min )
    {
        fgCap0Capture = 1;  //Capture flag
        // if( fgCap0Tail )
        // iCap0CaptureTail = (int_least32_t)wCopy - (int_least32_t)wCap0Target250us;
        if( bCap0PacketStutureIndex==1)
        {
//            bCap0PeriodIndex+= 32;
            bCap0PreambleCounter++;
            if(bCap0PreambleCounter>=15)
            {
                wPOWERTRANSFER_COMPATIABLE_COUNT=0;
            }
        }
        else if( bCap0PacketStutureIndex >0)
        {
            if( fgCap0HalfClk )
            {
//                bCap0BitBufferIndex++;
                // bCap0BitBuffer = bCap0BitBuffer<<1 | 0x01;
                //Catch 1
                fgCap0HalfClk = 0;
                //iCap0CaptureTail = 0;
                //******************************************************
                bBitoneCNT0++;
                //if(bBitIndex==9 && (bBitoneCNT&0x01)==1)fgClear=255;
                bCaptureData0[bCaptureDataIndex0] |= (1<<bBitIndex0);
                bBitIndex0++;
                //******************************************************
            }
            else
            {
                fgCap0HalfClk = 1;
            }
        }
        if( bCap0PacketStutureIndex==0)
        {
            bCap0PacketStutureIndex = 1;
        }
        fgClear = 0;
    }
    else
    {
        fgClear = 1;
    }
    if( fgClear)
    {
        cdi=0;
        if( fgCap0HalfClk )
            fgCap0EndHalfBYTE = 1;

        bCap0PacketStutureIndex = 0;
        fgCap0HalfClk = 0;
        fgCap0SignalStatus = 0;
        // fgCap0Tail = 0;
        //************************************
        bBitoneCNT0=0;

        //************************************
    }
//--------------------------------------------------------------
}



//void CAP_INT(void )  using RegBank_2  interrupt 12
//{
//
//    if(CAPCON0 & SET_BIT0) //P01 电压解码
//    {
//
//		    Capturebit2();
//				clr_CAPF0;
//    }
//		// CAPCON0 &= ~SET_BIT6; //关掉电流解码
//
//    if(CAPCON0 & SET_BIT2)          //fgCap2Flag = 1;  //wCap2BufBackup =(WORD)(C2L)+ (WORD)(C2H<<8);//CAP0;    ;//P15 电流解码 CAP2;
//    {
//			  Capturebit0();
//				clr_CAPF2;
//    }
//
//
//
//
//
//}

#endif

