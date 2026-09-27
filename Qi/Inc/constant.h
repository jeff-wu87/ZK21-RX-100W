
#ifndef __CONSTANT_H__
#define __CONSTANT_H__
//============================================================= 
//	Wireless Charger Transmitter Defince
//=============================================================
#define SELECTION           0x0
#define PING                0x1
#define ID_CONF             0x2
#define POWERTRANSFER       0x3

#define PING_APPLE          0x0a
/*
1  P05    input voltage
2  P06    TX/SS_C2
3  P07    Current
4  NRST 	Reset
5  P30 	  Test/SS_C1	
6  P17 	  Complete
7  VSS
8  SDA
9  VDD
10 P15    Dec2
	
11 P14   QC_in
12 P13  Charge
13 P12   PWM0
14 P11   /PWM0
15 P10	 /PWM1
16 P00   PWM1
17 P01   Dec1
18 P02   SCK
19 P03   Protect
20 P04   ADC_Temp
*/
//============================================================= 
//	Wireless Charger Transmitter Defince
//=============================================================
#define SELECTION           0x0
#define PING                0x1
#define ID_CONF             0x2
#define POWERTRANSFER       0x3

#define D_QI_SignalStrength         0x1
#define D_QI_EdnPowerTransfer       0x2
#define D_QI_ControlError           0x3
#define D_QI_RectifiedPower         0x4
#define D_QI_ChargeStatus           0x5
#define D_QI_PowerControlHold_off   0x6
#define D_QI_Configuration          0x51
#define D_QI_Identification         0x71
#define D_QI_ExtendedIdentification 0x81

#define D_QI_GeneralRequest         0x07
#define D_QI_SpecificRequest        0x20
#define D_QI_FODStatus              0x22
#define D_QI_Renegotiate            0x09
#define D_QI_24bitReceiverPower     0x31 

#define D_QI_Proprietary_0x18       0x18          
#define D_QI_Proprietary_0x19       0x19          
#define D_QI_Proprietary_0x28       0x28          
#define D_QI_Proprietary_0x29       0x29          
#define D_QI_Proprietary_0x38       0x38          
#define D_QI_Proprietary_0x48       0x48          
#define D_QI_Proprietary_0x58       0x58          
#define D_QI_Proprietary_0x68       0x68          
#define D_QI_Proprietary_0x78       0x78          
#define D_QI_Proprietary_0x84       0x84          
#define D_QI_Proprietary_0xA4       0xa4          
#define D_QI_Proprietary_0xC4       0xc4          
#define D_QI_Proprietary_0xE2       0xe2          
#define D_QI_Proprietary_0xF2       0xf2

//========Selection Phase Define====================
#define D_QI_todi   490         //unit: ms      Object detection interval,      ==P67==
#define D_QI_todd   150         //unit: us      Object detection duration,      ==P67==
#define D_QI_todm    20         //unit: 0.5 us  Object detection measurement,   ==P67==
#define D_Dectect_Current_Threshold     0x208
#define D_AC_Current_Offset 6 //unit: 10mV
#define D_AC_Current_Offset_Code    (uint16_t)(D_AC_Current_Offset*0x3FF/50) //(uint16_t)(D_AC_Current_Offset*0xFFF/50) //
#define D_OverTemperature_WakeupTime         300    //unit: s   when get end powertransfer with overtemperature
#define D_OverTemperature_WakeupTime_Code    D_OverTemperature_WakeupTime*20/(D_QI_todi/50)

#define D_OverVoltage_Soft_Start_WakeupTime         1    //unit: s   when get end powertransfer with overtemperature
#define D_OverVoltage_Soft_Start_WakeupTime_Code    D_OverVoltage_Soft_Start_WakeupTime*20/(D_QI_todi/50)

//======================================================================
#define D_D_QI_todd_CodeH   (uint8_t)((0x10000- D_QI_todd*24.5/4)/256)
#define D_D_QI_todd_CodeL   (uint8_t)(0x10000- D_QI_todd*24.5/4)

#define D_D_QI_todm_CodeH   (uint8_t)((0x10000- D_QI_todm*24.5/8)/256)
#define D_D_QI_todm_CodeL   (uint8_t)(0x10000- D_QI_todm*24.5/8)
//======================================================================

#define D_GeneralTime_No_Signal 0x0
#define D_GeneralTime_Signal    0x1
#define D_QI_Wakeup_After_ChargeComplete_Time   62  //uni

