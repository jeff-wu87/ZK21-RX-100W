#ifndef QI_TX_PROTOCOL_H
#define QI_TX_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Qi TX telemetry sender.
 *
 * Call qi_tx_init() once.
 * Call qi_tx_set_telemetry() whenever ADC / power values update.
 * Call qi_tx_start() to begin periodic packets.
 * Call qi_tx_250us_tick() from the 250 us timer interrupt.
 * Call qi_tx_1ms_tick() from the 1 ms timer interrupt.
 */

#ifndef QI_TX_PACKET_MAX_BYTES
#define QI_TX_PACKET_MAX_BYTES 16u
#endif


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



typedef enum
{
    QI_TX_STATE_IDLE = 0,
    QI_TX_STATE_WAIT_PERIOD,
    QI_TX_STATE_SENDING
} qi_tx_state_t;

typedef struct
{
    uint16_t voltage_mv;
    uint16_t current_ma;
    uint8_t temperature_c;
    uint16_t power_mw;
} qi_tx_telemetry_t;

typedef struct
{
    uint16_t packet_period_ms;
    uint8_t preamble_bits;
    bool alternate_output_channel;
} qi_tx_config_t;

void qi_tx_init(const qi_tx_config_t *config);
void qi_tx_start(void);
void qi_tx_stop(void);

void qi_tx_set_telemetry(uint16_t voltage_mv,
                         uint16_t current_ma,
                         uint8_t temperature_c,
                         uint16_t power_mw);

void qi_tx_1ms_tick(void);
void qi_tx_250us_tick(void);
uint16_t qi_crc16_modbus(const uint8_t *data, uint8_t len);
bool qi_tx_is_busy(void);
qi_tx_state_t qi_tx_get_state(void);
uint8_t qi_tx_get_last_packet(uint8_t *out, uint8_t out_size);
 
#endif
