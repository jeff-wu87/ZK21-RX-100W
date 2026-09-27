
#ifndef __VARIABLE_H__
#define __VARIABLE_H__
#include "constant.h"
#include <stdint.h>
typedef unsigned char   bit;
//============================================================= 
//	Wireless Charger Transmitter Communication Function define.
//=============================================================
extern void F_Cap0_Assembly_Catch_Code(void);
extern void F_Cap2_Assembly_Catch_Code(void);
extern void F_Cap2_Receiver_Code(void); 
extern uint8_t F_Packet_CheckSum(uint8_t  * bCapDecode , uint8_t  bCapRowCount);
extern void F_LED_Handler(void);
//QI Phase Fucntion
extern void F_QI_Change_Phase_Judgement(void);
extern void F_QI_ChangePhase_Power_Transfer_Common(void);
extern void F_QI_Selection_Phase_Manage(void);
extern void F_QI_Ping_Phase_Manage(void);
extern void F_QI_ID_CONF_Phase_Manage(void);
extern void F_POWERTRANSFER_Phase_Manage(void);
extern uint16_t F_Measure_Control_Current(void);
extern void F_QI_PowerTransfer_Control(void);
extern void F_Measure_Real_Current(void);
extern void F_QI_POWERTRANSFER_Phase_EndPowertransfer_Handler(void);
extern void F_QI_POWERTRANSFER_Phase_Stop_handler(void);

extern void F_QI_POWERTRANSFER_Phase_Thermal_Stop_handler(void);
extern void F_QI_POWERTRANSFER_Phase_Error_Handler(void);
//extern void F_Print(void);
extern void F_Check_Write(void);
extern void F_Limit_Power(uint8_t bCheck);
extern void TMR0_Period_Count_Config(void);


//============================================================= 
//	Wireless Charger Transmitter Phase  variables
//=============================================================
extern uint8_t  bPhase,bPhaseBefore;      //0: Selection  1: Ping 2: Identification & configuration   3: Power transfer   ==P35==
extern uint8_t  b_QI_Signal_Strength_Value;   //Signal strength value which was got at Ping Phase. This is used for free positon to show the best postion. ==P55==
extern uint8_t  b_GeneralTime_Status;        // 0: no Signal Status 1: Signal Status
extern uint8_t  bErrorTerminateCounter;
//QI Phase variable
extern bit  fgChangePhase;
extern bit  fgHeat;
extern uint8_t  bVddOverCnt;
extern uint8_t  bIdleFodCnt;
extern uint8_t  bError,bPacketErrorCounter;
extern uint8_t  bx_QI_Identification[7];             //Identification content        ==58==
extern uint8_t  bx_QI_Configuration[5];              //Configuration content        ==P59==
extern uint8_t  bx_QI_Extended_Identification[8];    //extended Identification content ==P60==
extern uint8_t  b_QI_PowerHoldOff_Time;  //Power Hold off time setting value ==P41==
extern int8_t  i_QI_Error;
extern uint8_t  b_QI_Retified_Power;
extern uint8_t  b_QI_Retified_Power_Before;
extern uint8_t  b_QI_Charge_Status;
extern uint8_t  bTargetStatus;
extern uint16_t  wAC_Zero_Current_Code;
extern uint16_t  wColdTimeCounter;
extern uint8_t bCaptureData2[D_CAP_BUFF];
extern uint8_t bCaptureDataIndex2,bBitIndex2;
extern uint8_t bBitoneCNT2;
extern uint8_t bCaptureData0[D_CAP_BUFF];
extern uint8_t bCaptureDataIndex0,bBitIndex0;
extern uint8_t bBitoneCNT0;
//extern uint8_t fgCap2Flag;
extern uint16_t wCap2BufBackup,wCap0BufBackup;
extern uint16_t wCap2Before,wCap0Before;

extern uint16_t   wCap2CNT;
extern uint16_t   wCap0CNT;


extern uint16_t Volt25_2vfull;
extern uint16_t Current_1Afull;