//========PING Phase Define====================
//-----------------------------------------
#define D_CalculateChecksum_Delay   3   //unit: ms CalculateCheck sum delay
#define D_QI_tping              65  //unit: ms Maximum Digital Ping duration    ==P38==
#define D_QI_tterminate         (25-D_CalculateChecksum_Delay)  //unit: ms Power Signal termination time    ==P38==
#define D_QI_tfirst             18  //unit: ms First Packet time out            ==P38==
#define D_QI_texpire            (25-D_CalculateChecksum_Delay)  //unit: ms Power Signal expiration time     ==P38==
#define D_QI_tping_withDelay    ( D_QI_tping +  D_CalculateChecksum_Delay )
#define D_QI_tfirst_withDelay   ( D_QI_tfirst+  D_CalculateChecksum_Delay ) //unit: ms First Packet time out            ==P38==
//-----------------------------------------
#define D_QI_PING_ERROR_A       1   //Ping Error A, not receive start at tping time                                     ==P39==
#define D_QI_PING_ERROR_C       2   //Ping Error C, Transmitter don't want enter ID_CONF Phase,                         ==P39==  not apply here
#define D_QI_PING_ERROR_D       3   //Ping Error D, Recieve Start , but tfirst time digital PING don't receive stop     ==P39==
#define D_QI_PING_ERROR_E       4   //Ping Error E, Recieve packet but it is not signal Strength Packet                 ==P39==
//------------------------------------------
//========ID CONF Phase Define====================
//-----------------------------------------
#define D_QI_ID_CONF_tnext      20          //unit: ms ID_CONF next packet time out     ==P41==
#define D_QI_ID_CONF_tmax      170          //unit: ms ID_CONF Maximum Packet length    ==P41==
// ID and CONF phase recieve code sequence. ==P39==
#define D_ID_CONF_Identification_Packet             0       
#define D_ID_CONF_Ext_Identification_Packet         1
#define D_ID_CONF_Optional_Configuration_Packet1    2
#define D_ID_CONF_Optional_Configuration_Packet2    3
#define D_ID_CONF_Optional_Configuration_Packet3    4
#define D_ID_CONF_Optional_Configuration_Packet4    5
#define D_ID_CONF_Optional_Configuration_Packet5    6
#define D_ID_CONF_Optional_Configuration_Packet6    7
#define D_ID_CONF_Optional_Configuration_Packet7    8
#define D_ID_CONF_Configuration_Packet              9
//-----------------------------------------
//========
#define D_QI_Control_Hold_off_Time_min  5   //unit: ms , Power control Hold off Time Minimum.   ==P41==
#define D_QI_Control_Hold_off_Time_max  205 //unit: ms , Power control Hold off Time Maximum    ==P41==
//==========
#define D_QI_ID_CONF_ERROR_A       1   //ID CONF Error A, ID_CONF errorA , at tnext time , no start    ==P41==
#define D_QI_ID_CONF_ERROR_B       2   //ID CONF Error C, ID_CONF errorB , at tmax time , no stop      ==P41==
#define D_QI_ID_CONF_ERROR_C       3   //ID CONF Error D, ID_CONF errorC , packet series error         ==P41==
#define D_QI_ID_CONF_ERROR_E       4   //ID CONF Error E, ID_CONF errorE , get end power transfer      ==P41==
#define D_QI_ID_CONF_ERROR_F       5   //ID CONF Error F, ID_CONF errorF , Control Hold of Timer error      ==P41==
//------------------------------------------
// Power Transfer Phase Define
#define D_QI_POWERTRANSFER_ttimeout     1700    //unit: ms  control error packet time out at POWERTRANSFER Phase ==P44==
#define D_QI_POWERTRANSFER_tactive      20      //unit: ms  Power control active time   at Powertransfer phase      ==P44==
#define D_QI_POWERTRANSFER_tsettle      3       //unit: ms  Power control setting time  at powertransfer phase      ==P44==
#define D_QI_POWERTRANSFER_tpower       23500   //unit: ms  rectified power packet time at Powertransfer phase      ==P44==
#define D_QI_POWERTRANSFER_tPowerPhaseTime  1       //unit: time    reconfigfrom PowerTransfer ==P72==
//==========
#define D_QI_POWERTRANSFER_ERROR_A      1       // don't Receive any error packet at ttimeout period    ==P43==
#define D_QI_POWERTRANSFER_ERROR_C      2       // packet is not belong to powertranfer phase           ==P43==
#define D_QI_POWERTRANSFER_ERROR_E      3       // Get end power transfer packet , stop power output at texpire time    ==P43==
#define D_QI_POWERTRANSFER_ERROR_F      4       // at tpower time , not recieve retified power packet at tpower time    ==P43==
#define D_QI_POWERTRANSFER_ERROR_B      5       // receive packet start at power control phase          == P43==
//==========
#define D_QI_End_Power_Transfer_Unknown             0x0     // Reciever endpower unkown   ==P56==
//#define D_QI_End_Power_Transfer_Unknown             0x0     // Reciever endpower unkown   ==P56==
//==========
#define D_QI_User_SignalStrength 0x01       //Check Signal Strength at Ping Phase
#define D_CheckPacketContent_OK             0x0
#define D_CheckPacketContent_nothispacket   0x10
#define D_CheckPacketContent_CheckSumError  0x20
#define D_CheckPacketContent_ReceiveError   0x30
#define D_CheckPacketContent_Decode_OK                  0x0
#define D_CheckPacketContent_Decode_StartStopError      0x1
#define D_CheckPacketContent_Decode_ParityError         0x2
#define D_CheckPacketContent_Decode_ByteCountError      0x3
//-------------------------------------------------------------
#define D_QI_Fod        175//140         //unit: kHz Frequency operation detection     ==P67==
#define D_QI_Fop_INI    130//175//183//175//175         //unit: kHz Frequency operation point           ==P13==
//-------------------------------------------------------------
#define Feq_12V_INI_Duty            50      //unit: % maximum 50% min: 15%
#define Feq_QI_Fod_Code                 (uint16_t)((64000/D_QI_Fod))
//#define Feq_QI_Fod_half_Code            (uint32_t)(24500/D_QI_Fod/2) //Setting PWM Duty. it is set at 50% now, or you can
#define Feq_QI_Fop_INI_Code             (uint16_t)((64000/D_QI_Fop_INI))
//#define Feq_QI_Fop_INI_12V_Code         (uint8_t)((uint32_t)24500/D_QI_Fop_INI*Feq_12V_INI_Duty/100) //Setting PWM Duty.        ==P13==
#define D_QI_Fod_period_Time            (1000/D_QI_Fod)       //select more prepare value.                   
//-------------------------------------------------------------
#define D_QI_EndPowerTransfer_Unkown                0x0         //EndPowerTransferValue     ==P56==
#define D_QI_EndPowerTransfer_ChargeComplete        0x1
#define D_QI_EndPowerTransfer_InternalFault         0x2
#define D_QI_EndPowerTransfer_OverTemerature        0x3
#define D_QI_EndPowerTransfer_OverVoltage           0x4
#define D_QI_EndPowerTransfer_OverCurrent           0x5
#define D_QI_EndPowerTransfer_BatteryFailure        0x6
#define D_QI_EndPowerTransfer_Reconfigure           0x7
#define D_QI_EndPowerTransfer_NoResponse            0x8
//============================================================= 
//	PID Control Define
//=============================================================
#define D_QI_A1_PID_Kp      3//3      //unit: mA-1
#define D_QI_A1_PID_Ki      0.2//0.2    //unit: mA-1ms-1
#define D_QI_A1_PID_Kd      0       //unit: mA-1ms
#define D_QI_A1_PID_Kp_ADC_Count    (D_QI_A1_PID_Kp*4.88)      //unit: count-1       , Maximum = 16
#define D_QI_A1_PID_Ki_ADC_Count    (D_QI_A1_PID_Ki*488)      //unit: count-1ms-1
#define D_QI_A1_PID_Kd_ADC_Count    (D_QI_A1_PID_Kd*4.88)      //unit: count-1ms
#define D_QI_A1_PID_tinner          1       //unit: ms
#define D_QI_AI_Sv_110_140      1     //A1 Scalar factor  ==P14==
#define D_QI_AI_Sv_140_160      1       
#define D_QI_AI_Sv_160_180      1
#define D_QI_AI_Sv_180_205      1
#define D_QI_AI_Sv_Lv0_10W      122         //unit :kHz
#define D_QI_AI_Sv_Lv1_10W      135//145         //unit :kHz
#define D_QI_AI_Sv_Lv1_128      128         //unit :kHz
#define D_QI_AI_Sv_Lv0       136 //115//125//115         //unit :kHz
#define D_QI_AI_Sv_Lv1      140         //unit :kHz
#define D_QI_AI_Sv_Lv2      160         //unit :kHz
#define D_QI_AI_Sv_Lv3      180         //unit :kHz
#define D_QI_AI_Sv_Lv4      205//250//206         //unit :kHz
#define D_QI_AI_Sv_Below_Lv4_min_duty   25//4//2//20//10  //unit: %
#define D_QI_12V_INI_HF_Limit    D_QI_Fop_INI
#define D_QI_AI_Sv_Lv0_10W_Code      (64000/D_QI_AI_Sv_Lv0_10W))//16000/D_QI_AI_Sv_Lv0_10W         //unit :Count
#define D_QI_AI_Sv_Lv1_10W_Code      ((64000/D_QI_AI_Sv_Lv1_10W))//16000/D_QI_AI_Sv_Lv1_10W         //unit :Count
#define D_QI_AI_Sv_Lv1_128_Code      ((64000/D_QI_AI_Sv_Lv1_128))//16000/D_QI_AI_Sv_Lv1_128       //unit :kHz
#define D_QI_AI_Sv_Lv0_Code      ((64000/D_QI_AI_Sv_Lv0))//16000/D_QI_AI_Sv_Lv0         //unit :Count
#define D_QI_AI_Sv_Lv1_Code      ((64000/D_QI_AI_Sv_Lv1))//16000/D_QI_AI_Sv_Lv1         //unit :Count
#define D_QI_AI_Sv_Lv2_Code      ((64000/D_QI_AI_Sv_Lv2))//16000/D_QI_AI_Sv_Lv2         //unit :Count
#define D_QI_AI_Sv_Lv3_Code      ((64000/D_QI_AI_Sv_Lv3))//16000/D_QI_AI_Sv_Lv3         //unit :Count
#define D_QI_AI_Sv_Lv4_Code      ((64000/D_QI_AI_Sv_Lv4))//16000/D_QI_AI_Sv_Lv4         //unit :Count
#define D_QI_AI_Sv_apple_Code      (504)//(377)//48000/127)//16000/128         //unit :Count

