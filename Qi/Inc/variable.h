
#ifndef __VARIABLE_H__
#define __VARIABLE_H__
#include "constant.h"
#include <stdint.h>
typedef unsigned char   bit;
//============================================================= 
//	Wireless Charger Transmitter Communication Function define.
//=============================================================
bit BIT_TMP;
uint8_t  bPhase,bPhaseBefore;      //0: Selection  1: Ping 2: Identification & configuration   3: Power transfer   ==P35==
uint8_t   b_QI_Signal_Strength_Value;   //Signal strength value which was got at Ping Phase. This is used for free positon to show the best postion. ==P55==
uint8_t  b_GeneralTime_Status;        // 0: no Signal Status 1: Signal Status
uint8_t  bErrorTerminateCounter;
//QI Phase variable
bit  fgChangePhase;
bit  fgHeat;

uint8_t  bVddOverCnt=0;

//uint16_t Volt25_2vfull  = (6*256+96-41);
uint16_t Volt25_2vfull  =  1583;
uint16_t Voltchgfull  =  1349;


uint16_t Current_1Afull  = 80;


uint8_t  bIdleFodCnt;
uint8_t  bError,bPacketErrorCounter;
uint8_t  bx_QI_Identification[7];             //Identification content        ==58==
uint8_t  bx_QI_Configuration[5];              //Configuration content        ==P59==
uint8_t  bx_QI_Extended_Identification[8];    //extended Identification content ==P60==
uint8_t  b_QI_PowerHoldOff_Time;  //Power Hold off time setting value ==P41==
int8_t  i_QI_Error;
uint8_t  b_QI_Retified_Power;
uint8_t  b_QI_Retified_Power_Before;
uint8_t  b_QI_Charge_Status;
uint8_t  bTargetStatus;
uint16_t  wColdTimeCounter;
uint8_t  bsumsung10WMOD;
//uint8_t fgCap2Flag;
uint16_t wCap2BufBackup,wCap0BufBackup;
uint16_t wCap2Before,wCap0Before;

uint16_t   wCap2CNT;
uint16_t   wCap0CNT;
///============================================================= 
//	Wireless Charger Transmitter variables
//=============================================================
//Decode Variable at catch Interrupt
bit  fgCap2Capture, fgCap2SignalStatus,fgCap2SignalStatusBefore,fgCap0Capture, fgCap0SignalStatus,fgCap0SignalStatusBefore;         
bit  fgPacketStart , fgPacketStop , fgCap2PacketStart,fgCap2PacketStop,fgCap0PacketStart,fgCap0PacketStop;
bit  fgPacketCheckSumError ;
bit  fgPrint;
uint16_t wPrintdata2[6];
uint8_t  bPrinti=0;
//uint8_t bCap2BackupbBitBufferIndex,bCap2BackupbBitBuffer ;
//uint8_t bObjectDebounce;
uint8_t bCaptureData2[D_CAP_BUFF]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t bCaptureDataIndex2=0,bBitIndex2=0;
uint8_t bBitoneCNT2=0;
uint8_t bCaptureData0[D_CAP_BUFF]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t bCaptureDataIndex0=0,bBitIndex0=0;
uint8_t bBitoneCNT0=0;
//int_least32_t iCap2CaptureTail ;
//uint32_t wCap2Target500us, wCap2Target250us,wCap2Target500us, wCap2Target250us ;
//uint8_t fgCap2Tail ;
//uint8_t fgCap2ModifyLastBit ;
//uint8_t bCap2PeriodIndex ;
//Decode Variable at Timer0 catch
uint8_t bCap2PacketStutureIndex,bCap0PacketStutureIndex ;  //1: Preamble   2:Header    3:Message   4:CheckSum
bit fgCap2HalfClk,fgCap0HalfClk ;
uint8_t bCap2ReceiveCodeEnd,bCap0ReceiveCodeEnd ;
//uint8_t bCap2BitBuffer,bCap2BitBufferIndex ;
uint8_t bCap2PreambleCounter,bCap0PreambleCounter ;
//uint8_t bCatchCodeIndex;
//Decode Variable with parity
//uint8_t bCap2Decode_Row_ByteIndex; //,bCap2Decode_Row_Index;
uint8_t bDecodeIndex;
uint8_t bDecodeIndex0;
uint8_t bDecodeIndex2;

