/*
 ******************************************************************************
 * @file     qi_tx_protocol.c
 * @author  wu.jianming
 * @version V1.0.0
 * @date    2026-08-16
 * @brief    qi协议发送端
 * 
 ******************************************************************************
*/

#include "qi_tx_protocol.h"
#include "qi_tx_hw.h"

#define QI_TX_PACKET_ID_TELEMETRY 0xF2u
#define QI_TX_BITS_PER_FRAME      11u

typedef enum
{
    QI_TX_ENC_IDLE = 0,
    QI_TX_ENC_PREAMBLE,
    QI_TX_ENC_DATA,
    QI_TX_ENC_END
} qi_tx_encoder_phase_t;


/* Phase definitions */
typedef enum
{
    QI_PHASE_PING = 0,
    QI_PHASE_ID_CONF,
    QI_PHASE_POWERTRANSFER,
    QI_PHASE_MAX_ENUM
} qi_app_phase_t;



typedef struct
{
    qi_tx_config_t config;
    qi_tx_telemetry_t telemetry;
    qi_tx_state_t state;

    uint16_t period_counter_ms;
    bool enabled;

    uint8_t raw_packet[QI_TX_PACKET_MAX_BYTES];
    uint16_t encoded_packet[QI_TX_PACKET_MAX_BYTES];
    uint8_t packet_len;

    qi_tx_encoder_phase_t enc_phase;
    qi_tx_hw_channel_t enc_channel;
    bool enc_half;
    bool enc_next_bit;
    uint8_t enc_preamble_index;
    uint8_t enc_byte_index;
    uint8_t enc_bit_index;
} qi_tx_context_t;

static qi_tx_context_t g_qi_tx;

#if 0
static uint8_t qi_tx_xor(const uint8_t *data, uint8_t len)
{
    uint8_t i;
    uint8_t value;

    value = 0u;
    for (i = 0u; i < len; i++)
    {
        value ^= data[i];
    }

    return value;
}
#endif