#define D_QI_12V_INI_HF_Limit_Code      ((24000/D_QI_12V_INI_HF_Limit)*2)//16000/D_QI_12V_INI_HF_Limit         //unit :Count
#define D_QI_AI_Sv_Below_Lv4_min_duty_Code      ((24000*D_QI_AI_Sv_Below_Lv4_min_duty/100/D_QI_AI_Sv_Lv4)*2)//16000*D_QI_AI_Sv_Below_Lv4_min_duty/100/D_QI_AI_Sv_Lv4         //unit :Count
#define D_QI_AI_Sv_Below_Lv1_128_min_duty_Code      ((24000*D_QI_AI_Sv_Below_Lv4_min_duty/100/D_QI_AI_Sv_Lv1_128)*2)//16000*D_QI_AI_Sv_Below_Lv4_min_duty/100/D_QI_AI_Sv_Lv1_128         //unit :Count
#define D_QI_AI_Sv_Lv4_Half_duty_Code      D_QI_AI_Sv_Lv4_Code/2         //unit :Count
#define D_Nokia_OverShoot_Code_Lv0      3
#define D_Nokia_OverShoot_Code_Lv1      5
#define D_Nokia_OverShoot_Code_Lv2      5
#define D_Nokia_OverShoot_Code_Lv3      5
#define D_Nokia_OverShoot_Code_Lv4          5
#define D_Learning_Frequncy_Limit	130
#define D_Learning_Frequncy_Limit_Code      	((24000/D_Learning_Frequncy_Limit)*2)//16000/D_Learning_Frequncy_Limit         //unit :Count
#define D_Below_L4  1           // 1: yes, with min duty , 0: no. limit at 50%
#define D_Postive_max_110_140   0x08//0x2             //0x5
#define D_Postive_max_140_160   0x0C//0x3             //0x9
#define D_Postive_max_160_180   0x18//0x6            //0x10
#define D_Postive_max_180_205   0x28//0xa            //0x12
#define D_Postive_max_205       0x08//0x02//0x4            //0x12
#define D_Negtive_max_110_140   -D_Postive_max_110_140
#define D_Negtive_max_140_160   -D_Postive_max_140_160
#define D_Negtive_max_160_180   -D_Postive_max_160_180
#define D_Negtive_max_180_205   -D_Postive_max_180_205
#define D_Negtive_max_205       -D_Postive_max_205
#define D_Micro_Postive_max_110_140   0x1             //0x5
#define D_Micro_Postive_max_140_160   0x1             //0x9
#define D_Micro_Postive_max_160_180   0x1            //0x10
#define D_Micro_Postive_max_180_205   0x1            //0x12
#define D_Micro_Postive_max_205       0x1            //0x12
#define D_Micro_Negtive_max_110_140   -D_Micro_Postive_max_110_140
#define D_Micro_Negtive_max_140_160   -D_Micro_Postive_max_140_160
#define D_Micro_Negtive_max_160_180   -D_Micro_Postive_max_160_180
#define D_Micro_Negtive_max_180_205   -D_Micro_Postive_max_180_205
#define D_Micro_Negtive_max_205       -D_Micro_Postive_max_205
#define D_QI_I_max      3000
#define D_QI_I_min      -3000
#define D_QI_PID_max    20000
#define D_QI_PID_min    -20000      
//============================================================= 
//	PWM Control Define
//=============================================================
//#define D_PWM_OVERRIDE_Full (PWM0_OutputLow | PWM1_OutputLow |PWM2_OutputLow | PWM3_OutputLow)
//#define D_PWM_Open_Full  (PWMSync_AvoidGlitch | PWM0_TimeBaseControl  |PWM1_TimeBaseControl | PWM2_TimeBaseControl  |PWM3_TimeBaseControl )
//#define D_PWM_Clear (PWMSync_AvoidGlitch)
//#define D_PWMCON9_SET PWM0_POL | PWM3_POL

