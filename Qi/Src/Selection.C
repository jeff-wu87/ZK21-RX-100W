#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"

extern uint16_t FODcounter;

/***********************************
// SELECTION Phase Manage using Register Bank 1
************************************/
void F_QI_Selection_Phase_Manage()
{
//    uint8_t bi,bjj, bj = 10;
    fgCompatiableMode=0;
    wObjectDetectionIntervalCounter++;
    if( wObjectDetectionIntervalCounter > D_QI_todi )
    {
        //P22=0;
//        for(bi=0; bi<50; bi++)
//            for(bjj=0; bjj<226; bjj++)
//                // P22=~P22;
//                // P22=0;
//                ;


        // PWM_ConfigOutputChannel(PWM, 0, Feq_QI_Fop_INI_Code, 50);
        // PWM_ConfigOutputChannel(PWM, 1, Feq_QI_Fop_INI_Code, 50);
        // PWM_EnableOutput(PWM, 0x03);

//        for(bi=0; bi<5; bi++)
//            for(bj=0; bj<200; bj++);
        // PWM_DisableOutput(PWM, 0x03);
        
        fgPrint = 1;
        wPrintdata2[0] = (uint16_t)0x7f;
        wPrintdata2[1] = (uint16_t)123;//wDectect_Current_Threshold;
        wPrintdata2[2] = (uint16_t)wRC_Low_Pass_Current;
        wPrintdata2[3] = (uint16_t)wAC_Low_Pass_Voltage;

        bPhaseBefore = bPhase;
        bPhase = PING;
        fgChangePhase = 1;
        bTargetStatus = D_Target_Status_None;

        wObjectDetectionIntervalCounter = 0;
        fgPacketCheckSumError = 0;
		find36wflg=0;

    }
}
