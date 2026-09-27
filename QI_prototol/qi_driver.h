/*
 * qi_driver.h
 * Qi Wireless Power - Driver Interface
 *
 * Combines packet builder + slot encoder into one easy-to-use API.
 *
 * Usage:
 *   1. qi_driver_init()          - one-time initialization
 *   2. qi_driver_set_*()         - set voltage/current/temp
 *   3. qi_driver_start()         - start packet building + timer
 *   4. In 250us interrupt: qi_driver_250us_tick()
 *   5. In 1ms interrupt:  qi_driver_1ms_tick()
 *   6. qi_driver_is_transmitting() - check status
 *   7. qi_driver_get_current_phase() - debug phase info
 */

#ifndef QI_DRIVER_H
#define QI_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/* ===================== Public API ========================================= */

/**
 * @brief  Initialize the full Qi driver
 * @note   Call once at startup (init GPIO, timer, reset state)
 */
void qi_driver_init(void);

/**
 * @brief  Start Qi transmission sequence
 * @note   Begins with PING phase, then auto-advances through protocol
 */
void qi_driver_start(void);

/**
 * @brief  Stop Qi transmission
 */
void qi_driver_stop(void);

/* ===================== Data Input (call before/during transmission) ======== */

/**
 * @brief  Set rectifier voltage (mV)
 */
void qi_driver_set_voltage(uint16_t voltage_mv);

/**
 * @brief  Set current measurement
 * @param  raw         raw ADC/sensor reading
 * @param  scale_1a    calibration: 300 = 1.0A
 */
void qi_driver_set_current(uint16_t raw, uint16_t scale_1a);

/**
 * @brief  Set coil temperature (0-100 scale)
 */
void qi_driver_set_temperature(uint8_t temp);

/**
 * @brief  Set battery charging state
 */
void qi_driver_set_battery_charging(bool charging);

/**
 * @brief  Set error code from receiver
 */
void qi_driver_set_error_code(uint8_t code);

/* ===================== Interrupt Handlers (call from TIM interrupt) ======== */

/**
 * @brief  250us timer tick handler
 * @note   Call every 250us in timer interrupt
 */
void qi_driver_250us_tick(void);

/**
 * @brief  1ms timer tick handler
 * @note   Call every 1ms in timer interrupt
 */
void qi_driver_1ms_tick(void);

/* ===================== Status Query ======================================= */

/**
 * @brief  Is transmission active?
 * @retval true  transmitting
 * @retval false idle
 */
bool qi_driver_is_transmitting(void);

/**
 * @brief  Get current phase name
 */
const char* qi_driver_get_phase_name(void);

/**
 * @brief  Get sub-control info
 * @retval 0 = ControlError phase
 * @retval 1 = RectifiedPower phase
 */
uint8_t qi_driver_get_subctrl(void);

/**
 * @brief  Get power transfer iteration count
 */
uint8_t qi_driver_get_power_iter(void);

/**
 * @brief  Get last built packet data pointer
 * @note   Valid until next packet built
 */
uint16_t* qi_driver_get_packet(void);

/**
 * @brief  Get last built packet length
 */
uint8_t qi_driver_get_packet_len(void);

#endif /* QI_DRIVER_H */
