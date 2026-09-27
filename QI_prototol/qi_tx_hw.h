#ifndef QI_TX_HW_H
#define QI_TX_HW_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Hardware adapter for Qi TX communication pins.
 *
 * Porting only needs these functions:
 * - qi_tx_hw_init()
 * - qi_tx_hw_write()
 * - qi_tx_hw_toggle()
 * - qi_tx_hw_disable()
 *
 * Channel 0 and channel 1 map to the old COM / COM1 pins.
 */

typedef enum
{
    QI_TX_HW_CH_COM = 0,
    QI_TX_HW_CH_COM1 = 1
} qi_tx_hw_channel_t;

void qi_tx_hw_init(void);
void qi_tx_hw_write(qi_tx_hw_channel_t channel, bool high);
void qi_tx_hw_toggle(qi_tx_hw_channel_t channel);
void qi_tx_hw_disable(void);
void qi_app_init(void);
void qi_time_1ms_cb(void);
void qi_time_250us_cb(void);
void qi_tx_hw_setdat(uint16_t vout,uint16_t c,uint16_t t,uint16_t vin);
#endif
