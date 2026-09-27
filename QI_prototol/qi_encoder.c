/*


   公司：中惠创智
   作者：wujianming
   date:2026.6.27
   
   解码发包
   
   
   每 bit 占 500us（两个250us半周期）：

   bit '1'：COM翻转2次（first half + second half）→ 强信号
   bit '0'：COM翻转1次（仅 first half）            → 弱信号

   数据包：[Header: 3个'1'] [Data: N字节×11位] [End: 清COM]


 * qi_encoder.c
 * Qi Wireless Power - Slot Encoder Implementation
 *
 * Replicates the original Fun_SendCode_250us() function
 * in a clean, hardware-independent form.
 *
 * Differential Manchester Encoding:
 *
 *   Slot timing: 500us total = 2 x 250us half-periods
 *
 *   Slot N processing (called every 250us):
 *
 *   Half-period 0 (fg_half == false):
 *     1. Flip COM/COM1 unconditionally
 *     2. Process bit index, update fg_nextbit
 *     3. Toggle fg_half -> true
 *
 *   Half-period 1 (fg_half == true):
 *     1. If fg_nextbit (data bit == '1'), flip again
 *     2. Process bit position for next slot
 *     3. Toggle fg_half -> false
 *
 *   This gives differential encoding:
 *     bit '1' -> two transitions in one slot
 *     bit '0' -> one transition in one slot
 *
 * Packet transmission order:
 *   [Header: 3 bits of '1'] [Data: N bytes x 11 bits] [End]
 *
 *   Each byte is transmitted LSB first.
 *   End marker: after last bit, clear COM/COM1.
 */

#include "qi_encoder.h"
#include "qi_hw.h"

/* ===================== Internal State ===================================== */
#define QI_SEND_HEADER_BITS 3
#define QI_SEND_DAT_BITS   11 
typedef enum
{
    QI_ENC_IDLE = 0,
    QI_ENC_HEADER,
    QI_ENC_DATA,
    QI_ENC_END
} qi_enc_phase_t;

typedef struct
{
    qi_enc_phase_t  phase;       // STATE IDE
    const uint16_t *packet;      /* Pointer to data bytes */
    uint16_t        nbytes;      /* Total bytes in packet */

    uint8_t        byte_idx;   /* Current byte index (1-based, 0=header) */
    uint8_t        bit_idx;    /* Current bit index (0-10 for 11 bits) */
    bool           fg_half;    /* 0 = first half, 1 = second half */
    bool           fg_nextbit; /* Output flag: true = flip in second half */
    bool           fsendctl;   /* COM control: true=COM, false=COM1 */
    bool           done;       /* Transmission complete */
} qi_encoder_state_t;

static qi_encoder_state_t g_enc;

/* ===================== Internal Functions ================================= */

/**
 * @brief  Select and flip COM or COM1 pin
 *         Based on fsendctl bit (alternates per packet)
 */
static inline void encoder_flip_selected(void)
{
    if (g_enc.fsendctl)
        qi_hw_com_flip();
    else
        qi_hw_com1_flip();
}

/**
 * @brief  Set selected pin to specific level
 *         Used for end marker
 
 */
static inline void encoder_set_selected(bool high)
{
    if (g_enc.fsendctl)
    {
        if (high) qi_hw_com_set();
        else      qi_hw_com_clear();
    }
    else
    {
        if (high) qi_hw_com1_set();
        else      qi_hw_com1_clear();
    }
}

/**
 * @brief  Extract bit value from data byte (LSB first)
 */
static inline bool encoder_get_bit(uint16_t byte_val, uint8_t bit_pos)
{   
    return (byte_val & (0X01 << bit_pos)) ? true : false;
 
}

