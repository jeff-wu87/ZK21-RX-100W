/**
 * @file uart_protocol.h
 * @brief Protocol parser for UART frame handling
 *
 * Protocol: [0xA5][LEN][CMD][Data...][Checksum]
 *   Header(1B) Length(1B) Command(1B) Payload(LEN-1B) Checksum(1B)
 *   LEN = 1 + payload_bytes (>= 1)
 *   Checksum = (Header + LEN + CMD + Data...) & 0xFF
 *   Total frame = LEN + 3 bytes
 */

#ifndef UART_PROTOCOL_H
#define UART_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include "ring_buffer.h"

/* ==================== Protocol Constants ==================== */

#define PROTOCOL_HDR              0xA5
#define PROTOCOL_MAX_FRAME_SIZE   256
#define FRAME_DATA_LEN 32
/* ==================== Frame Structure ==================== */

typedef struct {
    uint8_t  command;       /* Command byte */
    uint8_t  data_len;      /* Payload length (LEN-1) */
    uint8_t  data[FRAME_DATA_LEN];     /* Payload data */
    uint8_t  frame_len;     /* Total frame length (LEN + 3) */
    uint8_t  checksum;      /* Received checksum byte */
} uart_protocol_frame_t;

/* ==================== Parser API ==================== */

void uart_protocol_init(ring_buffer_t *rx_buf);
bool uart_protocol_parse_frame(uart_protocol_frame_t *frame);
bool uart_protocol_is_receiving(void);

/* ==================== Sender API ==================== */

/**
 * Build and send a 0xA5 protocol frame
 * @param tx_buf   Pointer to TX ring buffer
 * @param command  Command byte
 * @param data     Payload data (NULL if no payload)
 * @param len      Payload length (0 = no payload, LEN=1)
 * @return true    Frame pushed to ring buffer
 */
bool uart_protocol_send(ring_buffer_t *tx_buf, uint8_t command,
                        const uint8_t *data, uint8_t len);

#endif /* UART_PROTOCOL_H */
