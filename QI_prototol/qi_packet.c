/*

   公司：中惠创智
   作者：wujianming
   date:2026.6.27
   
   应用层
   
 * qi_packet.c
 * Qi Wireless Power - Packet Builder Implementation
 *
 * Replicates the original F_Phase_Packet_Control() logic
 * in a clean, modular form.
 *
 * Phase sequence:
 *   PING (3 bytes) -> ID_CONF (5 bytes) -> POWERTRANSFER
 *     -> ControlError (3 bytes) x N times -> RectifiedPower (9 bytes) x 1 time
 *     -> back to ControlError...
 */

#include "qi_packet.h"

#define PACK_DLY 200

/* ===================== Internal State ===================================== */
extern void qi_reset_ecode(void);  /*复位状态，复位之后就可以发送*/
typedef struct
{
    qi_phase_t   phase;
    uint8_t      packet_len;
    uint16_t      data[QI_PACKET_MAX_BYTES];

    /* Power transfer state */
    uint8_t      power_iter;       /* ControlError iteration counter */
    uint8_t      subctrl_idx;      /* 0=ControlError, 1=RectifiedPower */
    uint16_t     t1ms_counter;     /* 1ms tick counter */

    /* User-set data */
    uint16_t     voltage_mv;       /* Rectified voltage in mV */
    uint16_t     current_raw;      /* Raw current reading */
    uint16_t     current_1afull;   /* Calibration: 300 = 1.0A */
    uint8_t      temperature;      /* Coil temperature 0-100 */
    int8_t       control_error;    /* Signed control error value */
    uint8_t      signal_strength;  /* Signal strength level */
    bool         battery_charging; /* Battery charging state */
    uint8_t      error_status;     /* Error code from receiver */

    /* Internal derived values */
    bool         chargeless_flag;  /* Charging active flag */
    bool         ctrlmos_on;       /* MOSFET control state */
    uint8_t      init_counter;     /* Initial charging counter */
    uint8_t      temp_backup;      /* Temperature backup */
} qi_packet_state_t;

static qi_packet_state_t g_qi;

/* ===================== Helper Functions =================================== */

/**
 * @brief  编码格式
 * @note    
 */
static uint16_t qi_encode_byte(uint8_t byte_val)
{
   
   
    uint16_t wResult = 0x400;
	uint8_t bi, bj = 0;
	for (bi = 0; bi <= 7; bi++)
	{
		if (byte_val & 1 << bi)
		{
			bj++;
			wResult |= 1 << (bi + 1);
		}
	}

	if (bj & 0x1)
		;
	else
		wResult |= 0x200;

	return wResult;
    
    
    
     
}


 
    
    
 
 

/**
 * @brief  Build PING packet
 */
static void qi_build_ping(void)
{
    g_qi.data[0] = qi_encode_byte(0x01);
    g_qi.data[1] = qi_encode_byte(0x80);
    g_qi.data[2] = qi_encode_byte(0x81);
    g_qi.packet_len = 3;
}

/**
 * @brief  Build ID_CONF packet
 */
static void qi_build_id_conf(void)
{
    uint8_t volt_h, volt_l;

    volt_h = g_qi.voltage_mv >> 8;
    volt_l = g_qi.voltage_mv & 0xFF;

    g_qi.data[0] = qi_encode_byte(D_QI_Proprietary_0xF2); /* Proprietary ID marker */
    g_qi.data[1] = qi_encode_byte(volt_h);
    g_qi.data[2] = qi_encode_byte(volt_l);
    g_qi.data[3] = qi_encode_byte(volt_h ^ D_QI_Proprietary_0xF2 ^ volt_l ); /* XOR checksum */

    g_qi.packet_len = 4;
}

/**
 * @brief  Build ControlError packet (3 bytes)
 */
