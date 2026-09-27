#include "bsp_uart.h"
#include "../inc/main.h"

#include "ring_buffer.h"
#include "uart_handler.h"

#define  FIFO_LEN   128
static uint8_t uart1_rx_buf[FIFO_LEN];
static uint8_t uart1_tx_buf[FIFO_LEN];
ring_buffer_t uart1_rx_rb;
ring_buffer_t uart1_tx_rb;




void UART1_IRQHandler(void)
{
    uint8_t txbyte;
    if (UART_GetITStatus(CW_UART1, UART_IT_RC) != RESET)
    {
          txbyte = UART_ReceiveData_8bit(CW_UART1);
		  uart_handler_isr_rx(txbyte);   				 
				
        UART_ClearITPendingBit(CW_UART1, UART_IT_RC);
    }
     
     /* TX interrupt */
    if (UART_GetITStatus(CW_UART1, UART_IT_TC) != RESET) {
        if (!uart_handler_isr_tx()) {
            UART_ITConfig(CW_UART1, UART_FLAG_TC, DISABLE); 
           
        }
        UART_ClearITPendingBit(CW_UART1, UART_IT_TC);
    }
    
    
    
    
}

void bsp_uart_poll()
{
     if (!uart_handler_isr_tx()) 
     {
             
        
     }
     
     
     
}

void uart_gpio_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pins = DEBUG_UART_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = DEBUG_UART_RX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStructure);

    //UART TX RX 复用
    DEBUG_UART_AFTX;
    DEBUG_UART_AFRX;
}

/**
 * @brief 配置NVIC
 *
 */
void uart_NVIC_Configuration(void)
{
    //优先级，无优先级分组
    NVIC_SetPriority(DEBUG_UART_IRQ, 0);
    //UARTx中断使能
    NVIC_EnableIRQ(DEBUG_UART_IRQ);
}



/**
 * @brief 配置UART
 *
 */
void bsp_uart_init(void)
{
	
    UART_InitTypeDef UART_InitStructure = {0};
    uart_gpio_Configuration();
    
    UART_InitStructure.UART_BaudRate = DEBUG_UART_BaudRate;
    UART_InitStructure.UART_Over = UART_Over_16;
    UART_InitStructure.UART_Source = UART_Source_PCLK;
    UART_InitStructure.UART_UclkFreq = DEBUG_UART_UclkFreq;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(DEBUG_UARTx, &UART_InitStructure);
	uart_NVIC_Configuration();
    
    
    ring_buffer_init(&uart1_tx_rb, uart1_tx_buf, sizeof(uart1_tx_buf));  /*配置队列*/
    ring_buffer_init(&uart1_rx_rb, uart1_rx_buf, sizeof(uart1_rx_buf));  /*配置队列*/
    uart_handler_init(&uart1_rx_rb, &uart1_tx_rb);
    
    
}

 


