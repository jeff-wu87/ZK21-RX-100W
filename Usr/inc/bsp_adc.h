#ifndef _BSP_ADC_H
#define _BSP_ADC_H

 


#include <stdint.h>
#include <stdbool.h>
 







typedef struct
{    
    
    float vin;
    float vbus;
    float vbus1; 
    float cur;
    float ntc_temp;
    //uint16_t s_adc_buf[e_ADC_MAX];
    
}_ADC_sample;

void bsp_ADC_PortInit(void);
void bsp_get_sample_val(_ADC_sample *pt);

#endif

