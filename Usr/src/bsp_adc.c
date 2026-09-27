#include "bsp_adc.h"
#include "../inc/main.h"
#include "cw32l010_adc.h"

typedef enum
{    
    e_VBUS = 0,
    e_VIN,
    e_VBUS1,
    e_NTC,
    e_CUR,
    e_ADC_MAX,
}e_adc_chn;

 
#define VBUS_FREF    0.00986569f  // 
#define VBUS1_FREF   0.00986569f  // 
#define VIN_FREF     0.0262f //0.01831f    // 10/200 k  *5/4096
#define CUR_FREF     0.004787f    // 3A * 51 = 0.765V  ( )  = 5/4095/51/0.005 


int16_t NTC_GetTemp_Rough(uint16_t adc_val);


uint16_t adc_deb_buf[e_ADC_MAX][e_ADC_MAX];

uint8_t gFlagIrq;
/**
 * @brief This funcation handles ADC
 */
void ADC_IRQHandler(void)
{
    /* USER CODE BEGIN */
    
    
   if (CW_ADC->ISR & ADC_ISR_EOS_Msk)
    {
        gFlagIrq = CW_ADC->ISR;  
         
          
            
    }
    CW_ADC->ICR = 0x00;
    /* USER CODE END */
}

static uint16_t adc_math_avg_num(uint16_t *p,uint16_t len)
{
    uint16_t tmp;
    uint16_t i;
    tmp = 0;
    for(i =0;i < len;i ++)
    {
        tmp += *p;
        p ++;
    }
    tmp = tmp / len;
    return tmp;
}   


void bsp_ADC_PortInit(void)
{
	 ADC_InitTypeDef ADC_InitStructure = {0};
    //打开GPIO时钟
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();

    //打开ADC时钟
    __SYSCTRL_ADC_CLK_ENABLE();

    //set PA00 as AIN0 INPUT
    PA00_ANALOG_ENABLE();
    //set PA01 as AIN1 INPUT
    PA01_ANALOG_ENABLE();
    
    PA02_ANALOG_ENABLE();        
        
    //set PB03 as AIN10 INPUT
    PB03_ANALOG_ENABLE();   
    //set PB06 as AIN13 INPUT
    PB06_ANALOG_ENABLE();  
     
     ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div1;    // PCLK/1
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Continuous;
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputCH0;
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime30Clk;  //ADC_SampTime6Clk;
    ADC_InitStructure.ADC_IN1.ADC_InputChannel = ADC_InputCH1;
    ADC_InitStructure.ADC_IN1.ADC_SampTime = ADC_SampTime30Clk;
    
    ADC_InitStructure.ADC_IN2.ADC_InputChannel = ADC_InputCH2;
    ADC_InitStructure.ADC_IN2.ADC_SampTime = ADC_SampTime30Clk;
    
    
    ADC_InitStructure.ADC_IN3.ADC_InputChannel = ADC_InputCH10;
    ADC_InitStructure.ADC_IN3.ADC_SampTime = ADC_SampTime30Clk;
    
    ADC_InitStructure.ADC_IN4.ADC_InputChannel = ADC_InputCH13;
    ADC_InitStructure.ADC_IN4.ADC_SampTime = ADC_SampTime30Clk;
 
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to4;
    
    ADC_Init(&ADC_InitStructure);
    
    ADC_ClearITPendingAll();
   // ADC_ITConfig(ADC_IT_EOS, ENABLE);
   // NVIC_EnableIRQ(ADC_IRQn);	


      //ADC使能
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);		
}

    


void bsp_get_sample_val(_ADC_sample *pt)
{
    uint8_t i;
    uint16_t adc_buf[e_ADC_MAX];
    static uint16_t cnt=0;
    
    if ( 1)
    {   
        ADC_SoftwareStartConvCmd(DISABLE);
        ADC_GetSqr0Result(&adc_deb_buf[0][cnt]);  
        ADC_GetSqr1Result(&adc_deb_buf[1][cnt]);
        ADC_GetSqr2Result(&adc_deb_buf[2][cnt]);
        ADC_GetSqr3Result(&adc_deb_buf[3][cnt]);
        ADC_GetSqr4Result(&adc_deb_buf[4][cnt]);  
        gFlagIrq = 0u;            
        ADC_SoftwareStartConvCmd(ENABLE);   
        
        cnt ++;
        if(cnt == e_ADC_MAX)
        {
            cnt = 0;
            for(i = 0;i < e_ADC_MAX;i ++)
            {
            
               adc_buf[i] =  adc_math_avg_num(adc_deb_buf[i],e_ADC_MAX);
            }
            
           pt->vin  =  adc_buf[e_VIN] * VIN_FREF;         
           pt->vbus =  adc_buf[e_VBUS] * VBUS_FREF;
           pt->vbus1 =  adc_buf[e_VBUS1] * VBUS_FREF; 
           pt->cur  =  adc_buf[e_CUR] * CUR_FREF;         
           pt->ntc_temp = NTC_GetTemp_Rough(adc_buf[e_NTC]);
            
        }   
      
    }
        
        
   

}


/**

这数组是3.3V的 NTC
**/
const uint16_t ntc_adc_table[] = {
3853,3841,3828,3815,3801,3787,3772,3756,3740,3723,
3706,3688,3670,3651,3631,3611,3590,3568,3546,3523,
3499,3475,3450,3424,3398,3371,3343,3315,3286,3256,
3226,3195,3163,3131,3098,3064,3030,2995,2960,2924,
2887,2851,2813,2776,2738,2699,2661,2622,2583,2543,
2504,2464,2424,2384,2344,2304,2264,2224,2184,2144,
2104,2064,2025,1985,1946,1907,1868,1830,1792,1754,
1717,1680,1644,1608,1572,1537,1503,1469,1435,1402,
1370,1338,1307,1276,1246,1217,1188,1160,1132,1105,
1079,1053,1028,1003,979,956,933,911,889,868,
848,828,809,790,772,754,737,721,705,689,
674,660,646,632,619,606,594,582,570,559
};
#define NTC_TEMP_MIN     1    // ℃
#define NTC_TEMP_MAX     110  // ℃
#define NTC_TABLE_CNT    (sizeof(ntc_adc_table)/sizeof(ntc_adc_table[0]))



int16_t NTC_GetTemp_Rough(uint16_t adc_val)
{
    uint16_t i;
    for(i=0; i<NTC_TABLE_CNT-1; i++)
    {
        if(adc_val >= ntc_adc_table[i+1] && adc_val < ntc_adc_table[i])
        {
            return i + NTC_TEMP_MIN;
        }
    }
    if(adc_val >= ntc_adc_table[0]) return NTC_TEMP_MIN;
    return NTC_TEMP_MAX;

}