uint16_t wAC_Zero_Current_Code;
bit fgCap2EndHalfBYTE,fgCap0EndHalfBYTE ;
//Decode Variable with CheckSum
uint8_t bDecode[D_CAP_BUFF] ;
uint8_t bDecode0[D_CAP_BUFF] ;
uint8_t bDecode2[D_CAP_BUFF] ;
uint8_t bPacketReceiveDebounce;
bit fgClear;
//Current
uint8_t  bCurrentCounter;
uint16_t  wRC_Low_Pass_Current;
uint16_t  wRC_Current=0;
uint16_t  wAC_Low_Pass_Voltage;
uint16_t  w_QI_Retified_Power_Limit;
//uint8_t  bOverCurrentCounter;
//uint16_t  wVirtualWorkCounter;
//uint32_t  dwProtectCurrent;
//SELECTION Phase Variable
uint16_t  wObjectDetectionIntervalCounter;
uint16_t  wADC_Backup;
//ID CONF Phase Variable
//uint8_t  b_ID_CONF_Timer_Status;  // 0: D_ID_CONF_Status_tnext 1: D_ID_CONF_Status_tmax 2: D_ID_CONF_Status_tterminate 3: D_ID_CONF_Status_expire
uint8_t  b_ID_CONF_Packet_Index;  //
int16_t  b_ID_CONF_private_setting_counter;
//uint16_t  w_Object_Frequency_max , w_Object_Frequency_min;
// send data
uint16_t wStart9VCounter=0;
uint16_t wStart9VCounter1=0;
uint8_t fFastcharge=0;
uint32_t bCRCLbackup1=0;
uint16_t bCRCLbackup=0;
uint32_t bCRCLbackup2=0;
uint8_t fFrequencyshift=0;
uint8_t firstinchargeFrequency=0;
uint8_t fmidlepower=0;
uint8_t fboostup=0;
uint16_t wsendTimecount=0;
uint8_t  bByteindexsend=0;
uint8_t bBitindexsend=0;
uint16_t bTimebase=0;
uint8_t fsendstart=0;
uint8_t fsendstart1=0;
uint8_t fsendbit=0;
uint8_t fhalfbitsend=0;
uint8_t  bByteindexsendCNT;
uint8_t  bBitindexsendCNT;
uint8_t  fgPacketStartbitsend;
uint8_t  fsendparity;
uint8_t fSendNeg; 
uint8_t fSendPolarity; 
uint8_t fSendDepth;
uint8_t f15Wstartup=0;
uint8_t   bDatatosend[6]={0x00,0x00,0x00,0x00,0x00,0x00};  
uint8_t const bSamsung10WData[2]={0x02,0x01};   //0010 0000 0110 1000 0000 111 //0x04,0x16,0x70  (10W)// 0100 0000 1000 0000  0x02 0x01 
uint8_t const bTX_ACK_Data[1]={0xFF};
uint8_t const bTX_NAK_Data[1]={0x00};
uint8_t const bTX_ND_Data[1]={0x55};
uint8_t const bTX_Identification_Data[5]={0x30,0x12,0x00,0x42,0x60};
uint8_t const bTX_Capability_Data[5]={0x31,0x1E,0x1E,0x00,0x31};
//1  TX->RX  :  0xFF
//2  RX->TX  :  0x22,0x00,0x6B,0x49
//3  TX->RX  :  0xFF
//4  RX->TX  :  0x07,0x30,0x37
//5  TX->RX  :  0x30,0x12,0x00,0x42,0x60
//6  RX->TX  :  0x07,0x31,0x36

//7  TX->RX  :  0x31,0x1E,0x1E,0x00,0x31//0x31,0x0A,0x1E,0x00,0x25
//8  RX->TX  :  0x20,0x02,0x31,0x13
//9  TX->RX  :  0xFF
//10 RX->TX  :  0x20,0x01,0x1E,0x3F
//11 TX->RX  :  0xFF
//12 RX->TX  :  0x20,0x04,0x1E,0x3A
//13 TX->RX  :  0xFF
//14 RX->TX  :  0x20,0x00,0x03,0x23
//15 TX->RX  :  0xFF
//16 RX->TX  :  0x03,0x37,0x34
//Power Transfer Variable
uint16_t  wPOWERTRANSFER_Error_Packet_Timeout_Counter;
uint16_t  wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter;
int32_t  lPOWERTRANSFER_COMPATIABLE_DELAY;
uint16_t  wPOWERTRANSFER_COMPATIABLE_COUNT;
uint8_t  bReconfigCounter;
uint8_t  bControlErrorCNTsend=0;
uint16_t wChargeCompleteCNT;
uint8_t  bChargeCompleteCNT;
bit  fgCompatiableMode=0;
uint8_t  bPOWERTRANSFER_Error_Control_Time_Counter;
bit  fgEndThisControl;
bit  fgErrorControl;
//uint8_t  fgLearningFrequncy;
bit  fgLimitFreBoundary;
//uint8_t  fgCompatiableNokia;
//uint8_t  fgControlagain;
//uint8_t  fgCurrentControlByProduct;
uint8_t  b_PP_ControlStatus;
//uint8_t   fgMorePower;
//uint16_t  wCMP0_Inverter;
//uint8_t  fgBelow205K;

