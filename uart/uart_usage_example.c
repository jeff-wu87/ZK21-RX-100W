/**
 * @file uart_usage_example.c
 * @brief Usage example for the 0xA5 UART protocol
 *
 * Protocol format (per spec):
 *   [0xA5][LEN][CMD][Data...][Checksum]
 *
 *   LEN = 1 + payload_bytes
 *   Checksum = (0xA5 + LEN + CMD + Data[0] + ... ) & 0xFF
 *   Total frame length = LEN + 3
 *
 * Example 1 - No payload, CMD=0x01:
 *   [0xA5][0x01][0x01][Checksum]
 *   Checksum = (0xA5 + 0x01 + 0x01) & 0xFF = 0xA7
 *   ???: A5 01 01 A7
 *
 * Example 2 - 2 bytes payload, CMD=0x02:
 *   [0xA5][0x03][0x02][0x10][0x20][Checksum]
 *   Checksum = (0xA5 + 0x03 + 0x02 + 0x10 + 0x20) & 0xFF = 0xDA
 *   ???: A5 03 02 10 20 17
 */

#include "ring_buffer.h"
#include "uart_handler.h"

/* ============================================================
 * 1. Allocate ring buffers
 *    Each UART channel needs its own ring buffers
 * ============================================================ */

/* UART1 */
static uint8_t uart1_rx_buf[256];
static uint8_t uart1_tx_buf[256];
static ring_buffer_t uart1_rx_rb;
static ring_buffer_t uart1_tx_rb;

/* UART2 (optional) */
static uint8_t uart2_rx_buf[256];
static uint8_t uart2_tx_buf[256];
static ring_buffer_t uart2_rx_rb;
static ring_buffer_t uart2_tx_rb;

/* ============================================================
 * 2. Initialization
 * ============================================================ */

void system_init(void)
{
    /* UART1 */
    ring_buffer_init(&uart1_rx_rb, uart1_rx_buf, sizeof(uart1_rx_buf));
    ring_buffer_init(&uart1_tx_rb, uart1_tx_buf, sizeof(uart1_tx_buf));
    uart_handler_init(&uart1_rx_rb, &uart1_tx_rb);

    /* UART2 */
    ring_buffer_init(&uart2_rx_rb, uart2_rx_buf, sizeof(uart2_rx_buf));
    ring_buffer_init(&uart2_tx_rb, uart2_tx_buf, sizeof(uart2_tx_buf));
    /* uart2_handler_init(&uart2_rx_rb, &uart2_tx_rb); */
}

/* ============================================================
 * 3. UART Interrupt Service Routines
 * ============================================================ */

void UART1_IRQHandler(void)
{
    /* RX interrupt */
    if (UART_GetITStatus(CW_UART1, UART_IT_RC) != RESET) {
        uint8_t byte = UART_ReceiveData_8bit(CW_UART1);
        uart_handler_isr_rx(byte);  /* Push to ring buffer */
        UART_ClearITPendingBit(CW_UART1, UART_IT_RC);
    }

    /* TX interrupt */
    if (UART_GetITStatus(CW_UART1, UART_IT_TC) != RESET) {
        if (!uart_handler_isr_tx()) {
            UART_DisableIT(CW_UART1, UART_IT_TC);  /* FIFO empty */
        }
        UART_ClearITPendingBit(CW_UART1, UART_IT_TC);
    }
}

/* ============================================================
 * 4. Main loop
 * ============================================================ */

int main(void)
{
    SystemInit();
    system_init();

    /* Enable UART interrupts */
    UART_EnableIT(CW_UART1, UART_IT_RC);

    while (1) {
        uart_handler_poll();  /* Parse frames + dispatch commands */
    }

    return 0;
}

/* ============================================================
 * 5. Sending frames - examples
 * ============================================================ */

/* Example 1: Send command with NO payload (LEN=1) */
void send_basic_status_request(void)
{
    /* Frame: [0xA5][0x01][0x01][0xA7]
     *   Header=0xA5, LEN=0x01, CMD=0x01
     *   Checksum = (0xA5+0x01+0x01) & 0xFF = 0xA7 */
    uart_handler_send(CMD_BASIC_STATUS, NULL, 0);
}

/* Example 2: Send command WITH payload */
void send_start_control(void)
{
    uint8_t data[] = {0x01, 0x02};  /* 2 bytes payload */
    /* Frame: [0xA5][0x03][0x04][0x01][0x02][Checksum]
     *   Header=0xA5, LEN=0x03 (1 cmd + 2 data), CMD=0x04
     *   Checksum = (0xA5+0x03+0x04+0x01+0x02) & 0xFF = 0x10 */
    uart_handler_send(CMD_START_STOP, data, sizeof(data));
}

/* Example 3: Send with 1 byte payload (LEN=2) */
void send_fault_code(uint8_t fault_code)
{
    /* Frame: [0xA5][0x02][0x02][fault_code][Checksum]
     *   Header=0xA5, LEN=0x02 (1 cmd + 1 data), CMD=0x02
     *   Checksum = (0xA5+0x02+0x02+fault_code) & 0xFF */
    uint8_t data[1] = { fault_code };
    uart_handler_send(CMD_FAULT_INFO, data, sizeof(data));
}

/* ============================================================
 * 6. Reusing ring buffer for other protocols (CAN, SPI, etc)
 * ============================================================ */

/* CAN ring buffer - same API, different protocol parser */
static uint8_t can_rx_buf[512];
static ring_buffer_t can_rx_rb;

void CAN1_RX0_IRQHandler(void)
{
    /* Same ring buffer push - works for any byte stream */
    uint8_t byte;
    while (can_get_byte(&byte)) {
        ring_buffer_push(&can_rx_rb, byte);
    }
}