//    P22 = 1;        //PWM0
//    P33 = 0;        // /PWM0
//    P24 = 0;        // /PWM1
//    P34 = 1;        //PWM1
//#define D_PWM_Clear  	  {set_PMEN3; set_PMEN2; set_PMEN1; set_PMEN0;} 
//#define D_PWM_Clear  	  {CCAPM0 &= ~PWM0; CCAPM1 &= ~PWM1; CCAPM2 &= ~PWM2; CCAPM3 &= ~PWM3;CR = 0;P22 = 1;P33 = 0;P24 = 0;P34 = 1;} 
//#define D_PWM_Clear  	  {EPWM_EnableFaultBrake(EPWM_BRK_SOFTWARE,EPWM_BRK_COUNT_CLEAR);EPWM_TrigSoftwareBrake();ACMP_Stop(ACMP0);ACMP_Stop(ACMP1);} 
//#define D_PWM_Clear  	  {
//	PWMFBKC &= ~(EPWM_PWMFBKC_PWMFBCCE_Msk);
//	PWMFBKC |= EPWM_BRK_COUNT_CLEAR;PWMFBKC |= EPWM_PWMFBKC_PWMFBKSW_Msk;C0CON0 &= ~(ACMP_C0CON0_C0EN_Msk);C1CON0 &= ~(ACMP_C1CON0_C1EN_Msk);} 
#define D_PWM_Clear  	  {EPWM_ConfigChannelSymDuty(EPWM2, 0);EPWM_ConfigChannelSymDuty(EPWM3, 0);}//EPWM_DisableOutput(EPWM_CH_2_MSK | EPWM_CH_3_MSK );}//EPWM_EnableMaskControl(EPWM_CH_0_MSK,0);EPWM_EnableMaskControl(EPWM_CH_1_MSK,1);C0CON0 &= ~(ACMP_C0CON0_C0EN_Msk);C1CON0 &= ~(ACMP_C1CON0_C1EN_Msk);} 