//============================================================= 
//	Wireless Charger Transmitter variables
//=============================================================
//Decode Variable at catch Interrupt
extern bit  fgCap2Capture, fgCap2SignalStatus,fgCap2SignalStatusBefore,fgCap0Capture, fgCap0SignalStatus,fgCap0SignalStatusBefore;                  
extern bit  fgPacketStart , fgPacketStop , fgCap2PacketStart,fgCap2PacketStop,fgCap0PacketStart,fgCap0PacketStop;
extern bit  fgPacketCheckSumError ;
extern bit  fgPrint;
extern uint8_t  bPrinti;
//extern uint8_t bCap2BackupbBitBufferIndex,bCap2BackupbBitBuffer ;
//extern uint8_t bObjectDebounce;
//extern int_least32_t iCap2CaptureTail;
//extern uint32_t wCap2Target500us, wCap2Target250us ;
//extern uint8_t fgCap2Tail ;
//extern uint8_t fgCap2ModifyLastBit ;
//extern uint8_t bCap2PeriodIndex ;
//Decode Variable at Timer0 catch
extern uint8_t bCap2PacketStutureIndex,bCap0PacketStutureIndex ; //1: Preamble   2:Header    3:Message   4:CheckSum
extern bit fgCap2HalfClk,fgCap0HalfClk ;
extern uint8_t bCap2ReceiveCodeEnd,bCap0ReceiveCodeEnd ;
//extern uint8_t bCap2BitBuffer,bCap2BitBufferIndex ;
extern uint8_t bCap2PreambleCounter,bCap0PreambleCounter ;
//extern uint8_t bCatchCodeIndex;
//Decode Variable with parity
//extern uint8_t bCap2Decode_Row_Index,bCap2Decode_Row_ByteIndex ;
extern uint8_t bDecodeIndex;
extern uint8_t bDecodeIndex0;
extern uint8_t bDecodeIndex2;
extern uint16_t wAC_Zero_Current_Code;
extern bit fgCap2EndHalfBYTE,fgCap0EndHalfBYTE ;
//Decode Variable with CheckSum
extern uint8_t bDecode[D_CAP_BUFF] ;
extern uint8_t bDecode0[D_CAP_BUFF] ;
extern uint8_t bDecode2[D_CAP_BUFF] ;

extern uint8_t bPacketReceiveDebounce;
extern bit fgClear;
//Current Counter
extern uint8_t  bCurrentCounter;
extern uint16_t  wRC_Low_Pass_Current;
extern uint16_t  wRC_Current;
extern uint16_t  wAC_Low_Pass_Voltage;
extern uint16_t  w_QI_Retified_Power_Limit;
//extern uint8_t  bOverCurrentCounter;
//extern uint16_t  wVirtualWorkCounter;
//extern uint32_t  dwProtectCurrent;
//SELECTION Phase Variable
extern uint16_t  wObjectDetectionIntervalCounter;
//extern uint8_t  bObjectDebounce;
extern uint16_t  wADC_Backup;
//ID CONF Phase Variable
//extern uint8_t  b_ID_CONF_Timer_Status;  // 0: D_ID_CONF_Status_tnext 1: D_ID_CONF_Status_tmax 2: D_ID_CONF_Status_tterminate 3: D_ID_CONF_Status_expire
extern uint8_t  b_ID_CONF_Packet_Index;  //
extern int16_t  b_ID_CONF_private_setting_counter;
//extern uint16_t  w_Object_Frequency_max , w_Object_Frequency_min;
//send data
extern uint16_t wStart9VCounter;
extern uint16_t wStart9VCounter1;
extern uint8_t fFastcharge;
extern uint32_t bCRCLbackup1;
extern uint32_t bCRCLbackup2;
extern uint16_t bCRCLbackup;

extern uint8_t fFrequencyshift;
extern uint8_t firstinchargeFrequency;
extern uint8_t fmidlepower;
extern uint8_t fboostup;
extern uint16_t wsendTimecount;
extern uint8_t  bByteindexsend;
extern uint8_t bBitindexsend;
extern uint16_t bTimebase;
extern uint8_t fsendstart;
extern uint8_t fsendstart1;
extern uint8_t fsendbit;
extern uint8_t fhalfbitsend;
extern uint8_t  bByteindexsendCNT;
extern uint8_t  bBitindexsendCNT;
extern uint8_t  fgPacketStartbitsend;
extern uint8_t  fsendparity;
extern uint8_t fSendNeg; 
extern uint8_t fSendPolarity; 
extern uint8_t fSendDepth;
extern uint8_t f15Wstartup;
extern uint8_t bDatatosend[6];
extern uint8_t const bSamsung10WData[2];
extern uint8_t const bTX_ACK_Data[1];
extern uint8_t const bTX_NAK_Data[1];
extern uint8_t const bTX_ND_Data[1];
extern uint8_t const bTX_Identification_Data[5];
extern uint8_t const bTX_Capability_Data[5];
//Power Transfer Variable
extern uint16_t  wPOWERTRANSFER_Error_Packet_Timeout_Counter;
extern uint16_t  wPOWERTRANSFER_Rectified_Power_Packet_TimeOut_Counter;
extern int32_t  lPOWERTRANSFER_COMPATIABLE_DELAY;
extern uint16_t  wPOWERTRANSFER_COMPATIABLE_COUNT;
extern uint8_t  bReconfigCounter;
extern uint8_t  bControlErrorCNTsend;
extern uint16_t wChargeCompleteCNT;
extern uint8_t  bChargeCompleteCNT;
extern bit  fgCompatiableMode;
extern uint8_t  bPOWERTRANSFER_Error_Control_Time_Counter;
extern bit  fgErrorControl;
extern bit  fgEndThisControl;
//extern uint8_t  fgLearningFrequncy;
extern bit  fgLimitFreBoundary;
//extern uint8_t  fgCompatiableNokia;
//extern uint8_t  fgBelow205K;

