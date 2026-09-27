 

#include "uart_protocol.h"
#include "ring_buffer.h"
#include <string.h>

/* ==================== Parser State ==================== */

typedef enum {
    STATE_IDLE = 0,       /* Waiting for frame header */
    STATE_GOT_HDR,        /* Got 0xA5 header, waiting for length */
    STATE_GOT_LEN,        /* Got header + length, accumulating data */
    STATE_GOT_DATA,       /* Accumulated data + command, waiting for checksum */
} rx_state_t;

static rx_state_t g_state = STATE_IDLE;
static ring_buffer_t *g_rx_rb = NULL;

/* Working buffer for current frame */
static uint8_t g_frame[PROTOCOL_MAX_FRAME_SIZE];
 

 

void uart_protocol_init(ring_buffer_t *rx_buf)
{
    g_state = STATE_IDLE;
    g_rx_rb = rx_buf;
 
    memset(g_frame, 0, sizeof(g_frame));
}

bool uart_protocol_is_receiving(void)
{
    return (g_state != STATE_IDLE);
}

/**
 * Compute checksum over a byte array (low 8 bits of sum)
 */
static uint8_t compute_checksum(const uint8_t *data, uint16_t len)
{
    uint16_t sum = 0;
    for (uint16_t i = 0; i < len; i++) {
        sum += data[i];
    }
    return (uint8_t)(sum & 0xFF);
}


uint8_t calc_checksum;


bool uart_protocol_parse_frame(uart_protocol_frame_t *frame)
{
    if (frame == NULL || g_rx_rb == NULL) {
        return false;
    }

    memset(frame, 0, sizeof(uart_protocol_frame_t));

    /* We need at least 5 bytes: Header + LEN + CMD + Checksum (no payload) */
    if (ring_buffer_available(g_rx_rb) < 4) {
        return false;
    }

    /* Step 1: Read header */
    if (ring_buffer_pop(g_rx_rb, &g_frame[0]) != true) {
        return false;
    }

    /* Must be 0xA5 */
    if (g_frame[0] != PROTOCOL_HDR) {
        g_state = STATE_IDLE;
       
        return false;
    }

    /* Step 2: Read length */
    uint8_t len_byte;
    if (ring_buffer_pop(g_rx_rb, &len_byte) != true) {
        /* Put header back and wait for more */
        ring_buffer_push(g_rx_rb, g_frame[0]);
        return false;
    }

    /* Validate length: LEN >= 1, frame total = LEN + 3 */
    uint8_t payload_len = len_byte - 1;  /* data bytes = LEN - 1 */
    uint16_t total_frame = len_byte + 3; /* header + len + cmd + data + checksum */

    if (len_byte < 1 || len_byte > 253) {
        /* Invalid length, discard */
        g_state = STATE_IDLE;
         
        return false;
    }

 
    g_frame[1] = len_byte;
    g_state = STATE_GOT_LEN;

    
    uint16_t needed = len_byte + 1;  /* cmd + payload + checksum */

    if ((int)ring_buffer_available(g_rx_rb) < needed) {
        /* Put what we have back */
        ring_buffer_push(g_rx_rb, g_frame[1]);
        ring_buffer_push(g_rx_rb, g_frame[0]);
        g_state = STATE_IDLE;
        
        return false;
    }

    /* Step 4: Read command */
    uint8_t cmd;
    if (ring_buffer_pop(g_rx_rb, &cmd) != true) {
        return false;
    }
    g_frame[2] = cmd;
    
    
    
    if(cmd == 0x15)   /*为何了上位机，上位机发送A5 01 15 BB 非标准协议*/
    {
        frame->command = cmd;
        frame->data_len = payload_len;
        return true ;
    }
    
    
    
    /* Step 5: Read payload */
    for (uint8_t i = 0; i < payload_len; i++) {
        uint8_t byte;
        if (ring_buffer_pop(g_rx_rb, &byte) != true) {
            return false;
        }
        g_frame[3 + i] = byte;
    }

    /* Step 6: Read checksum */
    uint8_t recv_checksum;
    if (ring_buffer_pop(g_rx_rb, &recv_checksum) != true) {
        return false;
    }
    if (ring_buffer_pop(g_rx_rb, &recv_checksum) != true) {
        return false;
    }
    g_frame[4 + payload_len] = recv_checksum;

    /* Step 7: Verify checksum */
    /* Checksum covers: header(1) + len(1) + cmd(1) + data(payload_len) */
    uint16_t check_len = 3 + payload_len;
      calc_checksum = compute_checksum(g_frame, check_len);

    if (calc_checksum != recv_checksum) {
        /* Checksum mismatch */
        g_state = STATE_IDLE;
        
        return false;
    }

    /* Step 8: Fill output frame structure */
    frame->command = cmd;
    frame->data_len = payload_len;
    frame->frame_len = (uint8_t)total_frame;
    frame->checksum = recv_checksum;

    if (payload_len > 0) {
        memcpy(frame->data, &g_frame[3], payload_len);
    }

    /* Reset state */
    g_state = STATE_IDLE;
     

    return true;
}


bool uart_protocol_send(ring_buffer_t *tx_buf, uint8_t command,
                        const uint8_t *data, uint8_t data_len)
{
    if (tx_buf == NULL) {
        return false;
    }

    /* Validate data_len: 0 ~ 254 (since LEN = 1 + data_len, max LEN = 253) */
    if (data_len > 253) {
        return false;
    }

    /* LEN field value */
    uint8_t len_field = 1 + data_len;

    /* Total frame size: header(1) + len(1) + cmd(1) + data(data_len) + checksum(1) */
    uint8_t frame_size = 4 + data_len; /* header(1) + len(1) + cmd(1) + data(N) + checksum(1) = 4+N */

    /* Check if TX ring buffer has enough space */
    if (ring_buffer_free_space(tx_buf) < frame_size) {
        return false;
    }

    /* Build frame in temporary buffer */
    uint8_t frame[128];

    /* Header */
    frame[0] = PROTOCOL_HDR;  /* 0xA5 */

    /* Length */
    frame[1] = len_field;

    /* Command */
    frame[2] = command;

    /* Payload data */
    if (data != NULL && data_len > 0) {
        memcpy(&frame[3], data, data_len);
    }

    /* Checksum = sum of all bytes before checksum byte */
    uint8_t checksum = compute_checksum(frame, frame_size - 1);
    frame[frame_size - 1] = checksum;

    /* Push frame to TX ring buffer */
    return ring_buffer_write(tx_buf, frame, frame_size) == frame_size;
}