static void qi_build_control_error(void)
{
    
    
    if(g_qi.voltage_mv < 3800)
        g_qi.control_error = 0;
    else g_qi.control_error = 0;
     
    g_qi.signal_strength = 0x01;
    g_qi.data[0] =  qi_encode_byte(g_qi.signal_strength);
    g_qi.data[1] = qi_encode_byte((uint8_t)g_qi.control_error);
    g_qi.data[2] = qi_encode_byte(g_qi.control_error ^ (uint8_t)g_qi.signal_strength); /* XOR checksum */

    g_qi.packet_len = 3;
    
    
//    uint8_t volt_h, volt_l, checksum;

//    volt_h = g_qi.voltage_mv >> 8;
//    volt_l = g_qi.voltage_mv & 0xFF;

//    g_qi.data[0] = qi_encode_byte(D_QI_Proprietary_0xF2); /* Proprietary ID marker */
//    g_qi.data[1] = qi_encode_byte(volt_h);
//    g_qi.data[2] = qi_encode_byte(volt_l);
//    g_qi.data[3] = qi_encode_byte(volt_h ^ D_QI_Proprietary_0xF2 ^ volt_l ); /* XOR checksum */

//    g_qi.packet_len = 4;
    
    
}

 
static void qi_build_control_power(void)
{

   uint8_t volt_h, volt_l;
    
    static uint8_t state_flag;
    
    if(state_flag != 0)
    {
        volt_h =  g_qi.voltage_mv >> 8;
        volt_l =  g_qi.voltage_mv & 0x00FF;
        state_flag = 0;
    } 
    else {
       state_flag = 1;
       volt_h =  0x80 | (g_qi.current_raw >> 8);
       volt_l =  g_qi.current_raw & 0x00FF; 
    
    }

    g_qi.data[0] =  qi_encode_byte(D_QI_Proprietary_0xF2); /* Proprietary ID marker */
    g_qi.data[1] = qi_encode_byte(volt_h);
    g_qi.data[2] = qi_encode_byte(volt_l);
    g_qi.data[3] = qi_encode_byte(0X1B);
    g_qi.data[4] = qi_encode_byte(volt_h ^ D_QI_Proprietary_0xF2 ^ volt_l ^ 0X1B); /* XOR checksum */
//     
//     g_qi.data[0] =  qi_encode_byte(D_QI_Proprietary_0xF2); /* Proprietary ID marker */
//     g_qi.data[1] = qi_encode_byte(2);// qi_encode_byte(volt_h);
//     g_qi.data[2] = qi_encode_byte(0X8A);//qi_encode_byte(volt_l);
//     g_qi.data[3] = qi_encode_byte(0X19);
//     g_qi.data[4] = qi_encode_byte(0X63);

    
    
    g_qi.packet_len = 5;
}    


static void qi_build_rectified_power(void)
{
    uint16_t current_scaled;
    uint8_t curr_h, curr_l;
    
    uint8_t xor_val;
   
   
    uint8_t i;
  //  uint8_t bIdentificationPacket_10W[9]=   {0x71,0x11,0x00,0xf2,0xe4,0xc2,0xbf,0x98,0x30}; 
    /* Calculate scaled current: current * Current_1Afull / 300 */
    current_scaled = (uint32_t)g_qi.current_raw ;
    curr_h = (uint8_t)(current_scaled >> 8);
    curr_l = (uint8_t)(current_scaled & 0xFF);

    /* Build raw identification packet */
    uint8_t ident[8];
    
    ident[0] = D_QI_Identification;
    ident[1] = (uint8_t)(g_qi.voltage_mv >> 8);  /* VOLT_H */
    ident[2] = (uint8_t)(g_qi.voltage_mv & 0xFF); /* VOLT_L */
    ident[3] = curr_h;
    ident[4] = curr_l;
    ident[5] = g_qi.temperature;
    ident[6] = 1;//g_qi.error_status;
    ident[7] = 0;//(g_qi.battery_charging) ? 1 : 0;
    
  
    /* Encode each byte and compute XOR checksum */
    xor_val = 0;
    for (  i = 0; i < 8; i++)
    {
        
        
        if( 0 == i )
           xor_val =  ident[i];
        else 
           xor_val = xor_val ^ ident[i]; 
        
        g_qi.data[i] = qi_encode_byte(ident[i]);
        
        
         
    }
  
    g_qi.data[8] = qi_encode_byte(xor_val); /* Checksum */

    
    g_qi.packet_len = 9;
}

/* ===================== Phase Management =================================== */

static void qi_phase_ping(void)
{
    
    
    g_qi.t1ms_counter++;

    /* Transition to ID_CONF after 10ms */
    if (g_qi.t1ms_counter >= 10)
    {
        g_qi.t1ms_counter = 0;
        g_qi.phase = QI_PHASE_ID_CONF;
        qi_build_ping();
        qi_reset_ecode(); 
        
    }
}

static void qi_phase_id_conf(void)
{
    
    g_qi.t1ms_counter++;

    /* Transition to POWERTRANSFER after timeout */
    if (g_qi.t1ms_counter >= 50) /* D_bOverFlowCode - D_QI_tsilent */
    {   
        qi_build_id_conf();
        g_qi.t1ms_counter = 0;
        g_qi.phase = QI_PHASE_POWERTRANSFER;
        g_qi.power_iter = 0;
        g_qi.subctrl_idx = QI_SUBCTRL_ERROR;
        qi_reset_ecode();
    }
}