uint16_t qi_crc16_modbus(const uint8_t *data, uint8_t len)
{
    uint16_t crc = 0xFFFF;
    uint8_t i,j;
    for(i=0; i<len; i++)
    {
        crc ^= data[i];
        for(j=0; j<8; j++)
        {
            if(crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    return crc;
}


uint8_t crc8_ccitt_bit(const uint8_t *data, uint16_t len, uint8_t init)
{
    uint8_t crc = init;
    for(uint16_t i=0; i<len; i++)
    {
        crc ^= data[i];
        for(uint8_t bit=0; bit<8; bit++)
        {
            if(crc & 0x80)
            {
                crc = (crc << 1) ^ 0x07;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}

static uint16_t qi_tx_encode_frame(uint8_t byte_value)
{
    uint8_t i;
    uint8_t ones;
    uint16_t frame;

    frame = 0x0400u; /* bit10: stop = 1, bit0: start = 0 */
    ones = 0u;

    for (i = 0u; i < 8u; i++)
    {
        if ((byte_value & (uint8_t)(1u << i)) != 0u)
        {
            frame |= (uint16_t)(1u << (i + 1u));
            ones++;
        }
    }

    if ((ones & 1u) == 0u)
    {
        frame |= 0x0200u; /* bit9: odd parity */
    }

    return frame;
}

static void qi_tx_build_telemetry_packet(void)
{
    uint8_t i;
    uint16_t checksum;
    uint16_t voltage;
    uint16_t current;
    uint16_t power;

    voltage = g_qi_tx.telemetry.voltage_mv;
    current = g_qi_tx.telemetry.current_ma;
    power = g_qi_tx.telemetry.power_mw;

    g_qi_tx.raw_packet[0] =  D_QI_Proprietary_0xE2;//QI_TX_PACKET_ID_TELEMETRY;
    g_qi_tx.raw_packet[1] = (uint8_t)(voltage & 0xFFu);     
    g_qi_tx.raw_packet[2] = (uint8_t)(voltage >> 8); 
    g_qi_tx.raw_packet[3] = (uint8_t)(current & 0xFFu);
    g_qi_tx.raw_packet[4] = (uint8_t)(current  >> 8);
    g_qi_tx.raw_packet[5] = (uint8_t)(power & 0xFFu);
    g_qi_tx.raw_packet[6] = (uint8_t)(power >> 8);
    g_qi_tx.raw_packet[7] = g_qi_tx.telemetry.temperature_c;

    
    
   // checksum = qi_tx_xor(g_qi_tx.raw_packet, 8u);
       
    checksum = qi_crc16_modbus(g_qi_tx.raw_packet, 8u);
    
    g_qi_tx.raw_packet[8] = checksum >> 8;
    g_qi_tx.raw_packet[9] = checksum &0x00ff;
    g_qi_tx.packet_len = 10u;
    
    
    
    for (i = 0u; i < g_qi_tx.packet_len; i++)
    {
        g_qi_tx.encoded_packet[i] = qi_tx_encode_frame(g_qi_tx.raw_packet[i]);
    }
}

/**
上电发固定命令
**/
static void qi_tx_build_ping(void)
{
    g_qi_tx.encoded_packet[0] = qi_tx_encode_frame(0x01);
    g_qi_tx.encoded_packet[1] = qi_tx_encode_frame(0x82);
    g_qi_tx.encoded_packet[2] = qi_tx_encode_frame(0x81);
    g_qi_tx.packet_len = 3u;
}

/*
 交替发电压，电流
电流|0X800； 温度不发
**/
static void qi_tx_build_control_power(void)
{

     uint8_t volt_h, volt_l;
    static uint8_t f;
    uint16_t checksum;
    uint8_t i;
    uint16_t voltage;
    uint16_t current;
    uint16_t power;

    
    if(f  == 0)
    {   voltage = g_qi_tx.telemetry.voltage_mv;
        
        volt_h =  (uint8_t)(voltage >> 8);
        volt_l =  (uint8_t)(voltage & 0x00FF);
        f = 1;
    }
    else if(f == 1)
    { 
         
        current = g_qi_tx.telemetry.current_ma;
        volt_h =  (uint8_t) (current >> 8);
        volt_l =  (uint8_t)(current & 0x00FF);
        volt_h |= 0x80;
        f = 2;
    }
    else if(f == 2) 
    {
        
         
        volt_l =   g_qi_tx.telemetry.temperature_c;
        volt_h  =  0xff;
        f = 0;
    }

    g_qi_tx.raw_packet[0]  = D_QI_Proprietary_0xF2;
    //g_qi_tx.raw_packet[0] = (g_qi_tx.telemetry.temperature_c & 0x00ff);// D_QI_Proprietary_0xF2; /* Proprietary ID marker */
    g_qi_tx.raw_packet[1] = volt_l;
    g_qi_tx.raw_packet[2] = volt_h;
     
    
    
    checksum = crc8_ccitt_bit(g_qi_tx.raw_packet, 3u,0);
    g_qi_tx.raw_packet[3] = checksum;
   
    g_qi_tx.packet_len = 4u;
    
//    checksum =  qi_crc16_modbus(g_qi_tx.raw_packet, 3u);
//    g_qi_tx.raw_packet[3] = (checksum >> 8);
//    g_qi_tx.raw_packet[4] = (checksum &0x00ff);
//    g_qi_tx.packet_len = 5u;
    
    for (i = 0u; i < g_qi_tx.packet_len; i++)
    {
        g_qi_tx.encoded_packet[i] = qi_tx_encode_frame(g_qi_tx.raw_packet[i]);
    }
}


static bool qi_tx_get_encoded_bit(void)
{
    uint16_t frame;

    frame = g_qi_tx.encoded_packet[g_qi_tx.enc_byte_index];
    return ((frame & (uint16_t)(1u << g_qi_tx.enc_bit_index)) != 0u);
}

/*
状态清零，发送数据

*/
static void qi_tx_rest_send(void)  
{
    if (g_qi_tx.config.alternate_output_channel)
    {
        if (g_qi_tx.enc_channel == QI_TX_HW_CH_COM)
        {
            g_qi_tx.enc_channel = QI_TX_HW_CH_COM1;
        }
        else
        {
            g_qi_tx.enc_channel = QI_TX_HW_CH_COM;
        }
    }
    else
    {
        g_qi_tx.enc_channel = QI_TX_HW_CH_COM;
    }

    g_qi_tx.enc_phase = QI_TX_ENC_PREAMBLE;
    g_qi_tx.enc_half = true;
    g_qi_tx.enc_next_bit = true;
    g_qi_tx.enc_preamble_index = 0u;
    g_qi_tx.enc_byte_index = 0u;
    g_qi_tx.enc_bit_index = 0u;
    g_qi_tx.state = QI_TX_STATE_SENDING;

}

static void qi_tx_encoder_start(void)
{
   static qi_app_phase_t run_phase  = QI_PHASE_PING;
  // static uint8_t cnt;
   static uint16_t ping_time;
    uint16_t voltage;
    
    switch (run_phase)
    {
        case QI_PHASE_PING:                                                                 
              voltage = g_qi_tx.telemetry.voltage_mv;
                if(voltage > 1500){
                    
                    qi_tx_build_ping();
                    
                }
                else g_qi_tx.packet_len = 0u;
                if((++ping_time > 3 ) && (voltage > 2000))
                {
                    run_phase  = QI_PHASE_ID_CONF;
                    g_qi_tx.config.packet_period_ms = 200u;   //200ms发一次
                    ping_time = 0;
                }
                
                g_qi_tx.config.packet_period_ms = 5u;
            
            break;
        
        case QI_PHASE_ID_CONF:
            g_qi_tx.config.packet_period_ms = 200u;   //200ms发一次
            qi_tx_build_control_power();
             run_phase  = QI_PHASE_ID_CONF; 
            break;
        case QI_PHASE_POWERTRANSFER:
            
//            if(cnt < 6)
//            {
//                qi_tx_build_control_power();
//            }
//            else 
//            {
//                qi_tx_build_telemetry_packet();
//                cnt = 0;
//            }
//            cnt ++;
            
             
             
             qi_tx_build_telemetry_packet();
        
             break;
        default:
            break;
    }
    
    
    
   // qi_tx_build_telemetry_packet();
    qi_tx_rest_send(); 
    
}

static void qi_tx_encoder_finish(void)
{
    qi_tx_hw_disable();
    g_qi_tx.enc_phase = QI_TX_ENC_IDLE;
    g_qi_tx.state = QI_TX_STATE_WAIT_PERIOD;
    g_qi_tx.period_counter_ms = 0u;
}

static void qi_tx_encoder_advance_bit(void)
{
    if (g_qi_tx.enc_phase == QI_TX_ENC_PREAMBLE)
    {
        g_qi_tx.enc_preamble_index++;
        if (g_qi_tx.enc_preamble_index >= g_qi_tx.config.preamble_bits)
        {
            g_qi_tx.enc_phase = QI_TX_ENC_DATA;
            g_qi_tx.enc_byte_index = 0u;
            g_qi_tx.enc_bit_index = 0u;
            g_qi_tx.enc_next_bit = qi_tx_get_encoded_bit();
        }
        else
        {
            g_qi_tx.enc_next_bit = true;
        }
        return;
    }

    if (g_qi_tx.enc_phase == QI_TX_ENC_DATA)
    {
        g_qi_tx.enc_bit_index++;
        if (g_qi_tx.enc_bit_index >= QI_TX_BITS_PER_FRAME)
        {
            g_qi_tx.enc_bit_index = 0u;
            g_qi_tx.enc_byte_index++;
            if (g_qi_tx.enc_byte_index >= g_qi_tx.packet_len)
            {
                g_qi_tx.enc_next_bit = false;
                g_qi_tx.enc_phase = QI_TX_ENC_END;
                return;
            }
        }

        g_qi_tx.enc_next_bit = qi_tx_get_encoded_bit();
    }
}

void qi_tx_init(const qi_tx_config_t *config)
{
    qi_tx_hw_init();

    if (config != 0)
    {
        g_qi_tx.config = *config;
    }
    else
    {
        g_qi_tx.config.packet_period_ms = 150u;
        
        g_qi_tx.config.preamble_bits = 16u;
        g_qi_tx.config.alternate_output_channel = true;
    }

    if (g_qi_tx.config.packet_period_ms == 0u)
    {
        g_qi_tx.config.packet_period_ms = 1u;
    }

    if (g_qi_tx.config.preamble_bits == 0u)
    {
        g_qi_tx.config.preamble_bits = 16u;
    }

    g_qi_tx.enabled = false;
    g_qi_tx.state = QI_TX_STATE_IDLE;
    g_qi_tx.period_counter_ms = 0u;
    g_qi_tx.packet_len = 0u;
    g_qi_tx.enc_phase = QI_TX_ENC_IDLE;
    g_qi_tx.enc_channel = QI_TX_HW_CH_COM1;
    qi_tx_hw_disable();
}

void qi_tx_start(void)
{
    g_qi_tx.enabled = true;
    g_qi_tx.period_counter_ms = g_qi_tx.config.packet_period_ms  + 3;
    g_qi_tx.state = QI_TX_STATE_WAIT_PERIOD;
}

void qi_tx_stop(void)
{
    g_qi_tx.enabled = false;
    g_qi_tx.state = QI_TX_STATE_IDLE;
    g_qi_tx.enc_phase = QI_TX_ENC_IDLE;
    qi_tx_hw_disable();
}

void qi_tx_set_telemetry(uint16_t voltage_mv,
                         uint16_t current_ma,
                         uint8_t temperature_c,
                         uint16_t power_mw)
{
    g_qi_tx.telemetry.voltage_mv = voltage_mv;
    g_qi_tx.telemetry.current_ma = current_ma;
    g_qi_tx.telemetry.temperature_c = temperature_c;
    g_qi_tx.telemetry.power_mw = power_mw;
}

void qi_tx_1ms_tick(void)
{
    if (!g_qi_tx.enabled)
    {
        return;
    }

    if (g_qi_tx.state == QI_TX_STATE_SENDING)
    {
        return;
    }

    if (g_qi_tx.period_counter_ms < g_qi_tx.config.packet_period_ms)
    {
        g_qi_tx.period_counter_ms++;
        return;
    }
     
    qi_tx_encoder_start();
}

void qi_tx_250us_tick(void)
{
    if ((!g_qi_tx.enabled) || (g_qi_tx.enc_phase == QI_TX_ENC_IDLE))
    {
        return;
    }

    if (g_qi_tx.enc_phase == QI_TX_ENC_END)
    {
        qi_tx_encoder_finish();
        return;
    }

    if (g_qi_tx.enc_half)
    {
        if (g_qi_tx.enc_next_bit)
        {
            qi_tx_hw_toggle(g_qi_tx.enc_channel);
        }
        g_qi_tx.enc_half = false;
    }
    else
    {
        qi_tx_hw_toggle(g_qi_tx.enc_channel);
        qi_tx_encoder_advance_bit();
        g_qi_tx.enc_half = true;
    }
}

 


bool qi_tx_is_busy(void)
{
    return (g_qi_tx.state == QI_TX_STATE_SENDING);
}

qi_tx_state_t qi_tx_get_state(void)
{
    return g_qi_tx.state;
}

uint8_t qi_tx_get_last_packet(uint8_t *out, uint8_t out_size)
{
    uint8_t i;

    if ((out == 0) || (out_size < g_qi_tx.packet_len))
    {
        return 0u;
    }

    for (i = 0u; i < g_qi_tx.packet_len; i++)
    {
        out[i] = g_qi_tx.raw_packet[i];
    }

    return g_qi_tx.packet_len;
}


