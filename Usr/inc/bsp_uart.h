#ifndef _BSP_UART_H
#define _BSP_UART_H

/**
 * @brief  UART Init Structure definition
 */
//UARTx
#define  DEBUG_UARTx                   CW_UART1
#define  DEBUG_UART_CLK                SYSCTRL_APB1_PERIPH_UART1
#define  DEBUG_UART_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  DEBUG_UART_BaudRate           115200//38400    //9600
#define  DEBUG_UART_UclkFreq           8000000

//UARTx GPIO
#define  DEBUG_UART_GPIO_CLK           (SYSCTRL_AHB_PERIPH_GPIOB)
#define  DEBUG_UART_TX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_PIN_1
#define  DEBUG_UART_RX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_PIN_0

//GPIO AF
#define  DEBUG_UART_AFTX               PB01_AFx_UART1TXD()
#define  DEBUG_UART_AFRX               PB00_AFx_UART1RXD()

//中断
#define  DEBUG_UART_IRQ                UART1_IRQn


void bsp_uart_init(void);


#endif


