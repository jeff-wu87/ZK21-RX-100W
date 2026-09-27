
/*
 ******************************************************************************
 * @file    main.c
 * @author  wu.jianming
 * @version V1.0.0
 * @date    2026-08-25
 * @brief   zk21-100w 接收板子，主函数
 * 修改内容：
 * 改成通信发送4个字节。。。26.9.24
 ******************************************************************************
*/

#include "../inc/main.h"
#include "cw32l010_adc.h"
#include "constant.h"
#include "bsp_adc.h"
#include "app_control.h"
#include "app_led.h"
#include "bsp_uart.h"
#include "bsp_io.h"
#include "bsp_flash.h"
 
 
#include "bsp_vc.h"
#include "ring_buffer.h"
#include "uart_handler.h"
#include "qi_tx_hw.h"

#define UART_DEBUG_ENABLE 0


extern void F_Phase_Packet_Control(void);
extern void UART_Configuration(void);


void UART_SendString(UART_TypeDef *UARTx, char *String);
void Delay(uint16_t nCount);

uint8_t bDelay1msFlag;


 
int32_t main(void)
{ 
 
       
	  SYSCTRL_Configuration();
	  REGBITS_SET(CW_SYSCTRL->AHBEN, (0x5A5A0000 |bv1));
      REGBITS_SET(CW_SYSCTRL->AHBEN, (0x5A5A0000 |bv5));
	  
    
      qi_app_init();
	  BTIM_Period_Count_Config();
	
	  ATIM_Code_Count_Config();
	  
      GPIO_Config();  
      bsp_LedInit();
   //   bsp_vc_init();//
    

	  bsp_ADC_PortInit();
	 
	  bsp_uart_init();
	  
	  UART_ITConfig(DEBUG_UARTx, UART_IT_RC, ENABLE);
     /* UART_ITConfig(DEBUG_UARTx, UART_FLAG_TC, ENABLE);*/
      UART_ClearITPendingBit(CW_UART1, UART_IT_RC);
     // UART_SendString(DEBUG_UARTx, " \r\n");
     // UART_SendString(DEBUG_UARTx, "zk21-100w-v1.0\r\n");
#if UART_DEBUG_ENABLE
      printf("software time:%s%s\r\n", __DATE__, __TIME__);

      printf("zk21-100w-v1.0 \r\n");
      uint8_t i;
      uint8_t tmp;
      uint8_t test_fl_buf[128]; 
//      for(i =0;i < 128;i++)
//      {
//        test_fl_buf[i] = i;
//      }
//      
//      tmp = bsp_flash_write(test_fl_buf,128);
//      if(tmp ==TRUE)
//      {
//           printf("flash wrtie seccss!\r\n");
//      }
//      else {
//           printf("flash wrtie false!\r\n");
//      }
//       for(i =0;i < 128;i++)
//      {
//        test_fl_buf[i] = 0;
//      }
//      bsp_flash_read(test_fl_buf,128);
//       for(i =0;i < 128;i++)
//      {
//           printf("%d\r\n",test_fl_buf[i]);
//      }
#endif
	   
      
	  IWDT_SetPeriod(IWDT_2_SECS); 
      app_sys_init();
    
      bsp_buck_enable(0);

   
    while(1)
    {
	    
        
        
        if(bDelay1msFlag == 1)
        {
            bDelay1msFlag = 0;

            app_control();
            
            uart_handler_poll();
            app_Led_Task_1ms();
            IWDT_Refresh(); 
            
        }
      
        
    }

}


/**
 * @brief This funcation handles BTIM1
1MS  
 */
void BTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */
	
    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_UPDATE))
    {			  			
			 	// Fun_RxPhaseControl_1ms();
			   // Fun_SendCode_250us();
         bDelay1msFlag = 1;
        //qi_time_1ms_cb();
         BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_UPDATE);
			
      //  PA03_TOG();
    }
	

    /* USER CODE END */
}


/**
 * @brief This funcation handles ATIM
250US 
 */
void ATIM_IRQHandler(void)
{
    /* USER CODE BEGIN */
     static uint8_t cnt;
	  if (ATIM_GetITStatus(ATIM_STATE_UIF))
    {
    

         #if 1
       // qi_driver_250us_tick();
      qi_time_250us_cb();
        if(++cnt >= 4){
            cnt = 0;
         
            qi_time_1ms_cb();
        }
        #endif
        
        ATIM_ClearITPendingBit(ATIM_STATE_UIF);
      
    }


    /* USER CODE END */
}
/**
* @brief 简单延时
 *
 * @param nCount
 */
void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}


#if UART_DEBUG_ENABLE
int __retarget_putchar(int ch);
/* MicroLib ֻȏ fputc/fgetc */
int fputc(int ch, FILE *f)
{
    return __retarget_putchar(ch);
}
int fgetc(FILE *f)        /* ȧШ scanf£¬һ²¢ʵϖ */
{
    /* Ӄ»§¿ɔڴ˴¦²¹½ӊմú« */
    return 0;
}


/**
 * @brief Retargets the C library printf function to the UART.
 *
 */
int __retarget_putchar(int ch)
{
    UART_SendData_8bit(DEBUG_UARTx, (uint8_t)ch);

    while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);

    return ch;
}

size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    size_t nChars = 0;

    if (buffer == 0)
    {
        /*
         * This means that we should flush internal buffers.  Since we
         * don't we just return.  (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return 0;
    }


    for (/* Empty */; size != 0; --size)
    {
        UART_SendData_8bit(DEBUG_UARTx, *buffer++);
        while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);
        ++nChars;
    }

    return nChars;
}
#endif


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
 /**
   * @brief  Reports the name of the source file and the source line number
   *         where the assert_param error has occurred.
   * @param  file: pointer to the source file name
   * @param  line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif

