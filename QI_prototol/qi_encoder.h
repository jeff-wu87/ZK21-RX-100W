/*
 * qi_encoder.h
 * Qi Wireless Power - Slot Encoder
 *
 * Drives COM/COM1 pins at 4kHz (250us half-period)
 * using differential Manchester encoding.
 *
 * Qi Slot Format (500us total):
 *
 *   Slot N (bit b):
 *   ┌────────────── First Half 250us ──────┬──── Second Half 250us ─────┐
 *   │  COM/COM1 always flips                │  Flip again if b==1       │
 *   │  Process bit index                    │                           │
 *   └───────────────────────────────────────┴───────────────────────────┘
 *
 *   Bit '1' -> flips twice per slot (both halves)
 *   Bit '0' -> flips once per slot  (first half only)
 *
 * Usage:
 *   1. qi_encoder_init() - initialize state
 *   2. Call qi_encoder_tick() in 250us timer interrupt
 *   3. qi_encoder_is_done() - check if transmission complete
 */

#ifndef QI_ENCODER_H
#define QI_ENCODER_H

#include <stdint.h>
#include <stdbool.h>

/* ===================== Public API ========================================= */

/**
 * @brief  Initialize encoder state
 * @param  packet: pointer to data bytes to send (8-bit encoded values)
 * @param  nbytes: number of bytes to send
 * @note   Must be called before starting transmission
 */
void qi_encoder_init(const uint16_t *packet, uint8_t nbytes);

/**
 * @brief  Process one 250us timer tick (call in interrupt)
 *
 *  First half: always flip COM/COM1, update bit index
 *  Second half: flip again if current bit is '1'
 *
 *  Differential encoding:
 *    bit '1' -> COM toggles twice (stronger signal)
 *    bit '0' -> COM toggles once
 */
void qi_encoder_tick(void);

/**
 * @brief  Check if transmission is complete
 * @retval true  all bits sent
 * @retval false still transmitting
 */
bool qi_encoder_is_done(void);

/**
 * @brief  Check if encoder is idle (not transmitting)
 * @retval true  idle
 * @retval false transmitting
 */
bool qi_encoder_is_idle(void);

#endif /* QI_ENCODER_H */