//#define D_PWM_Start         {EPWM_DisableMaskControl(EPWM_CH_1_MSK);EPWM_DisableSoftwareBrake();EPWM_ClearFaultBrakeIntFlag();EPWM_Start(EPWM_CH_0_MSK|EPWM_CH_1_MSK);ACMP_Start(ACMP0);ACMP_Start(ACMP1);} 
//#define D_PWM_Start         {EPWM_DisableMaskControl(EPWM_CH_1_MSK);EPWM_DisableSoftwareBrake();EPWM_ClearFaultBrakeIntFlag();EPWM_Start(EPWM_CH_0_MSK|EPWM_CH_1_MSK);C0CON0 |= (ACMP_C0CON0_C0EN_Msk);C1CON0 |= (ACMP_C1CON0_C1EN_Msk);} 
#define D_PWM_Start         {EPWM_EnableOutput( EPWM_CH_2_MSK | EPWM_CH_3_MSK );}//EPWM_Start(EPWM_CH_0_MSK|EPWM_CH_1_MSK);EPWM_EnableOutput(EPWM_CH_0_MSK|EPWM_CH_1_MSK);C0CON0 |= (ACMP_C0CON0_C0EN_Msk);C1CON0 |= (ACMP_C1CON0_C1EN_Msk);} 
//#define D_PWM_OpenAll  	{CCAPM0 |= PWM0; CCAPM1 |= PWM1; CCAPM2 |= PWM2; CCAPM3 |= PWM3;} 
#define D_PWM_OpenAll  	{bPWM_HALFB = 0;}//EPWM_EnableOutput(EPWM_CH_2_MSK | EPWM_CH_3_MSK );}//EPWM_DisableMaskControl(EPWM_CH_0_MSK|EPWM_CH_1_MSK);PWMCNTCLR = 0x03;} 
//#define D_PWM_OpenHalf  {CCAPM0 &= ~PWM0; CCAPM1 &= ~PWM1; CCAPM2 |= PWM2; CCAPM3 |= PWM3;} 
//#define D_PWM_OpenHalf  {EPWM_DisableOutput(EPWM_CH_0_MSK);EPWM_EnableOutput(EPWM_CH_1_MSK);} 
#define D_PWM_OpenHalf  {bPWM_HALFB = 1;EPWM_ConfigChannelSymDuty(EPWM3, 0xffff);}//EPWM_EnableOutput( EPWM_CH_2_MSK );}//EPWM_DisableOutput( EPWM_CH_3_MSK );}//EPWM_EnableMaskControl(EPWM_CH_1_MSK,0);EPWM_DisableMaskControl(EPWM_CH_0_MSK);} //EPWM_DisableOutput(EPWM_CH_1_MSK);
//=============================================================
// PWM Control Phase Definition
//=============================================================
#define D_PP_ConrolPhase_1         0x1
#define D_PP_ConrolPhase_2          0x2
#define D_PP_ConrolPhase_3   0x3
#define D_PP_ConrolPhase_4   0x4
#define D_PP_ConrolPhase_5   0x5
//============================================================= 
//	ADC Control Define
//=============================================================
#define D_ADCFG_Setting ( ADCClock_By8 | ADCSampleHold_4T | ADC_12Bit )
//#define D_VDD_Limit 41//45  //unit: 0.1V
//#define D_Refer_Voltage 12  //unit: 0.1 V
//#define D_VDD_Limit_Code (uint16_t)((uint32_t)D_Refer_Voltage*0xfff/D_VDD_Limit)//(uint16_t)((uint32_t)D_Refer_Voltage*0xfff/D_VDD_Limit)
#define D_ADC_Low_Level 10  //unit: 0.1V
#define D_ADC_High_Level 40  //unit: 0.1V
#define D_ADC_Low_Level_Code (uint16_t)((uint32_t)D_ADC_Low_Level*0xfff/50)
#define D_ADC_High_Level_Code (uint16_t)((uint32_t)D_ADC_High_Level*0xfff/50)


#define D_ADC_V_MAX 50