/* ===================== Public API Implementation ========================== */
/*
     初始化。。。。。
*/
void qi_encoder_init(const uint16_t *packet, uint8_t nbytes)
{
    if (packet == 0 || nbytes == 0)
        return;

    g_enc.phase     = QI_ENC_HEADER;
    g_enc.packet    =  packet;
    g_enc.nbytes    = nbytes;
    g_enc.byte_idx  = 0;
    g_enc.bit_idx   = 0;
    g_enc.fg_half   = true;
    g_enc.fg_nextbit = true;   /* Header bits are always '1' */
    
   // g_enc.fsendctl  = false;    /* Start on COM channel */
   // 
    if(g_enc.fsendctl == 1)
         g_enc.fsendctl = 0;
    else  g_enc.fsendctl = 1;
    
    
    g_enc.done      = false;
}



void qi_reset_ecode() // using 1
{
	 
     
    g_enc.phase     = QI_ENC_IDLE;
    
    g_enc.byte_idx  = 0;
    g_enc.bit_idx   = 0;
    g_enc.fg_half   = true;
    g_enc.fg_nextbit = true;   /* Header bits are always '1' */
    
//    if(g_enc.fsendctl == 1)
//         g_enc.fsendctl = 0;
//    else  g_enc.fsendctl = 1;
    
     //g_enc.fsendctl  = true;    /* Start on COM channel */
    
    
	 
}


/**
     250us执行一次 

**/

#define QI_SEND_HEADER  16

void qi_encoder_tick(void)
{
    bool half = g_enc.fg_half;
    static uint16_t data_byte;
    
    if((g_enc.phase != QI_ENC_END) && (g_enc.phase != QI_ENC_IDLE))
    {
        if(half)
        {
            /* ============= Second Half of Slot ============= */

            /* Only flip if data bit is '1' */
            if (g_enc.fg_nextbit)
            {
                encoder_flip_selected();
            }

            g_enc.fg_half = false;
        }
        else  
        {    
            
             g_enc.fg_half = true;
            
            /* ============= First Half of Slot ============= */
          
            /* Always flip selected COM pin */
            encoder_flip_selected();

            switch (g_enc.phase)
            {
                case QI_ENC_HEADER:
                    /* Count header bits (3 bits, all '1') */
                    g_enc.bit_idx++;
                    if (g_enc.bit_idx >= QI_SEND_HEADER)
                    {
                        /* Header complete, switch to data */
                        g_enc.bit_idx = 0;
                        g_enc.byte_idx = 1;      /* 1-based: first data byte */
                        g_enc.phase = QI_ENC_DATA;
                        g_enc.fg_nextbit = false ;
                    }
                    else  g_enc.fg_nextbit = true;  /* Header always outputs '1' */
                    
                    break;

                case QI_ENC_DATA:
                    /* Count data bits (11 per byte) */
                    g_enc.bit_idx++;
                    if (g_enc.bit_idx >= QI_SEND_DAT_BITS)
                    {
                        /* Byte complete */
                        g_enc.fg_nextbit = false;
                        if (g_enc.byte_idx >= g_enc.nbytes)
                        {
                            /* Last byte complete -> end marker */
                            g_enc.phase = QI_ENC_END;
                            g_enc.fg_nextbit = false;
                            /* End: clear selected pin */
                            encoder_set_selected(false);
                        }
                        else
                        {  
                            
                            g_enc.bit_idx = 0;
                            /* Next byte */
                            g_enc.byte_idx++;
                        }
                    }
                    else
                    {
                        /* Get bit value for this position (LSB first) */
                        data_byte = g_enc.packet[g_enc.byte_idx - 1];  
                        g_enc.fg_nextbit = encoder_get_bit(data_byte, g_enc.bit_idx);
                    }
                    break;

                default:
                    break;
            }

             
        }
    }
    else{
    encoder_set_selected(false );
    
    }
     
}



bool qi_encoder_is_done(void)
{
    return g_enc.done || (g_enc.phase == QI_ENC_END);
}

bool qi_encoder_is_idle(void)
{
    
    if(g_enc.phase == QI_ENC_IDLE)
    {   
        
        return 1;
    }
    else {
        return 0;
    
    }
   // return (g_enc.phase == QI_ENC_IDLE);
}