uint16_t  wCompatiableLEDDelay;
uint16_t  wPOWERTRANSFER_Temperature_Counter;
//uint32_t  wBackupMDPRD;
//uint16_t  wBackupDuty;
//uint16_t  wCommErrorCounter;
uint8_t  bErrorFrom;
uint16_t  wADCRadio;
uint16_t  wPowerTransferCounter;
//PID Control Variable
//uint16_t  w_QI_Control_tda_j_1 , w_QI_Control_td_j;
//uint16_t   w_QI_Control_ta_j_i_1 , w_QI_Control_ta_j_1;
//int16_t  i_QI_Control_e_j_i , i_QI_Control_e_j_i_1;
//uint16_t  w_QI_Control_ta_j_1_Before;
//int16_t  i_QI_Error_Before;
//int16_t  i_QI_Control_P_j_i;
//int16_t  i_QI_Control_I_j_i_1 , i_QI_Control_I_j_i;
//int16_t  i_QI_Control_D_j_i;
//int32_t  l_QI_Control_PID_j_i;
//int32_t  l_v_j_i,l_v_j_i_1;
//==========================================
//LED Show
uint8_t  bLED_Status;
//===========================================
//Learning
//uint8_t  fgLearning=0;
//uint16_t const wDectect_Current_Threshold=100 ;//_at_ D_Dectect_Current_Threshold_Address;
//uint16_t  wLearningRecord[10];
//uint8_t  bLearningBackupIndex;
//uint16_t  wLearning_Note[16];
uint8_t  wLearning_NoteIndex;
//uint8_t  bCounter;
//uint16_t  wFrequncyLimit;
//uint8_t  bControlErrorTheSameCounter;
uint8_t  bPacketReceiveDebounce;
uint8_t  bInvertCounter;
bit   fgMicroFunction;
uint8_t  bZeroDelayCounter;
uint8_t  bManufactor;
uint16_t  wControlErrorTimeDelay;
uint16_t  wVDD;
bit   fgCheckVDD;
bit  fgUSBPower=1;
uint8_t  bReceivedPowerStatus;
uint8_t  bReceivedPowerCounter;
uint8_t  bPOWERtRANSFER_MaxPower;
uint8_t  bPOWERtRANSFER_PowerClass;
uint8_t  b_QI_effciency_table;
uint16_t  w_QI_Receive_Power;
uint8_t  bFOD_EffTable[6];
uint8_t  bFOD_Comm_Table[6]={30,40,40,40,40,40};//{53,62,63,64,65,65}; //_at_ (D_Dectect_Current_Threshold_Address+2);
uint8_t  bFOD_Ti_Table[6]={30,40,40,40,40,40};//{53,62,63,64,65,65};   //_at_ (D_Dectect_Current_Threshold_Address+8);
uint8_t  bFOD_FOXCON_Table[6]={30,40,40,40,40,40};//{53,62,63,64,65,65};   //_at_ (D_Dectect_Current_Threshold_Address+14);
bit  fgFOD,fgFODStart;
uint8_t  bReceivePowerCounter;
uint16_t  wUSBPowerCounter;
uint8_t  bMOSFETDriver_Setting;
uint16_t  wAC_Limit_Voltage;
uint8_t  b_N4_ErrorStayCounter;
uint8_t  b_ErrorStayCounter;
bit  fgRemove;
uint8_t  bMP_ID;

bit  fgWrite;
//uint8_t  bWriteData[14];
uint8_t  bUSB_LED_Debounce;
uint8_t  bIDRXCounter;
uint8_t  bLEDBreathingT1=0;
uint8_t  bLEDBreathingT2=0;
bit  fgLEDBreathingC=0;
uint8_t  LEDshiftCNT=0;
uint16_t  bLEDBreathingC1=11040;

uint16_t wVDD_V=0;


uint16_t wCompleteCounter;
uint16_t wVPPvoltage=0;
uint16_t wSendPower=0;
uint16_t wCal;
uint8_t fgPrintf;

uint32_t dwAppleLowCurrentCnt=0;

uint32_t QC_DelayCnt=0;
uint8_t bInitVddFlag=0;
uint8_t bTempFODCnt=0;
uint8_t bAppleIsFind=0;