#define D_Refer_Voltage (76)//(80)  //unit: 0.1 V
#define D_VDD_Limit_Code (uint16_t)((uint32_t)D_Refer_Voltage*0xfff/(6*D_ADC_V_MAX))//(uint16_t)((uint32_t)D_Refer_Voltage*0xfff/D_VDD_Limit)

#define D_Refer_5V_Voltage (42)  //unit: 0.1 V
#define D_5V_Limit_Code (uint16_t)((uint32_t)D_Refer_5V_Voltage*0xfff/(6*D_ADC_V_MAX))//(uint16_t)((uint32_t)D_Refer_Voltage*0xfff/D_VDD_Limit)

//#define D_VDDMAX_Voltage 32  //unit: 0.1 V
//#define D_VDDMAX_Voltage_Code (uint16_t)((uint32_t)D_VDDMAX_Voltage*0xfff/50)//(uint16_t)((uint32_t)D_VDDMAX_Voltage*0xfff/50)

//============================================================= 
//	Other Define
//=============================================================
//#define D_T_1MS_H   (uint8_t)((0xFFFF-24500/4)/256)    //Define 1ms High byte    
//#define D_T_1MS_L   (uint8_t)(0xFFFF-24500/4)          //Define 1ms Low byte
#define D_T_1MS_H   (0xFFFF-4000)/256  //Define 1ms High byte    //1209
#define D_T_1MS_L   (0xFFFF-4000)%256 
#define D_T_50uS_H   (0xFFFF-200)/256  //Define 1ms High byte    //1209
#define D_T_50uS_L   (0xFFFF-200)%256 
//#define D_T_500uS_H   (uint8_t)((0xFFFF-24500/4/4)/256)    //Define 500us High byte    
//#define D_T_500uS_L   (uint8_t)(0xFFFF-24500/4/4)          //Define 500us Low byte
#define D_T_250us   4000/4
#define D_Delta     4000/4/3
#define D_T_250us_Max   (uint16_t)(D_T_250us+D_Delta)
#define D_T_250us_Min   (uint16_t)(110*4)//(D_T_250us-D_Delta)//E66
#define D_T_Current_One (uint16_t)(D_T_250us-D_Delta*4)
#define D_T_500us   4000/2
#define D_Delta2    4000/2/3
#define D_T_500us_Max   (uint16_t)(680*4)//(D_T_500us+D_Delta2)//3999
#define D_T_500us_Min   (uint16_t)(D_T_500us-D_Delta2)
#define D_T_250us_500us_Middle   (uint16_t)(380*4)//(D_T_500us*3/4)//2066
#define D_T_Sleep   (uint16_t)(D_T_500us*100)
//==========================
//LED Define
#define D_LED_None          0x00
#define D_LED_Charging      0x01
#define D_LED_Fail          0x02
#define D_LED_Complet       0x03
#define D_LED_PING_ID_Phase 0x04
#define D_LED_Learing       0x05
//==========================
#define D_Target_Status_None        0x00
#define D_Target_Status_Complete    0x01
#define D_Target_Status_Error       0x02
#define D_Target_Status_Pinging     0x03
#define D_Target_Status_Pinged      0x04
#define D_Target_Status_Learning    0x05
#define D_Target_Status_Device_Error 0x06
#define D_Target_Status_OverTemperature 0x07
//#define D_Target_Status_Compatiable_no_good    0x08
//#define D_Target_Status_OVP_Charging            0x09
#define D_Target_Status_Control_Error_not_Zero  0x0a
#define D_Target_Status_NoResponse  0x0b
#define D_Target_Reconfigure        0x0c
//#define D_Target_Status_Soft_Error 0x0d
#define D_Target_Status_USB_Power  0x0e
#define D_Target_Status_FOD           0xf
#define D_Target_Status_TI_Complete   0x10
#define D_Target_Status_APPLE_Complete   0x20
//==========================
//Temperature protection
#define D_T_Protection_High     67//70  //unit: degree c
//#define D_T_Protection_Low       0  //unit: degree c  not need
//75
//#define D_T_Protection_High_Code     3253//3050//800//860//797  //unit: degree c   //10bit with  y = 6.3708x  + 352
//80
#define D_T_Protection_High_Code     3356//3050//800//860//797  //unit: degree c   //10bit with  y = 6.3708x  + 352
//88
//#define D_T_Protection_High_Code     3500//3050//800//860//797  //unit: degree c   //10bit with  y = 6.3708x  + 352
////105
//#define D_T_Protection_High_Code     3720//3050//800//860//797  //unit: degree c   //10bit with  y = 6.3708x  + 352
//110
//#define D_T_Protection_High_Code     3767//3050//800//860//797  //unit: degree c   //10bit with  y = 6.3708x  + 352
//#define D_T_Protection_Low_Code      352  //unit: degree c        //not need

#define D_T_Protection_Start_Code     2998//3050//800//860//797  //unit: degree c   //10bit with  y = 6.3708x  + 352

