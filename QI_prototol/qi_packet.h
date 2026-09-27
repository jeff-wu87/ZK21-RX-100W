/*
 * qi_packet.h
 * Qi Wireless Power - Packet Builder
 *
 * Builds wx_Packet[] according to Qi protocol phases:
 *   PING       -> [0x01, 0x80, 0x81]
 *   ID_CONF    -> [0xF2, VOLT_H, VOLT_L, TEMP, CHECKSUM]
 *   POWERTRANSFER
 *     - ControlError   -> [SIGNAL_STRENGTH, CONTROL_ERROR, CHECKSUM]
 *     - RectifiedPower -> [VOLT_H, VOLT_L, CURR_H, CURR_L, TEMP, ERR, 0x01, CHECKSUM]
 *
 * Interface:   从zk16的状态解析过来
 *   qi_packet_build()   -> fills wx_Packet[] based on current phase
 *   qi_packet_get_data() -> returns pointer to built packet
 *   qi_packet_get_len()  -> returns packet byte count
 *   qi_packet_advance()  -> advances to next phase
 *
 * User sets data via qi_packet_set_*() functions.
 * Then calls qi_packet_build() periodically (every 1ms).
 */

#ifndef QI_PACKET_H
#define QI_PACKET_H

#include <stdint.h>
#include <stdbool.h>

/* ===================== Qi Protocol Constants ============================== */

#define QI_PHASE_MAX          4
#define QI_PACKET_MAX_BYTES   15   /* max bytes per packet (RectifiedPower = 9 bytes) */
#define QI_HEADER_BITS        3    /* number of header bits */
#define QI_DATA_BITS_PER_BYTE 11   /* bits per data byte in slot encoding */

/* Phase definitions */
typedef enum
{
    QI_PHASE_PING = 0,
    QI_PHASE_ID_CONF,
    QI_PHASE_POWERTRANSFER,
    QI_PHASE_MAX_ENUM
} qi_phase_t;

/* Power transfer sub-phase (ControlError vs RectifiedPower) */
typedef enum
{
    QI_SUBCTRL_ERROR = 0,
    QI_SUBCTRL_RECTIFIED_POWER,
    QI_SUBCTRL_MAX
} qi_subctrl_t;



/* Signal strength levels (Qi spec) */
#define QI_SIGNAL_STRENGTH_0      0x00
#define QI_SIGNAL_STRENGTH_1      0x01
#define QI_SIGNAL_STRENGTH_2      0x02
#define QI_SIGNAL_STRENGTH_3      0x03

/* ===================== User Data Interface ================================
 *
 * Before calling qi_packet_build(), set these values:
 */

/**
 * @brief  Set rectifier voltage (mV), e.g. 1260 for 12.6V
 */


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



void qi_packet_set_voltage(uint16_t voltage_mv);

/**
 * @brief  Set output current (scaled by Current_1Afull)
 *         Raw current value, scales internally by Current_1Afull/300
 * @param  current_raw  raw current reading
 * @param  current_1afull  calibration factor (300 = 1.0A)
 */
void qi_packet_set_current(uint16_t current_raw, uint16_t current_1afull);

/**
 * @brief  Set coil temperature (0-100 scale)
 */
void qi_packet_set_temperature(uint8_t temp);

/**
 * @brief  Set control error value (signed, range -127~127)
 *         Bit 7 is power flag (>50% power set to 1)
 */
void qi_packet_set_control_error(int8_t error);

/**
 * @brief  Set signal strength level
 */
void qi_packet_set_signal_strength(uint8_t strength);

/**
 * @brief  Set battery state (charging/discharging flag)
 */
void qi_packet_set_battery_state(bool charging);

/**
 * @brief  Set error status from receiver
 */
void qi_packet_set_error_status(uint8_t error_code);

/* ===================== Public API ========================================= */

/**
 * @brief  Build wx_Packet[] for current phase
 * @note   Call every 1ms. Fills wx_Packet[] and sets packet_len.
 */
void qi_packet_build(void);

/**
 * @brief  Advance to next phase
 *         PING -> ID_CONF -> POWERTRANSFER (ControlError) -> POWERTRANSFER (RectifiedPower) -> ControlError...
 */
void qi_packet_advance(void);

/**
 * @brief  Get current phase name (for debug)
 */
const char* qi_packet_get_phase_name(qi_phase_t phase);

/* ===================== Access Built Packet ================================
 *
 * After qi_packet_build(), access via:
 */

/**
 * @brief  Get pointer to built packet data
 * @note   Valid until next qi_packet_build() call
 */
uint16_t* qi_packet_get_data(void);

/**
 * @brief  Get packet byte count (set by qi_packet_build)
 */
uint8_t qi_packet_get_len(void);

/**
 * @brief  Get current phase
 */
qi_phase_t qi_packet_get_current_phase(void);

/**
 * @brief  Get sub-control index (0 = ControlError, 1 = RectifiedPower)
 */
uint8_t qi_packet_get_subctrl_index(void);

/**
 * @brief  Get power transfer iteration count (how many ControlError packets sent)
 */
uint8_t qi_packet_get_power_iter(void);

/**
 * @brief  Reset all internal counters to initial state
 */
void qi_packet_reset(void);

#endif /* QI_PACKET_H */