extern uint8_t  b_PP_ControlStatus;
extern uint16_t  wCompatiableLEDDelay;
extern uint16_t   wPOWERTRANSFER_Temperature_Counter;
//extern uint32_t   wBackupMDPRD;
//extern uint16_t   wBackupDuty;
//extern uint16_t   wCommErrorCounter;
extern uint8_t   bErrorFrom;
extern uint16_t   wADCRadio;
extern uint16_t   wPowerTransferCounter;
//===============================================
//Control
//extern uint16_t   w_QI_Control_tda_j_1 , w_QI_Control_td_j;
//extern uint16_t    w_QI_Control_ta_j_i_1 , w_QI_Control_ta_j_1;
//extern int16_t   i_QI_Control_e_j_i , i_QI_Control_e_j_i_1;
//extern uint16_t   w_QI_Control_ta_j_1_Before;
//extern int16_t   i_QI_Error_Before;
//extern int16_t   i_QI_Control_P_j_i;
//extern int16_t   i_QI_Control_I_j_i_1 , i_QI_Control_I_j_i;
//extern int16_t   i_QI_Control_D_j_i;
//extern int32_t   l_QI_Control_PID_j_i;
//extern int32_t   l_v_j_i,l_v_j_i_1;
//===============================================
//Show
extern uint8_t  bLED_Status;
extern bit    fgPrint;
//extern uint8_t    fgMorePower;
extern uint16_t wPrintdata2[6];
//===============================================
//Learning
//extern uint8_t   fgLearning;
//extern uint16_t const wDectect_Current_Threshold;
//extern uint16_t   wLearningRecord[];
//extern uint8_t   bLearningBackupIndex;
//extern uint16_t   wLearning_Note[];
extern uint8_t   wLearning_NoteIndex;
//extern uint8_t   bCounter;
//extern uint16_t   wFrequncyLimit;
//extern uint8_t   bControlErrorTheSameCounter;
extern uint8_t   bPacketReceiveDebounce;
//extern uint16_t   wCMP0_Inverter;
extern uint8_t   bManufactor;
extern uint8_t   bInvertCounter;
extern bit    fgMicroFunction;
extern uint8_t   bZeroDelayCounter;
extern uint16_t   wVDD;
extern bit   fgCheckVDD;
extern bit   fgUSBPower;
extern uint8_t   bReceivedPowerStatus;
extern uint8_t   bReceivedPowerCounter;
extern uint8_t   bPOWERtRANSFER_MaxPower;
extern uint8_t   bPOWERtRANSFER_PowerClass;
extern uint8_t   b_QI_effciency_table;
extern uint16_t   w_QI_Receive_Power;
extern uint8_t   bFOD_EffTable[];
//extern uint8_t const bFOD_Comm_Table[];
//extern uint8_t const bFOD_Ti_Table[];
//extern uint8_t const bFOD_FOXCON_Table[];
extern bit   fgFOD,fgFODStart;
extern uint8_t   bReceivePowerCounter;
extern uint16_t   wUSBPowerCounter;
extern uint8_t   bMOSFETDriver_Setting;
extern uint16_t   wAC_Limit_Voltage;
extern uint8_t   b_N4_ErrorStayCounter;
extern uint8_t   b_ErrorStayCounter;
extern bit   fgRemove;
extern uint8_t   bMP_ID;

extern uint8_t  bFOD_Comm_Table[];
extern uint8_t  bFOD_Ti_Table[];
extern uint8_t  bFOD_FOXCON_Table[];
extern bit   fgWrite;
//extern uint8_t   bWriteData[];
extern uint8_t   bUSB_LED_Debounce;
extern uint8_t   bIDRXCounter;
extern uint8_t   bLEDBreathingT1;
extern uint8_t   bLEDBreathingT2;
extern bit   fgLEDBreathingC;
extern uint8_t   LEDshiftCNT;
extern uint16_t   bLEDBreathingC1;

