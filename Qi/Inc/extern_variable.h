
extern bit  fgHalf, fgSendPacketEnd, fgNextbit;

extern u8  bPhase;
extern u16  wx_Packet[9];
extern u8  bPacketArrayIndex,bPacketFinal,bPacketBitIndex;


extern u8  bPhasePacketIndex, bPhasePacketIndexBefore;
extern u8  bPowerTransferPacketIndex;

extern bit  fgChargeCompleteOption;


//Parameter at system.
extern u8  bControlError, bRetified_Power;
extern u8  bEndPowerTransferValue;
extern u8  bInitialCounter;



extern bit   fgEndPowerTransfer;
extern bit  fgCtrlError;


extern u16  wT1_1ms_Counter;

extern bit  fgResetIni;

extern u16  wDC_OUT;

extern u16  wZeroCurrent;

extern bit  fgLoading;

extern u8  bCtrl_Error;

extern u16  wCurrent;

extern u8 bMotorMode;
extern u16 wMotorDelay;

extern ST_ID_DATA_DEF  st_LockID;

extern u16 wDelayTimeCnt;
extern u16 wDelay10msTimeCnt;
extern u8 bDelay1sFlag;
extern u8 bDelay1msFlag;

// 5W
extern const u8 bIdentificationPacket_5W[9];
extern const u8 bConfigurationPacket_5W[];
// 10W
extern  u8 bIdentificationPacket_10W[9];
extern const u8 bConfigurationPacket_10W[];
// 7.5W
extern const u8 bIdentificationPacket_7_5W[9];
extern const u8 bConfigurationPacket_7_5W[];
// 15W
extern const u8 bIdentificationPacket_15W[9];
extern const u8 bConfigurationPacket_15W[];

//decode
extern bit  fgCap2Capture, fgCap2SignalStatus,fgCap2SignalStatusBefore;
extern u8 fgClear;
extern uint16_t wCap2BufBackup;
extern uint16_t wCap2Before;
extern uint8_t bCap2PreambleCounter;
extern uint8_t bCap2PacketStutureIndex;
extern uint8_t bCaptureData2[D_CAP_BUFF];
extern uint8_t bCaptureDataIndex2,bBitIndex2;
extern uint8_t bBitoneCNT2;
extern bit fgCap2HalfClk;
extern bit fgCap2EndHalfBYTE;
extern uint8_t fgPacketStart, fgPacketStop, fgCap2PacketStart,fgCap2PacketStop;
extern uint8_t bCap2ReceiveCodeEnd;
extern uint8_t bDecode[D_CAP_BUFF];
extern uint8_t bDecode2[D_CAP_BUFF];
extern uint8_t bDecodeIndex2;
extern uint8_t bPacketReceiveDebounce;

extern uint8_t bDecodeIndex;
