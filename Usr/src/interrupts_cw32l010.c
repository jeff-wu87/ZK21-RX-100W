/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    interrupts.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 CW.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "../inc/main.h"
#include "../inc/interrupts_cw32l010.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
/* USER CODE END TD */


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */


/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */


/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* USER CODE END EV */


/******************************************************************************/
/*           Cortex-M0P Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn */

    /* USER CODE END NonMaskableInt_IRQn */
}

/**
  * @brief This function handles Hard fault interrupt.
  */


    

void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn */

    
   
      
    
    
    /* USER CODE END HardFault_IRQn */
    while (1)
    {
        
         PA03_SETLOW(); 
         PA04_SETHIGH();  //姝绘満
          
        /* USER CODE BEGIN W1_HardFault_IRQn */

        /* USER CODE END W1_HardFault_IRQn */
    }
}

 
/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn */

    /* USER CODE END SVCall_IRQn */
}


/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn */

    /* USER CODE END PendSV_IRQn */
}


/******************************************************************************/
/* CW030 Peripheral Interrupt Handlers                                        */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_ps030.s).                        */
/******************************************************************************/

/**
 * @brief This funcation handles WDT
 */
void WDT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles LVD
 */
void LVD_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles RTC
 */
void RTC_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles FLASHRAM
 */
void FLASHRAM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles SYSCTRL
 */
void SYSCTRL_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOA
 */
void GPIOA_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOB
 */
void GPIOB_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}


 
 



/**
 * @brief This funcation handles VC2
 */
void VC2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GTIM1
 */
void GTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles LPTIM
 */
void LPTIM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

 
/**
 * @brief This funcation handles BTIM2
 */
void BTIM2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM3
 */
void BTIM3_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles I2C1
 */
void I2C1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles SPI1
 */
void SPI1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART1
 */
//extern unsigned char rec_sum;
//uint8_t sendOffset, receiveOffset=0;
//extern uint8_t Received_data_length;
//extern unsigned char UART_RX_DATA[16];
//extern int DATA_LEN;
//extern int end_send;
//extern uint8_t Recdly;
//extern uint8_t FirstFrameFlag;
//void UART1_IRQHandler(void)
//{
//    /* USER CODE BEGIN */
//	  uint8_t TxRxBuffer;
//    if (UART_GetITStatus(CW_UART1, UART_IT_RC) != RESET)
//    {
////           TxRxBuffer = UART_ReceiveData_8bit(CW_UART1);
////			  				//	Rxtemp = UART1->RBR;
////							Recdly=45;
////			  				
////				if ((TxRxBuffer == 0xA5||(TxRxBuffer == 0xC5))&&(FirstFrameFlag==0)) //接收到帧头 /A frame header is received
////			 // if (TxRxBuffer == 0xC5)
////					{
////						FirstFrameFlag = 1;
////						rec_sum=0;
////						receiveOffset=0;
////						Received_data_length=0;
////					}
////									
////					if (FirstFrameFlag)
////					{
////						 UART_RX_DATA[receiveOffset] = TxRxBuffer; //保存接收数据/Save received data
////						 Received_data_length++;
////						 receiveOffset++;
////						
////						if(UART_RX_DATA[0]==0xA5)
////						{
////								if (TxRxBuffer == rec_sum) //接收到帧尾 /End of frame received
////								{
////									DATA_LEN=receiveOffset;
////									//UART1_receivedata();//当接收到的信息满足 头,长度,校验和 三者时会根据命令(UART_RX_DATA[2])做出处理
////									end_send=1;
////									FirstFrameFlag = 0;									
////								}
////								else
////								{	
////									 rec_sum =TxRxBuffer+rec_sum;
////								}
////						 }
////					} 
////					
//      //    UART_SendData_8bit(CW_UART1, TxRxBuffer);     //test
//				
//        UART_ClearITPendingBit(CW_UART1, UART_IT_RC);
//    }
//    /* USER CODE END */
//}

/**
 * @brief This funcation handles UART2
 */
void UART2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}


/**
 * @brief This funcation handles FAULT
 */
void CLKFAULT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/************************ (C) COPYRIGHT CW *****END OF FILE****/