static void qi_phase_power_transfer(void)
{
    g_qi.t1ms_counter++;
     
    if (g_qi.subctrl_idx == QI_SUBCTRL_ERROR)
        
    {
        if(g_qi.t1ms_counter > PACK_DLY) 
        {   
            g_qi.t1ms_counter = 0;
            /* ControlError sub-phase */
            /* Update control error every few iterations */
            if (g_qi.power_iter < 5)
            {
                  qi_build_control_power();
            }
            else    qi_build_control_error();
                
            //qi_build_control_error();
                
            //qi_build_control_power();
                
           
           qi_reset_ecode();
    
            
             
            /* After enough ControlError packets, switch to RectifiedPower */
            if (g_qi.power_iter++ >= 7)
            {
                g_qi.subctrl_idx = QI_SUBCTRL_RECTIFIED_POWER;
                g_qi.power_iter = 0;
                g_qi.t1ms_counter = 0;
            }
            
        }
    }
    else /* RectifiedPower */
    {
        qi_build_rectified_power();
        qi_reset_ecode();
        //g_qi.t1ms_counter = 120;

        /* After sending RectifiedPower, go back to ControlError */
        if (g_qi.t1ms_counter++ >= PACK_DLY)
        {
            g_qi.subctrl_idx = QI_SUBCTRL_ERROR;
            g_qi.power_iter = 0;
            g_qi.t1ms_counter = 0;
        }
    }
    
}

/* ===================== API========================== */
/**
设置

**/
void qi_packet_set_voltage(uint16_t voltage_mv)
{
    g_qi.voltage_mv = voltage_mv;
}

void qi_packet_set_current(uint16_t current_raw, uint16_t current_1afull)
{
    g_qi.current_raw = current_raw;
    g_qi.current_1afull = current_1afull;
}

void qi_packet_set_temperature(uint8_t temp)
{
    g_qi.temperature = temp;
}

void qi_packet_set_control_error(int8_t error)
{
    g_qi.control_error = error;
}

void qi_packet_set_signal_strength(uint8_t strength)
{
    g_qi.signal_strength = strength;
}

void qi_packet_set_battery_state(bool charging)
{
    g_qi.battery_charging = charging;
}

void qi_packet_set_error_status(uint8_t error_code)
{
    g_qi.error_status = error_code;
}


/**
建立 协议 状态机
**/
void qi_packet_build(void)
{ 
    
     static uint16_t cnt;
         
    switch (g_qi.phase)
    {
        case QI_PHASE_PING:
            
            qi_phase_ping();
        
            break;
        case QI_PHASE_ID_CONF:
            
            qi_phase_id_conf();
        
            break;
        case QI_PHASE_POWERTRANSFER:
            
        
            qi_phase_power_transfer();
             
              
            if(cnt++ > 180)
            {
                //qi_reset_ecode();   
                cnt  = 0;
            
            }
             
        
             break;
        default:
            break;
    }
}




void qi_packet_advance(void)
{
    /* Force advance to next phase */
    switch (g_qi.phase)
    {
        case QI_PHASE_PING:
            g_qi.phase = QI_PHASE_ID_CONF;
            g_qi.t1ms_counter = 0;
            break;
        case QI_PHASE_ID_CONF:
            g_qi.phase = QI_PHASE_POWERTRANSFER;
            g_qi.power_iter = 0;
            g_qi.subctrl_idx = QI_SUBCTRL_ERROR;
            g_qi.t1ms_counter = 0;
            break;
        case QI_PHASE_POWERTRANSFER:
            /* Toggle sub-phase */
            if (g_qi.subctrl_idx == QI_SUBCTRL_ERROR)
                g_qi.subctrl_idx = QI_SUBCTRL_RECTIFIED_POWER;
            else
            {
                g_qi.subctrl_idx = QI_SUBCTRL_ERROR;
                g_qi.power_iter = 0;
            }
            g_qi.t1ms_counter = 0;
            
            
           // g_qi.phase = QI_PHASE_PING;
            break;
        default:
            break;
    }
}

const char* qi_packet_get_phase_name(qi_phase_t phase)
{
    switch (phase)
    {
        case QI_PHASE_PING:           return "PING";
        case QI_PHASE_ID_CONF:        return "ID_CONF";
        case QI_PHASE_POWERTRANSFER:  return "POWERTRANSFER";
        default:                      return "UNKNOWN";
    }
}

uint16_t* qi_packet_get_data(void)
{
    return g_qi.data;
}

uint8_t qi_packet_get_len(void)
{
    return g_qi.packet_len;
}

qi_phase_t qi_packet_get_current_phase(void)
{
    return g_qi.phase;
}

uint8_t qi_packet_get_subctrl_index(void)
{
    return g_qi.subctrl_idx;
}

uint8_t qi_packet_get_power_iter(void)
{
    return g_qi.power_iter;
}

void qi_packet_reset(void)
{
    g_qi.phase = QI_PHASE_PING;
    g_qi.packet_len = 0;
    g_qi.power_iter = 0;
    g_qi.subctrl_idx = QI_SUBCTRL_ERROR;
    g_qi.t1ms_counter = 0;
}