//==========================
//Current protection
#define D_Current_Check_Period    2    //unit: ms  maximum: 254
#define D_Input_Voltage           5    //unit: 1V
#define D_Current_Sensor    50  //unit:m ohm
#define D_Current_Amp_k 18
#define D_Selection_Ping_ID_CONF_Current_Mos_Shoutdown  19//25//20//28//5//15//40//15//30 //unit:0.01V
#define D_Selection_Ping_ID_CONF_Current_Limit_Code     (uint16_t)( (uint32_t)D_Selection_Ping_ID_CONF_Current_Mos_Shoutdown*0xfff/(D_ADC_V_MAX*10)  )
#define D_PowerTransfer_Current_Limit_Percentage 25  //unit:10%  maximum: 31%
#define D_Ping_Current_Protect 1
//#define D_Current_Amp_k 10
//#define D_PowerTransfer_Current_Mos_Shoutdown_Voltage   240 //unit:0.01V
//#define D_PowerTransfer_Current_Mos_Shoutdown_Code    (uint16_t)( (uint32_t)D_PowerTransfer_Current_Mos_Shoutdown_Voltage*0x3ff/500  )
#define D_PowerTransfer_Current_Mos_Limit_Voltage   210//180//200//240 //unit:0.01V
#define D_PowerTransfer_Current_Mos_Limit_Code  (uint16_t)((uint32_t)D_PowerTransfer_Current_Mos_Limit_Voltage*0xfff/(D_ADC_V_MAX*10) )
#define D_PowerTransfer_Current_Mos_12V_Limit_Voltage   190//135//160//180//240 //unit:0.01V
#define D_PowerTransfer_Current_Mos_12V_Limit_Code  (uint16_t)((uint32_t)D_PowerTransfer_Current_Mos_12V_Limit_Voltage*0xfff/(D_ADC_V_MAX*10) )

//#define D_PowerTransfer_Current_Mos_xx_Limit_Voltage   80//240 //unit:0.01V
//#define D_PowerTransfer_Current_Mos_xx_Limit_Code  (uint16_t)((uint32_t)D_PowerTransfer_Current_Mos_xx_Limit_Voltage*0xfff/(D_ADC_V_MAX*10) )

#define D_PowerTransfer_Current_Mos_5V_Limit_Voltage   230//240 //unit:0.01V
#define D_PowerTransfer_Current_Mos_5V_Limit_Code  (uint16_t)((uint32_t)D_PowerTransfer_Current_Mos_5V_Limit_Voltage*0xfff/(D_ADC_V_MAX*10) )
//#define D_AC_Voltage_Limit  1200  //unit : mv
//#define D_AC_Voltage_Limit_Code (uint16_t)( (uint32_t)D_AC_Voltage_Limit*0x3ff/5000 )
#define D_AC_Voltage_MD_Less_Limit  1000//2700//3000//4400//1200  //unit : mv
#define D_AC_Voltage_MD_Less_Limit_Code (uint16_t)( (uint32_t)D_AC_Voltage_MD_Less_Limit*0xfff/(D_ADC_V_MAX*100) )
//#define D_AC_Voltage_MD_Less_Limit_Code 491
#define D_USB_LED_Debounce_Time     30 //unit: ms //only for LED flashing. not effect the decrease power function.
//==========================
//FOD Function
#define D_FOD_Power_efficiency_L1  1000    //unit: mW
#define D_FOD_Power_efficiency_L2  2000    //unit: mW
#define D_FOD_Power_efficiency_L3  3000    //unit: mW
#define D_FOD_Power_efficiency_L4  4000    //unit: mW
#define D_FOD_Power_efficiency_L5  5000    //unit: mW

