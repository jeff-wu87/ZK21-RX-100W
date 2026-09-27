/*

   公司：中惠创智
   作者：wujianming
   date:2026.6.27

 * qi_driver.c
 * Qi Wireless Power - Driver Implementation
 *
 * Combines:
 *   - qi_packet  (1ms packet building)
 *   - qi_encoder (250us slot encoding)
 *
 * Flow:
 *   1ms tick   -> qi_packet_build() -> fills wx_Packet[]
 *   250us tick -> qi_encoder_tick() -> drives COM/COM1 pins
 */

#include "qi_driver.h"
#include "qi_packet.h"
#include "qi_encoder.h"
#include "qi_hw.h"

/* ===================== Internal State ===================================== */

static bool g_transmitting = false;

/* ===================== Public API Implementation ========================== */

void qi_driver_init(void)
{
    qi_hw_init();
    qi_packet_reset();
    g_transmitting = false;
}

void qi_driver_start(void)
{
    qi_packet_reset();
    g_transmitting = true;
}

void qi_driver_stop(void)
{
    g_transmitting = false;
    /* Clear COM pins */
    qi_hw_com_clear();
    qi_hw_com1_clear();
}

/* ===================== Data Input ========================================= */
//对外接口


void qi_driver_set_voltage(uint16_t voltage_mv)
{
    qi_packet_set_voltage(voltage_mv);
}

void qi_driver_set_current(uint16_t raw, uint16_t scale_1a)
{
    qi_packet_set_current(raw, scale_1a);
}

void qi_driver_set_temperature(uint8_t temp)
{
    qi_packet_set_temperature(temp);
}

void qi_driver_set_battery_charging(bool charging)
{
    qi_packet_set_battery_state(charging);
}

void qi_driver_set_error_code(uint8_t code)
{
    qi_packet_set_error_status(code);
}

/* ===================== Interrupt Handlers ================================= */

uint8_t len_d;
void qi_driver_250us_tick(void)
{
    if (!g_transmitting)
        return;

    /* Get current packet data for encoder */
    uint16_t* pkt = qi_packet_get_data();
         len_d = qi_packet_get_len();

    /* Initialize encoder if packet is ready and encoder is idle */
    if (len_d > 0 && qi_encoder_is_idle())
    {
        qi_encoder_init(pkt, len_d);
        /* Toggle control bit for COM/COM1 alternation */
        /* This is handled internally by the encoder per-packet */
    }

    /* Process one slot tick */
    qi_encoder_tick();

    /* If encoder done and we need new packet, trigger build */
    /* (will be handled in next 1ms tick) */
}

void qi_driver_1ms_tick(void)
{
    if (!g_transmitting)
        return;

//    /* 完成一帧数据发送 */
    if (qi_encoder_is_done())
    {
        /* Encoder done, need to advance phase and build new packet */
        //qi_packet_advance();

        /* Clear encoder done state */
       // qi_encoder_init(0, 1);  //NULL
    }

    /* Build next packet */
    qi_packet_build();
}

/* ===================== Status Query ======================================= */

bool qi_driver_is_transmitting(void)
{
    return g_transmitting;
}

const char* qi_driver_get_phase_name(void)
{
    return qi_packet_get_phase_name(qi_packet_get_current_phase());
}

uint8_t qi_driver_get_subctrl(void)
{
    return qi_packet_get_subctrl_index();
}

uint8_t qi_driver_get_power_iter(void)
{
    return qi_packet_get_power_iter();
}

uint16_t* qi_driver_get_packet(void)
{
    return qi_packet_get_data();
}

uint8_t qi_driver_get_packet_len(void)
{
    return qi_packet_get_len();
}
