/**
 * @file uart_handler.h
 * @brief UART handler - integrates ring buffer and protocol layer
 *
 * Protocol: [0xA5][LEN][CMD][Data...][Checksum]
 *
 * @par Usage
 *   @code
 *   uint8_t rx_buf[256], tx_buf[256];
 *   ring_buffer_t rx_rb, tx_rb;
 *
 *   ring_buffer_init(&rx_rb, rx_buf, sizeof(rx_buf));
 *   ring_buffer_init(&tx_rb, tx_buf, sizeof(tx_buf));
 *   uart_handler_init(&rx_rb, &tx_rb);
 *   @endcode
 */

#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include "ring_buffer.h"

/* ==================== Command IDs ==================== */

#define CMD_BASIC_STATUS      1    /* ?????? */
#define CMD_FAULT_INFO        2    /* ???? */
#define CMD_INTERN_STATE      3    /* ?????? */
#define CMD_START_STOP        4    /* ???? */
#define CMD_SERIAL_NUMBER     5    /* ????? */

#define CMD_VOL_DAT    0X15
/* ==================== Application Callbacks ==================== */

extern void module_basicstatus_infamation(void);
extern void module_fault_infamation(void);
extern void module_internstate_infamation(void);
extern void module_startstop_infamation(void);
extern void module_serialnum_infamation(void);

/* ==================== UART Handler API ==================== */

/**
 * Initialize UART handler
 * @param rx_rb  Pointer to RX ring buffer
 * @param tx_rb  Pointer to TX ring buffer
 */
void uart_handler_init(ring_buffer_t *rx_rb, ring_buffer_t *tx_rb);

/**
 * UART RX ISR - push received byte into ring buffer
 * @param byte The received byte
 */
void uart_handler_isr_rx(uint8_t byte);

/**
 * UART TX ISR - pop next byte from ring buffer for transmission
 * @return true if byte was sent
 */
bool uart_handler_isr_tx(void);

/** Main loop polling - parse frames and dispatch commands */
void uart_handler_poll(void);

/**
 * Send a 0xA5 protocol frame
 * @param command Command byte (CMD_*)
 * @param data    Payload data (NULL if no payload)
 * @param len     Payload length (0 = no payload, LEN=1)
 * @return true   Frame pushed to TX ring buffer
 */
bool uart_handler_send(uint8_t command, const uint8_t *data, uint8_t len);
void uart_handler_write_buf(const uint8_t *data,uint8_t l_size);
#endif /* UART_HANDLER_H */