#define D_FOD_Limit_Power_Under_Time   3500//2500    //unit: mW
#define D_FOD_Limit_Power_Over_Time   3500//2500//500    //unit: mW
#define D_FOD_Limit_Power_Time          15 //unit: second
#define D_FOD_Limit_Power_Time_FOXCON   5  //unit: second
#define D_FOD_Eff_Limit D_TI_11_FOD_Eff_L1  //unit: %
#define D_Received_Power_Debounce 1//2
#define D_Received_Power_Debounce_FOXCON 1
//==========================
//UART Define
#define D_Baudrate      4800  //unit: 100 bps.
#define D_BoudRate_bps  (0x100-24500000/D_Baudrate/4/16)
//==========================
//Production
#define D_ID_BYTE4_N4   0x36
#define D_ID_BYTE5_N4   0xab
#define D_ID_BYTE6_N4   0x69
#define D_ID_BYTE4_GP   0x03
#define D_ID_BYTE5_GP   0x54
#define D_ID_BYTE6_GP   0x45
#define D_GP_Fre_max    205
#define D_GP_Fre_min    110
#define D_GP_Fre_max_code      ((24000/D_GP_Fre_max)*2)//16000/D_GP_Fre_max         //unit :Count
#define D_GP_Fre_min_code      ((24000/D_GP_Fre_min)*2)//16000/D_GP_Fre_min         //unit :Count
#define D_ID_BYTE1_Maxwell_1F   0x00
#define D_ID_BYTE2_Maxwell_1F   0x18
#define D_ID_BYTE4_Maxwell_1F   0x03
#define D_ID_BYTE5_Maxwell_1F   0x1F
#define D_ID_BYTE6_Maxwell_1F   0x94
#define D_Maxwell_1F_Fre_max    205
#define D_Maxwell_1F_Fre_min    110
#define D_Maxwell_1F_Fre_max_code      ((24000/D_Maxwell_1F_Fre_max)*2)//16000/D_Maxwell_1F_Fre_max         //unit :Count
#define D_Maxwell_1F_Fre_min_code      ((24000/D_Maxwell_1F_Fre_min)*2)//16000/D_Maxwell_1F_Fre_min         //unit :Count
#define D_ID_BYTE4_Maxwell_2F   0x03
#define D_ID_BYTE5_Maxwell_2F   0x4B
#define D_ID_BYTE6_Maxwell_2F   0x2F
#define D_Maxwell_2F_Fre_max    205
#define D_Maxwell_2F_Fre_min    110
#define D_Maxwell_2F_Fre_max_code      ((24000/D_Maxwell_2F_Fre_max)*2)//16000/D_Maxwell_2F_Fre_max         //unit :Count
#define D_Maxwell_2F_Fre_min_code      ((24000/D_Maxwell_2F_Fre_min)*2)//16000/D_Maxwell_2F_Fre_min         //unit :Count
#define D_Maxwell_5W_Rule 0
#define D_Maxwell_5W_no 3
#define D_Maxwell_5W_Roll_back  ( D_QI_AI_Sv_Lv0_Code - D_QI_12V_INI_HF_Limit_Code)
#define D_Others_Fre_max    205
#define D_Others_Fre_min    110
#define D_Others_Fre_max_code      ((24000/D_Others_Fre_max)*2)//16000/D_Others_Fre_max         //unit :Count
#define D_Others_Fre_min_code      ((24000/D_Others_Fre_min)*2)//16000/D_Others_Fre_min         //unit :Count
//==========================
//FOD Function
#define D_QI_WPC_Version_12 0x12
#define D_QI_WPC_Version_11 0x11
#define D_QI_WPC_Version_10 0x10
#define D_QI_Received_Power_Status_Nothing  0x0
#define D_QI_Received_Power_Status_active   0x1
#define D_QI_Received_Power_Status_Freeze   0x2
//==========================
//Learning
#define D_Dectect_Current_Threshold_Address 0x7800
#define D_Dectect_upon_limit    0x1
#define D_Dectect_downoff_limit   0x40
//Error
#define D_ErrorFrom_None                0x0
#define D_ErrorFrom_PING_OverCurrent    0xFF
#define D_ErrorFrom_ID_CONF_OverCurrent 0xFE
#define D_ErrorFrom_PowerTransfer_TX_OverCurrent        0xFD
#define D_ErrorFrom_PowerTransfer_TX_OverTemperature    0xFC
#define D_ErrorFrom_PowerTransfer_TX_VirtualWork  0xFB
#define D_Manufactor_none 0x00
#define D_Manufactor_TI 0x01
#define D_Manufactor_sumsung 0x02
#define D_Manufactor_apple 0x03
#define D_Manufactor_QI 0x04
#define D_Manufactor_HuaWei 0x05
#define D_Manufactor_XiaoMi 0x06


#define D_Sound_PORT  P3_0
#define D_AC_Sound_PORT  P3_6
#define D_Sound_ON    1
#define D_Sound_Off   0
#define D_Sound_On_Time 200    //unit: ms maximum  
#define D_Sound_Error_B_Time    200     //unit
#define D_Sound_Type_ON         0x10    
#define D_Sound_Type_Error      0x05    
#define D_PWMIE_Set     CAP2IE|CAP1IE|CAP0IE
#define D_PWMIE_Clear   0x0
#define D_PacketReceiveDebounceTime D_QI_Control_Hold_off_Time_min  
#define D_DecodeByteArray_Buf   26
#define D_RowDecodeByteArray_Buf   28
#define D_Protection_Release_Time   60*1000 //unit:ms
#define D_QI_Received_Power_Status_Nothing  0x0
#define D_QI_Received_Power_Status_active   0x1
#define D_QI_Received_Power_Status_Freeze   0x2
//--Hardware Mosfet driver setting--
#define D_MOSFET_Hardware_None_Driver   0x2
#define D_MOSFET_Hardware_TC4427        0x0
#define D_MOSFET_Hardware_TPS28225D     0x1
#define D_MOSFET_Hardware_8BJT          0x4
#define D_MOSFET_Hardware_FOXCON        0x7
#define D_Remove    0
#define D_Stay      1
#define D_MPID_None   0x0
#define D_MPID_N4   0x2
#define D_Error_Reset_Time 5

#define D_P33_Set_Standby   0
#define D_AC_BuzzerFrequency			2 //unit: khz
#define D_AC_BuzzerFrequencyCode		(0xFFFF-(uint16_t)(24500/D_AC_BuzzerFrequency/12))
#define D_AC_BuzzerFrequencyDutyCode    (0xFFFF-(uint16_t)(24500/D_AC_BuzzerFrequency/12/2))

#define D_CAP_BUFF		26
#endif