extern uint16_t wVDD_V;
extern uint16_t wVPPvoltage;
extern uint16_t  wSendPower;
extern uint16_t wCal;
extern uint8_t fgPrintf;
extern uint32_t dwAppleLowCurrentCnt;

extern uint32_t QC_DelayCnt;
extern uint8_t bInitVddFlag;
extern uint8_t bTempFODCnt;
extern uint8_t bAppleIsFind;
extern uint8_t bSamsungIsFull;

extern uint8_t FODflag;

extern uint16_t wCheckOverTemp_Delay;
extern uint8_t bNowTempValue;
extern uint8_t bNowTempValueBackup;

extern int16_t wQiTotalError;
extern uint8_t bQiRPAdjFlag;

extern uint16_t wPWM_DUTY_0;
extern uint16_t wPWM_DUTY_1;
extern uint8_t bPWM_HALFB;
extern uint8_t find36wflg;

extern uint16_t  Capturebit0_intdly;

extern uint16_t  wbackupVlt;


extern bit  fgHalf, fgSendPacketEnd, fgNextbit,fsendctlbit;

extern uint8_t  bPhase;
extern uint16_t  wx_Packet[9];
extern uint8_t  bPacketArrayIndex,bPacketFinal,bPacketBitIndex;


extern uint8_t  bPhasePacketIndex, bPhasePacketIndexBefore;
extern uint8_t  bPowerTransferPacketIndex;

extern bit  fgChargeCompleteOption;


//Parameter at system.
extern uint8_t  bControlError, bRetified_Power;
extern uint8_t  bEndPowerTransferValue;
extern uint8_t  bInitialCounter;



extern bit   fgEndPowerTransfer;
extern bit  fgCtrlError;


extern uint16_t  wT1_1ms_Counter;

extern bit  fgResetIni;

extern uint16_t  wDC_OUT;
extern uint8_t  wDCflg;


extern uint16_t  wZeroCurrent;

extern bit  fgLoading;

extern uint8_t  bCtrl_Error;
extern uint8_t  bCampmosflg;
extern uint8_t  bchargelessflg;


extern uint16_t  wCurrent;
extern uint16_t  bCurctlcnt;

extern uint8_t  blowpwcnt;


extern uint16_t  wCoilTemperature;
extern uint16_t  wPcbTemperature;
extern uint16_t  wOutVolt;
extern uint16_t  lowcurrencnt;


extern uint16_t  wI2Current;
extern uint16_t  wI2COutVolt;
extern uint16_t  wI2CDC_OUT;

extern uint8_t  blowcurSt;

extern uint8_t  b29vSt;
extern uint8_t  b29vStcnt;

extern uint8_t  bexit25vSt;


extern uint8_t bMotorMode;
extern uint16_t wMotorDelay;

//extern ST_ID_DATA_DEF  st_LockID;

extern uint16_t wDelayTimeCnt;
extern uint16_t wDelay10msTimeCnt;
extern uint8_t bDelay1sFlag;
extern uint8_t bDelay1msFlag;

// 5W
extern const uint8_t bIdentificationPacket_5W[9];
extern const uint8_t bConfigurationPacket_5W[];
// 10W
extern  uint8_t bIdentificationPacket_10W[9];
extern const uint8_t bConfigurationPacket_10W[];
// 7.5W
extern const uint8_t bIdentificationPacket_7_5W[9];
extern const uint8_t bConfigurationPacket_7_5W[];
// 15W
extern const uint8_t bIdentificationPacket_15W[9];
extern const uint8_t bConfigurationPacket_15W[];

extern uint8_t  bCtrlmos;
extern uint16_t  wClrmostm;

extern uint16_t  b2ACurrentcnt;



/////////////////////////////////////////////////////////////////////////////////////
//extern fun
extern uint16_t PWM_Duty_Get(uint8_t ch);
extern void adcInit(uint8_t ch);
extern void adcStart(void);
extern void adcWait(void);


//////
extern void F_PacketDecode_Handler(void);

extern void my_itoa(int16_t n,uint8_t flag);

//extern void memcpy_(void *dst, const void *src, size_t len);
//extern void my_itoa(int8_t n,uint8_t flag);

extern void F_CaptureData0_Clear(void);
extern void F_CaptureData2_Clear(void);

extern uint16_t ADC_GetADCResult(uint8_t ch);
#endif
