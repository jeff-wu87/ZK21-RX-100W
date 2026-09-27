#include "qi_tx_hw.h"
#include "qi_tx_protocol.h"
/*
 * Replace this include with your MCU GPIO header.
 * The default implementation below uses your old PA06 / PA05 macros.
 */
#include "main.h"
#include "bsp_io.h"

void qi_app_init(void)
{
    qi_tx_config_t cfg = {
        .packet_period_ms = 10,  //
        .preamble_bits = 16,
        .alternate_output_channel = true,
    };
   
    qi_tx_init(&cfg);
    qi_tx_hw_init();
    qi_tx_start();
    
    qi_tx_set_telemetry(2400, 0, 0, 2400);
    
     
}

void qi_time_250us_cb(void)
{
     qi_tx_250us_tick();

}
void qi_time_1ms_cb(void)
{
     qi_tx_1ms_tick();

}



void qi_tx_hw_init(void)
{
    qi_tx_hw_disable();
}
//对接硬件接口
void qi_tx_hw_write(qi_tx_hw_channel_t channel, bool high)
{
    if (channel == QI_TX_HW_CH_COM)
    {
        if (high)
        {
            PA06_SETHIGH();
        }
        else
        {
            PA06_SETLOW();
        }
    }
    else
    {
        if (high)
        {
            PA05_SETHIGH();
        }
        else
        {
            PA05_SETLOW();
        }
    }
}

void qi_tx_hw_toggle(qi_tx_hw_channel_t channel)
{
    if (channel == QI_TX_HW_CH_COM)
    {
        PA06_TOG();
    }
    else
    {
        PA05_TOG();
    }
}

void qi_tx_hw_disable(void)
{
    qi_tx_hw_write(QI_TX_HW_CH_COM, false);
    qi_tx_hw_write(QI_TX_HW_CH_COM1, false);
}

void qi_tx_hw_setdat(uint16_t vout,uint16_t c,uint16_t t,uint16_t vin)
{
     qi_tx_set_telemetry(vout,c,t,vin);
}
