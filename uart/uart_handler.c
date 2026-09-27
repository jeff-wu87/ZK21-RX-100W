/**
 * @file uart_handler.c
 * @brief UART handler implementation
 *  中断。。。跟发送。跟硬件结合
 * Protocol: [0xA5][LEN][CMD][Data...][Checksum]
 *
 * @par Data flow
 *
 *   Receive:
 *     UART ISR -> ring_buffer_push() -> uart_protocol_parse_frame() -> dispatch
 *
 *   Transmit:
 *     Application -> ring_buffer_write() -> UART ISR -> hardware
 */

#include "uart_handler.h"
#include "uart_protocol.h"
#include "ring_buffer.h"
#include "../inc/main.h"


#include "app_control.h"
 
/* ==================== Handler State ==================== */

/* Flag: frame received and parsed */
static volatile bool g_frame_ready = false;
static uart_protocol_frame_t g_parsed_frame;

/* Ring buffer pointers */
static ring_buffer_t *g_rx_rb = NULL;
static ring_buffer_t *g_tx_rb = NULL;

/* ==================== Application Callbacks ==================== */

void module_basicstatus_infamation(void) { /* TODO */ }
void module_fault_infamation(void)       { /* TODO */ }
void module_internstate_infamation(void) { /* TODO */ }
void module_startstop_infamation(void)   { /* TODO */ }
void module_serialnum_infamation(void)   { /* TODO */ }

/* ==================== Initialization ==================== */

void uart_handler_init(ring_buffer_t *rx_rb, ring_buffer_t *tx_rb)
{
    g_rx_rb = rx_rb;
    g_tx_rb = tx_rb;
    g_frame_ready = false;

    /* Initialize protocol parser with RX ring buffer */
    uart_protocol_init(rx_rb);
}

/* ==================== ISR Handlers ==================== */

void uart_handler_isr_rx(uint8_t byte)
{
    /* Push byte into ring buffer */
    if (!ring_buffer_push(g_rx_rb, byte))
    {
        
        
        /* Overflow - ring buffer full */
    }
  
}

bool uart_handler_isr_tx(void)
{
    uint8_t byte;

    if (ring_buffer_pop(g_tx_rb, &byte)) {
        /* Send byte to UART hardware */
        UART_SendData_8bit(CW_UART1, byte); 
        return true;
    }

    /* TX ring buffer empty - disable TX interrupt */
    /* UART_DisableIT(CW_UART1, UART_IT_TC); */
    return false;
}


void uart_ack_dat(void)
{   
     /*发固定命令*/
    uint8_t data[6] = {0XA5, 0X0E ,0X01 ,0X00 ,0X00 ,0XB4};  /* 2 bytes payload */
     
    uart_handler_write_buf(data,6); 
}


/* ==================== Polling ==================== */

void uart_handler_poll(void)
{   
    uint8_t rxbyte;
    uint16_t tmp;
    /* ---- Parse frames from ring buffer ---- */
    if (g_frame_ready) {
        /* Process previously parsed frame */
        switch (g_parsed_frame.command) {
        case CMD_BASIC_STATUS:
            module_basicstatus_infamation();
            break;
        case CMD_FAULT_INFO:
            module_fault_infamation();
            break;
        case CMD_INTERN_STATE:
            module_internstate_infamation();
            break;
        case CMD_START_STOP:
            module_startstop_infamation();
            break;
        case CMD_SERIAL_NUMBER:
            tmp = g_parsed_frame.data[0]<<8 | g_parsed_frame.data[1];
            app_set_uvptime(tmp);  
            uart_ack_dat();        
            break;
        
        case CMD_VOL_DAT:
             
            app_send_sys_dat(CMD_VOL_DAT);
        
        
            break;
        default:
            /* Unknown command */
            break;
        }
        g_frame_ready = false;
    }

    /* Try to parse a new frame */
    uart_protocol_frame_t frame;
    if (uart_protocol_parse_frame(&frame)) {
        /* Store frame for processing (to keep ISR clean) */
        g_parsed_frame = frame;
        g_frame_ready = true;
    }

    /* ---- Ensure TX interrupt is enabled if data pending ---- */
    
    while (!ring_buffer_is_empty(g_tx_rb)) {
         ring_buffer_pop(g_tx_rb,&rxbyte);
         UART_SendData_8bit(CW_UART1, rxbyte); 
         //UART_ITConfig(CW_UART1, UART_FLAG_TC, ENABLE);  
    }
}

/* ==================== 发送数据功能 ==================== */

bool uart_handler_send(uint8_t command, const uint8_t *data, uint8_t len)
{
    /* Build frame and push to TX ring buffer */
    return uart_protocol_send(g_tx_rb, command, data, len);
}

void uart_handler_write_buf(const uint8_t *data,uint8_t l_size)
{
       ring_buffer_write(g_tx_rb, data, l_size);
}
