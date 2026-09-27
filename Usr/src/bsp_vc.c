    
 #include "../inc/main.h"
#include "bsp_vc.h"

 
	void VC_PortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    //打开VC时钟
    //REGBITS_SET(CW_SYSCTRL->APBEN1,SYSCTRL_APBEN1_VC_Msk );
    __SYSCTRL_VC_CLK_ENABLE();
    //set PA02 as output
    GPIO_InitStruct.Pins = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
    //set PA02 as VC output
   // PA02_AFx_VC1OUT();             
    //set PB05 as VC1_CH0 P-INPUT
    PB04_ANALOG_ENABLE();
    //set PA00 as VC1_CH1 N-INPUT
    //PA00_ANALOG_ENABLE();
}



void bsp_vc_init()
{


    VC_InitTypeDef VC_InitStruct;
    VC_BlankTypeDef VC_BlankStruct;
    //LED初始化
   
    //配置测试IO口
    VC_PortInit();
    //VC通道初始化
    VC_InitStruct.VC_InputP = VC_InputP_Ch3;
    VC_InitStruct.VC_InputN = VC_InputN_Bgr1P2;    //VC_InputN_Ch1;    //
    VC_InitStruct.VC_Hys = VC_Hys_25mV;
    VC_InitStruct.VC_Resp = VC_Resp_High;
    VC_InitStruct.VC_FilterClk = VC_FltClk_PCLK;
    VC_InitStruct.VC_FilterTime = VC_FilterTime_4N6;
    VC_InitStruct.VC_Window = VC_Window_Disable;
    VC_InitStruct.VC_Polarity = VC_Polarity_High;
    VC1_ChannelInit(&VC_InitStruct);
    VC1VC2_INNInit(&VC_InitStruct);//负端输入信号初始化;
    //VC Blank窗口初始化
    VC1VC2_BlankInit(&VC_BlankStruct);
    VC_BlankStruct.VC_BlankLevel=VC_BlankLevel_high ;
	VC_BlankStruct.VC_BlankTime=VC_BlankTime_512Clk ;
   	VC_BlankStruct.VC_BlankCh5= VC_BlankCh4_Enable ;
    VC1_BlankCfg(&VC_BlankStruct);
    //VC中断设置
    VC1_ITConfig(VC_IT_HIGH, ENABLE);
    VC1_EnableIrq(VC_INT_PRIORITY);
    VC1_ClearIrq();
    VC1_EnableChannel();
    
    
}


void VC1_IRQHandler(void)
{
    /* USER CODE BEGIN */
	   REGBITS_CLR(CW_VC1->SR, VCx_SR_INTF_Msk);
    
				VC_DisableNvic(VC1_IRQn);
    /* USER CODE END */
}