uint8_t bSamsungIsFull=0;
uint8_t  FODflag=0;
uint8_t find36wflg=0;



unsigned int AFC_DelayCnt = 0xffff;

uint16_t wCheckOverTemp_Delay = 0;
uint8_t bNowTempValue;
uint8_t bNowTempValueBackup=0;

int16_t wQiTotalError=0;
uint8_t bQiRPAdjFlag = 0;

uint16_t wPWM_DUTY_0 = 0;
uint16_t wPWM_DUTY_1 = 0;

uint8_t bPWM_HALFB = 0;

uint16_t  Capturebit0_intdly=0;


bit  fgHalf, fgSendPacketEnd, fgNextbit,fsendctlbit;


uint8_t  bPhase;
uint16_t  wx_Packet[9];
uint8_t  bPacketArrayIndex,bPacketFinal,bPacketBitIndex;

uint8_t  bPhasePacketIndex, bPhasePacketIndexBefore;
uint8_t  bPowerTransferPacketIndex;


bit  fgChargeCompleteOption;



//Parameter at system.
uint8_t  bControlError, bRetified_Power;
uint8_t  bEndPowerTransferValue;
uint8_t  bInitialCounter;




bit   fgEndPowerTransfer;
bit  fgCtrlError;
uint16_t  wT1_1ms_Counter;
//uint16_t  ctlmostm=0;
uint16_t  wClrmostm=0;

uint8_t  bchargelessflg=1;



uint8_t  bCtrlmos=0;
uint16_t  lowcurrencnt=0;
uint8_t  blowcurSt=0;

uint8_t  b29vSt=0;
uint8_t  bexit25vSt=0;

uint8_t  b29vStcnt=0;
uint16_t  b2ACurrentcnt=0;



uint8_t  blowpwcnt=0;



uint8_t  bCampmosflg=0;






bit  fgResetIni;

uint16_t  wDC_OUT;
uint8_t  wDCflg;



uint16_t  wZeroCurrent;

bit  fgLoading;

uint8_t  bCtrl_Error=0;

uint16_t  wCurrent;
uint16_t  bCurctlcnt;

uint16_t  wCoilTemperature;
uint16_t  wPcbTemperature;
uint16_t  wOutVolt;
uint16_t  wbackupVlt;
uint8_t bconstvlt_cnt = 0;
uint8_t b18v_lowcurrcnt = 0;
uint8_t  wCurstatusCnt=0;


uint16_t  wI2Current;
uint16_t  wI2COutVolt;
uint16_t  wI2CDC_OUT;



uint8_t bconstant_curcnt = 0;
uint16_t Current_limitpw  = 275;

uint16_t frontend_factor_wDC_OUT;
uint16_t  wbackCur;
uint8_t Recdly=0;


uint8_t bMotorMode;
uint16_t wMotorDelay;

//ST_ID_DATA_DEF  st_LockID;

uint16_t wDelayTimeCnt;
uint16_t wChglessTime=0;
uint16_t wDelay10msTimeCnt;
uint8_t bDelay1sFlag;
 

// 5W
const uint8_t bIdentificationPacket_5W[9]=      {0x71,0x11,0x00,0x10,0x00,0x48,0x68,0x15,0x30};
const uint8_t bConfigurationPacket_5W[]=       {0x51,0x0A,0x0,0x0,0x84,0x0};
// 7.5W
const uint8_t bIdentificationPacket_7_5W[9]=      {0x71,0x12,0x00,0x5A,0x62,0x1C,0xEE,0xBD,0x30};
const uint8_t bConfigurationPacket_7_5W[]=       {0x51,0x0A,0x0,0x0,0x43,0x90};
// 10W
uint8_t bIdentificationPacket_10W[9]=   {0x71,0x11,0x00,0xf2,0xe4,0xc2,0xbf,0x98,0x30}; // {0x71,0x11,0x00,0x42,0x64,0x32,0x3f,0x98,0x30};//{0x71,0x19,0x0b,0x12,0xa4,0x32,0x3f,0x98,0x30};  //{0x71,0x11,0x00,0x42,0x64,0x32,0x3f,0x98,0x30};
const uint8_t bConfigurationPacket_10W[]=       {0x51,0x42,0x0,0x0,0x0,0x0};
// 15W
const uint8_t bIdentificationPacket_15W[9]=      {0x71,0x12,0x00,0x33,0x00,0x00,0x00,0x00,0x30};
const uint8_t bConfigurationPacket_15W[]=       {0x51,0x0a,0x0,0x0,0x22,0xf0};


#endif

